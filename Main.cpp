//Audio Converter 
//Version 0.1
//El programa permite realizar las siguientes 
//conversiones de audio:
//Wma a Wav 
//Cda a Wav 
//Mp2 a Wav 
//Mp3 a Wav 
//Wma a Mp3 
//Cda a Mp3 
//Mp2 a Mp3 
//Wav a Mp3 
//Programa compilado con Visual C++ 6.0
//http://www.programacioncpp.irandohosting.0lx.net
#pragma warning(disable:4786)
#include <windows.h>
#include "resource.h"
#include "CMain.h"
#include "hyperlink.h"
#include "HTMLHELP\htmlhelp.h"

using namespace std;

#pragma comment(lib, "winmm.lib")
#pragma comment(lib,"comctl32.lib")


string strIn;
string strOut;
CMain c;

void ClearString();
BOOL CALLBACK CdaToMp3Proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK Mp2ToMp3Proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK WmaToMp3Proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK WavToMp3Proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK CdaToWavProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK Mp3ToWavProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK Mp2ToWavProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK WmaToWavProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK MainProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK AboutProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrev, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;
	    
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, (DLGPROC)MainProc);
	while (GetMessage (&msg, NULL, 0, 0))
	{
		TranslateMessage (&msg);
		DispatchMessage (&msg);
	}
   
	return (msg.wParam);
}

BOOL CALLBACK AboutProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
 HYPCTRL hc;
 switch(msg)
 {
	case WM_INITDIALOG:	
		 c.Win32.SetDlgIcon(IDI_ICON1,hWnd);
		 
		 InitHypCtrl(&hc);
		 hc.ulStyle	 = ulHover;
		 hc.szLink	 = TEXT("http://www.programacioncpp.irandohosting.0lx.net");
		 hc.szTooltip = TEXT("Visitar mi pagina Web");
		 CreateHypCtrl(hWnd, &hc,18, 102, 0, 0);
		 break;
	case WM_CLOSE:
		 EndDialog(hWnd,0);			 
	     break;
    case WM_COMMAND:
		 switch(LOWORD(wParam))
		 {
		   case IDC_ABOUT_OK:
			    EndDialog(hWnd,0);	
			    break;
		 }
		 break;
	case WM_DESTROY:	
        return TRUE;
	default:
		return FALSE;
 }
 return DefWindowProc(hWnd, msg, wParam, lParam);
}
  
