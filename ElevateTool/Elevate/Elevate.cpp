#include "stdafx.h"

struct elevateArgs
{
	wchar_t* pFile;
	wchar_t* pParameters;

};

void printHelp()
{
	//..
}

elevateArgs* parseCommandLine(int argc, wchar_t** argv)
{
	elevateArgs* pArgs = new elevateArgs();
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
		int remainingSize = len + 1 + spaceCount;
		wchar_t* pParameters = new wchar_t[remainingSize];
		wchar_t* pTemp = pParameters;
		for (int i = 1, pos = 0; i < argc; i++)
		{
			int curLen = lstrlenW(argv[i]);
			StringCchCopyW(pTemp, remainingSize, argv[i]);
			remainingSize -= curLen;
			pTemp += curLen;

			if (spaceCount > 0)
			{
				StringCchCopyW(pTemp, remainingSize, L" ");
				remainingSize -= 1;
				pTemp += 1;
				spaceCount--;
			}
		}
		pArgs->pParameters = pParameters;
	}
	return pArgs;
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
	sei.lpFile = args->pFile;
	sei.lpParameters = args->pParameters;
	sei.nShow = SW_SHOWNORMAL;
	if (TRUE == ShellExecuteExW(&sei))
	{
		CloseHandle(sei.hProcess);
	}

	return 0;
}
