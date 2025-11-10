// ----------------------------------------------------------------------------
// Backend dispatcher.
// You must put your backend initialize functions under the function below.
// ----------------------------------------------------------------------------
#include <windows.h>
#include <mutex>

#include "imgui.h"

#include "htinternal.h"
#include "includes/htconfig.h"

typedef int (HTMLAPI *PFN_HTiGameEditionCheck)(
  HTGameEdition);

static i32 checkEditionDefault(
  HTGameEdition);
static CRITICAL_SECTION gGraphicInitMutex;
static PFN_HTiGameEditionCheck gEditionCheck = checkEditionDefault;

static i32 checkEditionDefault(
  HTGameEdition edition
) {
  return edition == gGameStatus.edition;
}

int HTiBackendGLEnterCritical() {
  EnterCriticalSection(&gGraphicInitMutex);
  return !ImGui::GetIO().BackendRendererUserData;
}

int HTiBackendGLLeaveCritical() {
  LeaveCriticalSection(&gGraphicInitMutex);
  return 1;
}

int HTiBackendGLInitComplete() {
  SetEvent(gEventGuiInit);
  return 1;
}

int HTiBackendCheckEdition(
  HTGameEdition edition
) {
  return gEditionCheck(edition);
}

int HTiBackendSetEditionCheckFunc(
  PFN_HTVoidFunction func
) {
  gEditionCheck = (PFN_HTiGameEditionCheck)func;
  return 1;
}

int HTiBackendExpectProcess() {
  int success = 0;

  // Setup all game backends.
#ifdef USE_IMPL_SKY
  // Expect Sky.exe
  extern int HTi_ImplSky_ExpectProcess();
  success |= HTi_ImplSky_ExpectProcess();
#endif
#ifdef USE_IMPL_MCBE
  // Expect Minecraft.Windows.exe
  extern void HTi_ImplMCBE_ExpectedProcess();
  success |= HTi_ImplMCBE_ExpectedProcess();
#endif

  return success;
}

int HTiBackendSetupAll() {
  int success = 0;

  InitializeCriticalSection(&gGraphicInitMutex);

  // Setup all graphic backends.
#ifdef USE_IMPL_VKLAYER
  // Setup vulkan layer.
  extern int HTi_ImplVkLayer_Init();
  success |= HTi_ImplVkLayer_Init();
#endif
#ifdef USE_IMPL_OPENGL3
  // Setup OpenGL3.
  extern void HTi_ImplOpenGL3_Init();
  success |= HTi_ImplOpenGL3_Init();
#endif

  // Setup all game backends.
#ifdef USE_IMPL_SKY
  // Setup Sky:CotL.
  extern int HTi_ImplSky_Init();
  success |= HTi_ImplSky_Init();
#endif
#ifdef USE_IMPL_MCBE
  // Setup Minecraft:Bedrock.
  extern void HTi_ImplMCBE_Init();
  success |= HTi_ImplMCBE_Init();
#endif

  return success;
}