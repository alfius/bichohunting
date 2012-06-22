#include "RegistryKey.h"

RegistryKey::RegistryKey(HKEY hKey, UINT isRoot)
{
	_handle = hKey;
	_isRoot = isRoot;
}

RegistryKey::~RegistryKey()
{
	if (!_isRoot)
	{
		RegCloseKey(_handle);
	}
}

RegistryKey* RegistryKey::ClassesRoot()
{
	return new RegistryKey(HKEY_CLASSES_ROOT, 1);
}

RegistryKey* RegistryKey::CurrentUser()
{
	return new RegistryKey(HKEY_CURRENT_USER, 1);
}

RegistryKey* RegistryKey::LocalMachine()
{
	return new RegistryKey(HKEY_LOCAL_MACHINE, 1);
}

RegistryKey* RegistryKey::Users()
{
	return new RegistryKey(HKEY_USERS, 1);
}

RegistryKey* RegistryKey::OpenKey(const char* subKey, UINT readOnly)
{
	HKEY handle;
	if (ERROR_SUCCESS == RegOpenKeyEx(_handle, subKey, 0, readOnly?KEY_READ:KEY_ALL_ACCESS,&handle))
	{
		return new RegistryKey(handle);
	}
	else
	{
		return 0;
	}
}

RegistryKey* RegistryKey::CreateKey(const char* subKey)
{
	HKEY handle;
	DWORD result;
	RegCreateKeyEx(_handle, subKey, 0, 0, 0, KEY_ALL_ACCESS, 0, &handle, &result);
	return new RegistryKey(handle);
}

void RegistryKey::DeleteKey(const char* subKey)
{
	RegDeleteKey(_handle, subKey);
}

long RegistryKey::GetLong(const char* valueName)
{
	long* data = new long();
	DWORD sizeAsDWord = (DWORD)sizeof(long);
	if (RegQueryValueEx(_handle, valueName, 0, 0, (LPBYTE)data, &sizeAsDWord) == ERROR_FILE_NOT_FOUND)
		throw new RegistryKeyException();
	long res = *data;
	delete data;
	return res;
}

void RegistryKey::GetString(const char* valueName, char* buffer, size_t size)
{
	DWORD sizeAsDWord = (DWORD)size;
	if (RegQueryValueEx(_handle, valueName, 0, 0, (LPBYTE)buffer, &sizeAsDWord) == ERROR_FILE_NOT_FOUND)
		throw new RegistryKeyException();
}

void RegistryKey::GetBinary(const char* valueName, void* destStruct, size_t size)
{
	DWORD sizeAsDWord = (DWORD)size;
	if (RegQueryValueEx(_handle, valueName, 0, 0, (LPBYTE)destStruct, &sizeAsDWord) == ERROR_FILE_NOT_FOUND)
		throw new RegistryKeyException();
}

void RegistryKey::SetLong(const char* valueName, long value)
{
	long* data = new long();
	*data = value;
	RegSetValueEx(_handle, valueName, 0, REG_DWORD, (LPBYTE)data, sizeof(long));
	delete data;
}

void RegistryKey::SetString(const char* valueName, const char* value)
{
	RegSetValueEx(_handle, valueName, 0, REG_SZ, (LPBYTE)value, strlen(value)+1);
}

void RegistryKey::SetBinary(const char* valueName, const void* destStruct, size_t size)
{
	RegSetValueEx(_handle, valueName, 0, REG_BINARY, (LPBYTE)destStruct, size);
}