BOOL CALLBACK MainProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
 
 switch(msg)
 {
	case WM_INITDIALOG:	
		 c.Win32.SetDlgIcon(IDI_ICON1,hWnd);		
		 break;
	case WM_CLOSE:
		 EndDialog(hWnd,0);			 
	     break;
	
	case WM_COMMAND:
		 switch(LOWORD(wParam))
		 {
		  case IDC_ABOUT:				
			   DialogBox((HINSTANCE)GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_ABOUT), NULL, (DLGPROC)AboutProc);
			   break;
		  case IDC_CDATOMP3:
			   EnableWindow(GetDlgItem(hWnd,IDC_NEXT),TRUE); 
			   break;
		  case IDC_WAVTOMP3:
			   EnableWindow(GetDlgItem(hWnd,IDC_NEXT),TRUE);  
			   break;
		  case IDC_MP2TOMP3:
			   EnableWindow(GetDlgItem(hWnd,IDC_NEXT),TRUE);  
			   break;
		  case IDC_WMATOMP3:
			   EnableWindow(GetDlgItem(hWnd,IDC_NEXT),TRUE);  
			   break;
		  case IDC_MP3TOWAV:
			   EnableWindow(GetDlgItem(hWnd,IDC_NEXT),TRUE);   			   
               break;
		  case IDC_MP2TOWAV:
			   EnableWindow(GetDlgItem(hWnd,IDC_NEXT),TRUE);  
			   break;
		  case IDC_WMATOWAV:
			   EnableWindow(GetDlgItem(hWnd,IDC_NEXT),TRUE);  
			   break;
		  case IDC_CDATOWAV:
			   EnableWindow(GetDlgItem(hWnd,IDC_NEXT),TRUE);  
			   break;
		  case IDC_CANCEL:
			   EndDialog(hWnd,0);
			   break;		  
		  case IDC_NEXT:
			   if(IsDlgButtonChecked(hWnd, IDC_MP3TOWAV) == BST_CHECKED){
			    DialogBox((HINSTANCE)GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_CONVERTER), NULL, (DLGPROC)Mp3ToWavProc);
			    SendDlgItemMessage(hWnd, IDC_MP3TOWAV, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
			    EnableWindow(GetDlgItem(hWnd,IDC_NEXT),FALSE);
			   }
               if(IsDlgButtonChecked(hWnd, IDC_MP2TOWAV) == BST_CHECKED){
			    DialogBox((HINSTANCE)GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_CONVERTER), NULL, (DLGPROC)Mp2ToWavProc);
			    SendDlgItemMessage(hWnd, IDC_MP2TOWAV, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
			    EnableWindow(GetDlgItem(hWnd,IDC_NEXT),FALSE);
			   }

			   if(IsDlgButtonChecked(hWnd, IDC_WMATOWAV) == BST_CHECKED){
			    DialogBox((HINSTANCE)GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_CONVERTER), NULL, (DLGPROC)WmaToWavProc);
			    SendDlgItemMessage(hWnd, IDC_WMATOWAV, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
			    EnableWindow(GetDlgItem(hWnd,IDC_NEXT),FALSE);
			   }
			   if(IsDlgButtonChecked(hWnd, IDC_CDATOWAV) == BST_CHECKED){				
				DialogBox((HINSTANCE)GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_CONVERTER2), NULL, (DLGPROC)CdaToWavProc);
                SendDlgItemMessage(hWnd, IDC_CDATOWAV, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
			    EnableWindow(GetDlgItem(hWnd,IDC_NEXT),FALSE);
			   }
			   if(IsDlgButtonChecked(hWnd, IDC_WAVTOMP3) == BST_CHECKED){			
				DialogBox((HINSTANCE)GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_CONVERTER), NULL, (DLGPROC)WavToMp3Proc);
                SendDlgItemMessage(hWnd, IDC_WAVTOMP3, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
			    EnableWindow(GetDlgItem(hWnd,IDC_NEXT),FALSE);
			   }
			   if(IsDlgButtonChecked(hWnd, IDC_WMATOMP3) == BST_CHECKED){				
				DialogBox((HINSTANCE)GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_CONVERTER), NULL, (DLGPROC)WmaToMp3Proc);
                SendDlgItemMessage(hWnd, IDC_WMATOMP3, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
			    EnableWindow(GetDlgItem(hWnd,IDC_NEXT),FALSE);
			   }
			   if(IsDlgButtonChecked(hWnd, IDC_MP2TOMP3) == BST_CHECKED){				
				DialogBox((HINSTANCE)GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_CONVERTER), NULL, (DLGPROC)Mp2ToMp3Proc);
                SendDlgItemMessage(hWnd, IDC_MP2TOMP3, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
			    EnableWindow(GetDlgItem(hWnd,IDC_NEXT),FALSE);
			   }
			   if(IsDlgButtonChecked(hWnd, IDC_CDATOMP3) == BST_CHECKED){				
				DialogBox((HINSTANCE)GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_CONVERTER2), NULL, (DLGPROC)CdaToMp3Proc);
                SendDlgItemMessage(hWnd, IDC_CDATOMP3, BM_SETCHECK, (WPARAM)BST_UNCHECKED, 0);
			    EnableWindow(GetDlgItem(hWnd,IDC_NEXT),FALSE);
			   }
			   break;
		 }
		 break;
	case WM_DESTROY:
        PostQuitMessage(0);
		
        return TRUE;
	default:
		return FALSE;
 }
 return DefWindowProc(hWnd, msg, wParam, lParam);
}


void ClearString()
{
 strOut.erase(strOut.begin(),strOut.end());
 strIn.erase(strIn.begin(),strIn.end());
}

BOOL CALLBACK CdaToMp3Proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
 char cDrive;
 int iTrack;

 switch(msg)
 {
	case WM_INITDIALOG:	
		 c.Win32.SetDlgIcon(IDI_ICON3,hWnd);
		 SetWindowText(hWnd,"Converter Cda To Mp3");
		 c.InitProgressBar(hWnd,IDC_CONV2_PROGRESS1,COLOR_GRAY);		        
         c.SetDriveCDRom(hWnd);				 		
		 break;
	case WM_CLOSE:
		 EndDialog(hWnd,0);			 
	     break;
	case WM_COMMAND:
		 switch(LOWORD(wParam))
		 {
		   case IDC_CONV2_LOADCD:
			    c.Win32.SetHWNDCombo(hWnd,IDC_CONV2_DRIVE);  
			    c.IndexDevice=c.Win32.GetCurSel(); 
			    c.SetNumTracksCD(hWnd,c.IndexDevice);				
			    break;
		   case IDC_CONV2_CONVERTER:			   
			    c.Win32.SetHWNDCombo(hWnd,IDC_CONV2_DRIVE);  
			    c.IndexDevice=c.Win32.GetCurSel(); 
			    cDrive=c.cddevice[c.IndexDevice];

                c.Win32.SetHWNDCombo(hWnd,IDC_CONV2_TRACK);
                iTrack=c.Win32.GetCurSel() + 1;
				
				EnableWindow(GetDlgItem(hWnd,IDC_CONV2_CONVERTER),FALSE);
				EnableWindow(GetDlgItem(hWnd,IDC_CONV2_CANCEL),FALSE);
				c.ConverterCdaToMp3(hWnd,cDrive,iTrack,strOut);
			
				EnableWindow(GetDlgItem(hWnd,IDC_CONV2_DRIVE),TRUE);
				EnableWindow(GetDlgItem(hWnd,IDC_CONV2_LOADCD),TRUE);
				EnableWindow(GetDlgItem(hWnd,IDC_CONV2_CANCEL),TRUE);
				c.Clear();
				ClearString();
			    break;
		   case IDC_CONV2_HELP2:
			    HtmlHelp (hWnd,"Audio_Converter.chm", HH_DISPLAY_TOPIC ,NULL);
			    break;
		   case IDC_CONV2_CANCEL:
			    EndDialog(hWnd,0);
			    break;
		   case IDC_CONV2_SAVE:
			    if(c.Win32.Save(hWnd, "Archivos *.mp3\0*.mp3\0", "Guardar archivo",MP3)){
				 strOut="";
				 strOut+=c.Win32.szbufferOut;
				 SetDlgItemText(hWnd,IDC_CONV2_FILENAME,c.Win32.szbufferOut);					 	
				}				 			
				else{       
                  EnableWindow(GetDlgItem(hWnd,IDC_CONV2_SAVE),FALSE);
				}
			
		        break;
		 }
		 break;
	case WM_DESTROY:
	
        return TRUE;
	default:
		return FALSE;
 }
 return DefWindowProc(hWnd, msg, wParam, lParam);
}


