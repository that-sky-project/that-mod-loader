#include <windows.h>
#include "imgui.h"

#include "utils/globals.h"
#include "htinternal.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
  HWND hWnd,
  UINT msg,
  WPARAM wParam,
  LPARAM lParam);

// Original window process of the game.
static WNDPROC gWndProcOrigin = nullptr;

static bool isVkDown(int vk) {
  return (GetKeyState(vk) & 0x8000) != 0;
}

/**
 * Modified from ImGui. Map VK_* to HTKeyCode_*.
 */
static HTKeyCode vkToInternalKey(
  WPARAM wParam,
  LPARAM lParam
) {
  if ((wParam == VK_RETURN) && (HIWORD(lParam) & KF_EXTENDED))
    return HTKey_KeypadEnter;

  switch (wParam) {
    case VK_TAB: return HTKey_Tab;
    case VK_LEFT: return HTKey_LeftArrow;
    case VK_RIGHT: return HTKey_RightArrow;
    case VK_UP: return HTKey_UpArrow;
    case VK_DOWN: return HTKey_DownArrow;
    case VK_PRIOR: return HTKey_PageUp;
    case VK_NEXT: return HTKey_PageDown;
    case VK_HOME: return HTKey_Home;
    case VK_END: return HTKey_End;
    case VK_INSERT: return HTKey_Insert;
    case VK_DELETE: return HTKey_Delete;
    case VK_BACK: return HTKey_Backspace;
    case VK_SPACE: return HTKey_Space;
    case VK_RETURN: return HTKey_Enter;
    case VK_ESCAPE: return HTKey_Escape;
    case VK_OEM_COMMA: return HTKey_Comma;
    case VK_OEM_PERIOD: return HTKey_Period;
    case VK_CAPITAL: return HTKey_CapsLock;
    case VK_SCROLL: return HTKey_ScrollLock;
    case VK_NUMLOCK: return HTKey_NumLock;
    case VK_SNAPSHOT: return HTKey_PrintScreen;
    case VK_PAUSE: return HTKey_Pause;
    case VK_NUMPAD0: return HTKey_Keypad0;
    case VK_NUMPAD1: return HTKey_Keypad1;
    case VK_NUMPAD2: return HTKey_Keypad2;
    case VK_NUMPAD3: return HTKey_Keypad3;
    case VK_NUMPAD4: return HTKey_Keypad4;
    case VK_NUMPAD5: return HTKey_Keypad5;
    case VK_NUMPAD6: return HTKey_Keypad6;
    case VK_NUMPAD7: return HTKey_Keypad7;
    case VK_NUMPAD8: return HTKey_Keypad8;
    case VK_NUMPAD9: return HTKey_Keypad9;
    case VK_DECIMAL: return HTKey_KeypadDecimal;
    case VK_DIVIDE: return HTKey_KeypadDivide;
    case VK_MULTIPLY: return HTKey_KeypadMultiply;
    case VK_SUBTRACT: return HTKey_KeypadSubtract;
    case VK_ADD: return HTKey_KeypadAdd;
    case VK_LSHIFT: return HTKey_LeftShift;
    case VK_LCONTROL: return HTKey_LeftCtrl;
    case VK_LMENU: return HTKey_LeftAlt;
    case VK_LWIN: return HTKey_LeftSuper;
    case VK_RSHIFT: return HTKey_RightShift;
    case VK_RCONTROL: return HTKey_RightCtrl;
    case VK_RMENU: return HTKey_RightAlt;
    case VK_RWIN: return HTKey_RightSuper;
    case VK_APPS: return HTKey_Menu;
    case '0': return HTKey_0;
    case '1': return HTKey_1;
    case '2': return HTKey_2;
    case '3': return HTKey_3;
    case '4': return HTKey_4;
    case '5': return HTKey_5;
    case '6': return HTKey_6;
    case '7': return HTKey_7;
    case '8': return HTKey_8;
    case '9': return HTKey_9;
    case 'A': return HTKey_A;
    case 'B': return HTKey_B;
    case 'C': return HTKey_C;
    case 'D': return HTKey_D;
    case 'E': return HTKey_E;
    case 'F': return HTKey_F;
    case 'G': return HTKey_G;
    case 'H': return HTKey_H;
    case 'I': return HTKey_I;
    case 'J': return HTKey_J;
    case 'K': return HTKey_K;
    case 'L': return HTKey_L;
    case 'M': return HTKey_M;
    case 'N': return HTKey_N;
    case 'O': return HTKey_O;
    case 'P': return HTKey_P;
    case 'Q': return HTKey_Q;
    case 'R': return HTKey_R;
    case 'S': return HTKey_S;
    case 'T': return HTKey_T;
    case 'U': return HTKey_U;
    case 'V': return HTKey_V;
    case 'W': return HTKey_W;
    case 'X': return HTKey_X;
    case 'Y': return HTKey_Y;
    case 'Z': return HTKey_Z;
    case VK_F1: return HTKey_F1;
    case VK_F2: return HTKey_F2;
    case VK_F3: return HTKey_F3;
    case VK_F4: return HTKey_F4;
    case VK_F5: return HTKey_F5;
    case VK_F6: return HTKey_F6;
    case VK_F7: return HTKey_F7;
    case VK_F8: return HTKey_F8;
    case VK_F9: return HTKey_F9;
    case VK_F10: return HTKey_F10;
    case VK_F11: return HTKey_F11;
    case VK_F12: return HTKey_F12;
    case VK_F13: return HTKey_F13;
    case VK_F14: return HTKey_F14;
    case VK_F15: return HTKey_F15;
    case VK_F16: return HTKey_F16;
    case VK_F17: return HTKey_F17;
    case VK_F18: return HTKey_F18;
    case VK_F19: return HTKey_F19;
    case VK_F20: return HTKey_F20;
    case VK_F21: return HTKey_F21;
    case VK_F22: return HTKey_F22;
    case VK_F23: return HTKey_F23;
    case VK_F24: return HTKey_F24;
    case VK_BROWSER_BACK: return HTKey_AppBack;
    case VK_BROWSER_FORWARD: return HTKey_AppForward;
    default: break;
  }

  i32 scanCode = (i32)LOBYTE(HIWORD(lParam));
  switch (scanCode) {
    case 41: return HTKey_GraveAccent;
    case 12: return HTKey_Minus;
    case 13: return HTKey_Equal;
    case 26: return HTKey_LeftBracket;
    case 27: return HTKey_RightBracket;
    case 86: return HTKey_Oem102;
    case 43: return HTKey_Backslash;
    case 39: return HTKey_Semicolon;
    case 40: return HTKey_Apostrophe;
    case 51: return HTKey_Comma;
    case 52: return HTKey_Period;
    case 53: return HTKey_Slash;
  }

  return HTKey_None;
}

