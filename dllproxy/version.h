#define WIN32_LEAN_AND_MEAN
#include <windows.h>

extern void on_attach(void);
extern void on_detach(void);

static HMODULE hVersion;

typedef BOOL (WINAPI *GetFileVersionInfoA_t)(LPCSTR, DWORD, DWORD, LPVOID);
typedef BOOL (WINAPI *GetFileVersionInfoByHandle_t)(DWORD, HANDLE, LPVOID*, PDWORD);
typedef BOOL (WINAPI *GetFileVersionInfoExA_t)(DWORD, LPCSTR, DWORD, DWORD, LPVOID);
typedef BOOL (WINAPI *GetFileVersionInfoExW_t)(DWORD, LPCWSTR, DWORD, DWORD, LPVOID);
typedef DWORD (WINAPI *GetFileVersionInfoSizeA_t)(LPCSTR, LPDWORD);
typedef DWORD (WINAPI *GetFileVersionInfoSizeExA_t)(DWORD, LPCSTR, LPDWORD);
typedef DWORD (WINAPI *GetFileVersionInfoSizeExW_t)(DWORD, LPCWSTR, LPDWORD);
typedef DWORD (WINAPI *GetFileVersionInfoSizeW_t)(LPCWSTR, LPDWORD);
typedef BOOL (WINAPI *GetFileVersionInfoW_t)(LPCWSTR, DWORD, DWORD, LPVOID);
typedef DWORD (WINAPI *VerFindFileA_t)(DWORD, LPCSTR, LPCSTR, LPCSTR, LPSTR, PUINT, LPSTR, PUINT);
typedef DWORD (WINAPI *VerFindFileW_t)(DWORD, LPCWSTR, LPCWSTR, LPCWSTR, LPWSTR, PUINT, LPWSTR, PUINT);
typedef DWORD (WINAPI *VerInstallFileA_t)(DWORD, LPCSTR, LPCSTR, LPCSTR, LPCSTR, LPCSTR, LPSTR, PUINT);
typedef DWORD (WINAPI *VerInstallFileW_t)(DWORD, LPCWSTR, LPCWSTR, LPCWSTR, LPCWSTR, LPCWSTR, LPWSTR, PUINT);
typedef DWORD (WINAPI *VerLanguageNameA_t)(DWORD, LPSTR, DWORD);
typedef DWORD (WINAPI *VerLanguageNameW_t)(DWORD, LPWSTR, DWORD);
typedef BOOL (WINAPI *VerQueryValueA_t)(LPCVOID, LPCSTR, LPVOID*, PUINT);
typedef BOOL (WINAPI *VerQueryValueW_t)(LPCVOID, LPCWSTR, LPVOID*, PUINT);

static GetFileVersionInfoA_t pGetFileVersionInfoA = NULL;
static GetFileVersionInfoByHandle_t pGetFileVersionInfoByHandle = NULL;
static GetFileVersionInfoExA_t pGetFileVersionInfoExA = NULL;
static GetFileVersionInfoExW_t pGetFileVersionInfoExW = NULL;
static GetFileVersionInfoSizeA_t pGetFileVersionInfoSizeA = NULL;
static GetFileVersionInfoSizeExA_t pGetFileVersionInfoSizeExA = NULL;
static GetFileVersionInfoSizeExW_t pGetFileVersionInfoSizeExW = NULL;
static GetFileVersionInfoSizeW_t pGetFileVersionInfoSizeW = NULL;
static GetFileVersionInfoW_t pGetFileVersionInfoW = NULL;
static VerFindFileA_t pVerFindFileA = NULL;
static VerFindFileW_t pVerFindFileW = NULL;
static VerInstallFileA_t pVerInstallFileA = NULL;
static VerInstallFileW_t pVerInstallFileW = NULL;
static VerLanguageNameA_t pVerLanguageNameA = NULL;
static VerLanguageNameW_t pVerLanguageNameW = NULL;
static VerQueryValueA_t pVerQueryValueA = NULL;
static VerQueryValueW_t pVerQueryValueW = NULL;

