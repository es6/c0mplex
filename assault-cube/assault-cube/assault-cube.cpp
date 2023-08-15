#include <iostream>
#include <Windows.h>

using namespace std;


class Bypass {
public: 
	Bypass() {
		// Empty constructor for now
	}

	~Bypass() {
		if (m_hProcess != NULL) {
			BOOL closeHandle = CloseHandle(m_hProcess);
			if (closeHandle == 0) {
				cout << "CloseHandle failed, GetLastError() = " << GetLastError();
				system("pause");
			}
		}
	}

	BOOL attach(DWORD pid) {
		m_hProcess = OpenProcess(PROCESS_VM_OPERATION, FALSE, pid);
		if (m_hProcess == NULL) {
			return false;
		}
		else {
			return true;
		}
	}

	BOOL read(uintptr_t lpBaseAddress, void* lpBuffer, SIZE_T nSize) {
		BOOL rpm = ReadProcessMemory(m_hProcess, (LPCVOID)lpBaseAddress, lpBuffer, nSize, NULL);
		if (rpm == FALSE) {
			return false;
		}
		else {
			return true;
		}
	}

	BOOL write(uintptr_t lpBaseAddress, void* lpBuffer, SIZE_T nSize) {
		BOOL wpm = WriteProcessMemory(m_hProcess, (LPVOID)lpBaseAddress, lpBuffer, nSize, NULL);
		if (wpm == FALSE) {
			return false;
		}
		else {
			return true;
		}
	}
private:
	HANDLE m_hProcess = NULL;
};