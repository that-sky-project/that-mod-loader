// ----------------------------------------------------------------------------
// Basic APIs of HT's Mod Loader.
// ----------------------------------------------------------------------------
#include "utils/globals.h"

HTMLAPIATTR void HTMLAPI HTGetGameStatus(HTGameStatus *status) {
  if (status)
    *status = gGameStatus;
}

HTMLAPIATTR void HTMLAPI HTGetGameExeFolder(char *result, u64 maxLen) {
  if (!result)
    return;
  strcpy_s(result, maxLen, gPathGameExe);
}

HTMLAPIATTR void HTMLAPI HTGetModFolder(char *result, u64 maxLen) {
  if (!result)
    return;
  strcpy_s(result, maxLen, gPathMods);
}
