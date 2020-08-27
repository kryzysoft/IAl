#ifndef IFAL_H
#define IFAL_H

#include "stdint.h"

typedef int32_t FileHandle;

enum FileMode
{
  FM_READ = 1,
  FM_WRITE = 2
};

class IFal
{
public:
  virtual FileHandle Open(const char *fileName, FileMode fileMode) = 0;
  virtual void Close(FileHandle fileHandle) = 0;
  virtual uint32_t Read(FileHandle file, void* buffer, uint32_t bytesToRead) = 0;
  virtual uint32_t Write(FileHandle file, void* buffer, uint32_t bytesToWrite) = 0;
  virtual ~IFal()
  {
  }
};

#endif // IFAL_H
