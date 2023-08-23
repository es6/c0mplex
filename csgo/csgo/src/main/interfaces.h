#pragma once
#include "../valve/centity.h"
#include "datatable.h"
#include "globals.h"
#include "../valve/icliententitylist.h"
#include "../valve/iengineclient.h"
#include "../valve/ienginetraceclient.h"

namespace interfaces {
	// call once to setup interfaces
	void Setup() noexcept;

	// capture interfaces via version striings
	// using exported CreateInterface function
	template <typename T>
	T* GetInterface(const char* name, const char* lib) noexcept;

	inline IClientEntityList* entityList = nullptr;
	inline IEngineClient* engine = nullptr;
	inline IEngineTraceClient* trace = nullptr;

	inline void* client = nullptr;
	inline IClient* client2 = nullptr;
	inline void* clientMode = nullptr;
}

class IClientEntityList {
private:
	virtual void Unused0() = 0;
	virtual void Unused1() = 0;
	virtual void Unused2() = 0;
public:
	virtual CEntity* GetClientEntity(int index) = 0;
};

class IClient {
private:
	virtual void Unused0() = 0;
	virtual void Unused1() = 0;
	virtual void Unused2() = 0;
	virtual void Unused3() = 0;
	virtual void Unused4() = 0;
	virtual void Unused5() = 0;
	virtual void Unused6() = 0;
	virtual void Unused7() = 0;
public:
	virtual ClientClass* GetAllClasses(void) = 0;
};