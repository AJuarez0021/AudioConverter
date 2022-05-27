#pragma warning(disable:4786)
#include "CMain.h"

using namespace std;

void CMain::SetNumTracksCD(HWND hWnd,int Index)
{ 
  string strTracks="";
  char strBuffer[3];
  Win32.SetHWNDCombo(hWnd,IDC_CONV2_TRACK);
  InitAudio();
  
  int iNumTrack=FSOUND_CD_GetNumTracks(cddevice[Index]); 
  if(iNumTrack==0){
   MessageBox(hWnd,"Error: No hay Cd Audio\nPor favor inserte un Cd.","Cda To Wav",MB_OK|MB_ICONINFORMATION);
   EnableWindow(GetDlgItem(hWnd,IDC_CONV2_DRIVE),TRUE);
   EnableWindow(GetDlgItem(hWnd,IDC_CONV2_LOADCD),TRUE);
   EnableWindow(GetDlgItem(hWnd,IDC_CONV2_CONVERTER),FALSE);
  }
  else{
	EnableWindow(GetDlgItem(hWnd,IDC_CONV2_DRIVE),FALSE);
	EnableWindow(GetDlgItem(hWnd,IDC_CONV2_LOADCD),FALSE);
	EnableWindow(GetDlgItem(hWnd,IDC_CONV2_CONVERTER),TRUE);
  }
   for(int i=1;i<=iNumTrack;i++){
    sprintf(strBuffer,"%d",i);
    strTracks="";
    strTracks+=strBuffer;
    Win32.Add(strTracks);
   }
  Win32.Select("1");
  CloseAudio();
}

void CMain::SetDriveCDRom(HWND hWnd)
{
    int count;
    char str[256];
    cddevice.clear();
	Win32.SetHWNDCombo(hWnd,IDC_CONV2_DRIVE);  
    for (count=2;count < 26;count++){
        sprintf(str, "%c:\\", (char)('A' + count));
        if (GetDriveType(str) == DRIVE_CDROM){                
			cddevice.push_back(str[0]);        
        }
    }
	string strDrive="";

    for(count=0;count < cddevice.size(); count++){
	 strDrive="";
	 strDrive+=cddevice[count];
	 strDrive+=":";
	 Win32.Add(strDrive);
	}
	strDrive="";
	strDrive+=cddevice[0];
	strDrive+=":";
	
	Win32.Select(strDrive);
	
}

void CMain::Clear()
{
 register int i=0;
 for(i=0;i<MAX_PATH;i++){
  Win32.szbufferIn[i]=0;
  Win32.szbufferOut[i]=0;
 }
 v.clear();
 
}

 

void CMain::GetToken(string strCad,char *strSeps)
{
 char *token;
 string strTokens;
 token = strtok( (char*)strCad.c_str(), strSeps );

 while( token != NULL ){         
  strTokens=token;
  v.push_back(strTokens);
  token = strtok( NULL, strSeps );
 }    
}


string CMain::GetFileName(char *Path)
{
 string strPath=Path;
 int iMax;
 GetToken(strPath,"\\");
 iMax=(int)v.size()-1;
 string strName=v[iMax];
 return strName;
}

string CMain::ConverterFileName(char *strPath,char *strExt)
{
 string strFileNameOut="";
 string strResult="";

 strFileNameOut=GetFileName(strPath);
 char *token = strtok( (char*)strFileNameOut.c_str(), ".");
 strResult=token;
 strResult+=strExt;  
 return strResult;
}

void CMain::ShowTime(HWND hWnd,int IDDlg)
{
 char strText[40];
 int iMin,iSec;
 GetElapsedTime(iMin,iSec);
 sprintf(strText,"Tiempo Transcurrido: %02d:%02d",iMin,iSec);
 SetDlgItemText(hWnd,IDDlg,strText);
}

void CMain::ConverterToMp3(HWND hWnd,string strCaption,string In,string Out)
{
 string strPathOut="";
 string strAccion="";

 strPathOut=Win32.GetPathExe();
 strPathOut+="\\temp.wav";

 strAccion="Accion: Convirtiendo ";
 strAccion+=ConverterFileName(Win32.szbufferOut,".mp3");

 SetDlgItemText(hWnd,IDC_STATIC_ACCION,"Accion: Convirtiendo temp.wav");
 SetDlgItemText(hWnd,IDC_STATIC_TEXT,"Convirtiendo Archivo 1 de 1 a Wav");
 if(!ConverterToWav(In,strPathOut)){
  Win32.ShowError(hWnd,"Error al realizar la conversion a .wav",strCaption);
  EndDialog(hWnd,1);
 }
 ShowTime(hWnd,IDC_STATIC_TEXT);
 SetDlgItemText(hWnd,IDC_STATIC_ACCION,"Accion: Conversion Completa");
 
 SetDlgItemText(hWnd,IDC_STATIC_ACCION,strAccion.c_str());
 SetDlgItemText(hWnd,IDC_STATIC_TEXT,"Convirtiendo Archivo 1 de 1 a Mp3");
 if(!ConverterWavToMp3(strPathOut,Out)){
  Win32.ShowError(hWnd,"Error al realizar la conversion a .mp3",strCaption);
  EndDialog(hWnd,1);
 }
 ShowTime(hWnd,IDC_STATIC_TEXT);
 SetDlgItemText(hWnd,IDC_STATIC_ACCION,"Accion: Conversion Completa");
 DeleteFile(strPathOut.c_str());
}

void CMain::ConverterCdaToMp3(HWND hWnd,char cDrive,int iTrack,string Out)
{
 string strPathOut="";
 string strAccion="";

 strPathOut=Win32.GetPathExe();
 strPathOut+="\\temp.wav";
 strAccion="Accion: Convirtiendo ";
 strAccion+=ConverterFileName(Win32.szbufferOut,".mp3");

 SetDlgItemText(hWnd,IDC_CONV2_STATIC,"Convirtiendo Archivo 1 de 1 a Wav");
 SetDlgItemText(hWnd,IDC_CONV2_ACTION,"Accion: Convirtiendo temp.wav");
 if(!ConverterCdaToWav(cDrive,iTrack,strPathOut)){
    Win32.ShowError(hWnd,"Error no se pudo realizar la conversion de Cd a .wav","Cda To Mp3");
	EndDialog(hWnd,1);
 }
 
 SetDlgItemText(hWnd,IDC_CONV2_ACTION,"Accion: Conversion Completa");
 ShowTime(hWnd,IDC_CONV2_STATIC);

 SetDlgItemText(hWnd,IDC_CONV2_STATIC,"Convirtiendo Archivo 1 de 1 a Mp3");
 SetDlgItemText(hWnd,IDC_CONV2_ACTION,strAccion.c_str());

 if(!ConverterWavToMp3(strPathOut,Out)){
  Win32.ShowError(hWnd,"Error no se pudo realizar la conversion de Cd a .mp3","Cda To Mp3");
  EndDialog(hWnd,1);
 }
 SetDlgItemText(hWnd,IDC_CONV2_FILENAME,"");
 SetDlgItemText(hWnd,IDC_CONV2_ACTION,"Accion: Conversion Completa");
 ShowTime(hWnd,IDC_CONV2_STATIC);
 DeleteFile(strPathOut.c_str());
}