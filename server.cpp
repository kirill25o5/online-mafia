﻿
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






int connectionToClients(WSADATA &wsaData ,std::vector<SOCKET> &sockets, int numberOfPlayers) {

    
    int iResult;

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




   



    sockets.clear();
    while (sockets.size() != numberOfPlayers) {
        // Accept a client socket
        ClientSocket = accept(ListenSocket, NULL, NULL);


        if (std::find(sockets.begin(), sockets.end(), ClientSocket)== sockets.end()) {
            sockets.push_back(ClientSocket);
        }



        if (ClientSocket == INVALID_SOCKET) {
            printf("accept failed with error: %d\n", WSAGetLastError());
            closesocket(ListenSocket);
            WSACleanup();
            return 1;
        }


        std::cout << "\n\n" << sockets.size() << " of " << numberOfPlayers << "\n\n";

    }
    // No longer need server socket
    closesocket(ListenSocket);

    return 0;
}

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
        roles={ 3,1 };
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


int sendInfoAboutStartingGame(const std::vector<SOCKET>& sockets, std::vector<Player>& players, int numberOfPlayers) {
    int recvHosts = 0;
    int iResult;
    int iSendResult;
    /*    char recvbuf[DEFAULT_BUFLEN];
        int recvbuflen = DEFAULT_BUFLEN;*/

    int recvBuf_len = 255;
    char* recvBuf = new char[recvBuf_len];
    int id = 0;
    std::string sendBuf;
    std::vector roles = setRoles(numberOfPlayers);

    for (auto ClientSocket : sockets) {
        iResult = recv(ClientSocket, recvBuf, recvBuf_len, 0);
        if (iResult > 0) {
            players.push_back(Player(recvBuf, id, ClientSocket, roles[id]));
            sendBuf="sa"+std::to_string(numberOfPlayers)+"r"+std::to_string(roles[id++])+"i0";
        }
        else {
                printf("recv failed with error: %d\n", WSAGetLastError());
                closesocket(ClientSocket);
                WSACleanup();
                return 1;
        }

     
        iSendResult = send(ClientSocket, sendBuf.c_str(), sendBuf.size(), 0);
        if (iSendResult == SOCKET_ERROR) {
            printf("send failed with error: %d\n", WSAGetLastError());
            closesocket(ClientSocket);
            WSACleanup();
            return 1;
        }
    }
    
    return 0;
}


