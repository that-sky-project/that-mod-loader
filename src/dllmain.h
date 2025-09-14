#include <windows.h>
#include <ntstatus.h>
#include <stdio.h>
#include <unordered_map>
#include "MinHook.h"

#include "includes/aliases.h"
#include "utils/globals.h"
#include "utils/logger.h"
#include "loader.h"
#include "proxy/winhttp-proxy.h"
#include "utils/texts.h"