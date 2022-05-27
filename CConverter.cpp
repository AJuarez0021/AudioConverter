#pragma warning(disable:4786)
#include <windows.h>
#include "CConverter.h"
#include "Lame\BladeMP3EncDll.h"
#include <conio.h>
#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;


signed char  StreamEnded=FALSE; 
unsigned int BytesWritten=0; 
CFileOut f_Out;

 namespace Mp3
{
 unsigned char     *WaveformData;
 char startID[12];

 struct FormatChunk{
    char            chunkID[4];
    long            chunkSize;
    short           wFormatTag;
    unsigned short  wChannels;
    unsigned long   dwSamplesPerSec;
    unsigned long   dwAvgBytesPerSec;
    unsigned short  wBlockAlign;
    unsigned short  wBitsPerSample;
 };
 struct Chunk{
    char            chunkID[4];
    long            chunkSize;
 };
 typedef struct{        
	HINSTANCE	        mDLL;
	BE_VERSION	        Version;    
    BEINITSTREAM        beInitStream;
    BEENCODECHUNK       beEncodeChunk;
    BEDEINITSTREAM      beDeinitStream;
    BECLOSESTREAM       beCloseStream;
    BEVERSION           beVersion;
    BEWRITEVBRHEADER    beWriteVBRHeader;

 }outputmp3_state;
}

namespace Wav{
 typedef struct{
	signed char id[4];
	int 		Size;
 }RiffChunk;

 struct{
    RiffChunk       Chunk;
    unsigned short	wFormatTag;    
    unsigned short	nChannels;    
    unsigned int	nSamplesPerSec;    
    unsigned int	nAvgBytesPerSec;    
    unsigned short	nBlockAlign;   
    unsigned short	wBitsPerSample;    
 }FmtChunk  = { {{'f','m','t',' '}, sizeof(FmtChunk) - sizeof(RiffChunk) }, 1, 2, 44100, 44100 * 2 * 16 / 8, 1 * 2 * 16 / 8, 16 };

 struct{
  RiffChunk   Chunk;
 } DataChunk = { {{'d','a','t','a'}, 0 } };

 struct{
    RiffChunk   Chunk;
	signed char RiffType[4];
 }WavHeader = { {{'R','I','F','F'}, 0 }, {'W','A','V','E'} };



 signed char F_CALLBACKAPI EndCallback(FSOUND_STREAM *Stream, void *Buffer, int iLength, void *Param)
 {
    StreamEnded = TRUE;
    return TRUE;
 }

 void *F_CALLBACKAPI DSP_RawWriteCallback(void *OriginalBuffer, void *NewBuffer, int iLength, void *Param)
 {
    if (!StreamEnded){			
		f_Out.Write(NewBuffer,(iLength << 2));
        BytesWritten += (iLength << 2);
    }
  return NewBuffer;
 }
}


void  CConverter::GetElapsedTime(int &iMin,int &iSec)
{
 iMin=TiempoTranscurrido / 1000 / 60;
 iSec=TiempoTranscurrido / 1000 % 60;
}

void CConverter::InitProgressBar(HWND hWnd,UINT uID,COLORREF Color)
{
 Win32.CreateProgressBar();
 Win32.SetWNDProgressBar(hWnd,uID);
 Win32.SetRangeProgressBar(1,100);
 Win32.SetStepProgressBar(1);
 Win32.ReinitProgressBar();
 Win32.SetBarColorProgressBar(Color);  
}