BOOL CALLBACK Mp2ToMp3Proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

 string strAccion="";

 switch(msg)
 {
	case WM_INITDIALOG:	
		 c.Win32.SetDlgIcon(IDI_ICON2,hWnd);
		 SetWindowText(hWnd,"Converter Mp2 To Mp3");
		 c.InitProgressBar(hWnd,IDC_PROGRESS1,COLOR_GRAY);
		 break;
	case WM_CLOSE:
		 EndDialog(hWnd,0);			 
	     break;
	case WM_COMMAND:
		 switch(LOWORD(wParam))
		 {
		   case IDC_CONVERTER_HELP:
			    HtmlHelp (hWnd, "Audio_Converter.chm", HH_DISPLAY_TOPIC ,NULL);
			    break;
		   case IDC_OPEN:			    
			    if(c.Win32.Open(hWnd, "Archivos *.mp2\0*.mp2\0", "Abrir archivo")){
				 SetDlgItemText(hWnd,IDC_IN,c.Win32.szbufferIn);				
				 strIn="";
				 strIn+=c.Win32.szbufferIn; 
						
				 strOut="";						
                 strOut+=c.Win32.Get_CurrentDirectory(); 
				 strOut+="\\";
				 strOut+=c.ConverterFileName(c.Win32.szbufferIn,".mp3");

				 SetDlgItemText(hWnd,IDC_OUT,strOut.c_str());
				 EnableWindow(GetDlgItem(hWnd,IDC_CONVERTER),TRUE);
				 EnableWindow(GetDlgItem(hWnd,IDC_OPEN2),TRUE);
				}

				else{
                  EnableWindow(GetDlgItem(hWnd,IDC_OPEN2),FALSE);
                  EnableWindow(GetDlgItem(hWnd,IDC_CONVERTER),FALSE);
				}
				 
			    break;
		   case IDC_OPEN2:
			    if(c.Win32.Save(hWnd, "Archivos *.mp3\0*.mp3\0", "Guardar archivo",MP3)){
				 strOut="";
				 strOut+=c.Win32.szbufferOut;
				 SetDlgItemText(hWnd,IDC_OUT,c.Win32.szbufferOut);	
				 EnableWindow(GetDlgItem(hWnd,IDC_CONVERTER),TRUE);
				}
				else{
                  EnableWindow(GetDlgItem(hWnd,IDC_OPEN2),FALSE);
                  EnableWindow(GetDlgItem(hWnd,IDC_CONVERTER),FALSE);
				}
			    break;
		   case IDC_CONVERTER_CANCEL:
			    EndDialog(hWnd,0);
			    break;
		   case IDC_CONVERTER:
			    EnableWindow(GetDlgItem(hWnd,IDC_CONVERTER),FALSE);
				EnableWindow(GetDlgItem(hWnd,IDC_CONVERTER_CANCEL),FALSE);
			   	c.ConverterToMp3(hWnd,"Mp2 To Mp3",strIn,strOut);			  
				EnableWindow(GetDlgItem(hWnd,IDC_CONVERTER_CANCEL),TRUE);                  				 				 				
			    c.Clear();
				ClearString();
                SetDlgItemText(hWnd,IDC_OUT,"");
				SetDlgItemText(hWnd,IDC_IN,"");										
			    break;
		 }
		 break;
	case WM_DESTROY:
        return TRUE;
	default:
		return FALSE;
 }
 return DefWindowProc(hWnd, msg, wParam, lParam);
}

