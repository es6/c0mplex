#pragma once
#include "../valve/centity.h"
#include "globals.h"
#include "../valve/icliententitylist.h"
#include "../valve/ivengineclient.h"
#include "../valve/ienginetraceclient.h"
#include "../valve/istudiorender.h"
#include "../valve/cmatrix.h"
#include "../valve/imaterialsystems.h"
#include "../valve/ibaseclientdll.h"
#include "../valve/iclientmodeshared.h"
#include "../valve/ivmodelinfo.h"

namespace interfaces {
	// call once to setup interfaces
	void Setup() noexcept;

	// capture interfaces via version striings
	// using exported CreateInterface function
	template <typename Interface>
	Interface* GetInterface(const char* name, const char* lib) noexcept;

	// interface pointers
	inline IBaseClientDLL* client = nullptr;
	inline IClientModeShared* clientMode = nullptr;
	inline IClientEntityList* entityList = nullptr;
	inline IVEngineClient* engine = nullptr;
	inline IEngineTraceClient* trace = nullptr;
	inline IStudioRender* studioRender = nullptr;
	inline IMaterialSystem* materialSystem = nullptr;
	inline IVModelInfo* modelInfo = nullptr;

	// keyValuesSystem
	inline void* keyValuesSystem = nullptr;
}