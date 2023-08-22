#include <Windows.h>
#include <iostream>

template <typename T>
T* GetInterface(const char* name, const char* library) {
	const auto handle = GetModuleHandle(library);

	if (!handle)
		return nullptr;

	const auto functionAddress = GetProcAddress(handle, "CreateInterface");

	if (!functionAddress)
		return nullptr;

	/*
	Gets the address of CreateInterface and cast it to our own "Fn" function pointer

	*/

	using Fn = T * (*)(const char*, int*);
	const auto CreateInterface = (Fn)functionAddress;

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