int __cdecl main(void)
{
   

    std::vector<SOCKET> sockets;
    std::vector<Player> players;

    int numberOfPlayers=2;
    //std::cin >> numberOfPlayers;
    //while (numberOfPlayers > 7 || numberOfPlayers < 4 || typeid(numberOfPlayers) != typeid(int)) {
    //    std::cout << "Wrong input\n";
    //    std::cin >> numberOfPlayers;
    //}

    WSADATA wsaData;
   
    if (connectionToClients(wsaData,sockets, numberOfPlayers) != 0) return 1;

    if (sendInfoAboutStartingGame(sockets, players, numberOfPlayers) != 0) return 1;

    std::cout << "start game\n";

    for (auto player : players) {
        std::cout << player.getPlayerName() << "\n";
    }

    //// Receive until the peer shuts down the connection
    //do {
    //
    //    iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
    //    if (iResult > 0) {
    //        printf("Bytes received: %d\n", iResult);
    //        // Echo the buffer back to the sender
    //        /*iSendResult = send(ClientSocket, recvbuf, iResult, 0);
    //        if (iSendResult == SOCKET_ERROR) {
    //            printf("send failed with error: %d\n", WSAGetLastError());
    //            closesocket(ClientSocket);
    //            WSACleanup();
    //            return 1;
    //        }*/
    //
    //
    //
    //
    //        for (int i = 0; i < iResult; i++)
    //        {
    //            //printf(recvbuf[i]);
    //            std::cout << recvbuf[i];
    //        }
    //        std::cout << '\n';
    //        printf("Bytes sent: %d\n", iResult);
    //
    //        std::cout << '\n';
    //    }
    //    else if (iResult == 0)
    //        printf("Connection closing...\n");
    //    else {
    //        printf("recv failed with error: %d\n", WSAGetLastError());
    //        closesocket(ClientSocket);
    //        WSACleanup();
    //        return 1;
    //    }
    //
    //
    //
    //    if (recvbuf[0] == 'd')
    //    {
    //        sendbuf[0] = 'e';
    //
    //        iSendResult = send(ClientSocket, sendbuf, iResult, 0);
    //        if (iSendResult == SOCKET_ERROR) {
    //            printf("send failed with error: %d\n", WSAGetLastError());
    //            closesocket(ClientSocket);
    //            WSACleanup();
    //            return 1;
    //        }
    //    }
    //    /*sendbuf[0] = 'e';
    //    iSendResult = send(ClientSocket, sendbuf, iResult, 0);
    //    if (iSendResult == SOCKET_ERROR) {
    //        printf("send failed with error: %d\n", WSAGetLastError());
    //        closesocket(ClientSocket);
    //        WSACleanup();
    //        return 1;
    //    }*/
    //
    //
    //    std::cout << 1;
    //} while (iResult > 0);
    //
    //// shutdown the connection since we're done
    //iResult = shutdown(ClientSocket, SD_SEND);
    //if (iResult == SOCKET_ERROR) {
    //    printf("shutdown failed with error: %d\n", WSAGetLastError());
    //    closesocket(ClientSocket);
    //    WSACleanup();
    //    return 1;
    //}
    //
    //// cleanup
    //

    for (auto ClientSocket : sockets) {
        closesocket(ClientSocket);
    }
    WSACleanup();

    return 0;
}












/*
#include <winsock2.h>
#include <stdio.h>
#pragma comment(lib, "WS2_32.lib")
int main(void)
{
    WORD sockVer;
    WSADATA wsaData;
    int retVal;
    sockVer = MAKEWORD(2, 2);
    WSAStartup(sockVer, &wsaData);
    //Создаем сокет
    SOCKET servSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (servSock == INVALID_SOCKET)
    {
        printf("Unable to create socket\n");
        WSACleanup();
        return SOCKET_ERROR;
    }
    SOCKADDR_IN sin;
    sin.sin_family = PF_INET;
    sin.sin_port = htons(1111);
    sin.sin_addr.s_addr = INADDR_ANY;
    retVal = bind(servSock, (LPSOCKADDR)&sin, sizeof(sin));
    if (retVal == SOCKET_ERROR)
    {
        printf("Unable to bind\n");
        WSACleanup();
        return SOCKET_ERROR;
    }
    //Пытаемся начать слушать сокет
    retVal = listen(servSock, 10);
    if (retVal == SOCKET_ERROR)
    {
        printf("Unable to listen\n");
        WSACleanup();
        return SOCKET_ERROR;
    }
    //Ждем клиента
    SOCKET clientSock;
    clientSock = accept(servSock, NULL, NULL);
    if (clientSock == INVALID_SOCKET)
    {
        printf("Unable to accept\n");
        WSACleanup();
        return SOCKET_ERROR;
    }
    char szReq[8];
    retVal = recv(clientSock, szReq, 8, 0);
    if (retVal == SOCKET_ERROR)
    {
        printf("Unable to recv\n");
        return SOCKET_ERROR;
    }
    printf("Got the request from client\n%s\n", szReq);
    const char* szResp = "Response";
    printf("Sending response from server\n");
    retVal = send(clientSock, szResp, strlen(szResp), 0);
    if (retVal == SOCKET_ERROR)
    {
        printf("Unable to send\n");
        return SOCKET_ERROR;
    }
    //Закрываем сокет
    closesocket(clientSock);
    closesocket(servSock);
    WSACleanup();
    return 0;
}*/