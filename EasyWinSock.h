#pragma once

#pragma comment(lib, "ws2_32.lib")

#include <iostream>

#include <winsock2.h>
#include <WS2tcpip.h>


struct easy_win_sock {
	char* hostname;
	const char* ip = "ptsv2.com";
	const char* port = "80";

	WSADATA *wsaData; 

    addrinfo* result;
    addrinfo* ptr;

    SOCKET *sock; 


	easy_win_sock(const char* ip, const char* hostname, const char* port) {
		//this->hostname = hostname; 
		//this->ip = hostname; 
		//this->port = hostname; 

        this->sock = new SOCKET; 
        this->wsaData = new WSADATA;

	}

	void init_win_sock() {
        

		int i_result = WSAStartup(MAKEWORD(2, 2), wsaData);
		if (i_result != 0) {
			std::cout << "WASStartup failed: " << i_result << std::endl;
			exit(1); 
		}
	}

	void create_win_sock() {
        this->result = NULL;
        this->ptr = NULL;
        addrinfo hints;

        ZeroMemory(&hints, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;

        // get ip adress of hostname 
        int i_result = getaddrinfo(this->ip, this->port, &hints, &this->result);
        if (i_result != 0) {
            std::cout << "getaddrinfo failed: " << i_result << std::endl;
            WSACleanup();
            exit(1); 
        }

        this->hostname = this->result->ai_canonname; 

        // create socket
        *this->sock = INVALID_SOCKET;

        // Attempt to connect to the first adress returned by the call to getaddrinfo
        this->ptr = this->result;

        // create socket for connecting to server
        *this->sock = socket(this->ptr->ai_family, this->ptr->ai_socktype, this->ptr->ai_protocol);

        if (*this->sock == INVALID_SOCKET) {
            std::cout << "Error at socket(): " << WSAGetLastError() << std::endl;
            freeaddrinfo(this->result);
            WSACleanup();
            exit(1); 
        }
	}


    void connect_win_sock() {
        // connect to server
        int i_result = connect(*this->sock, this->ptr->ai_addr, (int)this->ptr->ai_addrlen);
        if (i_result == SOCKET_ERROR) {
            closesocket(*this->sock);
            *this->sock = INVALID_SOCKET;
        }

        // if connection failed we just close everithing instead of trying other adresses from getadressinfo()
        freeaddrinfo(result);

        if (*this->sock == INVALID_SOCKET) {
            std::cout << "Unable to connect to server!" << std::endl;
            WSACleanup();
            exit(1); 
        }

    }

    void send_win_sock(char* data, int buf_len) {
        int i_result = send(*this->sock, data, buf_len, 0);
        if (i_result == SOCKET_ERROR) {
            std::cout << "Send failed: " << WSAGetLastError() << std::endl;
            closesocket(*this->sock);
            WSACleanup();
            exit(1); 
        }
        std::cout << "Bytes sent: " << i_result << std::endl;

    }

    char* recieve_win_sock(int buf_size) {
        char* recvbuf = new char[buf_size]; 

        // recieve data till server closes connection 
        int i_result = 1;  // number of recieved bytes
        while (i_result > 0) {
            i_result = recv(*this->sock, recvbuf, 512, 0);
            if (i_result > 0) {
                std::cout << "Bytes recieved: " << i_result << std::endl;
                recvbuf[i_result] = '\0';

                std::cout << "\n\n" << recvbuf << "\n\n" << std::endl;
            }
            else if (i_result == 0) {
                std::cout << "Connection closed" << std::endl;
            }
            else {
                std::cout << "recv failed: " << WSAGetLastError() << std::endl;
            }
        }
        return recvbuf; 
    }

    void cleanup_win_sock() {
        // shutdown the connection for sending 
        // still can recieve
        int i_result = shutdown(*this->sock, SD_SEND);
        if (i_result == SOCKET_ERROR) {
            std::cout << "Shutdown failed: " << WSAGetLastError() << std::endl;
            closesocket(*this->sock);
            WSACleanup();
            exit(0); 
        }

        closesocket(*this->sock);
        WSACleanup();

        delete this->sock; 
        delete this->wsaData; 
    }
};