bool CConverter::ConverterToWav(string In,string Out)
{
 using namespace Wav;
 int  LeerPorcentaje;
 char *Dato;
 ofstream Log;
 SYSTEMTIME lpSystemTime; 
 FSOUND_STREAM *Stream=NULL;
 FSOUND_DSPUNIT *rawwrite_dsp;

 string strPathExe=Win32.GetPathExe();
 strPathExe+="\\status_converter_to_wav.txt";

 Log.open(strPathExe.c_str());

 if(In.empty()){
  Log << "Error no hay cadena de entrada" << endl;
  return false;
 }
 if(Out.empty()){	 
  Log << "Error no hay cadena de salida" << endl;
  return false;
 }

 if(!strstr(In.c_str(),".mp2") && !strstr(In.c_str(),".mp3") && !strstr(In.c_str(),".wma")){
  Log << "Error no es un archivo compatible" << endl;
  return false;
 }

 if(!strstr(Out.c_str(),".wav")){
  Log << "Error el archivo de salida debe tener la extension .wav" << endl;
  return false;
 }

 Log << "Archivo de entrada: " << In.c_str() << endl;
 Log << "Archivo de salida: " << Out.c_str() << endl;
 Log << endl;
 
 Timer.Start();
 Log << "Iniciando Timer" << endl;
 GetLocalTime(&lpSystemTime);
 Log << "Fecha: " << lpSystemTime.wDay << "/" << lpSystemTime.wMonth << "/" <<lpSystemTime.wYear << endl;
 Log << "Hora: " << setw(2) << setfill('0') << lpSystemTime.wHour << ":" << setw(2) << setfill('0') << lpSystemTime.wMinute << ":" << setw(2) << setfill('0') << lpSystemTime.wSecond << endl;
 SetOutPut(NOSOUND_NONREALTIME);
 Log << "poniendo salida de sonido NOSOUND_NONREALTIME" << endl;

 if(!InitAudio(44100,32,0)){
  Log << "Error al iniciar FMOD" << endl;
  return false;
 }
 Log << "Iniciacion de FMOD correcta\n";

 SetBufferSize(2000);
 Log << "Poniendo tamaño de buffer\n";

 f_In.openFile(In.c_str());
 if(!f_In.IsOpen()){
  CloseAudio();
  Log << "Error al leer el archivo: " << In.c_str() << endl;
  return false;
 }
 Log << "Se abrio el archivo: " << In.c_str() <<" correctamente\n";

 long lLongitud=f_In.GetfileSize();
 Log << "Longitud del archivo: " << lLongitud << endl;

 Dato=new char[lLongitud];
 if(!Dato){  
  Log << "Error no hay memoria\n" << endl;
  return false;
 }
 Log << "Asignacion de memoria correcta" << endl;

 f_In.Read(Dato,lLongitud);
 Log << "Lectura de buffer correcta\n";

 f_Out.openFile(Out.c_str(),false);
 if(!f_Out.IsOpen()){
  Log << "Error al escribir en el archivo: " << Out.c_str() << endl;
  CloseAudio();
  return false;
 }
 Log << "Se abrio el archivo: " << Out.c_str() << " para escritura correctamente\n";

 f_Out.Seek(sizeof(WavHeader) + sizeof(FmtChunk) + sizeof(DataChunk),false);
 Log << "Poniendo desplazamiento en el archivo de salida\n";

 rawwrite_dsp=FSOUND_DSP_Create(&DSP_RawWriteCallback, FSOUND_DSP_DEFAULTPRIORITY_USER, 0);
 FSOUND_DSP_SetActive(rawwrite_dsp, TRUE);  
 Log << "Poniendo funcion Callback\n";

 Stream = FSOUND_Stream_Open(Dato, FSOUND_NORMAL | FSOUND_MPEGACCURATE | FSOUND_LOADMEMORY, 0, lLongitud);
 if(!Stream){       
  FSOUND_DSP_SetActive(rawwrite_dsp, FALSE);
  FSOUND_DSP_Free(rawwrite_dsp);
  CloseAudio();
  Log << "Error al crear el Stream\n";
  return false;
 }
 Log << "Creacion del Stream correcta\n";
 
 FSOUND_Stream_SetEndCallback(Stream, EndCallback, 0);
 FSOUND_Stream_Play(FSOUND_FREE, Stream);
 Log << "Reproduciendo el archivo de audio para la conversion\n";

 do{  
  LeerPorcentaje = (int)(((float)FSOUND_Stream_GetTime(Stream) / (float)FSOUND_Stream_GetLengthMs(Stream)) * 100.0f);
  Win32.SetValueProgressBar(LeerPorcentaje);
  Win32.UpdateProgressBar();
  UpdateAudio();
  //Timer.Delay(1);
 }while (!StreamEnded);


 FSOUND_Stream_Close(Stream);
 Log << "Cerrando Stream" << endl;

 FSOUND_DSP_SetActive(rawwrite_dsp, FALSE);
 FSOUND_DSP_Free(rawwrite_dsp);   
 Log << "Liberando funcion Callback" << endl;

 WavHeader.Chunk.Size=sizeof(FmtChunk) + sizeof(RiffChunk) + BytesWritten;
 Log << "WavHeader.Chunk.Size: " <<  WavHeader.Chunk.Size << endl;

 DataChunk.Chunk.Size = BytesWritten;
 Log << "DataChunk.Chunk.Size: " << DataChunk.Chunk.Size << endl;

 f_Out.Seek(0,false);
 Log << "Poniendo el desplazamiento al comienzo del archivo de salida" << endl;

 f_Out.Write(&WavHeader, sizeof(WavHeader));
 Log << "Escribiendo WavHeader" << endl;

 f_Out.Write(&FmtChunk,  sizeof(FmtChunk));
 Log << "Escribiendo FmtChunk" << endl;

 f_Out.Write(&DataChunk, sizeof(DataChunk));
 Log << "Escribiendo DataChunk" << endl;

 CloseAudio();
 Log << "Cerrando FMOD" << endl;

 Timer.Stop();
 Log << "Finalizando Timer" << endl;

 TiempoTranscurrido = Timer.GetElapsed();
 BytesWritten = 0;
 StreamEnded=FALSE; 

 int iMin,iSec;
 GetElapsedTime(iMin,iSec);
 Log << "Tiempo Transcurrido: " << setw(2) << setfill('0') << iMin 
	 << ":" << setw(2) << setfill('0') << iSec << endl;
 
 if(Dato!=0)
  delete [] Dato;
 Log << "Liberando memoria" << endl;

 Win32.ReinitProgressBar();
 Log << "Reiniciando ProgressBar" << endl;

 Log << "Cerrando archivos" << endl;
 Log.close();
 f_Out.CloseFile();
 f_In.CloseFile();
 return true;
}

