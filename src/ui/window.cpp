#include <windows.h>
#include "imgui.h"

#include "utils/texts.h"
#include "htinternal.h"

void HTiWindowMain(bool *show) {
  // Resize window on the first frame.
  ImGui::SetNextWindowSize(ImVec2(480, 320), ImGuiCond_FirstUseEver);

  if (!ImGui::Begin("HTML Main Menu", show))
    return (void)ImGui::End();

  ImGui::PushID("HTMLWindowMain");
  
  if (ImGui::BeginTabBar("HTNavMain")) {
    if (ImGui::BeginTabItem("Abouts")) {
      HTiMenuAbouts();
      ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem("Console")) {
      HTiMenuConsole();
      ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem("Mods")) {
      HTiMenuModList();
      ImGui::EndTabItem();
    }
    if (ImGui::BeginTabItem("Settings")) {
      HTiMenuSettings();
      ImGui::EndTabItem();
    }
    ImGui::EndTabBar();
  }

  ImGui::PopID();

  ImGui::End();
}

void HTiWindowDebugger(bool *show) {
  if (!ImGui::Begin("HTML Debugger", show))
    return (void)ImGui::End();

  ImGui::PushID("HTMLWindowDebugger");

  if (ImGui::CollapsingHeader("Hooks")) {

  }

  ImGui::PopID();

  ImGui::End();
}
