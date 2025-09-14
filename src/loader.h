#ifndef __LOADER_H__
#define __LOADER_H__

#include "includes/htmodloader.h"

extern "C" {
  HTStatus HTLoadMods();
  void HTLoadSingleMod();
  void HTUnloadSingleMod();
  HTStatus HTInjectDll(const wchar_t *path);
  HTStatus HTRejectDll();
}

#endif