BOOL CALLBACK WmaToMp3Proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

 string strAccion="";

 switch(msg)
 {
	case WM_INITDIALOG:	
		 c.Win32.SetDlgIcon(IDI_ICON2,hWnd);
		 SetWindowText(hWnd,"Converter Wma To Mp3");
		 c.InitProgressBar(hWnd,IDC_PROGRESS1,COLOR_GRAY);
		 break;
	case WM_CLOSE:
		 EndDialog(hWnd,0);			 
	     break;
	case WM_COMMAND:
		 switch(LOWORD(wParam))
		 {
		   case IDC_CONVERTER_HELP:
			    HtmlHelp (hWnd, "Audio_Converter.chm", HH_DISPLAY_TOPIC ,NULL);
			    break;
		   case IDC_OPEN:
			    
			    if(c.Win32.Open(hWnd, "Archivos *.wma\0*.wma\0", "Abrir archivo")){
				 SetDlgItemText(hWnd,IDC_IN,c.Win32.szbufferIn);			
				 strIn="";
				 strIn+=c.Win32.szbufferIn; 
						
				 strOut="";						
                 strOut+=c.Win32.Get_CurrentDirectory();				
				 strOut+="\\";
				 strOut+=c.ConverterFileName(c.Win32.szbufferIn,".mp3");

				 SetDlgItemText(hWnd,IDC_OUT,strOut.c_str());
				 EnableWindow(GetDlgItem(hWnd,IDC_CONVERTER),TRUE);
				 EnableWindow(GetDlgItem(hWnd,IDC_OPEN2),TRUE);
				}
				else{
                  EnableWindow(GetDlgItem(hWnd,IDC_OPEN2),FALSE);
                  EnableWindow(GetDlgItem(hWnd,IDC_CONVERTER),FALSE);
				}				 
			    break;
		   case IDC_OPEN2:
			    if(c.Win32.Save(hWnd, "Archivos *.mp3\0*.mp3\0", "Guardar archivo",MP3)){
				 strOut="";
				 strOut+=c.Win32.szbufferOut;
				 SetDlgItemText(hWnd,IDC_OUT,c.Win32.szbufferOut);	
				 EnableWindow(GetDlgItem(hWnd,IDC_CONVERTER),TRUE);
				}
				else{
                  EnableWindow(GetDlgItem(hWnd,IDC_OPEN2),FALSE);
                  EnableWindow(GetDlgItem(hWnd,IDC_CONVERTER),FALSE);
				}
			    break;
		   case IDC_CONVERTER_CANCEL:
			    EndDialog(hWnd,0);
			    break;
		   case IDC_CONVERTER:
			    EnableWindow(GetDlgItem(hWnd,IDC_CONVERTER),FALSE);
				EnableWindow(GetDlgItem(hWnd,IDC_CONVERTER_CANCEL),FALSE);
			   	c.ConverterToMp3(hWnd,"Wma To Mp3",strIn,strOut);			  
				EnableWindow(GetDlgItem(hWnd,IDC_CONVERTER_CANCEL),TRUE);                  				 				 				
			    c.Clear();
				ClearString();
                SetDlgItemText(hWnd,IDC_OUT,"");
				SetDlgItemText(hWnd,IDC_IN,"");										
			    break;
		 }
		 break;
	case WM_DESTROY:
        return TRUE;
	default:
		return FALSE;
 }
 return DefWindowProc(hWnd, msg, wParam, lParam);
}