static void wrapper_setup(void) {
    WCHAR sysdir[MAX_PATH];
    GetSystemDirectoryW(sysdir, MAX_PATH);
    lstrcatW(sysdir, L"\\version.dll");
    hVersion = LoadLibraryW(sysdir);
    if (!hVersion) return;

    pGetFileVersionInfoA = (GetFileVersionInfoA_t)GetProcAddress(hVersion, "GetFileVersionInfoA");
    pGetFileVersionInfoByHandle = (GetFileVersionInfoByHandle_t)GetProcAddress(hVersion, "GetFileVersionInfoByHandle");
    pGetFileVersionInfoExA = (GetFileVersionInfoExA_t)GetProcAddress(hVersion, "GetFileVersionInfoExA");
    pGetFileVersionInfoExW = (GetFileVersionInfoExW_t)GetProcAddress(hVersion, "GetFileVersionInfoExW");
    pGetFileVersionInfoSizeA = (GetFileVersionInfoSizeA_t)GetProcAddress(hVersion, "GetFileVersionInfoSizeA");
    pGetFileVersionInfoSizeExA = (GetFileVersionInfoSizeExA_t)GetProcAddress(hVersion, "GetFileVersionInfoSizeExA");
    pGetFileVersionInfoSizeExW = (GetFileVersionInfoSizeExW_t)GetProcAddress(hVersion, "GetFileVersionInfoSizeExW");
    pGetFileVersionInfoSizeW = (GetFileVersionInfoSizeW_t)GetProcAddress(hVersion, "GetFileVersionInfoSizeW");
    pGetFileVersionInfoW = (GetFileVersionInfoW_t)GetProcAddress(hVersion, "GetFileVersionInfoW");
    pVerFindFileA = (VerFindFileA_t)GetProcAddress(hVersion, "VerFindFileA");
    pVerFindFileW = (VerFindFileW_t)GetProcAddress(hVersion, "VerFindFileW");
    pVerInstallFileA = (VerInstallFileA_t)GetProcAddress(hVersion, "VerInstallFileA");
    pVerInstallFileW = (VerInstallFileW_t)GetProcAddress(hVersion, "VerInstallFileW");
    pVerLanguageNameA = (VerLanguageNameA_t)GetProcAddress(hVersion, "VerLanguageNameA");
    pVerLanguageNameW = (VerLanguageNameW_t)GetProcAddress(hVersion, "VerLanguageNameW");
    pVerQueryValueA = (VerQueryValueA_t)GetProcAddress(hVersion, "VerQueryValueA");
    pVerQueryValueW = (VerQueryValueW_t)GetProcAddress(hVersion, "VerQueryValueW");
}

static void wrapper_cleanup(void) {
    FreeLibrary(hVersion);
    hVersion = NULL;
}

BOOL WINAPI _GetFileVersionInfoA(LPCSTR lptstrFilename, DWORD dwHandle, DWORD dwLen, LPVOID lpData) {
    if (!pGetFileVersionInfoA) return FALSE;
    return pGetFileVersionInfoA(lptstrFilename, dwHandle, dwLen, lpData);
}

BOOL WINAPI _GetFileVersionInfoByHandle(DWORD dwFlags, HANDLE hFile, LPVOID *lplpData, PDWORD pdwLen) {
    if (!pGetFileVersionInfoByHandle) return FALSE;
    return pGetFileVersionInfoByHandle(dwFlags, hFile, lplpData, pdwLen);
}

BOOL WINAPI _GetFileVersionInfoExA(DWORD dwFlags, LPCSTR lpwstrFilename, DWORD dwHandle, DWORD dwLen, LPVOID lpData) {
    if (!pGetFileVersionInfoExA) return FALSE;
    return pGetFileVersionInfoExA(dwFlags, lpwstrFilename, dwHandle, dwLen, lpData);
}

BOOL WINAPI _GetFileVersionInfoExW(DWORD dwFlags, LPCWSTR lpwstrFilename, DWORD dwHandle, DWORD dwLen, LPVOID lpData) {
    if (!pGetFileVersionInfoExW) return FALSE;
    return pGetFileVersionInfoExW(dwFlags, lpwstrFilename, dwHandle, dwLen, lpData);
}

DWORD WINAPI _GetFileVersionInfoSizeA(LPCSTR lptstrFilename, LPDWORD lpdwHandle) {
    if (!pGetFileVersionInfoSizeA) return 0;
    return pGetFileVersionInfoSizeA(lptstrFilename, lpdwHandle);
}

DWORD WINAPI _GetFileVersionInfoSizeExA(DWORD dwFlags, LPCSTR lpwstrFilename, LPDWORD lpdwHandle) {
    if (!pGetFileVersionInfoSizeExA) return 0;
    return pGetFileVersionInfoSizeExA(dwFlags, lpwstrFilename, lpdwHandle);
}

