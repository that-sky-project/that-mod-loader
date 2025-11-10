// ----------------------------------------------------------------------------
// Game-specified implementation for MCBE of HTML.
// By HTMonkeyG.
// ----------------------------------------------------------------------------

#include <windows.h>
#include "MinHook.h"

#include "htinternal.h"
#include "includes/backends/html_impl_mcbe.h"
#include "includes/htconfig.h"

#ifdef USE_IMPL_MCBE

#define HTTexts_WndNamePostfixW L"Minecraft"
#define HTTexts_WndClassW L"OGLES"

typedef HWND (WINAPI *PFN_CreateWindowExA)(
  DWORD, LPCSTR, LPCSTR, DWORD, i32, i32, i32, i32, HWND, HMENU, HINSTANCE, LPVOID);
typedef HWND (WINAPI *PFN_CreateWindowExW)(
  DWORD, LPCWSTR, LPCWSTR, DWORD, i32, i32, i32, i32, HWND, HMENU, HINSTANCE, LPVOID);

static PFN_CreateWindowExA fn_CreateWindowExA;
static PFN_CreateWindowExW fn_CreateWindowExW;

static i32 editionCheck(
  HTGameEdition edition
) {
  HTGameEdition local = gGameStatus.edition;
  if (
    edition == HT_ImplMCBE_EditionAll
    && (local == HT_ImplMCBE_EditionChinese || local == HT_ImplMCBE_EditionInternation)
  )
    return 1;

  if (edition == local)
    return 1;

  return 0;
}

static i32 checkWindowAndSetupAW(
  HWND hWnd
) {
  wchar_t buffer[32];
  HTGameStatus status;
  HTGameEdition edition = HT_ImplNull_EditionUnknown;

  if (gGameStatus.window)
    return 0;

  // Get the game edition from window name.
  GetWindowTextW(hWnd, buffer, 32);
  buffer[31] = 0;
  if (wcsstr(buffer, HTTexts_WndNamePostfixW))
    edition = HT_ImplMCBE_EditionChinese;
  else
    return 0;

  // Check the window's class name.
  GetClassNameW(hWnd, buffer, 32);
  buffer[31] = 0;
  if (wcscmp(buffer, HTTexts_WndClassW))
    return 0;

  // Set game edition and hWnd.
  status.baseAddr = (void *)GetModuleHandleA("Minecraft.Windows.exe");
  status.edition = edition;
  status.pid = GetCurrentProcessId();
  status.window = hWnd;
  HTiSetGameStatus(&status);

  // Set edition check function.
  HTiBackendSetEditionCheckFunc((PFN_HTVoidFunction)editionCheck);

  // Load mods.
  HTiSetupAll();

  return 1;
}

static HWND WINAPI hook_CreateWindowExA(
  DWORD dwExStyle,
  LPCSTR lpClassName,
  LPCSTR lpWindowName,
  DWORD dwStyle,
  int X,
  int Y,
  int nWidth,
  int nHeight,
  HWND hWndParent,
  HMENU hMenu,
  HINSTANCE hInstance,
  LPVOID lpParam
) {
  HWND result = fn_CreateWindowExA(
    dwExStyle,
    lpClassName,
    lpWindowName,
    dwStyle,
    X,
    Y,
    nWidth,
    nHeight,
    hWndParent,
    hMenu,
    hInstance,
    lpParam);
  DWORD lastError = GetLastError();

  if (!result)
    return result;

  checkWindowAndSetupAW(result);

  SetLastError(lastError);
  return result;
}

static HWND WINAPI hook_CreateWindowExW(
  DWORD dwExStyle,
  LPCWSTR lpClassName,
  LPCWSTR lpWindowName,
  DWORD dwStyle,
  int X,
  int Y,
  int nWidth,
  int nHeight,
  HWND hWndParent,
  HMENU hMenu,
  HINSTANCE hInstance,
  LPVOID lpParam
) {
  HWND result = fn_CreateWindowExW(
    dwExStyle,
    lpClassName,
    lpWindowName,
    dwStyle,
    X,
    Y,
    nWidth,
    nHeight,
    hWndParent,
    hMenu,
    hInstance,
    lpParam);
  DWORD lastError = GetLastError();

  if (!result)
    return result;

  checkWindowAndSetupAW(result);

  SetLastError(lastError);
  return result;
}

int HTi_ImplMCBE_ExpectProcess() {
  return !!GetModuleHandleA("Minecraft.Windows.exe");
}

/**
 * Install hooks on WinAPI functions that we need. Setup procedure is in the
 * detour functions on CreateWindowEx().
 */
int HTi_ImplMCBE_Init() {
  MH_STATUS s;

  if (!HTi_ImplMCBE_ExpectProcess())
    return 0;

  s = MH_CreateHookApi(
    L"user32.dll",
    "CreateWindowExA",
    (void *)hook_CreateWindowExA,
    (void **)&fn_CreateWindowExA
  );
  if (s != MH_OK)
    return 0;

  s = MH_CreateHookApi(
    L"user32.dll",
    "CreateWindowExW",
    (void *)hook_CreateWindowExW,
    (void **)&fn_CreateWindowExW
  );
  if (s != MH_OK)
    return 0;

  s = MH_EnableHook(MH_ALL_HOOKS);
  if (s != MH_OK)
    return 0;

  return 1;
}

#endif
