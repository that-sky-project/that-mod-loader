#include <stdio.h>
#include <string>
#include "cJSON.h"
#include "includes/htmodloader.h"
#include "htinternal.h"

HTStatus HTLoadOptionsFromFile(
  const wchar_t *path
) {

}

HTStatus HTLoadOptionsFromMem() {

}

static void getKeyBinds(const cJSON *json) {
  cJSON *keyBinds = cJSON_GetObjectItemCaseSensitive(json, "key_binds")
    , *item;
  cJSON_ArrayForEach(item, keyBinds) {
    if (!cJSON_IsString(item))
      continue;
    const char *key = item->string;
    cJSON *value = cJSON_GetObjectItemCaseSensitive(json, key);
    if (cJSON_IsString(value))
      LOGI("key: %s, value: %s\n", key, value->valuestring);
  }
}

HTStatus HTWriteOptionsToFile() {

}
