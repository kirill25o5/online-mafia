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
#include "client.h"

// Need to link with Ws2_32.lib
//#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


int __cdecl main()
{
    bool is_game_server;

    std::vector<std::pair<std::string, int>> morning_status;

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

        int my_role = -1;
        while (true)
        {
            waiting_for_message(ConnectSocket, morning_status, recvbuf, recvbuf_len, my_role);
        }
        
        //get_morning_info(morning_status, recvbuf, my_role);




        // cleanup
        closesocket(ConnectSocket);
        WSACleanup();


        delete[] recvbuf;
    }

    return 0;
}