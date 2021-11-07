
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

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"



void rand_mix(std::vector<int>& arr) {
    //подключаем windows.h
    SYSTEMTIME tm;
    GetLocalTime(&tm);
    srand(tm.wMilliseconds);
    for (int i = 0; i < arr.size(); i++)
    {
        int change_pos = i + (rand()) % (arr.size() - i);
        int tmp = arr[i];
        arr[i] = arr[change_pos];
        arr[change_pos] = tmp;
    }
}

std::vector<int> setRoles(int numberOfPlayers) {
    std::vector<int> roles;
    switch (numberOfPlayers)
    {
    case 2:
        roles = { 3,1 };
        break;
    case 4:
        roles = { 3,1,0,0 };
        break;
    case 5:
        roles = { 3,3,1,0,0 };
        break;
    case 6:
        roles = { 3,3,2,1,0,0 };
        break;
    case 7:
        roles = { 3,3,3,2,1,0,0 };
        break;
    }
    rand_mix(roles);
    return roles;
}

int inputAmountOfPlayers() {
    int numberOfPlayers;
    std::cin >> numberOfPlayers;
    while (numberOfPlayers > 7 || numberOfPlayers < 4 || typeid(numberOfPlayers) != typeid(int)) {
        std::cout << "Wrong input\n";
        std::cin >> numberOfPlayers;
    }
    return numberOfPlayers;
}


int connectionToClients(std::vector<Player> &players, int numberOfPlayers) {

    
    int iResult;

    WSADATA wsaData;

    SOCKET ListenSocket = INVALID_SOCKET;
    SOCKET ClientSocket = INVALID_SOCKET;

    struct addrinfo* result = NULL;
    struct addrinfo hints;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    // Resolve the server address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Create a SOCKET for connecting to server
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Setup the TCP listening socket
    iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }




   

    std::vector roles = setRoles(numberOfPlayers);
    int recvBuf_len = 30;
    char* recvBuf = new char[recvBuf_len];
    int id = 0;
    players.clear();
    while (players.size() != numberOfPlayers) {
        // Accept a client socket
        ClientSocket = accept(ListenSocket, NULL, NULL);


        if (std::find_if(players.begin(), players.end(), [ClientSocket](Player& x) {return x.getSocket()== ClientSocket; }) == players.end()) {
            int iResult;
            iResult = recv(ClientSocket, recvBuf, recvBuf_len, 0);
            if (iResult > 0) {
                players.push_back(Player(recvBuf, id, ClientSocket, roles[id++]));
            }
            else {
                printf("recv failed with error: %d\n", WSAGetLastError());
                closesocket(ClientSocket);
                WSACleanup();
                return 1;
            }
            std::cout << "\n\n" << players.size() << " of " << numberOfPlayers << "\n\n";
        }



        if (ClientSocket == INVALID_SOCKET) {
            printf("accept failed with error: %d\n", WSAGetLastError());
            closesocket(ListenSocket);
            WSACleanup();
            return 1;
        }
    }
    delete[] recvBuf;
    // No longer need server socket
    closesocket(ListenSocket);



    std::string playersNames="n";
    for (auto player : players) {
        playersNames += player.getPlayerName()+",";
    }
    playersNames.erase(playersNames.end()-1);
    std::string sendBuf;
    int iSendResult;
    for (auto player : players) {
        sendBuf = "sa" + std::to_string(numberOfPlayers) + "r" + std::to_string(player.getPlayerRole()) + "i0"+playersNames;
        iSendResult = send(ClientSocket, sendBuf.c_str(), sendBuf.size()+1, 0);
        if (iSendResult == SOCKET_ERROR) {
            printf("send failed with error: %d\n", WSAGetLastError());
            closesocket(ClientSocket);
            WSACleanup();
            return 1;
        }
    }

    return 0;
}


int closeAllConections(const std::vector<Player>& players) {
    for (auto ClientSocket : players) {
        closesocket(ClientSocket.getSocket());
    }
    WSACleanup();
    return 0;
}

int __cdecl main(void)
{
   
    std::vector<Player> players;

    int numberOfPlayers = inputAmountOfPlayers();
   
    if (connectionToClients(players, numberOfPlayers) != 0) return 1;

    std::cout << "start game\n";

    for (auto player : players) {
        std::cout << player.getPlayerName() << "\n";
    }


    
    closeAllConections(players);
    return 0;
}




