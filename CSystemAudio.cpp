#include "CSystemAudio.h"

void CSystemAudio::CloseAudio()
{
 FSOUND_Close();
}

bool CSystemAudio::InitAudio(int iFrequency,int iChannels,unsigned int iFlags)
{
 if(FSOUND_GetVersion() < FMOD_VERSION)
   return false;

 if(!FSOUND_Init(iFrequency,iChannels,iFlags))
   return false;

 return true;
}

void CSystemAudio::SetBufferSize(int iSize)
{
 FSOUND_Stream_SetBufferSize(iSize);
}

void CSystemAudio::SetOutPut(int iSalida)
{
 switch (iSalida){    
    case 1 :  FSOUND_SetOutput(FSOUND_OUTPUT_DSOUND);
              break;
    case 2 :  FSOUND_SetOutput(FSOUND_OUTPUT_WINMM);
              break;
    case 3 :  FSOUND_SetOutput(FSOUND_OUTPUT_ASIO);
              break;   
    case 4 :  FSOUND_SetOutput(FSOUND_OUTPUT_NOSOUND);
		      break;
	case 5:   FSOUND_SetOutput(FSOUND_OUTPUT_NOSOUND_NONREALTIME);
		      break;
 }
}

