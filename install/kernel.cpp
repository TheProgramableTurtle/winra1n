#include "kernel.h"

HRESULT buildKernel() {
	HRESULT hResult = 0;
	system("wsl -d winra1n \"sudo chmod +x ../script/build_kernel.sh\"");
	system("wsl -d winra1n \"../script/build_kernel.sh\"");
	return hResult;
}

HRESULT setKernel() {
	HRESULT hResult = 0;
	char pBuf[256];
	char** usrDir = new char*;
	size_t* buffSize = new size_t(256);

	_dupenv_s(usrDir, buffSize, "USER");
	strcat_s(*usrDir, *buffSize, "/.wslconfig");
	size_t len = sizeof(pBuf);

	std::fstream file;
	file.open(*usrDir, std::ios::out);
	file << "[wsl2]\nkernel=" << GetModuleFileNameA(NULL, pBuf, len);
	file.close();

	return hResult;
}