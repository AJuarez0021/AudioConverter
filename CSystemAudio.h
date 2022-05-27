#ifndef __CSYSTEM_AUDIO_H
#define __CSYSTEM_AUDIO_H

 #include "FMOD/fmod.h"

 enum iFlags{DSOUND=1,
            WINMM, 
			ASIO, 
			NOSOUND,
			NOSOUND_NONREALTIME  
           };
 class CSystemAudio{
  public:
   bool  InitAudio(int iFrequency=44100,int iChannels=32,unsigned int iFlags=0);
   void  CloseAudio();
   void  SetBufferSize(int iSize);
   void  SetOutPut(int iFlag);
   void  UpdateAudio() { FSOUND_Update();             }
   float GetCPUUsage(){ return FSOUND_GetCPUUsage(); }
 };
#endif