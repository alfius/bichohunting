#ifndef REGISTRYKEY
#define REGISTRYKEY

#include <windows.h>

class RegistryKeyException {};

class RegistryKey
{
	HKEY _handle;
	UINT _isRoot;

	RegistryKey(HKEY, UINT = 0);
public:
	// Destructors
	~RegistryKey();

	// Root keys
	static RegistryKey* ClassesRoot();
	static RegistryKey* CurrentUser();
	static RegistryKey* LocalMachine();
	static RegistryKey* Users();

	// Keys creation/destruction
	RegistryKey* OpenKey(const char* subKey, UINT readOnly = 0);
	RegistryKey* CreateKey(const char* subKey);
	void DeleteKey(const char* subKey = 0);

	// Data handling
	long GetLong(const char* valueName);
	void GetString(const char* valueName, char* buffer, size_t size);
	void GetBinary(const char* valueName, void* destStruct, size_t size);

	void SetLong(const char* valueName, long value);
	void SetString(const char* valueName, const char* value);
	void SetBinary(const char* valueName, const void* destStruct, size_t size);
};

#endif
