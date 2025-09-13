#include "moddata.h"

std::map<std::string, ModManifest> gModDataLoader;
std::map<HMODULE, ModRuntime> gModDataRuntime;
std::mutex gModDataLock;
