#pragma once
#include "FNVHashing.h"
#include "datatable.h"

#include <unordered_map>

// Call once to dump netvars
void SetupNetvars();

// Recursive dump
void Dump(const char* baseClass, RecvTable* table, std::uint32_t offset = 0);

// Store offsets
// Store hash netvar name as key, value is the offset
inline std::unordered_map<std::uint32_t, std::uint32_t> netvars;

#define NETVAR(func_name, netvar, type) type& func_name() \
{ \
	static auto offset = netvars[fnv::HashConst(netvar)]; \
	return *(type*)(std::uint32_t(this) + offset); \
}