#pragma once
#include "install.h"

HRESULT ensureBuildCompat(IWbemServices*);
HRESULT ensureHypervisorCompat(IWbemServices*);
HRESULT ensureCompat();