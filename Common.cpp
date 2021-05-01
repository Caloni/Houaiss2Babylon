#include "Common.h"


int GetHouaissPath(char houaissPath[MAX_PATH])
{
	int ret = -1;
	HKEY houaissKey;
	DWORD maxPath = MAX_PATH;
	
	LONG err = RegOpenKeyEx(HKEY_LOCAL_MACHINE, 
		"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\Houaiss",
		0, KEY_QUERY_VALUE, &houaissKey);

	if( err == ERROR_SUCCESS )
	{
		err = RegQueryValueEx(houaissKey, "UninstallString", 0, NULL, 
			(PBYTE) houaissPath, &maxPath);

		if( err == ERROR_SUCCESS )
		{
			if( char* p = strrchr(houaissPath, '\\') )
				*p = 0;
			ret = 0;
		}

		RegCloseKey(houaissKey);
	}

	return ret;
}


int GetBabylonBuilderPath(char babBuildPath[MAX_PATH])
{
	int ret = -1;
	HKEY babKey;
	DWORD maxPath = MAX_PATH;
	
	LONG err = RegOpenKeyEx(HKEY_LOCAL_MACHINE, 
		"SOFTWARE\\Classes\\CLSID\\{3A207676-6B05-4AE6-BAF0-9BF312F52444}\\InprocServer32",
		0, KEY_QUERY_VALUE, &babKey);

	if( err == ERROR_SUCCESS )
	{
		err = RegQueryValueEx(babKey, "", 0, NULL, 
			(PBYTE) babBuildPath, &maxPath);

		if( err == ERROR_SUCCESS )
		{
			if( char* p = strrchr(babBuildPath, '\\') )
				*p = 0;
			ret = 0;
		}

		RegCloseKey(babKey);
	}

	return ret;
}
