#define HTTexts_DefaultLayerConfig "{\n"\
  "\"file_format_version\": \"1.0.0\",\n"\
  "\"layer\": {\n"\
    "\"name\": \"VK_LAYER_HT_MOD_LOADER\",\n"\
    "\"type\": \"GLOBAL\",\n"\
    "\"api_version\": \"1.3\",\n"\
    "\"library_path\":\".\\winhttp.dll\",\n"\
    "\"implementation_version\": \"1\",\n"\
    "\"description\": \"Layer for HT's Mod Loader\",\n"\
    "\"functions\":{\n"\
      "\"vkGetInstanceProcAddr\": \"HT_vkGetInstanceProcAddr\",\n"\
      "\"vkGetDeviceProcAddr\": \"HT_vkGetDeviceProcAddr\"\n"\
    "},\n"\
    "\"disable_environment\":{\n"\
      "\"DISABLE_HT_MOD_LOADER\":\"1\"\n"\
    "}\n"\
  "}\n"\
"}"
