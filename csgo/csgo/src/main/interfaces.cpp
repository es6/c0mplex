#include <Windows.h>
#include <iostream>
#include "../main/interfaces.h"

void interfaces::Setup() noexcept {
	auto entityList = GetInterface<IClientEntityList>("VClientEntityList003", "client.dll");
	engine = GetInterface<IEngineClient>("VEngineClient014", "engine.dll");
	trace = GetInterface<IEngineTraceClient>("EngineTraceClient004", "engine.dll");

	client = GetInterface<void>("VClient018", "client.dll");
	clientMode = **reinterpret_cast<void***>((*reinterpret_cast<unsigned int**>(client))[10] + 5);
};

template <typename Interface>
Interface* GetInterface(const char* name, const char* lib) {
	const auto handle = GetModuleHandle(lib);

	if (!handle)
		return nullptr;

	// Gets the address of CreateInterface and cast it to our own "Fn" function pointer
	const auto functionAddress = GetProcAddress(handle, "CreateInterface");

	if (!functionAddress)
		return nullptr;

	using Function = T * (*)(const char*, int*);
	Function CreateInterface = reinterpret_cast<Function>functionAddress;

	return CreateInterface(name, nullptr);
}

class CEntity {
public:
	const int& GetHealth() const noexcept {
		return *(int*)(std::uintptr_t(this) + 0x100);
	}
};

class IClientEntityList
{
public:
	// Get IClientNetworkable interface for specified entity
	virtual void* GetClientNetworkable(int entnum) = 0;
	virtual void* GetClientNetworkableFromHandle(unsigned long hEnt) = 0;
	virtual void* GetClientUnknownFromHandle(unsigned long hEnt) = 0;

	// NOTE: This function is only a convenience wrapper.
	// It returns GetClientNetworkable( entnum )->GetIClientEntity().
	virtual CEntity* GetClientEntity(int entnum) = 0;
	virtual CEntity* GetClientEntityFromHandle(unsigned long hEnt) = 0;

	// Returns number of entities currently in use
	virtual int					NumberOfEntities(bool bIncludeNonNetworkable) = 0;

	// Returns highest index actually used
	virtual int					GetHighestEntityIndex(void) = 0;

	// Sizes entity list to specified size
	virtual void				SetMaxEntities(int maxents) = 0;
	virtual int					GetMaxEntities() = 0;
};

/*
HackThread Example

void HackThread(HMODULE instance) {
	const auto entityList = GetInterface<IClientEntityList>("VClientEntityList003", "client.dll");

	// loop through entity list
	while (!GetAsyncKeyState(VK_END)) {
		if (GetAsyncKeySTate(VK_DELETE) & 1) {
			for (auto i = 1; i < 64; ++i) {
				const auto player = entityList->GetClientEntity(i);

				if (!player) 
					continue;
				std::cout << "Player " << i << " health = " << player->GetHealth() << endl;
			}
		}
	}
}


*/
