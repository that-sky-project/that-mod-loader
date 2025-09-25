# HT's Mod Loader for Sky:Cotl
Use HTML to execute code before the game does.

## Usage
1. Download latest mod loader in [releases](https://www.github.com/HTMonkeyG/HTML-Sky/releases/latest).
2. Put `winhttp.dll` and `html-config.json` (optional) under the same folder with `Sky.exe`.
3. Create `htmodloader` folder in the folder contains `Sky.exe`. Your directory should look like this:
```
<Game installation directory>
 ├─Sky.exe
 ├─htmodloader
 │ └─mods
 │   ├─<A single mod>
 │   └─<...>
 ├─html-config.json
 ├─winhttp.dll
 └─<...>
```
4. Place each mod in a separated folder within `mods`. Every mod should contain an executable file (dll) and a `manifest.json` at least.
5. Start the game, and view loaded mods in `HTML Main Menu`.

## FAQ
### How to create a mod?
1. Download the latest HTML SDK zip in [releases](https://www.github.com/HTMonkeyG/HTML-Sky/releases/latest).
2. Write your own dll codes with HTML APIs and internal ImGui. You should include the provided ImGui headers by the SDK.
3. Compile and link `htmodloader.lib` with MinGW. HTML is compiled under MinGW, and it may cause problems due to different ABIs if compiling with MSVC. The mod should use c11 and c++17 standard.
4. Write `manifest.json` for the mod.
```json
{
  // Internal mod id, must be unique.
  "package_name": "modtest",
  // Mod version.
  "version": "1.0.0",
  // Compatible game edition flags.
  // 1 for Chinese edition and 2 for international edition, 3 for both.
  "game_edition": 3,
  // Display name.
  "mod_name": "Mod Test",
  // Mod description.
  "description": "Test mod of HT's Mod Loader",
  // Executable file of the mod.
  "main": "modtest.dll",
  // Mod dependencies. Unused.
  "dependencies": {},
  // Unused.
  "keywords": [
    "test"
  ],
  // Mod author and license.
  "author": "HTMonkeyG",
  "license": "MIT",
  // Mod website.
  "website": ""
}
```
5. Put your mod under `htmods` and start the game.