#define HTHotkeyCheckLR(a, b) \
  if (isVkDown(VK_L ## a) == isKeyDown)\
    HTHotkeyDispatch(HTKey_Left ## a, isKeyDown, repeat);\
  if (isVkDown(VK_R ## a) == isKeyDown)\
    HTHotkeyDispatch(HTKey_Right ## b, isKeyDown, repeat);

/**
 * Modified from ImGui. Dispatch key events to registered callbacks.
 */
static void HTHotKeyWndProc(
  HWND hWnd,
  UINT uMsg,
  WPARAM wParam,
  LPARAM lParam
) {
  bool isKeyDown;

  switch (uMsg) {
    case WM_KEYDOWN:
    case WM_KEYUP:
    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP:
    {
      isKeyDown = (uMsg == WM_KEYDOWN || uMsg == WM_SYSKEYDOWN);
      if (wParam >= 256)
        return;

      HTKeyCode key = vkToInternalKey(wParam, lParam);
      i32 vk = (i32)wParam;
      bool repeat = (lParam & 0x40000000) && isKeyDown;

      if (key == HTKey_PrintScreen && !isKeyDown)
        HTHotkeyDispatch(key, true, repeat);
      else if (vk == VK_SHIFT) {
        if (isVkDown(VK_LSHIFT) == isKeyDown)
          HTHotkeyDispatch(HTKey_LeftShift, isKeyDown, repeat);
        if (isVkDown(VK_RSHIFT) == isKeyDown) 
          HTHotkeyDispatch(HTKey_RightShift, isKeyDown, repeat);
      } else if (vk == VK_CONTROL) {
        if (isVkDown(VK_LCONTROL) == isKeyDown)
          HTHotkeyDispatch(HTKey_LeftCtrl, isKeyDown, repeat);
        if (isVkDown(VK_RCONTROL) == isKeyDown)
          HTHotkeyDispatch(HTKey_RightCtrl, isKeyDown, repeat);
      } else if (vk == VK_MENU) {
        if (isVkDown(VK_LMENU) == isKeyDown)
          HTHotkeyDispatch(HTKey_LeftAlt, isKeyDown, repeat);
        if (isVkDown(VK_RMENU) == isKeyDown)
          HTHotkeyDispatch(HTKey_RightAlt, isKeyDown, repeat);
      } else if (key != HTKey_None)
        HTHotkeyDispatch(key, isKeyDown, repeat);
      break;
    }
    case WM_LBUTTONDOWN:
    case WM_LBUTTONDBLCLK:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONDBLCLK:
    case WM_MBUTTONDOWN:
    case WM_MBUTTONDBLCLK:
    case WM_XBUTTONDOWN:
    case WM_XBUTTONDBLCLK:
    {
      HTKeyCode button = HTKey_None;
      if (uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONDBLCLK)
        button = HTKey_MouseLeft;
      if (uMsg == WM_RBUTTONDOWN || uMsg == WM_RBUTTONDBLCLK)
        button = HTKey_MouseRight;
      if (uMsg == WM_MBUTTONDOWN || uMsg == WM_MBUTTONDBLCLK)
        button = HTKey_MouseMiddle;
      if (uMsg == WM_XBUTTONDOWN || uMsg == WM_XBUTTONDBLCLK)
        button = (GET_XBUTTON_WPARAM(wParam) == XBUTTON1)
          ? HTKey_MouseX1
          : HTKey_MouseX2;
      HTHotkeyDispatch(button, true, false);
      break;
    }
    case WM_LBUTTONUP:
    case WM_RBUTTONUP:
    case WM_MBUTTONUP:
    case WM_XBUTTONUP:
    {
      HTKeyCode button = HTKey_None;
      if (uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONDBLCLK)
        button = HTKey_MouseLeft;
      if (uMsg == WM_RBUTTONDOWN || uMsg == WM_RBUTTONDBLCLK)
        button = HTKey_MouseRight;
      if (uMsg == WM_MBUTTONDOWN || uMsg == WM_MBUTTONDBLCLK)
        button = HTKey_MouseMiddle;
      if (uMsg == WM_XBUTTONDOWN || uMsg == WM_XBUTTONDBLCLK)
        button = (GET_XBUTTON_WPARAM(wParam) == XBUTTON1)
          ? HTKey_MouseX1
          : HTKey_MouseX2;
      HTHotkeyDispatch(button, false, false);
      break;
    }
  }
}

/**
 * The window process used to pass window messages to ImGui and block window
 * message delivery to the game.
 */
static LRESULT APIENTRY HTWndProc(
  HWND hWnd,
  UINT uMsg,
  WPARAM wParam,
  LPARAM lParam
) {
  UINT block = 0;
  ImGuiIO &io = ImGui::GetIO();

  // Dispatch the window message to ImGui.
  (void)ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
  HTHotKeyWndProc(hWnd, uMsg, wParam, lParam);

  // Block the message. We only check for the key down message, in order to
  // avoid strange behaviors e.g. continuely moving characters.
  switch (uMsg) {
    case WM_LBUTTONDOWN:
    case WM_LBUTTONDBLCLK:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONDBLCLK:
    case WM_MBUTTONDOWN:
    case WM_MBUTTONDBLCLK:
    case WM_XBUTTONDOWN:
    case WM_XBUTTONDBLCLK:
    case WM_MOUSEHWHEEL:
    case WM_MOUSEWHEEL:
      block = io.WantCaptureMouse;
      break;
    case WM_SYSKEYDOWN:
    case WM_KEYDOWN:
    case WM_CHAR:
      block = io.WantCaptureKeyboard;
      break;
  }
  if (block)
    return 0;

  // Pass the window message to the game.
  return CallWindowProcW(
    gWndProcOrigin, hWnd, uMsg, wParam, lParam);
}

/**
 * Hook the window process of the game.
 */
void HTInstallInputHook() {
  if (!gGameStatus.window)
    return;
  gWndProcOrigin = (WNDPROC)SetWindowLongPtrW(
    gGameStatus.window,
    GWLP_WNDPROC,
    (LONG_PTR)HTWndProc);
}

/**
 * Release the window callback hook of the game.
 */
void HTUninstallInputHook() {
  if (gGameStatus.window && gWndProcOrigin)
    (void)SetWindowLongPtrW(
      gGameStatus.window,
      GWLP_WNDPROC,
      (LONG_PTR)gWndProcOrigin);
}
