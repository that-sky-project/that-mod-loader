#pragma once

// Define attributes for all API symbol declarations.
// - C++ API and class attributes.
//#define LEVELDB_DLLX __declspec(dllexport)
//#define LEVELDB_DLLX __declspec(dllimport)

// - C API attributes.
//#define LEVELDB_C_DLLX __declspec(dllexport)
//#define LEVELDB_C_DLLX __declspec(dllimport)

// Disable compressor declarations.
#define LEVELDB_DISABLE_SNAPPY
//#define LEVELDB_DISABLE_ZLIB
