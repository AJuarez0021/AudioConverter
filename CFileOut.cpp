#include "CFileOut.h"
#include <string.h>
#include <assert.h>

CFileOut::CFileOut()
{
 lFileSize=0;
}

CFileOut::~CFileOut()
{
 
}

void CFileOut::CloseFile()
{
 if(f)
  fclose(f);
}
bool CFileOut::IsOpen()
{
 return f!=0;
}

int CFileOut::Write(const void *Buffer,int iSize)
{
 if (!IsOpen())
		return 0;

 return fwrite(Buffer, 1, iSize, f);
}

long CFileOut::GetPosition()
{
 return ftell(f);
}

bool CFileOut::Seek(long lFinalPosition,bool bMovement)
{
 if (!IsOpen())
  return false;

return fseek(f, lFinalPosition, bMovement ? SEEK_CUR : SEEK_SET) == 0;
}

void CFileOut::openFile(const char *FileName,bool bAppend)
{
 Name=new char[strlen(FileName)+1];
 assert(Name!=0);

 strcpy(Name,FileName);

 f=fopen(FileName,bAppend ? "ab" : "wb");
 if(!IsOpen())
  return;

 fseek(f, 0L, SEEK_END);
 lFileSize = ftell(f);
 fseek(f, 0L, SEEK_SET);
}

char* CFileOut::GetFileName()
{
 return Name;
}

long CFileOut::GetfileSize()
{
 return lFileSize;
}

