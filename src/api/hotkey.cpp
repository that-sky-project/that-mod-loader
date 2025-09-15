// ----------------------------------------------------------------------------
// Hotkey APIs of HT's Mod Loader.
// ----------------------------------------------------------------------------
#include <string>
#include <unordered_map>
#include "imgui.h"
#include "includes/htkeycodes.h"
#include "includes/htmodloader.h"
#include "moddata.h"

HTMLAPIATTR HTHandle HTMLAPI HTHotkeyRegister(
  HMODULE hModule,
  const char *name,
  HTKeyCode defaultCode
) {
  std::lock_guard<std::mutex> lock(gModDataLock);
  ModRuntime *rt;
  HTHandle result;

  if (!hModule || !name)
    return HT_INVALID_HANDLE;

  rt = getModRuntime(hModule);
  if (!rt)
    return HT_INVALID_HANDLE;
  
  auto it = rt->keyBinds.find(name);
  if (it != rt->keyBinds.end()) {
    // Do not override the prewritten key code.
    it->second.keyName = name;
    it->second.displayName = name;
    it->second.defaultKey = defaultCode;

    result = (HTHandle)&it->second;
  } else {
    ModKeyBind *kb = &rt->keyBinds[name];

    kb->defaultKey = defaultCode;
    kb->key = defaultCode;
    kb->keyName = name;
    kb->displayName = name;

    result = (HTHandle)kb;
  }

  return result;
}

HTMLAPIATTR u32 HTMLAPI HTHotkeyPressed(
  HTHandle hKey
) {
  ModKeyBind *kb;

  if (!hKey)
    return 0;
  kb = (ModKeyBind *)hKey;

  return (u32)ImGui::IsKeyDown((ImGuiKey)kb->key);
}

HTMLAPIATTR HTStatus HTMLAPI HTHotkeyListen(
  HTHandle hKey,
  PFN_HTHotkeyCallback callback
) {

}

HTMLAPIATTR HTStatus HTMLAPI HTHotkeyUnlisten(
  HTHandle hKey,
  PFN_HTHotkeyCallback callback
) {

}