BOOL CALLBACK WavToMp3Proc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

 string strAccion="";

 switch(msg)
 {
	case WM_INITDIALOG:	
		 c.Win32.SetDlgIcon(IDI_ICON2,hWnd);
		 SetWindowText(hWnd,"Converter Wav To Mp3");
		 c.InitProgressBar(hWnd,IDC_PROGRESS1,COLOR_GRAY);			
		 break;
	case WM_CLOSE:
		 EndDialog(hWnd,0);			 
	     break;
	case WM_COMMAND:
		 switch(LOWORD(wParam))
		 {
		   case IDC_CONVERTER_HELP:
			    HtmlHelp (hWnd, "Audio_Converter.chm", HH_DISPLAY_TOPIC ,NULL);
			    break;
		   case IDC_OPEN:
			    
			    if(c.Win32.Open(hWnd, "Archivos *.wav\0*.wav\0", "Abrir archivo")){
				 SetDlgItemText(hWnd,IDC_IN,c.Win32.szbufferIn);				 
				 strIn="";
				 strIn+=c.Win32.szbufferIn; 
						
				 strOut="";						
                 strOut+=c.Win32.Get_CurrentDirectory();			
				 strOut+="\\";
				 strOut+=c.ConverterFileName(c.Win32.szbufferIn,".mp3");

				 SetDlgItemText(hWnd,IDC_OUT,strOut.c_str());
				 EnableWindow(GetDlgItem(hWnd,IDC_CONVERTER),TRUE);
				 EnableWindow(GetDlgItem(hWnd,IDC_OPEN2),TRUE);
				}

				else{
                  EnableWindow(GetDlgItem(hWnd,IDC_OPEN2),FALSE);
                  EnableWindow(GetDlgItem(hWnd,IDC_CONVERTER),FALSE);
				}				 
			    break;
		   case IDC_OPEN2:
			    if(c.Win32.Save(hWnd, "Archivos *.mp3\0*.mp3\0", "Guardar archivo",MP3)){
				 strOut="";
				 strOut+=c.Win32.szbufferOut;
				 SetDlgItemText(hWnd,IDC_OUT,c.Win32.szbufferOut);	
				 EnableWindow(GetDlgItem(hWnd,IDC_CONVERTER),TRUE);

				}
				else{
                  EnableWindow(GetDlgItem(hWnd,IDC_OPEN2),FALSE);
                  EnableWindow(GetDlgItem(hWnd,IDC_CONVERTER),FALSE);
				}
			    break;
		   case IDC_CONVERTER_CANCEL:
			    EndDialog(hWnd,0);
			    break;
		   case IDC_CONVERTER:				    
			    EnableWindow(GetDlgItem(hWnd,IDC_CONVERTER),FALSE);
				EnableWindow(GetDlgItem(hWnd,IDC_CONVERTER_CANCEL),FALSE);			  				
			    strAccion="Accion: Convirtiendo ";
			    strAccion+=c.ConverterFileName(c.Win32.szbufferOut,".mp3");
				SetDlgItemText(hWnd,IDC_STATIC_ACCION,strAccion.c_str());
				SetDlgItemText(hWnd,IDC_STATIC_TEXT,"Convirtiendo Archivo 1 de 1 a Mp3");			  
                if(!c.ConverterWavToMp3(strIn,strOut)){
				 c.Win32.ShowError(hWnd,"Error no se pudo realizar la conversion a .mp3","Wav To Mp3");
				 EndDialog(hWnd,1);
				}						
				c.ShowTime(hWnd,IDC_STATIC_TEXT);
                SetDlgItemText(hWnd,IDC_STATIC_ACCION,"Accion: Conversion Completa");				  
				EnableWindow(GetDlgItem(hWnd,IDC_CONVERTER_CANCEL),TRUE);                  				 				 				
			    c.Clear();
				ClearString();
                SetDlgItemText(hWnd,IDC_OUT,"");
				SetDlgItemText(hWnd,IDC_IN,"");										
			    break;
		 }
		 break;
	case WM_DESTROY:	
        return TRUE;		
	default:
		return FALSE;
 }
 return DefWindowProc(hWnd, msg, wParam, lParam);
}


BOOL CALLBACK CdaToWavProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
 char cDrive;
 int iTrack;
 string strAccion="";
 switch(msg)
 {
	case WM_INITDIALOG:	
		 c.Win32.SetDlgIcon(IDI_ICON3,hWnd);
		 SetWindowText(hWnd,"Converter Cda To Wav");
		 c.InitProgressBar(hWnd,IDC_CONV2_PROGRESS1,COLOR_GRAY);		        
         c.SetDriveCDRom(hWnd);				 		 
		 break;
	case WM_CLOSE:
		 EndDialog(hWnd,0);			 
	     break;
	case WM_COMMAND:
		 switch(LOWORD(wParam))
		 {
		   case IDC_CONV2_HELP2:
			    HtmlHelp (hWnd, "Audio_Converter.chm", HH_DISPLAY_TOPIC ,NULL);
			    break;
		   case IDC_CONV2_LOADCD:
			    c.Win32.SetHWNDCombo(hWnd,IDC_CONV2_DRIVE);  
			    c.IndexDevice=c.Win32.GetCurSel(); 
			    c.SetNumTracksCD(hWnd,c.IndexDevice);				
			    break;
		   case IDC_CONV2_CONVERTER:
			    strAccion="Accion: Convirtiendo ";
			    strAccion+=c.ConverterFileName(c.Win32.szbufferOut,".wav");

			    c.Win32.SetHWNDCombo(hWnd,IDC_CONV2_DRIVE);  
			    c.IndexDevice=c.Win32.GetCurSel(); 
			    cDrive=c.cddevice[c.IndexDevice];

                c.Win32.SetHWNDCombo(hWnd,IDC_CONV2_TRACK);
                iTrack=c.Win32.GetCurSel() + 1;
				
				EnableWindow(GetDlgItem(hWnd,IDC_CONV2_CONVERTER),FALSE);
				EnableWindow(GetDlgItem(hWnd,IDC_CONV2_CANCEL),FALSE);
				SetDlgItemText(hWnd,IDC_CONV2_STATIC,"Convirtiendo Archivo 1 de 1 a Wav");
				SetDlgItemText(hWnd,IDC_CONV2_ACTION,strAccion.c_str());
				if(!c.ConverterCdaToWav(cDrive,iTrack,strOut)){
                 c.Win32.ShowError(hWnd,"Error no se pudo realizar la conversion","Cda To Wav");
				 EndDialog(hWnd,1);
				}
			    EnableWindow(GetDlgItem(hWnd,IDC_CONV2_DRIVE),TRUE);
				EnableWindow(GetDlgItem(hWnd,IDC_CONV2_LOADCD),TRUE);
				EnableWindow(GetDlgItem(hWnd,IDC_CONV2_CANCEL),TRUE);
				SetDlgItemText(hWnd,IDC_CONV2_FILENAME,"");
				SetDlgItemText(hWnd,IDC_CONV2_ACTION,"Accion: Conversion Completa");
				c.ShowTime(hWnd,IDC_CONV2_STATIC);            
				c.Clear();
				ClearString();
			    break;
		   case IDC_CONV2_CANCEL:
			    EndDialog(hWnd,0);
			    break;
		   case IDC_CONV2_SAVE:
			    if(c.Win32.Save(hWnd, "Archivos *.wav\0*.wav\0", "Guardar archivo",WAV)){
				 strOut="";
				 strOut+=c.Win32.szbufferOut;
				 SetDlgItemText(hWnd,IDC_CONV2_FILENAME,c.Win32.szbufferOut);					 	
				}				 			
				else{       
                  EnableWindow(GetDlgItem(hWnd,IDC_CONV2_SAVE),FALSE);
				}		
		        break;
		 }
		 break;
	case WM_DESTROY:
        return TRUE;
	default:
		return FALSE;
 }
 return DefWindowProc(hWnd, msg, wParam, lParam);
}


