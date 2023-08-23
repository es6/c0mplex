#include "netvar.h"
#include "interfaces.h"

#include <ctype.h>
#include <format>

void SetupNetvars() {
	// loop through LL
	for (auto clientClass = interfaces::client2->GetAllClasses(); clientClass != nullptr; clientClass = clientClass->next) {
		// check for valid table
		if (clientClass->recvTable) {
			// recursive dump
			Dump(clientClass->networkName, clientClass->recvTable);
		}
	}
}

void Dump(const char* baseClass, RecvTable* table, std::uint32_t offset) {
	for (auto i = 0; i < table->propsCount; ++i) {
		const auto prop = &table->props[i];

		// checks if prop is valid and if the prop starts with a digit
		// - props with a digit are not valid for us
		if (!prop || isdigit(prop->varName[0]))
			continue;
		
		// Check that we're not storing a baseclass
		if (fnv::Hash(prop->varName) == fnv::HashConst("baseclass"))
			continue;

		/* Checks that our prop is of type DATATABLE
		   Then checks for valid data table
		   Then checks for the first table name to be D
		   If this is true, there are more props to dump
		*/
		if (prop->recvType == SendPropType::DATATABLE &&
			prop->dataTable &&
			prop->dataTable->tableName[0] == 'D')
			Dump(baseClass, prop->dataTable, offset + prop->offset);

		const auto netvarName = std::format("{}->{}", baseClass, prop->varName);

		netvars[fnv::Hash(netvarName.c_str())] = offset + prop->offset;
	}
}


/*
Netvar Usage Example with radar hack
 
void HackThread(HMODULE instance) {
	client = GetInterface<IClient>("VClient018", "client.dll");
	entityList = GetInterface<IEntityList>("VClientEntityList003", "client.dll");

	SetupNetvars();
	
	while (!GetAsyncKeyState(VK_END)) {
		for (auto i = 1; i <= 64, ++i) {
			const auto entity = entityList->GetClientEntity(i);

			if (!entity)
				continue;
			// check if dormant, if alive, if enemy

			entity->Spotted() = true;
		}
	}

	FreeLibraryAndExitThread(instance, 0);
}
*/