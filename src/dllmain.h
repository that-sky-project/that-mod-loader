#include <windows.h>
#include <ntstatus.h>
#include <stdio.h>
#include <unordered_map>
#include "MinHook.h"

#include "aliases.h"
#include "globals.h"
#include "logger.h"
#include "loader.h"
#include "proxy/winhttp-proxy.h"

static const char HTML_CONFIG_JSON_BASIC[] = "{\"file_format_version\":\"1.0.0\",\"layer\":{\"name\":\"VK_LAYER_HT_MOD_LOADER\",\"type\":\"GLOBAL\",\"api_version\":\"1.3\",\"library_path\":\".\\winhttp.dll\",\"implementation_version\":\"1\",\"description\":\"Layer for HT's Mod Loader\",\"functions\":{\"vkGetInstanceProcAddr\":\"HT_vkGetInstanceProcAddr\",\"vkGetDeviceProcAddr\":\"HT_vkGetDeviceProcAddr\"},\"disable_environment\":{\"DISABLE_HT_MOD_LOADER\":\"1\"}}}";
