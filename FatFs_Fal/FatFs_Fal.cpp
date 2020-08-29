#include "FatFs_Fal.h"
#include "DebugLog.h"

void FatFsFal::Init()
{
  FRESULT initResult = f_mount(&SDFatFS,"",1);
  DBG_ASSERT(initResult == FR_OK);
}

FileHandle FatFsFal::Open(const char *fileName, FileMode fileMode)
{
  // Right now, only one file can be opened at the same time.
  // This function always returns the same value.
  FRESULT fresult;
  if(fileMode==FM_READ)
  {
    fresult = f_open(&fileStruct,fileName,FA_READ);
  }
  else
  {
    fresult = f_open(&fileStruct,fileName,FA_WRITE);
  }
  DBG_ASSERT(fresult == FR_OK);
  return 0;
}

void FatFsFal::Close(FileHandle fileHandle)
{
  f_close(&fileStruct);
}

uint32_t FatFsFal::Read(FileHandle file, void* buffer, uint32_t bytesToRead)
{
  uint32_t bytesRead;
  FRESULT fresult = f_read(&fileStruct, buffer, bytesToRead, (UINT*)&bytesRead);
  DBG_ASSERT(fresult == FR_OK);
  return bytesRead;
}

uint32_t FatFsFal::Write(FileHandle file, void* buffer, uint32_t bytesToWrite)
{
  uint32_t bytesWritten;
  FRESULT fresult = f_write(&fileStruct, buffer, bytesToWrite, (UINT*)&bytesWritten);
  DBG_ASSERT(fresult == FR_OK);
  return bytesWritten;
}
