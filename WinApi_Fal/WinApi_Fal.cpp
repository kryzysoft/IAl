#include "WinApi_Fal.h"
#include "stdio.h"

FileHandle WinApi_Fal::Open(const char *fileName, FileMode fileMode)
{
  char readWriteMode[2];
  readWriteMode[1] = 0;


  FILE *file;

  if(fileMode == FM_READ)
  {
    readWriteMode[0] = 'r';
  }
  if(fileMode == FM_WRITE)
  {
    readWriteMode[0] = 'w';
  }
  file = fopen(fileName, readWriteMode);

  return (FileHandle)file;
}

void WinApi_Fal::Close(FileHandle fileHandle)
{
  fclose((FILE*)fileHandle);
}

uint32_t WinApi_Fal::Read(FileHandle file, void* buffer, uint32_t bytesToRead)
{
  return fread(buffer,1,bytesToRead,(FILE*)file);
}

uint32_t WinApi_Fal::Write(FileHandle file, void* buffer, uint32_t bytesToWrite)
{
  return fwrite(buffer,1,bytesToWrite,(FILE*)file);
}
