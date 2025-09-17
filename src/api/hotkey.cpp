// ----------------------------------------------------------------------------
// Hotkey APIs of HT's Mod Loader.
// ----------------------------------------------------------------------------
#include <string>
#include "imgui.h"
#include "includes/htkeycodes.h"
#include "includes/htmodloader.h"
#include "htinternal.h"

// Cooldown timer in frames.
#define HOTKEY_MODIFY_COOLDOWN 5

static std::map<HTKeyCode, std::set<ModKeyBind *>> gHotkeyCallbacks;
static i32 gKeyModifyCooldown = 0;

HTMLAPIATTR HTHandle HTMLAPI HTHotkeyRegister(
  HMODULE hModule,
  const char *name,
  HTKeyCode defaultCode
) {
  std::lock_guard<std::mutex> lock(gModDataLock);
  ModRuntime *rt;
  ModKeyBind *result;

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
    it->second.isRegistered = 1;

    result = &it->second;
  } else {
    ModKeyBind *kb = &rt->keyBinds[name];

    kb->defaultKey = defaultCode;
    kb->key = defaultCode;
    kb->keyName = name;
    kb->displayName = name;
    kb->isRegistered = 1;

    result = kb;
  }

  gHotkeyCallbacks[result->key].insert(result);
  registerHandle(result, HTHandleType_Hotkey);

  return result;
}

HTMLAPIATTR HTStatus HTMLAPI HTHotkeyBind(
  HTHandle hKey,
  HTKeyCode key
) {
  ModKeyBind *kb;

  if (!hKey)
    return HT_FAIL;

  kb = (ModKeyBind *)hKey;

  if (kb->key == key)
    return HT_SUCCESS;

  {
    std::lock_guard<std::mutex> lock(gModDataLock);
    gHotkeyCallbacks[kb->key].erase(kb);
    kb->key = key;
    gHotkeyCallbacks[key].insert(kb);
  }

  return HT_SUCCESS;
}

HTMLAPIATTR u32 HTMLAPI HTHotkeyPressed(
  HTHandle hKey
) {
  ModKeyBind *kb;

  if (!hKey)
    return 0;
  if (gKeyModifyCooldown)
    return 0;
  kb = (ModKeyBind *)hKey;

  return (u32)ImGui::IsKeyDown((ImGuiKey)kb->key);
}

HTMLAPIATTR HTStatus HTMLAPI HTHotkeyListen(
  HTHandle hKey,
  PFN_HTHotkeyCallback callback
) {
  ModKeyBind *kb;

  if (!hKey || !callback)
    return HT_FAIL;

  kb = (ModKeyBind *)hKey;
  {
    std::lock_guard<std::mutex> lock(gModDataLock);
    kb->listener = callback;
  }

  return HT_SUCCESS;
}

HTMLAPIATTR HTStatus HTMLAPI HTHotkeyUnlisten(
  HTHandle hKey,
  void *reserved
) {
  ModKeyBind *kb;

  if (!hKey)
    return HT_FAIL;

  kb = (ModKeyBind *)hKey;
  {
    std::lock_guard<std::mutex> lock(gModDataLock);
    kb->listener = nullptr;
  }

  return HT_SUCCESS;
}

/**
 * Call key event callback functions.
 */
void HTHotkeyDispatch(
  HTKeyCode key,
  bool down,
  bool repeat
) {
  std::vector<ModKeyBind *> localCallbacks;

  if (gKeyModifyCooldown)
    return;
  if (repeat)
    // We only dispatch the "real" physical key event.
    return;

  {
    std::lock_guard<std::mutex> lock(gModDataLock);
    auto it = gHotkeyCallbacks.find(key);
    if (it == gHotkeyCallbacks.end())
      return;
    localCallbacks.assign(it->second.begin(), it->second.end());
  }

  HTKeyEvent event;
  event.down = down;
  event.key = key;
  
  for (auto it = localCallbacks.begin(); it != localCallbacks.end(); it++) {
    PFN_HTHotkeyCallback cb = (*it)->listener;
    if (cb) {
      event.hKey = *it;
      cb(&event);
    }
  }
}

void HTHotkeySetCooldown() {
  gKeyModifyCooldown = HOTKEY_MODIFY_COOLDOWN;
}

void HTHotkeyUpdateCooldown() {
  if (gKeyModifyCooldown > 0)
    gKeyModifyCooldown--;
}
