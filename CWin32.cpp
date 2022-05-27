#include "CWin32.h"
using namespace std;

string CWin32::GetPathExe()
{
 register int i; 
 GetModuleFileName(GetModuleHandle(NULL),szFileName,MAX_PATH);
 for(i=strlen(szFileName); szFileName[i]!='\\';i--);
 szFileName[++i]=0;
 string strResult="";
 strResult+=szFileName;
 return strResult;
}

void CWin32::CreateProgressBar()
{
 INITCOMMONCONTROLSEX iccx;
 iccx.dwSize=sizeof(INITCOMMONCONTROLSEX);
 iccx.dwICC=ICC_PROGRESS_CLASS;    
 InitCommonControlsEx(&iccx);
}

void CWin32::ShowError(HWND hWnd,string strMsg,string strCaption)
{
 MessageBox(hWnd,strMsg.c_str(),strCaption.c_str(),MB_ICONERROR);	
}

void CWin32::SetValueProgressBar(int iValue)
{
 SendMessage(g_hwndProgress,PBM_SETPOS,iValue,0);
}

void CWin32::ResizeProgressBar(int x,int y,int iWidth,int iHeight)
{
 MoveWindow(g_hwndProgress,x,y,iWidth,iHeight,TRUE);
}

void CWin32::ReinitProgressBar()
{
 SendMessage(g_hwndProgress, PBM_SETPOS, (WPARAM) 0 , 0);
}

void CWin32::UpdateProgressBar()
{
 UpdateWindow(g_hwndProgress);
}

void CWin32::SetRangeProgressBar(int init,int end)
{

 SendMessage(g_hwndProgress, PBM_SETRANGE, 0, MAKELPARAM(init, end));
}

void CWin32::SetStepProgressBar(int iStep)
{
 SendMessage(g_hwndProgress, PBM_SETSTEP, (WPARAM) iStep, 0);         
}


void CWin32::SetBarColorProgressBar(COLORREF Color)
{
				 
 SendMessage(g_hwndProgress, PBM_SETBARCOLOR, 0, (LPARAM) Color);
}



void CWin32::SetDlgIcon(UINT nIDIcon,HWND hWnd)
{
 HICON hIcon;
 HINSTANCE hInst;
 hInst = (HINSTANCE) GetWindowLong( hWnd, GWL_HINSTANCE );
 hIcon=LoadIcon(hInst,MAKEINTRESOURCE(nIDIcon));
 SendMessage(hWnd,WM_SETICON,ICON_BIG, (LPARAM) hIcon);
 SendMessage(hWnd,WM_SETICON,ICON_SMALL,(LPARAM)hIcon);
}



int CWin32::GetCurSel()
{
 return (int)SendMessage(g_hwndCombo,CB_GETCURSEL,0,0);
}

void CWin32::Add(string strText)
{
 SendMessage(g_hwndCombo,CB_ADDSTRING,NULL,(LPARAM)strText.c_str());
}

void CWin32::Select(string strText)
{
 SendMessage(g_hwndCombo,CB_SELECTSTRING,(WPARAM)-1,(LPARAM)strText.c_str());
}

void CWin32::Fuente(HWND hWnd, LONG lTamano, BOOL Negrita, BOOL Cursiva, BOOL Subrayado, LPSTR lpszNombreFuente)
{
	LOGFONT fuente;
	HFONT hFuente;
	HDC hdc;
	ZeroMemory(&fuente, sizeof(LOGFONT));
	ZeroMemory(&hFuente, sizeof(HFONT));
	fuente.lfHeight = lTamano;
	fuente.lfWidth = 0;
	fuente.lfEscapement = 1;
	fuente.lfOrientation = 1;
	if (Negrita){
		fuente.lfWeight = FW_BOLD;
	}else{
		fuente.lfWeight = FW_NORMAL;
	}
	fuente.lfItalic = Cursiva;
	fuente.lfUnderline = Subrayado;
	fuente.lfStrikeOut = false;
	fuente.lfCharSet = DEFAULT_CHARSET;
	fuente.lfOutPrecision = OUT_DEFAULT_PRECIS;
	fuente.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	fuente.lfQuality = DEFAULT_QUALITY;
	fuente.lfPitchAndFamily = DEFAULT_PITCH|FF_ROMAN;
	lstrcpy(fuente.lfFaceName, lpszNombreFuente);
	hdc = GetDC(hWnd); 
	hFuente = CreateFontIndirect(&fuente);
    SendMessage(hWnd, WM_SETFONT, (WPARAM) hFuente, MAKELPARAM(true, 0));
}

long CWin32::GetTotalMemory()
{
 GlobalMemoryStatus(&mem);
 return mem.dwTotalPhys / 1024;
}

long CWin32::GetFreeMemory()
{
 GlobalMemoryStatus(&mem);
 return mem.dwAvailPhys / 1024;
}

long CWin32::GetLoadMemory()
{
 GlobalMemoryStatus(&mem);
 return mem.dwMemoryLoad;
}

string CWin32::Get_ComputerName()
{
 string strPC="";
 DWORD Size=256;
 GetComputerName(PC, &Size);
 strPC+=PC;
 return strPC;
}

BOOL CWin32::Open(HWND hWnd, LPSTR lpszFiltro, LPSTR lpszTitulo)
{
	
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFilter = (lpszFiltro);
	ofn.lpstrFile = szbufferIn;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFileTitle = (lpszTitulo);
	ofn.Flags = OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_HIDEREADONLY;
	return GetOpenFileName (&ofn);
}


BOOL CWin32::Save(HWND hWnd, LPSTR lpszFiltro, LPSTR lpszTitulo,UINT uFlag)
{
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hWnd;
	ofn.lpstrFilter = (lpszFiltro);
	ofn.lpstrFile = szbufferOut;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFileTitle = (lpszTitulo);
	if(uFlag==WAV)
	 ofn.lpstrDefExt = "wav";
	else
	 ofn.lpstrDefExt = "mp3";

	ofn.Flags = OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_HIDEREADONLY;
	return GetSaveFileName (&ofn);	
}

string CWin32::Get_CurrentDirectory()
{
 string strDirectory="";
 GetCurrentDirectory(MAX_PATH,lpDirectory);
 strDirectory+=lpDirectory;
 return strDirectory;
}