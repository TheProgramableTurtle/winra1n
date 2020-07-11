#include "device.h"

HRESULT ensureBuildCompat(IWbemServices* pSvc) {
	HRESULT hResult;

	IEnumWbemClassObject* pEnumerator = NULL;
	hResult = pSvc->ExecQuery(
		bstr_t("WQL"),
		bstr_t("SELECT * FROM Win32_OperatingSystem"),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
		NULL,
		&pEnumerator
	);

	if (FAILED(hResult)) {
		std::cout << "Query fail 0x" << std::hex << hResult << "\n";
		return hResult;
	}

	IWbemClassObject* pclsObj = NULL;
	ULONG uReturn = 0;

	while (pEnumerator) {
		hResult = pEnumerator->Next(
			WBEM_INFINITE,
			1,
			&pclsObj,
			&uReturn
		);

		if (!uReturn) {
			break;
		}

		VARIANT vtProp;

		std::cout << "\nChecking for a supported Windows build... ";
		hResult = pclsObj->Get(
			L"BuildNumber",
			0,
			&vtProp,
			0,
			0
		);

		if ((int)vtProp.bstrVal < 19041) {
			std::cout << "no\n\n";
			std::wcout << "Your device is running build " << vtProp.bstrVal << " but this software required build 19041 or later, update to Windows 10 version 2004.\n";
			return -1;
		}
		else {
			std::cout << "yes\n";
		}

		std::cout << "Cheking for a supported system architecture... ";
		hResult = pclsObj->Get(
			L"OSArchitecture",
			0,
			&vtProp,
			0,
			0
		);

		if (!strcmp((const char*)vtProp.bstrVal, "64-bit")) {
			std::cout << "no\n\n";
			std::wcout << "Your processor has a " << vtProp.bstrVal << " processor architecture, but this software requires a 64-bit processor.\n";
			return -1;
		}
		else {
			std::cout << "yes\n";
		}

		std::cout << "Cheking for a supported Windows version... ";
		hResult = pclsObj->Get(
			L"Caption",
			0,
			&vtProp,
			0,
			0
		);

		if (strcmp((const char*)vtProp.bstrVal, "Microsoft Windows 10 Home") || strcmp((const char*)vtProp.bstrVal, "Microsoft Windows 10 Mobile") || strcmp((const char*)vtProp.bstrVal, "Microsoft Windows 10 Mobile Enterprise")) {
			std::cout << "no\n\n";
			std::wcout << "Your system is running " << vtProp.bstrVal << ", but this software requires Windows 10 Pro, Windows 10 Enterprise, or Windows 10 Education.\n";
			return 0;
		}
		else {
			std::cout << "yes\n";
		}
		pclsObj->Release();
	}

	pEnumerator->Release();
	return hResult;
}
HRESULT ensureHyperVCompat(IWbemServices* pSvc) {
	HRESULT hResult;

	IEnumWbemClassObject* pEnumerator = NULL;
	hResult = pSvc->ExecQuery(
		bstr_t("WQL"),
		bstr_t("SELECT * FROM Win32_Processor"),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
		NULL,
		&pEnumerator
	);

	if (FAILED(hResult)) {
		std::cout << "Query fail 0x" << std::hex << hResult << "\n";
		return hResult;
	}

	IWbemClassObject* pclsObj = NULL;
	ULONG uReturn = 0;

	while (pEnumerator) {
		hResult = pEnumerator->Next(
			WBEM_INFINITE,
			1,
			&pclsObj,
			&uReturn
		);

		if (!uReturn) {
			break;
		}

		VARIANT vtProp;

		std::cout << "Checking device for Hyper-V compatibility...\n";
		std::cout << "Checking for VM monitor mode extensions... ";

		hResult = pclsObj->Get(
			L"VMMonitorModeExtensions",
			0,
			&vtProp,
			0,
			0
		);


		if (!vtProp.boolVal) {
			std::cout << "yes\n";
		}
		else {
			std::cout << "no\n\nYour device is not supported by Hyper-V because VM monitor mode extensions are not enabled.\nSee https://docs.microsoft.com/en-us/virtualization/hyper-v-on-windows/reference/hyper-v-requirements for more information\n";
			return -1;
		}

		std::cout << "Checking device for virtualization enabled in firmware... ";

		hResult = pclsObj->Get(
			L"VirtualizationFirmwareEnabled",
			0,
			&vtProp,
			0,
			0
		);

		if (vtProp.boolVal) {
			std::cout << "yes\n";
		}
		else {
			std::cout << "no\n\nYou must enable virtualization in your devices firmware settings (BIOS).\nSee https://docs.microsoft.com/en-us/virtualization/hyper-v-on-windows/reference/hyper-v-requirements for more information\n";
			return -1;
		}

		std::cout << "Checking device for second level address translation... ";

		hResult = pclsObj->Get(
			L"SecondLevelAddressTranslationExtensions",
			0,
			&vtProp,
			0,
			0
		);

		if (!vtProp.boolVal) {
			std::cout << "yes\n";
		}
		else {
			std::cout << "no\n\nYour device is not supported by Hyper-V because your processor does not support second level address translation.\nSee https://docs.microsoft.com/en-us/virtualization/hyper-v-on-windows/reference/hyper-v-requirements for more information\n";
			return -1;
		}
	}

	hResult = pSvc->ExecQuery(
		bstr_t("WQL"),
		bstr_t("SELECT * FROM Win32_OperatingSystem"),
		WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
		NULL,
		&pEnumerator
	);

	if (FAILED(hResult)) {
		std::cout << "Query fail 0x" << std::hex << hResult << "\n";
		return hResult;
	}

	while (pEnumerator) {
		hResult = pEnumerator->Next(
			WBEM_INFINITE,
			1,
			&pclsObj,
			&uReturn
		);

		if (!uReturn) { break; }

		VARIANT vtProp;

		std::cout << "Checking device for data execution prevention... ";

		hResult = pclsObj->Get(
			L"DataExecutionPrevention_Available",
			0,
			&vtProp,
			0,
			0
		);

		if (vtProp.boolVal) {
			std::cout << "yes\n";
		}
		else {
			std::cout << "no\n\nYour device is not supported by Hyper-V because data execution prevention is not avaliable\nSee https://docs.microsoft.com/en-us/virtualization/hyper-v-on-windows/reference/hyper-v-requirements for more information\n";
			return -1;
		}

		pclsObj->Release();
	}

	pEnumerator->Release();
	return hResult;
}

