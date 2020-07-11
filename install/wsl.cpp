#include "wsl.h"

HRESULT enableWSL() {
	HRESULT hResult = 0;
	system("powershell -command \"dism.exe /online /enable-feature /featurename:Microsoft-Windows-Subsystem-Linux /all /norestart\"");
	std::cout << "\nAgain, if an error occured, terminate the program. Otherwise, press enter to continue...\n\n";
	_getch();
	return hResult;
}

HRESULT setWSL2() {
	HRESULT hResult = 0;
	system("wsl --set-default-version 2");
	std::cout << "Your device will now use WSL2 by default. If any errors occured, or an update is required, terminate this program, install the update, and run the program again.\nOtherwise, press enter to continue...\n";
	_getch();
	return hResult;
}

HRESULT enableWSL2() {
	HRESULT hResult = 0;
	hResult = enableWSL();

	if (FAILED(hResult)) {
		return hResult;
	}

	hResult = setWSL2();

	if (FAILED(hResult)) {
		return hResult;
	}

	return hResult;
}