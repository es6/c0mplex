#pragma once
#include "entity.h"
#include "datatable.h"

class IEntityList {
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

inline IEntityList* entityList = nullptr;
inline IClient* client = nullptr;
static void* g_Client = nullptr;
static void* g_ClientMode = nullptr;