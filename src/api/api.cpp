// ----------------------------------------------------------------------------
// Basic APIs of HT's Mod Loader.
// ----------------------------------------------------------------------------
#include "utils/globals.h"
#include "htinternal.h"

HTMLAPIATTR void HTMLAPI HTGetGameStatus(
  HTGameStatus *status
) {
  if (status)
    *status = gGameStatus;
}

HTMLAPIATTR void HTMLAPI HTGetGameExeFolder(
  char *result,
  u64 maxLen
) {
  if (!result)
    return;
  strcpy_s(result, maxLen, gPathGameExe);
}

HTMLAPIATTR void HTMLAPI HTGetModFolder(
  char *result,
  u64 maxLen
) {
  if (!result)
    return;
  strcpy_s(result, maxLen, gPathMods);
}

HTMLAPIATTR HMODULE HTMLAPI HTGetModuleHandle(
  const char *module
) {
  HMODULE result = nullptr;

  if (!module) {
    // Get caller's module handle.
    void *callerAddr = __builtin_return_address(0);
    if (!GetModuleHandleExA(
      GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS
      | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
      (LPCSTR)callerAddr,
      &result
    ))
      return nullptr;

    if (!checkHandleType((HTHandle)result, HTHandleType_Mod))
      return nullptr;

    return result;
  }

  // Get module handle from name.
  auto it = gModDataLoader.find(module);
  if (it == gModDataLoader.end())
    return nullptr;

  return it->second.runtime->handle;
}
