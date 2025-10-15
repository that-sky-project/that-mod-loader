// ----------------------------------------------------------------------------
// Basic APIs of HT's Mod Loader.
// ----------------------------------------------------------------------------
#include "imgui.h"
#include "includes/htmodloader.h"
#include "htinternal.h"

static thread_local HTError gLastError = HTError_Success;

HTMLAPIATTR void HTMLAPI HTSetLastError(
  HTError dwError
) {
  gLastError = dwError;
}

HTMLAPIATTR HTError HTMLAPI HTGetLastError() {
  return gLastError;
}

HTMLAPIATTR void HTMLAPI HTGetLoaderVersion(
  UINT32 *result
) {
  *result = HTML_VERSION;
}

HTMLAPIATTR void HTMLAPI HTGetLoaderVersionName(
  LPSTR result,
  UINT32 max
) {
  if (!max)
    return;
  if (max == 1)
    *result = '\0';
  strncpy(result, HTML_VERSION_NAME, max - 1);
}

HTMLAPIATTR void HTMLAPI HTGetGameStatus(
  HTGameStatus *status
) {
  if (status)
    *status = gGameStatus;
}

HTMLAPIATTR void HTMLAPI HTGetGameExeFolder(
  LPSTR result,
  UINT64 maxLen
) {
  if (!result)
    return;
  strcpy_s(result, maxLen, gPathGameExe);
}

HTMLAPIATTR void HTMLAPI HTGetModFolder(
  LPSTR result,
  UINT64 maxLen
) {
  if (!result)
    return;
  strcpy_s(result, maxLen, gPathMods);
}

HTMLAPIATTR HMODULE HTMLAPI HTGetModuleHandle(
  LPCSTR module
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

    if (!HTiCheckHandleType((HTHandle)result, HTHandleType_Mod))
      return nullptr;

    return result;
  }

  // Get module handle from name.
  auto it = gModDataLoader.find(module);
  if (it == gModDataLoader.end())
    return nullptr;

  return it->second.runtime->handle;
}

HTMLAPIATTR HTHandle HTMLAPI HTGetModManifest(
  HMODULE hModule
) {
  std::lock_guard<std::mutex> lock(gModDataLock);

  if (!hModule)
    return HT_INVALID_HANDLE;

  ModRuntime *rt = HTiGetModRuntime(hModule);
  if (!rt)
    return HT_INVALID_HANDLE;

  HTiRegisterHandle((HTHandle)rt->manifest, HTHandleType_Manifest);

  return (HTHandle)rt->manifest;
}

HTMLAPIATTR u32 HTMLAPI HTGetModInfoFrom(
  HTHandle hManifest,
  HTModInfoFields info,
  LPVOID out,
  UINT32 maxLen
) {
  if (!HTiCheckHandleType(hManifest, HTHandleType_Manifest))
    return HTiErrAndRet(HTError_InvalidHandle, 0);

  ModManifest *manifest = (ModManifest *)hManifest;
  u64 size;
  void *result;

  if (maxLen && !out)
    return HTiErrAndRet(HTError_InvalidParam, 0);

  switch (info) {
    case HTModInfoFields_ModName:
      size = manifest->modName.length() + 1;
      result = (void *)manifest->modName.c_str();
      break;
    case HTModInfoFields_PackageName:
      size = manifest->meta.packageName.length() + 1;
      result = (void *)manifest->meta.packageName.c_str();
      break;
    case HTModInfoFields_Folder:
      size = (manifest->paths.folder.length() + 1) * sizeof(wchar_t);
      result = (void *)manifest->meta.packageName.c_str();
      break;
    default:
      return HTiErrAndRet(HTError_InvalidParam, 0);
  }

  if (!maxLen)
    return HTiErrAndRet(HTError_Success, size);
  if (size > maxLen)
    return HTiErrAndRet(HTError_InsufficientBuffer, 0);

  memcpy(out, result, size);

  return HTiErrAndRet(HTError_Success, size);
}
