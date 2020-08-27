#ifndef WIN_API_FAL_H
#define WIN_API_FAL_H

#include "IFal.h"

class WinApi_Fal: public IFal
{
  virtual FileHandle Open(const char *fileName, FileMode fileMode);
  virtual void Close(FileHandle fileHandle);
  virtual uint32_t Read(FileHandle file, void* buffer, uint32_t bytesToRead);
  virtual uint32_t Write(FileHandle file, void* buffer, uint32_t bytesToWrite);
};

#endif
