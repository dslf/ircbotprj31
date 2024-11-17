#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <stdlib.h>
#include <ws2tcpip.h>

int main(){
    WSADATA wsaData;
    SOCKET sock;
    struct sockaddr_in serverAddr;
    const char *IRCServer = "104.152.54.53";
    int IRCPort = 6667;
    int result;
    char send_buffer[512];

    result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        printf("WSAStartup failed: %d\n", result);
        return 1;
    }
    printf("WSAStartup works.\n");

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) {
        printf("Error creating: %d\n", WSAGetLastError());
        WSACleanup();
        return 1;
    }
    printf("Socket created.\n");

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(IRCPort);
    serverAddr.sin_addr.s_addr = inet_addr(IRCServer); 

    if (connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        printf("Connect failed: %d\n", WSAGetLastError());
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    printf("Connected to IRC server at %s:%d\n", IRCServer, IRCPort);
    strcpy(send_buffer, "NICK tezdsttter");  
    send(sock, send_buffer,strlen(send_buffer));
    
    strcpy(send_buffer, "USER tezdstttefr");  
    /* strcat(send_buffer, ""); */

    send(sock, send_buffer,strlen(send_buffer));

    closesocket(sock);
    WSACleanup();
    
    return 0;
}