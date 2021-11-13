#pragma once

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


SOCKET StartUpClient(std::string);

void check_send_warning(int);

void send_name(SOCKET, std::string);

//void waiting_for_start(SOCKET, char*, int);

void process_morning_info(std::vector< std::pair<std::string, int>>&, char*, int&);

void make_choise(SOCKET, std::vector<std::pair<std::string, int>>&, char*, int, int&);

void vote(SOCKET, std::vector<std::pair<std::string, int>>&, char*, int, int&);

void waiting_for_message(SOCKET, std::vector<std::pair<std::string, int>>&, char*, int, int&);