DWORD WINAPI _GetFileVersionInfoSizeExW(DWORD dwFlags, LPCWSTR lpwstrFilename, LPDWORD lpdwHandle) {
    if (!pGetFileVersionInfoSizeExW) return 0;
    return pGetFileVersionInfoSizeExW(dwFlags, lpwstrFilename, lpdwHandle);
}

DWORD WINAPI _GetFileVersionInfoSizeW(LPCWSTR lptstrFilename, LPDWORD lpdwHandle) {
    if (!pGetFileVersionInfoSizeW) return 0;
    return pGetFileVersionInfoSizeW(lptstrFilename, lpdwHandle);
}

BOOL WINAPI _GetFileVersionInfoW(LPCWSTR lptstrFilename, DWORD dwHandle, DWORD dwLen, LPVOID lpData) {
    if (!pGetFileVersionInfoW) return FALSE;
    return pGetFileVersionInfoW(lptstrFilename, dwHandle, dwLen, lpData);
}

DWORD WINAPI _VerFindFileA(DWORD uFlags, LPCSTR szFileName, LPCSTR szWinDir, LPCSTR szAppDir, LPSTR szCurDir, PUINT puCurDirLen, LPSTR szDestDir, PUINT puDestDirLen) {
    if (!pVerFindFileA) return 0xFFFF;
    return pVerFindFileA(uFlags, szFileName, szWinDir, szAppDir, szCurDir, puCurDirLen, szDestDir, puDestDirLen);
}

DWORD WINAPI _VerFindFileW(DWORD uFlags, LPCWSTR szFileName, LPCWSTR szWinDir, LPCWSTR szAppDir, LPWSTR szCurDir, PUINT puCurDirLen, LPWSTR szDestDir, PUINT puDestDirLen) {
    if (!pVerFindFileW) return 0xFFFF;
    return pVerFindFileW(uFlags, szFileName, szWinDir, szAppDir, szCurDir, puCurDirLen, szDestDir, puDestDirLen);
}

DWORD WINAPI _VerInstallFileA(DWORD uFlags, LPCSTR szSrcFileName, LPCSTR szDestFileName, LPCSTR szSrcDir, LPCSTR szDestDir, LPCSTR szCurDir, LPSTR szTmpFile, PUINT puTmpFileLen) {
    if (!pVerInstallFileA) return 0xFFFFFFFFL;
    return pVerInstallFileA(uFlags, szSrcFileName, szDestFileName, szSrcDir, szDestDir, szCurDir, szTmpFile, puTmpFileLen);
}

DWORD WINAPI _VerInstallFileW(DWORD uFlags, LPCWSTR szSrcFileName, LPCWSTR szDestFileName, LPCWSTR szSrcDir, LPCWSTR szDestDir, LPCWSTR szCurDir, LPWSTR szTmpFile, PUINT puTmpFileLen) {
    if (!pVerInstallFileW) return 0xFFFFFFFFL;
    return pVerInstallFileW(uFlags, szSrcFileName, szDestFileName, szSrcDir, szDestDir, szCurDir, szTmpFile, puTmpFileLen);
}

DWORD WINAPI _VerLanguageNameA(DWORD wLang, LPSTR szLang, DWORD cchLang) {
    if (!pVerLanguageNameA) return 0;
    return pVerLanguageNameA(wLang, szLang, cchLang);
}

DWORD WINAPI _VerLanguageNameW(DWORD wLang, LPWSTR szLang, DWORD cchLang) {
    if (!pVerLanguageNameW) return 0;
    return pVerLanguageNameW(wLang, szLang, cchLang);
}

BOOL WINAPI _VerQueryValueA(LPCVOID pBlock, LPCSTR lpSubBlock, LPVOID *lplpBuffer, PUINT puLen) {
    if (!pVerQueryValueA) return FALSE;
    return pVerQueryValueA(pBlock, lpSubBlock, lplpBuffer, puLen);
}

BOOL WINAPI _VerQueryValueW(LPCVOID pBlock, LPCWSTR lpSubBlock, LPVOID *lplpBuffer, PUINT puLen) {
    if (!pVerQueryValueW) return FALSE;
    return pVerQueryValueW(pBlock, lpSubBlock, lplpBuffer, puLen);
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved) {
    if (fdwReason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hinstDLL);
        wrapper_setup();
        on_attach();
    } else if (fdwReason == DLL_PROCESS_DETACH) {
        on_detach();
        wrapper_cleanup();
    }

    return TRUE;
}