BOOL CALLBACK Mp3ToWavProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

 string strAccion="";
 int nBufferLength=MAX_PATH;

 switch(msg)
 {
	case WM_INITDIALOG:	
		 c.Win32.SetDlgIcon(IDI_ICON2,hWnd);
		 SetWindowText(hWnd,"Converter Mp3 To Wav");
		 c.InitProgressBar(hWnd,IDC_PROGRESS1,COLOR_GRAY);
		 break;
	case WM_CLOSE:
		 EndDialog(hWnd,0);			 
	     break;
	case WM_COMMAND:
		 switch(LOWORD(wParam))
		 {
		   case IDC_CONVERTER_HELP:
			    HtmlHelp (hWnd, "Audio_Converter.chm", HH_DISPLAY_TOPIC ,NULL);
			    break;
		   case IDC_OPEN:
			    
			    if(c.Win32.Open(hWnd, "Archivos *.mp3\0*.mp3\0", "Abrir archivo")){
				 SetDlgItemText(hWnd,IDC_IN,c.Win32.szbufferIn);				 
				 strIn="";
				 strIn+=c.Win32.szbufferIn; 
						
				 strOut="";						
                 strOut+=c.Win32.Get_CurrentDirectory();			
				 strOut+="\\";
				 strOut+=c.ConverterFileName(c.Win32.szbufferIn,".wav");

				 SetDlgItemText(hWnd,IDC_OUT,strOut.c_str());
				 EnableWindow(GetDlgItem(hWnd,IDC_CONVERTER),TRUE);
				 EnableWindow(GetDlgItem(hWnd,IDC_OPEN2),TRUE);
				}

				else{
                  EnableWindow(GetDlgItem(hWnd,IDC_OPEN2),FALSE);
                  EnableWindow(GetDlgItem(hWnd,IDC_CONVERTER),FALSE);
				}				 
			    break;
		   case IDC_OPEN2:
			    if(c.Win32.Save(hWnd, "Archivos *.wav\0*.wav\0", "Guardar archivo",WAV)){
				 strOut="";
				 strOut+=c.Win32.szbufferOut;
				 SetDlgItemText(hWnd,IDC_OUT,c.Win32.szbufferOut);	
				 EnableWindow(GetDlgItem(hWnd,IDC_CONVERTER),TRUE);
				}
				else{
                  EnableWindow(GetDlgItem(hWnd,IDC_OPEN2),FALSE);
                  EnableWindow(GetDlgItem(hWnd,IDC_CONVERTER),FALSE);
				}
			    break;
		   case IDC_CONVERTER_CANCEL:
			    EndDialog(hWnd,0);
			    break;
		   case IDC_CONVERTER:
			    EnableWindow(GetDlgItem(hWnd,IDC_CONVERTER),FALSE);
				EnableWindow(GetDlgItem(hWnd,IDC_CONVERTER_CANCEL),FALSE);
			    strAccion="Accion: Convirtiendo ";
			    strAccion+=c.ConverterFileName(c.Win32.szbufferOut,".wav");
				SetDlgItemText(hWnd,IDC_STATIC_ACCION,strAccion.c_str());
				SetDlgItemText(hWnd,IDC_STATIC_TEXT,"Convirtiendo Archivo 1 de 1 a Wav");			  
                if(!c.ConverterToWav(strIn,strOut)){
				 c.Win32.ShowError(hWnd,"Error no se pudo realizar la conversion","Mp3 To Wav");
				 EndDialog(hWnd,1);
				}								 
                SetDlgItemText(hWnd,IDC_STATIC_ACCION,"Accion: Conversion Completa");				  
				c.ShowTime(hWnd,IDC_STATIC_TEXT);
				EnableWindow(GetDlgItem(hWnd,IDC_CONVERTER_CANCEL),TRUE);                  				 				 				
			    c.Clear();
				ClearString();
                SetDlgItemText(hWnd,IDC_OUT,"");
				SetDlgItemText(hWnd,IDC_IN,"");										
			    break;
		 }
		 break;
	case WM_DESTROY:
        return TRUE;
	default:
		return FALSE;
 }
 return DefWindowProc(hWnd, msg, wParam, lParam);
}

