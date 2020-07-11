#include "install.h"
#include "device.h"
#include "wsl.h"
#include "hypervisor.h"
#include "distro.h"
#include "kernel.h"
#include "checkra1n.h"

HRESULT checkState() {
	HRESULT hResult = 0;
	char state[20];
	std::fstream file;
	file.open("winra1n.dat", std::ios::out || std::ios::in);
	file.getline(state, 20);
	file.close();
	if (strcmp(state, "123454321")) {
		return 1;
	}
	else {
		return 0;
	}
}

HRESULT setState() {
	HRESULT hResult = 0;
	std::fstream file;
	file.open("winra1n.dat", std::ios::out || std::ios::in);
	file << "123454321";
	file.close();
	return hResult;
}

HRESULT displayWelcome() {
	std::cout << "winra1n Setup\n";
	std::cout << "This program will configure a WSL2 environment with USB forwarding enabled for checkra1n to run in, setup a USB/IP server on the Windows host, and configure a libusbK driver to capture your iDevice while in DFU mode.\n\n";
	std::cout << "This program is highly experimental and your computer will have to be placed in Test Mode. Kernel drivers will be installed, and your iDevice will not be usable with any other software while the drivers are configured for this program. These modifications will put your device at great risk. DO NOT USE THIS ON A PRIMARY DEVICE!\n";
	std::cout << "NO warranty is provided with this software and you assume all risks. If you accept these terms, enter YES.\n";
	std::string confirm;
	std::cin >> confirm;
	if (confirm != "YES") {
		return E_FAIL;
	}
	std::cin.clear();
	std::cout << "\nYour device must first be checked for compatibility. Make sure this script is run with Administrative priviliges.\n";
	std::cout << "WSL2 requires Windows 10, Version 2004, Build 19041 or later. This software is not supported by any other version of Windows (Windows 7, Windows 8, Windows XP, etc) and never will be.\n";
	std::cout << "Hyper-V, the virtualization platform WSL2 depends on, requires a 64 bit processor. Hyper-V does not support Windows 10 Home, Mobile, ME, or CE.\n";
	std::cout << "If your device does not meet these requirements, you will not be able to run checkra1n on WSL.\n";
	std::cout << "See https://github.com/foxlet/bootra1n for an alternative.\n";
	std::cout << "Your device will now be checked for compatibility. Press enter to continue.\n";
	_getch();
	return 0;
}

HRESULT restartDevice() {
	HRESULT hResult = 0;
	std::cout << "Your device must now be restarted. After restarting, you will see a Test Mode watermark at the bottom left of your desktop. Run this program again to continue with setup. Save all work and press enter to continue...\n";
	_getch();
	setState();
	system("shutdown -r");
	return hResult;
}

int main() {
	HRESULT hResult;

	hResult = CoInitializeEx(0, COINIT_MULTITHREADED);

	if (FAILED(hResult)) {
		std::cout << "COM fail 0x" << std::hex << hResult << "\n";
		return hResult;
	}

	hResult = CoInitializeSecurity(
		NULL,
		-1,
		NULL,
		NULL,
		RPC_C_AUTHN_LEVEL_DEFAULT,
		RPC_C_IMP_LEVEL_IMPERSONATE,
		NULL,
		EOAC_NONE,
		NULL
	);
	
	if (FAILED(hResult)) {
		std::cout << "COM security fail 0x" << std::hex << hResult << "\n";
		return hResult;
	}

	UINT8* uiCompat = new UINT8;

	if (SUCCEEDED(checkState())) {
		hResult = displayWelcome();

		if (FAILED(hResult)) {
			return hResult;
		}

		hResult = ensureCompat();

		if (FAILED(hResult)) {
			return hResult;
		}

		hResult = enableHypervisor();

		if (FAILED(hResult)) {
			return hResult;
		}

		hResult = enableWSL2();

		if (FAILED(hResult)) {
			return hResult;
		}

		hResult = enableTestMode();

		if (FAILED(hResult)) {
			return hResult;
		}

		hResult = restartDevice();

		if (FAILED(hResult)) {
			return hResult;
		}
	}
	else {
		hResult = installDistro();

		if (FAILED(hResult)) {
			return hResult;
		}

		hResult = buildKernel();

		if (FAILED(hResult)) {
			return hResult;
		}

		hResult = setKernel();

		if (FAILED(hResult)) {
			return hResult;
		}

		hResult = setupUSBIP();

		if (FAILED(hResult)) {
			return hResult;
		}

		hResult = installCheckra1n();

		if (FAILED(hResult)) {
			return hResult;
		}
	}

	CoUninitialize();

	delete uiCompat;

	return 0;
}