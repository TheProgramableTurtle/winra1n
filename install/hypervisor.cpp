#include "hypervisor.h"

HRESULT enableHypervisor() {
	HRESULT hResult = 0;
	system("dism.exe /online /enable-feature /featurename:VirtualMachinePlatform /all /norestart\"");
	std::cout << "\nIf an error occured, terminate the program. (I will do this properly with the DISM API later). Otherwise, press enter to continue...\n\n";
	_getch();
	return hResult;
}

HRESULT enableTestMode() {
	HRESULT hResult = 0;
	system("bcdedit /set testsigning on");
	std::cout << "\nLast time, if an error occured, terminate the program. Otherwise, press enter to continue...\n\n";
	_getch();
	return hResult;
}