#include "distro.h"

HRESULT installDistro() {
	HRESULT hResult = 0;
	system("../script/install_distro.bat");
	return hResult;
}