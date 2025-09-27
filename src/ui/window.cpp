#include <windows.h>
#include "imgui.h"

#include "utils/texts.h"
#include "htinternal.h"

static bool gFirstFrame = true;

void HTWindowMain(bool *show) {
  if (gFirstFrame && show) {
    // Resize window on the first frame.
    ImGui::SetNextWindowSize(ImVec2(480, 320));
    gFirstFrame = false;
  }

  if (!ImGui::Begin("HTML Main Menu", show))
    return (void)ImGui::End();

  ImGui::PushID("HTMLWindowMain");
  
  if (ImGui::BeginTabBar("HTNavMain")) {
    if (ImGui::BeginTabItem("Abouts")) {
      HTMenuAbouts();
      ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem("Console")) {
      HTMenuConsole();
      ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem("Mods")) {
      HTMenuModList();
      ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem("Settings")) {
      HTMenuSettings();
      ImGui::EndTabItem();
    }
    ImGui::EndTabBar();
  }

  ImGui::PopID();

  ImGui::End();
}

void HTWindowDebugger(bool *show) {
  if (!ImGui::Begin("HTML Debugger", show))
    return (void)ImGui::End();

  ImGui::PushID("HTMLWindowDebugger");

  if (ImGui::CollapsingHeader("Hooks")) {

  }

  ImGui::PopID();

  ImGui::End();
}
