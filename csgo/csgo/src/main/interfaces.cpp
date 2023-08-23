#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "../main/interfaces.h"

void interfaces::Setup() noexcept {
	client = GetInterface<IBaseClientDLL>("client.dll", "VClient018");
	entityList = GetInterface<IClientEntityList>("client.dll", "VClientEntityList003");
	engine = GetInterface<IVEngineClient>("engine.dll", "VEngineClient014");
	trace = GetInterface<IEngineTraceClient>("engine.dll", "EngineTraceClient004");
	clientMode = **reinterpret_cast<IClientModeShared***>((*reinterpret_cast<unsigned int**>(client))[10] + 5);
	materialSystem = GetInterface<IMaterialSystem>("VMaterialSystem080", "materialsystem.dll");
	studioRender = GetInterface<IStudioRender>("VStudioRender026", "studiorender.dll");
	modelInfo = GetInterface<IVModelInfo>("VModelInfoClient004", "engine.dll");
};

template <typename Interface>
Interface* interfaces::GetInterface(const char* name, const char* lib) noexcept {
	const auto handle = GetModuleHandle(lib);

	if (!handle)
		return nullptr;

	// Gets the address of CreateInterface and cast it to our own "Fn" function pointer
	const auto functionAddress = GetProcAddress(handle, "CreateInterface");

	if (!functionAddress)
		return nullptr;

	using Function = Interface * (*)(const char*, int*);
	Function CreateInterface = reinterpret_cast<Function>(functionAddress);

	return CreateInterface(name, nullptr);
}