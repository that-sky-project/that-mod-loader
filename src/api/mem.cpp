// ----------------------------------------------------------------------------
// Memory manager APIs of HT's Mod Loader.
// ----------------------------------------------------------------------------
#include <stdlib.h>
#include <unordered_set>
#include <mutex>
#include "includes/aliases.h"
#include "utils/globals.h"
#include "includes/htmodloader.h"

static std::mutex gMutex;
// Stores pointers to all allocated mem blocks.
static std::unordered_set<void *> gAllocated;

HTMLAPIATTR void *HTMLAPI HTMemAlloc(u64 size) {
  std::lock_guard<std::mutex> lock(gMutex);
  void *result = HeapAlloc(gHeap, 0, size);
  if (result)
    gAllocated.insert(result);
  return result;
}

HTMLAPIATTR void *HTMLAPI HTMemNew(u64 count, u64 size) {
  std::lock_guard<std::mutex> lock(gMutex);
  void *result = HeapAlloc(gHeap, 0, count * size);
  if (result)
    gAllocated.insert(result);
  return result;
}

HTMLAPIATTR HTStatus HTMLAPI HTMemFree(void *pointer) {
  std::lock_guard<std::mutex> lock(gMutex);
  auto it = gAllocated.find(pointer);

  if (it == gAllocated.end())
    return HT_FAIL;

  gAllocated.erase(pointer);
  HeapFree(gHeap, 0, pointer);

  return HT_SUCCESS;
}