BOOL CALLBACK Mp2ToWavProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

 string strAccion="";
 int nBufferLength=MAX_PATH;

 switch(msg)
 {
	case WM_INITDIALOG:	
		 c.Win32.SetDlgIcon(IDI_ICON2,hWnd);
		 SetWindowText(hWnd,"Converter Mp2 To Wav");
		 c.InitProgressBar(hWnd,IDC_PROGRESS1,COLOR_GRAY);
		 break;
	case WM_CLOSE:
		 EndDialog(hWnd,0);			 
	     break;
	case WM_COMMAND:
		 switch(LOWORD(wParam))
		 {
		   case IDC_CONVERTER_HELP:
			    HtmlHelp (hWnd, "Audio_Converter.chm", HH_DISPLAY_TOPIC ,NULL);
			    break;
		   case IDC_OPEN:			    
			    if(c.Win32.Open(hWnd, "Archivos *.mp2\0*.mp2\0", "Abrir archivo")){
				 SetDlgItemText(hWnd,IDC_IN,c.Win32.szbufferIn);			
				 strIn="";
				 strIn+=c.Win32.szbufferIn; 
						
				 strOut="";						
                 strOut+=c.Win32.Get_CurrentDirectory();			
				 strOut+="\\";
				 strOut+=c.ConverterFileName(c.Win32.szbufferIn,".wav");

				 SetDlgItemText(hWnd,IDC_OUT,strOut.c_str());
				 EnableWindow(GetDlgItem(hWnd,IDC_CONVERTER),TRUE);
				 EnableWindow(GetDlgItem(hWnd,IDC_OPEN2),TRUE);
				}

				else{
                  EnableWindow(GetDlgItem(hWnd,IDC_OPEN2),FALSE);
                  EnableWindow(GetDlgItem(hWnd,IDC_CONVERTER),FALSE);
				}
			    break;
		   case IDC_OPEN2:
			    if(c.Win32.Save(hWnd, "Archivos *.wav\0*.wav\0", "Guardar archivo",WAV)){
				 strOut="";
				 strOut+=c.Win32.szbufferOut;
				 SetDlgItemText(hWnd,IDC_OUT,c.Win32.szbufferOut);	
				 EnableWindow(GetDlgItem(hWnd,IDC_CONVERTER),TRUE);
				}
				else{
                  EnableWindow(GetDlgItem(hWnd,IDC_OPEN2),FALSE);
                  EnableWindow(GetDlgItem(hWnd,IDC_CONVERTER),FALSE);
				}
			    break;
		   case IDC_CONVERTER_CANCEL:
			    EndDialog(hWnd,0);
			    break;
		   case IDC_CONVERTER:
			    EnableWindow(GetDlgItem(hWnd,IDC_CONVERTER),FALSE);
				EnableWindow(GetDlgItem(hWnd,IDC_CONVERTER_CANCEL),FALSE);
			    strAccion="Accion: Convirtiendo ";
			    strAccion+=c.ConverterFileName(c.Win32.szbufferOut,".wav");
				SetDlgItemText(hWnd,IDC_STATIC_ACCION,strAccion.c_str());
				SetDlgItemText(hWnd,IDC_STATIC_TEXT,"Convirtiendo Archivo 1 de 1 a Wav");			  
                if(!c.ConverterToWav(strIn,strOut)){
				 c.Win32.ShowError(hWnd,"Error no se pudo realizar la conversion","Mp3 To Wav");
				 EndDialog(hWnd,1);
				}								 
                SetDlgItemText(hWnd,IDC_STATIC_ACCION,"Accion: Conversion Completa");
				c.ShowTime(hWnd,IDC_STATIC_TEXT);
				EnableWindow(GetDlgItem(hWnd,IDC_CONVERTER_CANCEL),TRUE);                  				 				 				
			    c.Clear();
				ClearString();
                SetDlgItemText(hWnd,IDC_OUT,"");
				SetDlgItemText(hWnd,IDC_IN,"");		
				SetDlgItemText(hWnd,IDC_STATIC_TEXT,"");				
			    break;
		 }
		 break;
	case WM_DESTROY:
        return TRUE;
	default:
		return FALSE;
 }
 return DefWindowProc(hWnd, msg, wParam, lParam);
}


