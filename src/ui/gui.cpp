#include <windows.h>
#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_vulkan.h"
#include "vulkan/vulkan.h"

#include "ui/input.h"
#include "ui/gui.h"
#include "ui/console.h"

#include "utils/globals.h"
#include "moddata.h"

static bool gShowMainMenu = true
  , gFirstFrame = true;
static const ImVec4 modNameColor(1, 1, 1, 1)
  , modDescColor(0.75, 0.75, 0.75, 1);

/**
 * Render mod list tab item.
 */
static void HTMenuAbouts() {
  ImGui::Text("HT's Mod Loader v" HTML_VERSION_NAME " by HTMonkeyG");
  ImGui::Text("A mod loader developed for Sky:CotL.");
  ImGui::Text("<https://www.github.com/HTMonkeyG/HTML-Sky>");
}

/**
 * Render mod list tab item.
 */
static void HTMenuModList() {
  i32 i = 0;

  if (!ImGui::BeginChild("##HTModList"))
    return (void)ImGui::EndChild();
  
  ImGui::PushID("##HTModListItems");
  ImVec2 size(0, ImGui::GetTextLineHeight() * 4);
  ImVec2 spacing = ImGui::GetStyle().ItemSpacing;
  spacing.y = 1;
  ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, spacing);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(3, 3));
  for (auto it = gModDataLoader.begin(); it != gModDataLoader.end(); ++it, i++) {
    ModManifest &manifest = it->second;

    // Show mod info.
    ImGuiID childId = ImGui::GetID((void *)(u64)i);
    ImGui::BeginChild(childId, size, ImGuiChildFlags_Borders);

    // Show mod name.
    ImGui::TextColored(modNameColor, "%s", manifest.modName.data());

    // Show mod description.
    ImGui::PushStyleColor(ImGuiCol_Text, modDescColor);
    ImGui::TextWrapped("%s", manifest.description.data());
    ImGui::PopStyleColor();

    ImGui::EndChild();
  }
  ImGui::PopStyleVar(2);
  ImGui::PopID();

  ImGui::EndChild();
}

static void displayAndUpdateKeys() {
  float windowPadding = ImGui::GetStyle().WindowPadding.x
    , cursor;
  
  // Calculate cursor pos for right alignment.
  cursor = ImGui::GetContentRegionAvail().x + ImGui::GetCursorPosX();
  cursor -= 150.0 + windowPadding;
  if (cursor <= 150.0)
    cursor = 150.0;

  for (auto modIt = gModDataRuntime.begin(); modIt != gModDataRuntime.end(); modIt++) {
    ModRuntime *rt = &modIt->second;
    if (rt->keyBinds.empty())
      continue;
    ImGui::SeparatorText(rt->manifest->modName.c_str());
    for (auto keyIt = rt->keyBinds.begin(); keyIt != rt->keyBinds.end(); keyIt++) {
      ModKeyBind *kb = &keyIt->second;
      ImGui::PushID((void *)kb);

      // Show key display name.
      ImGui::Text(kb->displayName.c_str());

      // Right align, show current key.
      ImGui::SameLine();
      ImGui::SetCursorPosX(cursor);
      ImGui::Button(
        ImGui::GetKeyName((ImGuiKey)kb->key),
        ImVec2(75, 0));

      // Show reset button.
      ImGui::SameLine();
      ImGui::BeginDisabled(kb->defaultKey == kb->key);
      ImGui::Button("Reset",
        ImVec2(75, 0));
      ImGui::EndDisabled();

      ImGui::PopID();
    }
  }
}

static void HTMenuSettings() {
  if (ImGui::CollapsingHeader("Key Bindings##HTModKeys", ImGuiTreeNodeFlags_None))
    displayAndUpdateKeys();
}

/**
 * Initialize ImGui context and window message hook.
 * 
 * This function only be called after the game window is catched.
 */
void HTInitGUI() {
  if (!gGameStatus.window)
    return;
  if (ImGui::GetCurrentContext())
    // Prevent duplicate initialization.
    return;

  // Initialize ImGui. ImGui_ImplVulkan_Init() is called in layer.cpp by
  // renderGui().
  ImGui::CreateContext();
  ImGui_ImplWin32_Init(gGameStatus.window);
  ImGuiIO &io = ImGui::GetIO();
  ImGuiStyle &style = ImGui::GetStyle();
  io.IniFilename = io.LogFilename = nullptr;

  // Scale the window by dpi.
  f32 dpiScale = ImGui_ImplWin32_GetDpiScaleForHwnd(gGameStatus.window);
  style.ScaleAllSizes(dpiScale);
  io.FontGlobalScale = dpiScale;

  // Setup colors.
  ImVec4 *colors = ImGui::GetStyle().Colors;
  colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 0.51f);
  colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.82f);
  colors[ImGuiCol_Border] = ImVec4(0.43f, 0.43f, 0.50f, 0.39f);
  colors[ImGuiCol_FrameBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.47f);
  colors[ImGuiCol_FrameBgHovered] = ImVec4(0.47f, 0.47f, 0.47f, 0.47f);
  colors[ImGuiCol_FrameBgActive] = ImVec4(0.00f, 0.00f, 0.00f, 0.47f);
  colors[ImGuiCol_TitleBg] = ImVec4(0.04f, 0.04f, 0.04f, 0.82f);
  colors[ImGuiCol_MenuBarBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
  colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.00f);
  colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
  colors[ImGuiCol_Separator] = ImVec4(0.63f, 0.63f, 0.59f, 0.75f);

  // Setup sizes.
  style.WindowBorderSize = style.ChildBorderSize = 1;
  style.PopupBorderSize = style.FrameBorderSize = 0;
  style.WindowRounding = style.ChildRounding = style.FrameRounding
    = style.PopupRounding = style.ScrollbarRounding = style.GrabRounding = 10;
  style.ScrollbarSize = 10;
  style.SeparatorTextAlign = ImVec2(0, 0.5);
  style.SeparatorTextPadding = ImVec2(0, 3);

  // Install window process hook.
  HTInstallInputHook();
}

/**
 * Show HTML Menus.
 */
void HTUpdateGUI() {
  // Press "~" key to show or hide.
  if (GetAsyncKeyState(VK_OEM_3) & 0x1)
    gShowMainMenu = !gShowMainMenu;

  if (!gShowMainMenu)
    return;

  ImGuiStyle &style = ImGui::GetStyle();
  style.ScrollbarRounding = 0.0f;
  style.WindowTitleAlign.x = 0.5f;
  style.WindowTitleAlign.y = 0.5f;
  ImGuiIO &io = ImGui::GetIO();
  ImGuiWindowFlags windowFlags = ImGuiWindowFlags_None;
  if (gFirstFrame) {
    // Resize window on the first frame.
    ImGui::SetNextWindowSize(ImVec2(480, 320));
    gFirstFrame = false;
  }

  if (!ImGui::Begin("HTML Main Menu", &gShowMainMenu, windowFlags))
    return (void)ImGui::End();
  
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

  for (auto it = gModDataRuntime.begin(); it != gModDataRuntime.end(); it++) {
    PFN_HTModRenderGui guiRenderer = (PFN_HTModRenderGui)it->second.loaderFunc.pfn_HTModRenderGui;
    if (guiRenderer)
      guiRenderer(io.DeltaTime, nullptr);
  }
  
  ImGui::End();
}

void HTDeinitGUI() {
  ImGui_ImplVulkan_Shutdown();
  ImGui_ImplWin32_Shutdown();
  ImGui::DestroyContext();
}
