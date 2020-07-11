#ifndef _CRT_SECURE_NO_WARNINGS
    #define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include <Windows.h>

STARTUPINFO g_si_usbipd;
PROCESS_INFORMATION g_pi_usbipd;

STARTUPINFO g_si_usbip_client;
PROCESS_INFORMATION g_pi_usbip_client;

bool start_usbipd() {
    ZeroMemory(&g_si_usbipd, sizeof(g_si_usbipd));
    g_si_usbipd.cb = sizeof(g_si_usbipd);
    ZeroMemory(&g_pi_usbipd, sizeof(g_pi_usbipd));

    char text[] = "usbipd.exe";
    wchar_t wtext[16];
    mbstowcs(wtext, text, strlen(text) + 1);
    LPWSTR lpCommandLine = wtext;
 
    if (!CreateProcess(NULL,
        lpCommandLine,
        NULL,
        NULL,
        FALSE,
        0,
        NULL,
        NULL,
        &g_si_usbipd,
        &g_pi_usbipd)
        )
    {
        std::cout << ("CreateProcess failed (%d).\n", GetLastError());
        return 1;
    }
    return 0;
}

bool start_usbip_client(char* busID) {
    ZeroMemory(&g_si_usbip_client, sizeof(g_si_usbip_client));
    g_si_usbip_client.cb = sizeof(g_si_usbip_client);
    ZeroMemory(&g_pi_usbip_client, sizeof(g_pi_usbip_client));

    std::string text = "wsl.exe -d winra1n usbip attach -r $(awk '/nameserver / {print $2; exit}' /etc/resolv.conf 2>/dev/null) -b %s" + *busID;
    wchar_t wtext[16];
    mbstowcs(wtext, text.c_str(), text.length());
    LPWSTR lpCommandLine = wtext;

    if (!CreateProcess(NULL,
        lpCommandLine,
        NULL,
        NULL,
        FALSE,
        0,
        NULL,
        NULL,
        &g_si_usbip_client,
        &g_pi_usbip_client)
        )
    {
        std::cout << ("CreateProcess failed (%d).\n", GetLastError());
        return 1;
    }
    return 0;
}

int main(int argc, char* argv[]) {
	std::cout << "winra1n - an environment for running checkra1n on Windows.\n(c) TheProgramableTurtle\n";
    if (argc == 1) {
        std::cout << "Need busID, pls\n";
        return 1;
    }

    std::cout << "Binding device...\n";
    system("usbip.exe winra1n");

    if (start_usbipd()) {
        system("usbip.exe winra1n -u unbind");
        return 1;
    }

    std::cout << "Attaching device...\n";

    if (start_usbip_client(argv[1])) {
        system("usbip.exe winra1n -u unbind");
        return 1;
    }

    system("wsl -d winra1n \"sudo checkra1n -c --debug\"");
    CloseHandle(g_pi_usbip_client.hProcess);
    CloseHandle(g_pi_usbip_client.hThread);

    CloseHandle(g_pi_usbipd.hProcess);
    CloseHandle(g_pi_usbipd.hThread);
    system("usbip.exe winra1n -u unbind");
    
	return 0;
}