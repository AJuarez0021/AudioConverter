#include <windows.h>
#include "resource.h"
#include "CConverter.h"
#include <string>
#include <vector>

class CMain : public CConverter{
 private:
  std::vector<std::string> v;
 
  void GetToken(std::string strCad,char *strSeps);
  std::string GetFileName(char *Path);
 public:
  int IndexDevice;
  std::vector<char> cddevice;
  void SetNumTracksCD(HWND hWnd,int Index);
  void SetDriveCDRom(HWND hWnd);
  void Clear();
  std::string ConverterFileName(char *strPath,char *strExt);
  void ShowTime(HWND hWnd,int IDDlg);
  void ConverterToMp3(HWND hWnd,std::string strCaption,std::string In,std::string Out);
  void ConverterCdaToMp3(HWND hWnd,char cDrive,int iTrack,std::string Out);
};
