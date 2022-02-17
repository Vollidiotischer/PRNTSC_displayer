#include <iostream>
#include <SFML/Graphics.hpp>

#include "EasyWinSock.h"

/*

connect to adress: 
    connect to server ("ptsv2.com") 
    send http get request with the path /t/6s70l-1645120638/post

    ->
    "GET /raw/9uL16CyN HTTP/1.1\r\n"
                   "Host: pastebin.com\r\n"
                   "Connection: close\r\n"
                   "\r\n";
*/

constexpr const char* ip = "ptsv2.com"; ///t/6s70l-1645120638/post
constexpr const char* port = "80"; 

int main() {

    easy_win_sock ews(ip, NULL, port); 

    ews.init_win_sock(); 
    ews.create_win_sock(); 
    ews.connect_win_sock(); 

    // SEND / RECIEVE

    char sendbuf[] = "GET /t/6s70l-1645120638/post HTTP/1.1\r\nHost: ptsv2.com\r\nConnection: close\r\n\r\n"; 

    ews.send_win_sock(sendbuf, (int)strlen(sendbuf)); 

    char* data = ews.recieve_win_sock(512); // dynamically allocated


    ews.cleanup_win_sock(); 

    return 0;
}

/*

    // INTIALIZE
    WSADATA wsaData;

    int i_result;

    // Initialize Winsock
    i_result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (i_result != 0) {
        std::cout << "WASStartup failed: " << i_result << std::endl;
        return 1;
    }


    // CREATE SOCKET
    addrinfo *result = NULL;
    addrinfo *ptr = NULL;
    addrinfo hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // get ip adress of hostname
    i_result = getaddrinfo(ip, port, &hints, &result);

    if (i_result != 0) {
        std::cout << "getaddrinfo failed: " << i_result << std::endl;
        WSACleanup();
        return 1;
    }

    // create socket
    SOCKET connect_socket = INVALID_SOCKET;

    // Attempt to connect to the first adress returned by the call to getaddrinfo
    ptr = result;

    // create socket for connecting to server
    connect_socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

    if (connect_socket == INVALID_SOCKET) {
        std::cout << "Error at socket(): " << WSAGetLastError() << std::endl;
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }


    // CONNECT USING THE SOCKET

    // connect to server
    i_result = connect(connect_socket, ptr->ai_addr, (int)ptr->ai_addrlen);
    if (i_result == SOCKET_ERROR) {
        closesocket(connect_socket);
        connect_socket = INVALID_SOCKET;
    }

    // if connection failed we just close everithing instead of trying other adresses from getadressinfo()

    freeaddrinfo(result);

    if (connect_socket == INVALID_SOCKET) {
        std::cout << "Unable to connect to server!" << std::endl;
        WSACleanup();
        return 1;
    }


*/