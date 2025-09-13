// ----------------------------------------------------------------------------
// Mod communication APIs of HT's Mod Loader.
// ----------------------------------------------------------------------------
#include <string>
#include <unordered_map>
#include <mutex>
#include <unordered_set>
#include "moddata.h"
#include "htmodloader.h"

static std::mutex gMutex;
static std::unordered_map<std::string, std::unordered_set<PFN_HTEventCallback>> gEventCallbacks;

HTMLAPI PFN_HTVoidFunction HTGetProcAddr(
  HMODULE hModule,
  const char *name
) {
  std::lock_guard<std::mutex> lock(gModDataLock);

  if (!hModule || !name)
    return nullptr;

  ModRuntime *rt = getModRuntime(hModule);
  if (!rt)
    return nullptr;

  auto it = rt->functions.find(name);
  if (it == rt->functions.end())
    return nullptr;

  return it->second;
}

HTMLAPI HTHandle HTGetModManifest(
  HMODULE hModule
) {
  std::lock_guard<std::mutex> lock(gModDataLock);

  if (!hModule)
    return nullptr;

  ModRuntime *rt = getModRuntime(hModule);
  if (!rt)
    return nullptr;

  return (HTHandle)rt->manifest;
}

HTMLAPI HTStatus HTCommRegFunction(
  HMODULE hModule,
  const char *name,
  PFN_HTVoidFunction func
) {
  std::lock_guard<std::mutex> lock(gModDataLock);

  if (!hModule || !name || !func)
    return HT_FAIL;

  ModRuntime *rt = getModRuntime(hModule);
  if (!rt)
    return HT_FAIL;

  rt->functions[name] = func;

  return HT_SUCCESS;
}

HTMLAPI HTStatus HTCommOnEvent(
  const char *name,
  PFN_HTEventCallback callback
) {
  std::lock_guard<std::mutex> lock(gMutex);

  if (!name || !callback)
    return HT_FAIL;
  
  gEventCallbacks[name].insert(callback);

  return HT_SUCCESS;
}

HTMLAPI HTStatus HTCommOffEvent(
  const char *name,
  PFN_HTEventCallback callback
) {
  std::lock_guard<std::mutex> lock(gMutex);

  if (!name || !callback)
    return HT_FAIL;
  
  auto it = gEventCallbacks.find(name);
  if (it != gEventCallbacks.end()) {
    it->second.erase(callback);
  
    if (it->second.empty())
      gEventCallbacks.erase(it);
  }

  return HT_SUCCESS;
}

HTMLAPI HTStatus HTCommEmitEvent(
  const char *name,
  void *data
) {
  std::lock_guard<std::mutex> lock(gMutex);

  if (!name)
    return HT_FAIL;
  
  auto it = gEventCallbacks.find(name);
  if (it == gEventCallbacks.end())
    return HT_FAIL;

  auto set = &it->second;
  for (auto it = set->begin(); it != set->end(); it++) {
    PFN_HTEventCallback cb = (*it);
    if (cb)
      cb(data);
  }

  return HT_SUCCESS;
}
