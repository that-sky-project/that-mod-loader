#include <windows.h>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <mutex>
#include "includes/aliases.h"
#include "includes/htmodloader.h"

#ifndef __MODDATA_H__
#define __MODDATA_H__

// Paths to related files of the mod.
struct ModPaths {
  // The folder contains manifest.json.
  std::wstring folder;
  // "main" in manifest.json. The name of the main executable file of the mod.
  std::wstring dll;
  // The path to manifest.json.
  std::wstring json;
};

// Identifier data of the mod.
struct ModMeta {
  // This name is used to identify mods and add dependencies, and must be
  // unique for every mod.
  std::string packageName;
  // The version number of the mod.
  u32 version[3];
};

struct ModRuntime;
// Mod manifest data. This struct is associated with package name.
struct ModManifest {
  // Mod identification data.
  ModMeta meta;
  // Paths to the related files of the mod.
  ModPaths paths;
  // This name is used to display mod basic information.
  std::string modName;
  // The description of the mod.
  std::string description;
  // The author of the mod.
  std::string author;
  // Game edition the mod supports.
  u08 gameEditionFlags;
  // Dependencies of the mod.
  std::vector<ModMeta> dependencies;
  // Mod runtime data.
  ModRuntime *runtime;
};

// HTML expected functions.
struct ModInternalFunctions {
  PFN_HTModOnInit pfn_HTModOnInit;
  PFN_HTModOnEnable pfn_HTModOnEnable;
  PFN_HTModRenderGui pfn_HTModRenderGui;
};

// Registered key bind of a mod.
struct ModKeyBind {
  // Key internal identifier. May be prewritten.
  std::string keyName;
  // Key code to be actually detected. May be prewritten.
  HTKeyCode key;
  // Display name of the key. In current version, it's simply equal to keyName.
  std::string displayName;
  // Default key code.
  HTKeyCode defaultKey;
  // Callbacks to be called when the state of this key is changed.
  std::set<PFN_HTHotkeyCallback> listeners;
};

// Mod runtime data. This struct is associated with mod handle.
struct ModRuntime {
  HMODULE handle;
  ModManifest *manifest;
  ModInternalFunctions loaderFunc;
  // Shared functions.
  std::map<std::string, PFN_HTVoidFunction> functions;
  // Registered hotkeys.
  std::map<std::string, ModKeyBind> keyBinds; 
};

extern std::map<std::string, ModManifest> gModDataLoader;
extern std::map<HMODULE, ModRuntime> gModDataRuntime;
// We use a single global lock to protect all of the above structs simply.
extern std::mutex gModDataLock;

static ModRuntime *getModRuntime(HMODULE hModule) {
  auto it = gModDataRuntime.find(hModule);
  if (it == gModDataRuntime.end())
    return nullptr;
  return &it->second;
}

#endif