HRESULT ensureCompat() {
	HRESULT hResult;

	IWbemLocator* pLoc = NULL;

	hResult = CoCreateInstance(
		CLSID_WbemLocator,
		0,
		CLSCTX_INPROC_SERVER,
		IID_IWbemLocator, (LPVOID*)&pLoc
	);

	if (FAILED(hResult)) {
		std::cout << "IWbem fail 0x" << std::hex << hResult << "\n";
		return hResult;
	}

	IWbemServices* pSvc = nullptr;

	hResult = pLoc->ConnectServer(
		_bstr_t(L"ROOT\\CIMV2"),
		NULL,
		NULL,
		0,
		NULL,
		0,
		0,
		&pSvc
	);

	if (FAILED(hResult)) {
		std::cout << "WMI connect fail 0x" << std::hex << hResult << "\n";
		pLoc->Release();
		CoUninitialize();
		return hResult;
	}

	hResult = CoSetProxyBlanket(
		pSvc,
		RPC_C_AUTHN_WINNT,
		RPC_C_AUTHZ_NONE,
		NULL,
		RPC_C_AUTHN_LEVEL_CALL,
		RPC_C_IMP_LEVEL_IMPERSONATE,
		NULL,
		EOAC_NONE
	);

	if (FAILED(hResult)) {
		std::cout << "Proxy blanket fail 0x" << std::hex << hResult << "\n";
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return hResult;
	}

	hResult = ensureBuildCompat(pSvc);

	if (FAILED(hResult)) {
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return hResult;
	}

	hResult = ensureHyperVCompat(pSvc);

	if (FAILED(hResult)) {
		pSvc->Release();
		pLoc->Release();
		CoUninitialize();
		return hResult;
	}

	pSvc->Release();
	pLoc->Release();
	CoUninitialize();

	return hResult;
}