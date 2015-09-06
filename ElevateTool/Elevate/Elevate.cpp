#include "stdafx.h"


void printHelp()
{
	//..
}

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc == 1)
	{
		printHelp();
		return 0;
	}

	//AttachConsole(ATTACH_PARENT_PROCESS);

	SHELLEXECUTEINFOW sei;
	ZeroMemory(&sei, sizeof(SHELLEXECUTEINFOW));
	sei.cbSize = sizeof(SHELLEXECUTEINFOW);
	sei.fMask = SEE_MASK_NOCLOSEPROCESS;
	if (IsWindowsVistaOrGreater())
	{
		sei.lpVerb = L"runas";
	}
	sei.lpFile = argv[1];
	sei.lpParameters = argc > 2 ? argv[2] : nullptr;
	sei.nShow = SW_SHOWNORMAL;
	if (TRUE == ShellExecuteExW(&sei))
	{
		CloseHandle(sei.hProcess);
	}

	return 0;
}
