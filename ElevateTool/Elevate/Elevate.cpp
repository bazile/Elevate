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
		//int len =
	}
	//pArgs->pParameters = argc > 1 ? argv[1] : nullptr;
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
