#include <windows.h>
#include <string>
#include <map>
#include <vector>
#include <mutex>
#include "aliases.h"
#include "htmodloader.h"

struct ModPaths {
  // The folder contains manifest.json.
  std::wstring folder;
  // "main" in manifest.json. The name of the main executable file of the mod.
  std::wstring dll;
  // The path to manifest.json.
  std::wstring json;
};

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

// Mod runtime data. This struct is associated with mod handle.
struct ModRuntime {
  HMODULE handle;
  ModManifest *manifest;
  // Shared functions.
  std::map<std::string, PFN_HTVoidFunction> functions;
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
