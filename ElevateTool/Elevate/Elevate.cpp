#include "stdafx.h"

struct elevateArgs
{
	wchar_t* pFile;
	wchar_t* pParameters;
	bool pauseBeforeExit;
};

void printHelp()
{
	//..
}

elevateArgs* parseCommandLine(int argc, wchar_t** argv)
{
	elevateArgs* pArgs = new elevateArgs();
	pArgs->pauseBeforeExit = false;
	pArgs->pFile = argc > 0 ? argv[0] : nullptr;
	if (argc <= 1)
		pArgs->pParameters = nullptr;
	else if (argc == 2)
		pArgs->pParameters = argv[1];
	else
	{
		int len = 0;
		for (int i = 1; i < argc; i++)
		{
			len += lstrlenW(argv[i]);
		}
		int spaceCount = argc - 2;
		len += spaceCount + 1;
		wchar_t* pParameters = new wchar_t[len];
		ZeroMemory(pParameters, len*sizeof(wchar_t));
		for (int i = 1; i < argc; i++)
		{
			StringCchCatW(pParameters, len, argv[i]);
			if (spaceCount > 0)
			{
				StringCchCatW(pParameters, len, L" ");
				spaceCount--;
			}
		}
		pArgs->pParameters = pParameters;
	}
	return pArgs;
}

LPCWSTR getCmdPath()
{
	LPWSTR lpComSpec = new wchar_t[MAX_PATH];
	ExpandEnvironmentStringsW(L"%ComSpec%", lpComSpec, MAX_PATH);
	return static_cast<LPCWSTR>(lpComSpec);
}

LPCWSTR getCmdParameters(LPCWSTR lpFile, LPCWSTR lpParameters)
{
	int newLen = lstrlenW(lpFile) + lstrlenW(lpParameters) + 30;
	LPWSTR lpCmdParameters = new wchar_t[newLen];
	StringCchCopyW(lpCmdParameters, newLen, L"/c \"\"");
	StringCchCatW(lpCmdParameters, newLen, lpFile);
	StringCchCatW(lpCmdParameters, newLen, L"\" ");
	StringCchCatW(lpCmdParameters, newLen, lpParameters);
	StringCchCatW(lpCmdParameters, newLen, L"\" & pause");
	return static_cast<LPCWSTR>(lpCmdParameters);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR lpCmdLine, int nCmdShow)
{
	int argc;
	LPWSTR* argv = CommandLineToArgvW(lpCmdLine, &argc);
	if (argc == 0)
	{
		printHelp();
		return 0;
	}
	elevateArgs* args = parseCommandLine(argc, argv);

	CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

	//AttachConsole(ATTACH_PARENT_PROCESS);

	SHELLEXECUTEINFOW sei;
	ZeroMemory(&sei, sizeof(SHELLEXECUTEINFOW));
	sei.cbSize = sizeof(SHELLEXECUTEINFOW);
	sei.fMask = SEE_MASK_NOCLOSEPROCESS;
	if (IsWindowsVistaOrGreater())
	{
		sei.lpVerb = L"runas";
	}
	if (args->pauseBeforeExit)
	{
		sei.lpFile = getCmdPath();
		sei.lpParameters = getCmdParameters(args->pFile, args->pParameters);
	}
	else
	{
		sei.lpFile = args->pFile;
		sei.lpParameters = args->pParameters;
	}
	sei.nShow = SW_SHOWNORMAL;
	if (TRUE == ShellExecuteExW(&sei))
	{
		CloseHandle(sei.hProcess);
	}

	return 0;
}
