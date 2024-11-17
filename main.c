#include <stdio.h>
#include <WinSock2.h>

int main(){
    WSADATA wsaData;
    int result;

    result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        printf("WSAStartup failed: %d\n", result);
        return 1;
    }

    printf("works.\n");
    WSACleanup();
    
    return 0;
}