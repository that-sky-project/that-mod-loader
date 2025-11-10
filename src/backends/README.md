# HTML Backends
HTML backends contain game-specified or platform-specified codes.

## Game Backends
Game backend implementation includes code strongly related to the target game, such as window handle acquisition and game version retrieval.

A basic game backend needs to call `HTiSetGameStatus()` to set the game status, then call `HTiSetupAll()` to load configuration files and all mods. The above functions must be called in sequence within the same thread.
