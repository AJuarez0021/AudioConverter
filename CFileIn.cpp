#include "CFileIn.h"
#include <string.h>
#include <assert.h>

CFileIn::CFileIn()
{
 lFileSize=0;
}

CFileIn::~CFileIn()
{
 
}

void CFileIn::CloseFile()
{
 if(f)
  fclose(f);
}
bool CFileIn::IsOpen()
{
 return f!=0;
}

long CFileIn::GetPosition()
{
 return ftell(f);
}

bool CFileIn::Seek(long lFinalPosition,bool bMovement)
{
 if (!IsOpen())
  return false;

return fseek(f, lFinalPosition, bMovement ? SEEK_CUR : SEEK_SET) == 0;
}

void CFileIn::openFile(const char *FileName)
{
 Name=new char[strlen(FileName)+1];
 assert(Name!=0);

 strcpy(Name,FileName);

 f=fopen(FileName,"rb");
 if(!IsOpen())
  return;

 fseek(f, 0L, SEEK_END);
 lFileSize = ftell(f);
 fseek(f, 0L, SEEK_SET);
}

char* CFileIn::GetFileName()
{
 return Name;
}

long CFileIn::GetfileSize()
{
 return lFileSize;
}

int CFileIn::Read(void *Buffer,int iSize)
{
 if(!IsOpen())
   return 0;

 return fread(Buffer,iSize,1,f);
}