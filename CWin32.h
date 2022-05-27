#include <windows.h>
#include <commctrl.h>
#include <string>

const COLOR_BLUE         = RGB(0,128,192);
const COLOR_GRAY         = RGB(128,128,128);
const COLOR_RED         = RGB(177,20,64);
const COLOR_YELLOW     = RGB(241,230,14);
const COLOR_GREEN        = RGB(89,172,172);
const COLOR_BLACK        = RGB(0,0,0);
const COLOR_WHITE       = RGB(255, 255, 255);

#define GET_KEYDOWN(Key) ((int)((GetAsyncKeyState(Key) & (1 << 15)) >> 15))
#define WAV 1
#define MP3 2

class CWin32{
private:
 HWND      g_hwndProgress; 
 HWND      g_hwndCombo;
 MEMORYSTATUS mem;	
 OPENFILENAME  ofn; 
 char lpDirectory[MAX_PATH];
 char szFileName[MAX_PATH];
 char PC[256];
public:
 CWin32(){ g_hwndProgress=NULL; 
           g_hwndCombo=NULL; 
 }

 char szbufferIn[MAX_PATH];
 char szbufferOut[MAX_PATH];
 //Funciones para el control ProgressBar
 void CreateProgressBar();
 void SetValueProgressBar(int Value);
 void ResizeProgressBar(int x,int y,int iWidth,int iHeight);
 void ReinitProgressBar();
 void UpdateProgressBar();
 void SetRangeProgressBar(int init,int end);
 void SetStepProgressBar(int iStep);
 void SetBarColorProgressBar(COLORREF Color);
 //Funciones de utilidad
 void ShowError(HWND hWnd,std::string strMessage,std::string strCaption);
 void SetDlgIcon(UINT nIDIcon,HWND hWnd);
 void SetDlgCursor(UINT nIDCursor,HWND hWnd);
 void Fuente(HWND hWnd, LONG lTamano, BOOL Negrita, BOOL Cursiva, BOOL Subrayado, LPSTR lpszNombreFuente);
 long GetFreeMemory();
 long GetTotalMemory();
 long GetLoadMemory();
 std::string Get_ComputerName();
 std::string GetPathExe();
 std::string Get_CurrentDirectory();
 BOOL Open(HWND hWnd, LPSTR lpszFiltro, LPSTR lpszTitulo);
 BOOL Save(HWND hWnd, LPSTR lpszFiltro, LPSTR lpszTitulo,UINT uFlag);

 void SetWNDProgressBar(HWND hWndProgress,UINT uID){ 
	  g_hwndProgress = GetDlgItem(hWndProgress,uID); 
 }
 void SetHWNDCombo(HWND hCombo,UINT uID){ 
	  g_hwndCombo= GetDlgItem(hCombo,uID); 
 }

 //Funciones para el control ComboBox
 int GetCurSel();
 void Select(std::string strText);
 void Add(std::string strText);
};