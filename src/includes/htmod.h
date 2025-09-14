// ----------------------------------------------------------------------------
// The mod should implement and export functions declared below.
// ----------------------------------------------------------------------------

#include <windows.h>
#include "includes/htmodloader.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * ImGui draw calls should only be put in this function. `timeElapesed`
 * indicates the time elapsed since the last frame.
 */
void HTMLAPI HTModRenderGui(
  f32 timeElapesed, void *reserved);
void HTMLAPI HTModOnInit(
  void *reserved);
void HTMLAPI HTModOnEnable(
  void *reserved);

#ifdef __cplusplus
}
#endif
