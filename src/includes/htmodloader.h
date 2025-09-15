// ----------------------------------------------------------------------------
// API exports of HT's Mod Loader.
// Copyright (c) HTMonkeyG 2025
// https://www.github.com/HTMonkeyG/HTML-Sky
// ----------------------------------------------------------------------------

// #pragma once
#ifndef __HTMODLOADER_H__
#define __HTMODLOADER_H__

// Throws an error when compiled on other architectures.
#if !(defined(_M_X64) || defined(_WIN64) || defined(__x86_64__) || defined(__amd64__))
#error HT's Mod Loader and it's related mods is only avaliable on x86-64!
#endif

// Mod loader version.
// Version number is used for pre-processing statements handling version
// compatibility.
#define HTML_VERSION 10000
#define HTML_VERSION_NAME "1.0.0"

#define HTMLAPI __stdcall
#ifndef HTMLAPIATTR
#define HTMLAPIATTR
#endif

// Includes.
#include <windows.h>
#include "includes/htkeycodes.h"
#include "includes/aliases.h"

#ifdef __cplusplus
extern "C" {
#endif

// ----------------------------------------------------------------------------
// [SECTION] HTML basic APIs and type declarations.
// ----------------------------------------------------------------------------

// Whether the execution was successful or not.
typedef enum {
  HT_FAIL = 0,
  HT_SUCCESS = 1
} HTStatus;

// Game editions.
typedef enum {
  // Uninitialized state.
  HT_EDITION_UNKNOWN = 0,
  // Chinese edition.
  HT_EDITION_CHINESE,
  // International edition.
  HT_EDITION_INTERNATIONAL
} HTGameEdition;

// Game status.
typedef struct {
  // Base address of game executable file.
  void *baseAddr;
  // The edition of the game.
  HTGameEdition edition;
  // The window handle of the game.
  HWND window;
  // The process id of the game.
  DWORD pid;
} HTGameStatus;

// Function prototype.
typedef void *(HTMLAPI *PFN_HTVoidFunction)(
  void);

/* Mod exported function prototypes. */

// Gui renderer.
typedef void (HTMLAPI *PFN_HTModRenderGui)(
  f32 timeElapsed, void *reserved);
// Initialize event
typedef HTStatus (HTMLAPI *PFN_HTModOnInit)(
  void *reserved);
// Mod enable event
typedef HTStatus (HTMLAPI *PFN_HTModOnEnable)(
  void *reserved);

/**
 * Get game status object.
 */
HTMLAPIATTR void HTMLAPI HTGetGameStatus(
  HTGameStatus *status);

/**
 * Get the folder where the game executable file is located.
 */
HTMLAPIATTR void HTMLAPI HTGetGameExeFolder(
  char *result, u64 maxLen);

/**
 * Get the folder where the mods is located. In most cases, the same as add
 * "\\htmods" to HTGetGameExeFolder()'s result.
 */
HTMLAPIATTR void HTMLAPI HTGetModFolder(
  char *result, u64 maxLen);

// ----------------------------------------------------------------------------
// [SECTION] HTML signature scan APIs.
// ----------------------------------------------------------------------------

// Method for obtaining the final address.
typedef enum {
  // The Signature represents the function body.
  HT_SCAN_DIRECT = 0,
  // The signature represents the E8 or E9 instruction that calls the function.
  HT_SCAN_E8,
  // The signature represents the FF15 instruction that calls the function.
  HT_SCAN_FF15,
} HTSigScanType;

// Signature code config.
typedef struct {
  // Signature code.
  const char *sig;
  // Function name, only for debug use.
  const char *name;
  // Method for obtaining the final address.
  HTSigScanType indirect;
  // The byte offset of 0xE8 or 0x15 byte for HT_SCAN_E8 and HT_SCAN_FF15, or
  // the byte offset to the first instruction for HT_SCAN_DIRECT.
  i32 offset;
} HTSignature;

/**
 * Scan with signature.
 */
HTMLAPIATTR void *HTMLAPI HTSigScan(
  const HTSignature *signature);
typedef void *(HTMLAPI *PFN_HTSigScan)(
  const HTSignature *signature);

// Function address config.
typedef struct {
  // The address of the detour function if hooked.
  void *detour;
  // The address of the scanned function.
  void *fn;
  // The address of the trampoline function if hooked.
  void *origin;
} HTHookFunction;

/**
 * Scan a single function.
 */
HTMLAPIATTR void *HTMLAPI HTSigScanFunc(
  const HTSignature *signature, HTHookFunction *func);
typedef void *(HTMLAPI *PFN_HTSigScanFunc)(
  const HTSignature *signature, HTHookFunction *func);

/**
 * Scan an array of functions.
 */
HTMLAPIATTR HTStatus HTMLAPI HTSigScanFuncEx(
  const HTSignature **signature, HTHookFunction **func, u32 size);
typedef HTStatus (HTMLAPI *PFN_HTSigScanFuncEx)(
  const HTSignature **signature, HTHookFunction **func, u32 size);

// ----------------------------------------------------------------------------
// [SECTION] HTML inline hook APIs.
// ----------------------------------------------------------------------------

/**
 * Install hook with MinHook.
 */
HTMLAPIATTR HTStatus HTMLAPI HTInstallHook(
  void *fn, void *detour, void **origin);
typedef HTStatus (HTMLAPI *PFN_HTInstallHook)(
  void *fn, void *detour, void **origin);

/**
 * Enable hook on specified function.
 */
HTMLAPIATTR HTStatus HTMLAPI HTEnableHook(
  void *fn);
typedef HTStatus (HTMLAPI *PFN_HTEnableHook)(
  void *fn);

/**
 * Disable hook on specified function.
 */
HTMLAPIATTR HTStatus HTMLAPI HTDisableHook(
  void *fn);
typedef HTStatus (HTMLAPI *PFN_HTDisableHook)(
  void *fn);

/**
 * Install hook from HTHookFunction struct.
 */
HTMLAPIATTR HTStatus HTMLAPI HTInstallHookEx(
  HTHookFunction *func);
typedef HTStatus (HTMLAPI *PFN_HTInstallHookEx)(
  HTHookFunction *func);

/**
 * Enable hook on specified function.
 */ 
HTMLAPIATTR HTStatus HTMLAPI HTEnableHookEx(
  HTHookFunction *func);
typedef void (HTMLAPI *PFN_HTEnableHookEx)(
  HTHookFunction *func);

/**
 * Disable hook on specified function.
 */
HTMLAPIATTR HTStatus HTMLAPI HTDisableHookEx(
  HTHookFunction *func);
typedef void (HTMLAPI *PFN_HTDisableHookEx)(
  HTHookFunction *func);

// ----------------------------------------------------------------------------
// [SECTION] HTML memory manager APIs.
// ----------------------------------------------------------------------------

/**
 * Allocate a sized memory block.
 */
HTMLAPIATTR void *HTMLAPI HTMemAlloc(
  u64 size);

/**
 * Allocate space for an array of `count` objects, each of `size` bytes.
 * Different from calloc(), HTMemNew won't initialize the memory block.
 */
HTMLAPIATTR void *HTMLAPI HTMemNew(
  u64 count, u64 size);

/**
 * Free a memory block allocated with HTMemAlloc() or HTMemNew(). Returns
 * HT_FAIL when the pointer is invalid or is already freed.
 * 
 * Mod needs to reset pointer variables to prevent dangling pointers.
 */
HTMLAPIATTR HTStatus HTMLAPI HTMemFree(
  void *pointer);

// ----------------------------------------------------------------------------
// [SECTION] HTML mod communication APIs.
// ----------------------------------------------------------------------------

// Event callback.
typedef void (HTMLAPI *PFN_HTEventCallback)(
  const void *data);

// Handle.
typedef void *HTHandle;

#define HT_INVALID_HANDLE NULL

/**
 * Get the address of a registered function.
 */
HTMLAPIATTR PFN_HTVoidFunction HTMLAPI HTGetProcAddr(
  HMODULE hModule, const char *name);

/**
 * Get a handle for the mod manifest.
 */
HTMLAPIATTR HTHandle HTMLAPI HTGetModManifest(
  HMODULE hModule);

/**
 * Register a function with name. Registered function can be called by other
 * mods with HTGetProcAddr(). If the same name is passed in and called more
 * than once, the value of the last call will be saved.
 * 
 * It is recommended to use namespace strings like `namespace:foobar` when
 * registering functions.
 */
HTMLAPIATTR HTStatus HTMLAPI HTCommRegFunction(
  HMODULE hModule, const char *name, PFN_HTVoidFunction func);

/**
 * Register an event listener with given event name.
 * 
 * The callback function should not modify the content pointed to by the
 * `data` pointer. The callback function must assume that the data pointer
 * is only valid before the callback function returns.
 */
HTMLAPIATTR HTStatus HTMLAPI HTCommOnEvent(
  const char *name, PFN_HTEventCallback callback);

#define HTCommAddEventListener HTCommOnEvent

/**
 * Remove a registered event listener.
 */
HTMLAPIATTR HTStatus HTMLAPI HTCommOffEvent(
  const char *name, PFN_HTEventCallback callback);

#define HTCommRemoveEventListener HTCommOffEvent
  
/**
 * Trigger an event with specified data.
 */
HTMLAPIATTR HTStatus HTMLAPI HTCommEmitEvent(
  const char *name, void *reserved, void *data);

// ----------------------------------------------------------------------------
// [SECTION] HTML hotkey register APIs.
// ----------------------------------------------------------------------------

typedef struct {
  HTHandle hKey;
  HTKeyCode key;
  u08 down;
} HTKeyEvent;

// Hotkey callback.
typedef void (HTMLAPI *PFN_HTHotkeyCallback)(
  const HTKeyEvent *event);

/**
 * Register a single key bind.
 */
HTMLAPIATTR HTHandle HTMLAPI HTHotkeyRegister(
  HMODULE hModule,
  const char *name,
  HTKeyCode defaultCode);

/**
 * Check if a registered shortcut key has been pressed.
 */
HTMLAPIATTR u32 HTMLAPI HTHotkeyPressed(
  HTHandle hKey);

#ifdef __cplusplus
}
#endif

#endif
