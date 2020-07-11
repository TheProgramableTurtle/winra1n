#pragma once
#define _WIN32_DCOM
#include <windows.h>
#include <iostream>
#include <conio.h>
#include <comdef.h>
#include <WbemIdl.h>
#include <fstream>

#pragma comment(lib, "wbemuuid.lib")

HRESULT checkState();
HRESULT setState();
HRESULT displayWelcome();
HRESULT restartDevice();