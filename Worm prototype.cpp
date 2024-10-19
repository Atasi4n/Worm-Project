//#include <winsock2.h>
//#include <stdio.h>
//#include <ws2tcpip.h>  // Necesario para InetPtonA
//
//#pragma comment(lib, "ws2_32.lib")  // Librer�a para usar sockets en Windows
//
//int main() {
//    WSADATA wsaData;
//    struct sockaddr_in target;
//    int sock;
//    const char* network = "192.168.1.";  // Red local a escanear
//    char ip[16];
//
//    // Inicializar Winsock
//    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
//        printf("Error inicializando Winsock.\n");
//        return 1;
//    }
//
//    for (int i = 1; i <= 254; i++) {
//        snprintf(ip, sizeof(ip), "%s%d", network, i);
//        sock = socket(AF_INET, SOCK_STREAM, 0);
//
//        target.sin_family = AF_INET;
//        target.sin_port = htons(445);  // Escaneo del puerto 445 (SMB)
//
//        // Usar InetPtonA (versi�n ASCII) para convertir la IP
//        if (InetPtonA(AF_INET, ip, &target.sin_addr) <= 0) {
//            printf("Error: direcci�n IP inv�lida.\n");
//            closesocket(sock);
//            continue;  // Ir a la siguiente iteraci�n del bucle
//        }
//
//        // Intentar conectar al puerto SMB del host
//        if (connect(sock, (struct sockaddr*)&target, sizeof(target)) == 0) {
//            printf("Host %s est� activo y tiene el puerto 445 abierto.\n", ip);
//        }
//
//        closesocket(sock);
//    }
//
//    WSACleanup();
//    return 0;
//}

#include <winsock2.h>
#include <windows.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable : 4996)

// Definir constantes del exploit
#define TARGET_IP "192.168.0.3"  // IP de la m�quina vulnerable
#define SMB_PORT 445  // Puerto de SMB

int main() {
    WSADATA wsa;
    SOCKET s;
    struct sockaddr_in server;

    // Inicializar Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        std::cerr << "Error al inicializar Winsock. C�digo de Error: " << WSAGetLastError() << std::endl;
        return 1;
    }

    // Crear socket
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        std::cerr << "No se pudo crear el socket. Error: " << WSAGetLastError() << std::endl;
        return 1;
    }

    // Configurar el servidor
    server.sin_addr.s_addr = inet_addr(TARGET_IP);
    server.sin_family = AF_INET;
    server.sin_port = htons(SMB_PORT);

    // Conectar al servidor vulnerable
    if (connect(s, (struct sockaddr*)&server, sizeof(server)) < 0) {
        std::cerr << "Error al conectar al servidor SMB." << std::endl;
        return 1;
    }

    std::cout << "Conexi�n establecida con el servidor SMB vulnerable.\n";

    // Aqu� deber�as enviar el paquete SMB malicioso

    closesocket(s);
    WSACleanup();

    return 0;
}
