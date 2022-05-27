#ifndef __CFILE_OUT_H
#define __CFILE_OUT_H

#include <stdio.h>

class CFileOut{
 private:
	 long lFileSize;
	 char *Name;
	 FILE *f;
 public:
	 CFileOut();
     ~CFileOut();
	 int Write(const void *Buffer,int iSize);
	 bool IsOpen();
	 char *GetFileName();
	 long GetPosition();
	 long GetfileSize();
	 bool Seek(long lFinalPosition,bool bMovement=false);
	 void openFile(const char *FileName,bool bAppend);
	 void CloseFile();

};

#endif