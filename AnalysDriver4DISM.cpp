// AnalysDriver4DISM.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <windows.h>
#include <tchar.h> 
#include <stdio.h>
#include <iostream>
#include <strsafe.h>
#pragma comment(lib, "User32.lib")
#pragma warning (disable:4996)

char InfoDir[260];
void DisplayErrorBox(LPTSTR lpszFunction);
void Add2DISM(LPTSTR lpszFunction);

int main()
{
    SetConsoleTitle("AnalysDriver4DISM v:1.00");
    char tmp[260];
    WIN32_FIND_DATA ffd;
   LARGE_INTEGER filesize;
   TCHAR szDir[MAX_PATH];
   size_t length_of_arg;
   HANDLE hFind = INVALID_HANDLE_VALUE;
   DWORD dwError=0;
   GetWindowsDirectory(InfoDir, 256);
   StringCchCat(InfoDir, MAX_PATH, TEXT("\\Inf"));
   StringCchLength(InfoDir, MAX_PATH, &length_of_arg);
   if (length_of_arg > (MAX_PATH - 3)){_tprintf(TEXT("\nChemin trop long.\n"));return (-1);}
   _tprintf(TEXT("\nAnalyse de fichiers de configuration Windows %s\n\n"), InfoDir);
   StringCchCopy(szDir, MAX_PATH, InfoDir);
   StringCchCat(szDir, MAX_PATH, TEXT("\\*"));
   hFind = FindFirstFile(szDir, &ffd);
   if (INVALID_HANDLE_VALUE == hFind) {DisplayErrorBox((LPTSTR)"FindFirstFile");return dwError;} 
   FILE *f = fopen("h:\driver.cmd", "w+");
   do  {
      if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
         _tprintf(TEXT("  %s   <DIR>\n"), ffd.cFileName);
      }else {
         filesize.LowPart = ffd.nFileSizeLow;
         filesize.HighPart = ffd.nFileSizeHigh;
         _tprintf(TEXT(" %s\\%s   %ld octets\n"),InfoDir, ffd.cFileName, filesize.QuadPart);
         fprintf(f, "Dism /Add-Driver / Image:\"h:\\WinPE_x64\\mount\" / Driver:\"%s\\%s\"\n", InfoDir, ffd.cFileName);
         wsprintf(tmp, "Dism / Add - Driver / Image:\"h:\WinPE_x64\mount\" / Driver : \"%s\\%s\"", InfoDir, ffd.cAlternateFileName);
         system(tmp);
      }
   }
   while (FindNextFile(hFind, &ffd) != 0); 
   dwError = GetLastError();
   if (dwError != ERROR_NO_MORE_FILES)    {      DisplayErrorBox((LPTSTR)"FindFirstFile");  }
   fclose(f);
   FindClose(hFind);
   return dwError;
}
void DisplayErrorBox(LPTSTR lpszFunction) { 
    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError(); 
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |FORMAT_MESSAGE_IGNORE_INSERTS,NULL,dw,MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),(LPTSTR) &lpMsgBuf,0, NULL );
    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,  (lstrlen((LPCTSTR)lpMsgBuf)+lstrlen((LPCTSTR)lpszFunction)+40)*sizeof(TCHAR)); 
    StringCchPrintf((LPTSTR)lpDisplayBuf,LocalSize(lpDisplayBuf) / sizeof(TCHAR), TEXT("%s failed with error %d: %s"),lpszFunction, dw, lpMsgBuf); 
    MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK); 
    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
}
void Add2DISM(LPTSTR lpszFunction) {
    char tmp[260];
    wsprintf(tmp, "Dism /Add-Driver /Image:\"h:\WinPE_x64\mount\" /Driver:\"%s\\%s\"",InfoDir, lpszFunction);
    system(tmp);

}
