#ifndef __CFILE_IN_H
#define __CFILE_IN_H

#include <stdio.h>

class CFileIn{
 private:
   long lFileSize;
   char *Name;
   FILE *f;
 public:
   CFileIn();
   ~CFileIn();
   int Read(void *Buffer,int iSize);
   bool IsOpen();
   char *GetFileName();
   long GetPosition();
   long GetfileSize();
   bool Seek(long lFinalPosition,bool bMovement=false);
   void openFile(const char *FileName);
   void CloseFile();
};

#endif