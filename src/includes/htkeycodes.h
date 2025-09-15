#ifndef __HTKEYCODES_H__
#define __HTKEYCODES_H__

#ifdef __cplusplus
extern "C" {
#endif

// Modified from ImGui, to keep compatibility.
typedef enum {
  HTKey_None = 0,

  HTKey_Tab = 512,
  HTKey_LeftArrow,
  HTKey_RightArrow,
  HTKey_UpArrow,
  HTKey_DownArrow,
  HTKey_PageUp,
  HTKey_PageDown,
  HTKey_Home,
  HTKey_End,
  HTKey_Insert,
  HTKey_Delete,
  HTKey_Backspace,
  HTKey_Space,
  HTKey_Enter,
  HTKey_Escape,
  HTKey_LeftCtrl, HTKey_LeftShift, HTKey_LeftAlt, HTKey_LeftSuper,
  HTKey_RightCtrl, HTKey_RightShift, HTKey_RightAlt, HTKey_RightSuper,
  HTKey_Menu,
  HTKey_0, HTKey_1, HTKey_2, HTKey_3, HTKey_4, HTKey_5, HTKey_6, HTKey_7, HTKey_8, HTKey_9,
  HTKey_A, HTKey_B, HTKey_C, HTKey_D, HTKey_E, HTKey_F, HTKey_G, HTKey_H, HTKey_I, HTKey_J,
  HTKey_K, HTKey_L, HTKey_M, HTKey_N, HTKey_O, HTKey_P, HTKey_Q, HTKey_R, HTKey_S, HTKey_T,
  HTKey_U, HTKey_V, HTKey_W, HTKey_X, HTKey_Y, HTKey_Z,
  HTKey_F1, HTKey_F2, HTKey_F3, HTKey_F4, HTKey_F5, HTKey_F6,
  HTKey_F7, HTKey_F8, HTKey_F9, HTKey_F10, HTKey_F11, HTKey_F12,
  HTKey_F13, HTKey_F14, HTKey_F15, HTKey_F16, HTKey_F17, HTKey_F18,
  HTKey_F19, HTKey_F20, HTKey_F21, HTKey_F22, HTKey_F23, HTKey_F24,
  // '
  HTKey_Apostrophe,
  // ,
  HTKey_Comma,
  // -
  HTKey_Minus,
  // .
  HTKey_Period,
  // /
  HTKey_Slash,
  // ;
  HTKey_Semicolon,
  // =
  HTKey_Equal,
  // [
  HTKey_LeftBracket,
  // \ (this text inhibit multiline comment caused by backslash)
  HTKey_Backslash,
  // ]
  HTKey_RightBracket,
  // `
  HTKey_GraveAccent,
  HTKey_CapsLock,
  HTKey_ScrollLock,
  HTKey_NumLock,
  HTKey_PrintScreen,
  HTKey_Pause,
  HTKey_Keypad0, HTKey_Keypad1, HTKey_Keypad2, HTKey_Keypad3, HTKey_Keypad4,
  HTKey_Keypad5, HTKey_Keypad6, HTKey_Keypad7, HTKey_Keypad8, HTKey_Keypad9,
  HTKey_KeypadDecimal,
  HTKey_KeypadDivide,
  HTKey_KeypadMultiply,
  HTKey_KeypadSubtract,
  HTKey_KeypadAdd,
  HTKey_KeypadEnter,
  HTKey_KeypadEqual,
  // Available on some keyboard/mouses. Often referred as "Browser Back"
  HTKey_AppBack,
  HTKey_AppForward,
  // Non-US backslash.
  HTKey_Oem102,

  // Mouse inputs.
  HTKey_MouseLeft = 656,
  HTKey_MouseRight,
  HTKey_MouseMiddle,
  HTKey_MouseX1,
  HTKey_MouseX2,
  HTKey_MouseWheelX,
  HTKey_MouseWheelY,

  HTKeyMod_None = 0,
  // Ctrl (non-macOS), Cmd (macOS)
  HTKeyMod_Ctrl = 1 << 12,
  // Shift
  HTKeyMod_Shift = 1 << 13,
  // Option/Menu
  HTKeyMod_Alt = 1 << 14,
  // Windows/Super (non-macOS), Ctrl (macOS)
  HTKeyMod_Super = 1 << 15,
} HTKeyCode;

#ifdef __cplusplus
}
#endif

#endif
