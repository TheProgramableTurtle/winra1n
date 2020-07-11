#include "checkra1n.h"

HRESULT setupUSBIP() {
	HRESULT hResult = 0;
	system("wsl -d winra1n \"sudo chmod +x ../script/setup_usbip.sh\"");
	system("wsl -d winra1n \"../script/setup_usbip.sh\"");
	return hResult;
}

HRESULT installCheckra1n() {
	HRESULT hResult = 0;
	system("wsl -d winra1n \"sudo chmod +x ../script/install_checkra1n.sh\"");
	system("wsl -d winra1n \"../script/install_checkra1n.sh\"");
	return hResult;
}