#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <stdlib.h>
#include <ws2tcpip.h>

#define NICKNAME "tezdsttter"
#define CHANNEL "#jkpa1234"

int main(){
    WSADATA wsaData;
    SOCKET sock;
    struct sockaddr_in serverAddr;
    const char *IRCServer = "104.152.54.53"; /* DALnet: halcyon.il.us.dal.net */
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
    sprintf(send_buffer, "NICK %s\r\n", NICKNAME);
    send(sock, send_buffer, strlen(send_buffer), 0);

    sprintf(send_buffer, "USER %s 8 * :%s\r\n", NICKNAME, NICKNAME);
    send(sock, send_buffer, strlen(send_buffer), 0);
    
    sprintf(send_buffer, "JOIN %s\r\n", CHANNEL);
    send(sock, send_buffer, strlen(send_buffer), 0);
    
    while (1) {
        memset(send_buffer, 0, sizeof(send_buffer));
        int bytesReceived = recv(sock, send_buffer, sizeof(send_buffer) - 1, 0);
        if (bytesReceived <= 0) {
            break; 
        }

        send_buffer[bytesReceived] = '\0';
        printf("%s", send_buffer);

        if (strstr(send_buffer, "PING") != NULL) {
            sprintf(send_buffer, "PONG %s\r\n", send_buffer + 5);
            send(sock, send_buffer, strlen(send_buffer), 0);
        }
    }

    closesocket(sock);
    WSACleanup();  
    return 0;
}