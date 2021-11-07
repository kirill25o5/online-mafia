#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <iostream>
#include <typeinfo>
#include "Player.h"
#include <string>
#include <string_view>
#include "server.h"

// Need to link with Ws2_32.lib
//#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"


SOCKET StartUpClient(std::string serv_ip) {

    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo* result = NULL,
        * ptr = NULL,
        hints;
    int iResult;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return INVALID_SOCKET;
    }

    ZeroMemory(&hints, sizeof(hints));
    /*hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;*/

    // Resolve the server address and port
    iResult = getaddrinfo(serv_ip.c_str(), DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return INVALID_SOCKET;
    }

    // Attempt to connect to an address until one succeeds
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return INVALID_SOCKET;
        }

        // Connect to server.
        iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return INVALID_SOCKET;
    }
    else {
        printf("Connection is created!\n");
    }

    return ConnectSocket;
}

void check_send_warning(int result) {
    if (result > 0) {
        printf("Bytes send: %d\n", result);
    }
    else if (result == 0)
        printf("Connection closed\n");
    else
        printf("send failed with error: %d\n", WSAGetLastError());
}

void send_name(SOCKET ConnectSocket, std::string name) {
    int iResult = send(ConnectSocket, name.c_str(), name.size() + 1, 0);
    check_send_warning(iResult);
}

void waiting_for_start(SOCKET ConnectSocket, char* recvbuf, int recvbuf_len) {
    bool is_game_begin = false;
    int iResult;
    do {
        iResult = recv(ConnectSocket, recvbuf, recvbuf_len, 0);
        if (iResult > 0) {
            printf("Bytes received: %d\n", iResult);
            if (recvbuf[0] == 's')
            {
                is_game_begin = true;
                std::cout << recvbuf;
            }
        }
        else if (iResult == 0)
            printf("Connection closed\n");
        else
            printf("recv failed with error: %d\n", WSAGetLastError());

    } while (is_game_begin != true);
}

void print_start_info(char* recvbuf, int& my_role) {
    int len = 0;
    while (recvbuf[len]) len++;
    if (len < 9)
    {
        std::cout << "Incorrect start message!\n";
        return;
    }
    if (recvbuf[0] != 's')
    {
        std::cout << "Incorrect start message!\n";
        return;
    }
    if (recvbuf[1] != 'a')
    {
        std::cout << "Incorrect start message!\n";
        return;
    }
    if (recvbuf[3] != 'r')
    {
        std::cout << "Incorrect start message!\n";
        return;
    }
    if (recvbuf[5] != 'i')
    {
        std::cout << "Incorrect start message!\n";
        return;
    }
    if (recvbuf[7] != 'n')
    {
        std::cout << "Incorrect start message!\n";
        return;
    }
    for (int i = 8; i < len; i++)
    {
        if (recvbuf[i] == ',')
            std::cout << '\n';
        else
            std::cout << recvbuf[i];
    }
}

int __cdecl main()
{
    bool is_game_server;
    std::cout << "serv or not serv (1 or 0)";
    char c;
    std::cin >> c;
    if (c == '1')
        is_game_server = true;
    else
        is_game_server = false;

    if (is_game_server)
    {
        std::vector<Player> players;

        int numberOfPlayers = inputAmountOfPlayers();

        if (connectionToClientsAndStartGame(players, numberOfPlayers) != 0) return 1;

        std::cout << "start game\n";

        showPlayers(players);

        closeAllConections(players);
    }
    else
    {
        int iResult;

        int recvbuf_len = 256;
        char* recvbuf = new char[recvbuf_len];

        std::cout << "enter serv ip:\n";
        std::string serv_ip;
        std::cin >> serv_ip;
        SOCKET ConnectSocket = StartUpClient(serv_ip);

        // Ñîåäèíèâøèñü ñ ñåðâåðîì, ïåðåäàåì åìó èìÿ ïîëüçîâàòåëÿ èãðîêà
        std::cout << "enter your name:\n";
        std::string name;
        std::cin >> name;

        send_name(ConnectSocket, name);

        waiting_for_start(ConnectSocket, recvbuf, recvbuf_len);

        int my_role = -1;

        print_start_info(recvbuf, my_role);

        // cleanup
        closesocket(ConnectSocket);
        WSACleanup();


        delete[] recvbuf;
    }

    return 0;
}