BOOL CALLBACK WmaToWavProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

 string strAccion="";
 int nBufferLength=MAX_PATH;

 switch(msg)
 {
	case WM_INITDIALOG:	
		 c.Win32.SetDlgIcon(IDI_ICON2,hWnd);
		 SetWindowText(hWnd,"Converter Wma To Wav");
		 c.InitProgressBar(hWnd,IDC_PROGRESS1,COLOR_GRAY);
		 break;
	case WM_CLOSE:
		 EndDialog(hWnd,0);			 
	     break;
	case WM_COMMAND:
		 switch(LOWORD(wParam))
		 {
		   case IDC_CONVERTER_HELP:
			    HtmlHelp (hWnd, "Audio_Converter.chm", HH_DISPLAY_TOPIC ,NULL);
			    break;
		   case IDC_OPEN:			    
			    if(c.Win32.Open(hWnd, "Archivos *.wma\0*.wma\0", "Abrir archivo")){
				 SetDlgItemText(hWnd,IDC_IN,c.Win32.szbufferIn);				
				 strIn="";
				 strIn+=c.Win32.szbufferIn; 
						
				 strOut="";						
                 strOut+=c.Win32.Get_CurrentDirectory();		
				 strOut+="\\";
				 strOut+=c.ConverterFileName(c.Win32.szbufferIn,".wav");

				 SetDlgItemText(hWnd,IDC_OUT,strOut.c_str());
				 EnableWindow(GetDlgItem(hWnd,IDC_CONVERTER),TRUE);
				 EnableWindow(GetDlgItem(hWnd,IDC_OPEN2),TRUE);
				}

				else{
                  EnableWindow(GetDlgItem(hWnd,IDC_OPEN2),FALSE);
                  EnableWindow(GetDlgItem(hWnd,IDC_CONVERTER),FALSE);
				}
			    break;
		   case IDC_OPEN2:
			    if(c.Win32.Save(hWnd, "Archivos *.wav\0*.wav\0", "Guardar archivo",WAV)){
				 strOut="";
				 strOut+=c.Win32.szbufferOut;
				 SetDlgItemText(hWnd,IDC_OUT,c.Win32.szbufferOut);	
				 EnableWindow(GetDlgItem(hWnd,IDC_CONVERTER),TRUE);
				}
				else{
                  EnableWindow(GetDlgItem(hWnd,IDC_OPEN2),FALSE);
                  EnableWindow(GetDlgItem(hWnd,IDC_CONVERTER),FALSE);
				}
			    break;
		   case IDC_CONVERTER_CANCEL:
			    EndDialog(hWnd,0);
			    break;
		   case IDC_CONVERTER:
			    EnableWindow(GetDlgItem(hWnd,IDC_CONVERTER),FALSE);
				EnableWindow(GetDlgItem(hWnd,IDC_CONVERTER_CANCEL),FALSE);
			    strAccion="Accion: Convirtiendo ";
			    strAccion+=c.ConverterFileName(c.Win32.szbufferOut,".wav");
				SetDlgItemText(hWnd,IDC_STATIC_ACCION,strAccion.c_str());
				SetDlgItemText(hWnd,IDC_STATIC_TEXT,"Convirtiendo Archivo 1 de 1 a Wav");			  
                if(!c.ConverterToWav(strIn,strOut)){
				 c.Win32.ShowError(hWnd,"Error no se pudo realizar la conversion","Mp3 To Wav");
				 EndDialog(hWnd,1);
				}								 
                SetDlgItemText(hWnd,IDC_STATIC_ACCION,"Accion: Conversion Completa");	
				c.ShowTime(hWnd,IDC_STATIC_TEXT);
				EnableWindow(GetDlgItem(hWnd,IDC_CONVERTER_CANCEL),TRUE);                  				 				 				
			    c.Clear();
				ClearString();
                SetDlgItemText(hWnd,IDC_OUT,"");
				SetDlgItemText(hWnd,IDC_IN,"");		
				SetDlgItemText(hWnd,IDC_STATIC_TEXT,"");				
			    break;
		 }
		 break;
	case WM_DESTROY:
        return TRUE;
	default:
		return FALSE;
 }
 return DefWindowProc(hWnd, msg, wParam, lParam);
}