bool CConverter::ConverterCdaToWav(char cDrive,int iTrackNum,string Out)
{
 using namespace Wav;
 int  LeerPorcentaje; 
 char drive_letter[3]; 
 FSOUND_STREAM *stream=NULL;
 FSOUND_DSPUNIT *rawwrite_dsp;
 SYSTEMTIME lpSystemTime;
 string strPathExe=Win32.GetPathExe();
 strPathExe+="\\status_cda_to_wav.txt";
 ofstream Log(strPathExe.c_str());

 
 if(Out.empty()){
  Log << "Error la cadena esta vacia" << endl;
  return false;
 }
 
 if(!strstr(Out.c_str(),".wav")){
  Log << "Error el archivo de salida debe tener la extension .wav" << endl;
  return false;
 }

 if (!((cDrive >= 'a' && cDrive <= 'z') || (cDrive >= 'A' && cDrive <= 'Z'))){
  Log << "No es el drive" << endl;
  return false;
 }

 Log << "Drive: " << cDrive << ":" << endl;
 Log << "Archivo de Salida: " << Out.c_str() << endl;
 Log << "Numero de Track a convertir: " << iTrackNum << endl;
 sprintf(drive_letter, "%c:",cDrive);
 Timer.Start();
 Log << "Iniciando Timer" << endl;
 GetLocalTime(&lpSystemTime);
 Log << "Fecha: " << lpSystemTime.wDay << "/" << lpSystemTime.wMonth << "/" <<lpSystemTime.wYear << endl;
 Log << "Hora: " << setw(2) << setfill('0') << lpSystemTime.wHour << ":" << setw(2) << setfill('0') << lpSystemTime.wMinute << ":" << setw(2) << setfill('0') << lpSystemTime.wSecond << endl;
 
 SetOutPut(NOSOUND_NONREALTIME);
 Log << "poniendo salida de sonido NOSOUND_NONREALTIME" << endl;

 if(!InitAudio(44100,32,0)){
  Log << "Error al iniciar fmod" << endl;
  return false;
 }
 Log << "Inicio de fmod" << endl;

 f_Out.openFile(Out.c_str(),false);
 if(!f_Out.IsOpen()){
  CloseAudio();
  Log << "Error al crear el archivo de salida" << endl;
  return false;
 }
 Log << "Se abrio el archivo: " << Out.c_str() << " para escritura correctamente\n";

 f_Out.Seek(sizeof(WavHeader) + sizeof(FmtChunk) + sizeof(DataChunk),false);
 Log << "Poniendo desplazamiento en el archivo de salida\n";

 rawwrite_dsp = FSOUND_DSP_Create(&DSP_RawWriteCallback, FSOUND_DSP_DEFAULTPRIORITY_USER, 0);
 Log << "creando funcion callback" << endl;

 FSOUND_DSP_SetActive(rawwrite_dsp, TRUE);
 Log << "Activando funcion callback" << endl;

 SetBufferSize(2000);
 Log << "Fijando el tamaño del buffer" << endl;

 stream = FSOUND_Stream_Open(drive_letter, 0,0,0); 
 if (!stream){       
   FSOUND_DSP_SetActive(rawwrite_dsp, FALSE);
   FSOUND_DSP_Free(rawwrite_dsp);
   CloseAudio();
   Log << "Error al crear el stream" << endl;   
   return false;
 }
 Log << "Stram creado correctamente" << endl;

 if ((iTrackNum < 1) || ((iTrackNum - 1) >= FSOUND_Stream_GetNumSubStreams(stream))){
  FSOUND_Stream_Close(stream);
  FSOUND_DSP_SetActive(rawwrite_dsp, FALSE);
  FSOUND_DSP_Free(rawwrite_dsp);
  CloseAudio();
  Log << "No es un track valido" << endl;
  return false;
 }
 FSOUND_Stream_SetEndCallback(stream, EndCallback, 0);
 FSOUND_Stream_SetSubStream(stream, iTrackNum - 1);
 Log << "Poniendo el numero de track a reproducir" << endl;
 FSOUND_Stream_Play(FSOUND_FREE, stream);
 Log << "Reproduciendo el track para conversion" << endl;
 do{
  LeerPorcentaje = (int)(((float)FSOUND_Stream_GetTime(stream) / (float)FSOUND_Stream_GetLengthMs(stream)) * 100.0f);
  Win32.SetValueProgressBar(LeerPorcentaje);
  Win32.UpdateProgressBar();
  UpdateAudio();
  //Timer.Delay(1);
 }while(!StreamEnded);
 FSOUND_Stream_Close(stream);
 Log << "Cerrando Stream" << endl;

 FSOUND_DSP_SetActive(rawwrite_dsp, FALSE);
 FSOUND_DSP_Free(rawwrite_dsp);
 Log << "Liberando funcion Callback" << endl;

 WavHeader.Chunk.Size = sizeof(FmtChunk) + sizeof(RiffChunk) + BytesWritten;
 Log << "WavHeader.Chunk.Size: " <<  WavHeader.Chunk.Size << endl;

 DataChunk.Chunk.Size = BytesWritten;
 Log << "DataChunk.Chunk.Size: " << DataChunk.Chunk.Size << endl;

 f_Out.Seek(0,false);
 Log << "Poniendo el desplazamiento al comienzo del archivo de salida" << endl;

 f_Out.Write(&WavHeader, sizeof(WavHeader));
 Log << "Escribiendo WavHeader" << endl;

 f_Out.Write(&FmtChunk,  sizeof(FmtChunk));
 Log << "Escribiendo FmtChunk" << endl;

 f_Out.Write(&DataChunk, sizeof(DataChunk));
 Log << "Escribiendo DataChunk" << endl;

 CloseAudio();
 Log << "Cerrando FMOD" << endl;

 Timer.Stop();
 Log << "Finalizando Timer" << endl;

 TiempoTranscurrido = Timer.GetElapsed();
 BytesWritten = 0;
 StreamEnded=FALSE; 

 int iMin,iSec;
 GetElapsedTime(iMin,iSec);
 Log << "Tiempo Transcurrido: " << setw(2) << setfill('0') << iMin 
	 << ":" << setw(2) << setfill('0') << iSec << endl;

 Log << "Reiniciando ProgressBar" << endl;
 Win32.ReinitProgressBar();
 Log << "Cerrando archivos" << endl;
 Log.close();
 f_Out.CloseFile();
 f_In.CloseFile();
 return true;
}



