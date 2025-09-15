// ----------------------------------------------------------------------------
// Inline hook APIs of HT's Mod Loader.
// ----------------------------------------------------------------------------
#include <vector>
#include <unordered_map>
#include <mutex>
#include "MinHook.h"

#include "includes/htmodloader.h"

static std::mutex gMutex;

HTMLAPIATTR HTStatus HTMLAPI HTInstallHook(
  void *fn,
  void *detour,
  void **origin
) {
  std::lock_guard<std::mutex> lock(gMutex);
  if (MH_CreateHook(fn, detour, origin) == MH_OK)
    return HT_SUCCESS;
  return HT_FAIL;
}

HTMLAPIATTR HTStatus HTMLAPI HTEnableHook(
  void *fn
) {
  std::lock_guard<std::mutex> lock(gMutex);
  if (MH_EnableHook(fn) == MH_OK)
    return HT_SUCCESS;
  return HT_FAIL;
}

HTMLAPIATTR HTStatus HTMLAPI HTDisableHook(
  void *fn
) {
  std::lock_guard<std::mutex> lock(gMutex);
  if (MH_DisableHook(fn) == MH_OK)
    return HT_SUCCESS;
  return HT_FAIL;
}

HTMLAPIATTR HTStatus HTMLAPI HTInstallHookEx(
  HTHookFunction *func
) {
  return HTInstallHook(func->fn, func->detour, &func->origin);
}

HTMLAPIATTR HTStatus HTMLAPI HTEnableHookEx(
  HTHookFunction *func
) {
  return HTEnableHook(func->fn);
}

HTMLAPIATTR HTStatus HTMLAPI HTDisableHookEx(
  HTHookFunction *func
) {
  return HTDisableHook(func->fn);
}
