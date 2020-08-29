#ifndef FAT_FS_FAL_H
#define FAT_FS_FAL_H

#include "IFal.h"
#include "FatFs.h"

class FatFsFal: public IFal
{
private:
  FIL fileStruct;
public:
  void Init();
  virtual FileHandle Open(const char *fileName, FileMode fileMode);
  virtual void Close(FileHandle fileHandle);
  virtual uint32_t Read(FileHandle file, void* buffer, uint32_t bytesToRead);
  virtual uint32_t Write(FileHandle file, void* buffer, uint32_t bytesToWrite);
};

#endif