bool CConverter::ConverterWavToMp3(string In,string Out)
{
	using namespace Mp3;
    FSOUND_STREAM *Stream=NULL;
	SYSTEMTIME lpSystemTime;
	string strPathExe=Win32.GetPathExe();
    strPathExe+="\\status_converter_to_mp3.txt";
    ofstream Log(strPathExe.c_str());


    outputmp3_state *state;
	BE_CONFIG bc={0,};

	Log << "Archivo de entrada: " << In.c_str() << endl;
    Log << "Archivo de salida: " << Out.c_str() << endl;
    Log << endl;

    Timer.Start();
    Log << "Iniciando Timer" << endl;

    GetLocalTime(&lpSystemTime);

    Log << "Fecha: " << lpSystemTime.wDay << "/" << lpSystemTime.wMonth << "/" <<lpSystemTime.wYear << endl;
    Log << "Hora: " << setw(2) << setfill('0') << lpSystemTime.wHour << ":" << setw(2) << setfill('0') << lpSystemTime.wMinute << ":" << setw(2) << setfill('0') << lpSystemTime.wSecond << endl;

	if(In.empty()){
	 Log << "La cadena de entrada vacia" << endl;
     return false;
	}

	if(Out.empty()){
	 Log << "La cadena de salida vacia" << endl;
     return false;
	}

	if(!strstr(Out.c_str(),".mp3")){
      Log << "Error el archivo de salida debe tener la extension .wav" << endl;
      return false;
	}

    state =  new outputmp3_state;
    if(!state){
     Log << "Error no hay memoria" << endl;
	}
	Log << "Asignacion de memoria correcta a la estructura outputmp3_state" << endl;

	state->mDLL = LoadLibrary("lame_enc.dll");
	if (!state->mDLL){
		Log << "Error al cargar lame_enc.dll" << endl;
		return false;
	}
	Log << "Se ha cargado lame_enc.dll correctamente" << endl;

	state->beInitStream	    = (BEINITSTREAM)     GetProcAddress(state->mDLL, TEXT_BEINITSTREAM);
	state->beEncodeChunk	= (BEENCODECHUNK)    GetProcAddress(state->mDLL, TEXT_BEENCODECHUNK);
	state->beDeinitStream	= (BEDEINITSTREAM)   GetProcAddress(state->mDLL, TEXT_BEDEINITSTREAM);
	state->beCloseStream	= (BECLOSESTREAM)    GetProcAddress(state->mDLL, TEXT_BECLOSESTREAM);
	state->beVersion		= (BEVERSION)        GetProcAddress(state->mDLL, TEXT_BEVERSION);
	state->beWriteVBRHeader = (BEWRITEVBRHEADER) GetProcAddress(state->mDLL, TEXT_BEWRITEVBRHEADER);

	if(!state->beInitStream || !state->beEncodeChunk || !state->beDeinitStream || !state->beCloseStream || !state->beVersion || !state->beWriteVBRHeader){
		Log << "Error al leer las funciones de lame_enc.dll" << endl;
		return false;
	}

	Log << "Se ha cargado las funciones de lame_enc.dll correctamente" << endl;

	state->beVersion( &state->Version );
    Log << "Obteniendo version de lame_enc.dll" << endl;

	SetOutPut(NOSOUND_NONREALTIME);
    if(!InitAudio(44100,32,0)){
     Log << "Error al iniciar FMOD" << endl;
     return false;
	}
    SetBufferSize(2000);
    Log << "Poniendo tamaño de buffer\n";

    f_In.openFile(In.c_str());
    if(!f_In.IsOpen()){
     CloseAudio();
     Log << "Error al leer el archivo: " << In.c_str() << endl;
     return false;
	}
    Log << "Se abrio el archivo: " << In.c_str() <<" correctamente\n";
 
    long lLongitud=f_In.GetfileSize();
    Log << "Longitud del archivo: " << lLongitud << endl;

    char *Dato=new char[lLongitud];
    if(!Dato){  
     Log << "Error no hay memoria\n" << endl;
     return false;
	}
    Log << "Asignacion de memoria correcta" << endl;
    f_In.Read(Dato,lLongitud);
    Log << "Lectura de buffer correcta\n";
    Stream = FSOUND_Stream_Open(Dato, FSOUND_NORMAL | FSOUND_MPEGACCURATE | FSOUND_LOADMEMORY,  0, lLongitud);
    if(!Stream){       
     CloseAudio();
     Log << "Error al crear el Stream\n";
     return false;
	}
    Log << "Creacion del Stream correcta\n";

	f_In.CloseFile();
    std::ifstream fin(In.c_str(), std::ios::binary);
    if(!fin){
	  cerr << "No existe el archivo: " << In.c_str() << endl;
      return false;
	}
    Log << "Se ha abierto el archivo: " << In.c_str() << endl;


    fin.read((char*)&startID, sizeof(startID)); 
    FormatChunk fc;
    fin.read((char*)&fc, sizeof(FormatChunk)); 
	Log << "Lectura de la estructura FormatChunk" << endl;
    if(strncmp(fc.chunkID, "fmt ", 4) != 0){        
		Log << "No es un archivo valido" << endl;
        return false;
    }

    Log << "ID del Chunk: " << fc.chunkID << " correcto" << endl;

    if(fc.wFormatTag!=1){        
		Log << "Error handle" << endl;
        return false;
    }
    
    Log << "Tag: " <<  fc.wFormatTag << " correcto" << endl;

	memset(&bc,0,sizeof(bc));
    bc.dwConfig = BE_CONFIG_MP3;
   
    if(fc.dwSamplesPerSec == 32000 || fc.dwSamplesPerSec == 44100 || fc.dwSamplesPerSec == 48000)
        bc.format.mp3.dwSampleRate = fc.dwSamplesPerSec;
    else
    {
        
		Log << "muestreo sin soportar" << endl;
        return false;
    }
	Log << "Muestreo correcto: " << fc.dwSamplesPerSec << endl;

    if(fc.wChannels == 1)
        bc.format.mp3.byMode = BE_MP3_MODE_MONO;
    else
        bc.format.mp3.byMode = BE_MP3_MODE_STEREO;

	Log << "Canales correctos " << bc.format.mp3.byMode << endl;
    bc.format.mp3.wBitrate = 192;
    bc.format.mp3.bCopyright = false;
    bc.format.mp3.bCRC = false;
    bc.format.mp3.bOriginal = false;
    bc.format.mp3.bPrivate = false;
    Log << "Bitrate: " << bc.format.mp3.wBitrate << endl;

    if(sizeof(FormatChunk) < int(8 + fc.chunkSize)){
        char c;
        for(int i=0; i< int(8 + fc.chunkSize - sizeof(FormatChunk)); i++)
            fin.get(c);
    }
   
    Chunk chunk;
    fin.read((char*)&chunk, sizeof(Chunk));
    Log << "Leyendo la estructura Chunk" << endl;

    while(strncmp(chunk.chunkID, "data", 4) != 0){
        char c;
        for(int i=0; i<chunk.chunkSize; i++)
            fin.get(c);
        fin.read((char*)&chunk,sizeof(Chunk));
    }
   
    DWORD dwNumberOfSamples;
    DWORD dwOutputBufferLength;
    HBE_STREAM hStream;


    if(state->beInitStream(&bc, &dwNumberOfSamples, &dwOutputBufferLength,&hStream) != BE_ERR_SUCCESSFUL){        
	 Log << "Error en la inicializacion del Stream, no se pudo comprimir" << endl;
     return false;
    }
	Log << "Se ha inicializado Stream correctamente" << endl;
    std::ofstream fout(Out.c_str(), ios::binary);

	if(fout.fail()){
     Log << "Error no se pudo crear el archivo: " << Out.c_str() << endl;
	 return false;
	}
	Log << "Se ha abierto el archivo de salida: " << Out.c_str() << " corretamente" << endl;

    char *Mp3Buffer = new char[dwOutputBufferLength];
    SHORT  *InputBuffer = new SHORT [dwNumberOfSamples];      
    
    if(!Mp3Buffer){
     Log  << "No hay memoria para el buffer Mp3" << endl;
	 return false;
	}
  
	Log << "Se ha asignado memoria para el Buffer Mp3 correctamente" << endl;

	if(!InputBuffer){
     Log << "No hay memoria para el buffer de entrada" << endl;
	 return false;
	}

	Log << "Se ha asignado memoria para el Buffer de entrada" << endl;

    int nSamplesPerformed=0;
    DWORD dwNumberOfSamplesEncoded;
	int LeerPorcentaje;

    FSOUND_Stream_Play(FSOUND_FREE, Stream);
	Log << "Reproduciendo el archivo de audio para la conversion\n";

    while(nSamplesPerformed < chunk.chunkSize){
        fin.read((char*)InputBuffer, dwNumberOfSamples * 2);
        nSamplesPerformed += dwNumberOfSamples * 2;				
        if(state->beEncodeChunk(hStream, dwNumberOfSamples, InputBuffer,(BYTE*)Mp3Buffer, &dwNumberOfSamplesEncoded) != BE_ERR_SUCCESSFUL){            
			Log << "Error no se pudo comprimir a .mp3" << endl;					
            return false;
		
        }
	    
        fout.write(Mp3Buffer, dwNumberOfSamplesEncoded);
		LeerPorcentaje = (int)(((float)FSOUND_Stream_GetTime(Stream) / (float)FSOUND_Stream_GetLengthMs(Stream)) * 100.0f);
		Win32.SetValueProgressBar(LeerPorcentaje);
        Win32.UpdateProgressBar();
		UpdateAudio();
		
    }

    state->beDeinitStream(hStream, (BYTE*)Mp3Buffer, &dwNumberOfSamplesEncoded);
    state->beCloseStream(hStream);
    Log << "Cerrando el Stream" << endl;

	Log << "Liberando memoria" << endl;
    delete Mp3Buffer;
    delete InputBuffer;
	delete state;
	delete Dato;

	Log << "Cerrando FMOD" << endl;
    FSOUND_Stream_Close(Stream);
	CloseAudio();

	Timer.Stop();
    Log << "Finalizando Timer" << endl;

    TiempoTranscurrido = Timer.GetElapsed();

    int iMin,iSec;
    GetElapsedTime(iMin,iSec);
    Log << "Tiempo Transcurrido: " << setw(2) << setfill('0') << iMin 
	    << ":" << setw(2) << setfill('0') << iSec << endl;

	Log << "Reiniciando ProgressBar" << endl;
	Win32.ReinitProgressBar();

	Log << "Cerrando archivos" << endl;
	Log.close();
	fout.close();
   	fin.close();
    return true;
}
