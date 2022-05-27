#ifndef __CCONVERTER_H
#define __CCONVERTER_H

#include "CSystemAudio.h"
#include "CTimer.h"
#include "CWin32.h"
#include <string>
#include "CFileIn.h"
#include "CFileOut.h"


enum Flags{ OK=1,CANCEL};

class CConverter : public CSystemAudio{
  private:
  CFileIn  f_In;
  CTimer Timer;
  
  int TiempoTranscurrido;
  public:
   CWin32 Win32;
   bool ConverterToWav(std::string In,std::string Out);
   bool ConverterWavToMp3(std::string In,std::string Out);
   bool ConverterCdaToWav(char cDrive,int iTrackNum,std::string Out);
   void InitProgressBar(HWND hWnd,UINT uID,COLORREF Color);
   void GetElapsedTime(int &iMin,int &iSec);
};
#endif