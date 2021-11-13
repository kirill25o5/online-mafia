#include "client.h"

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

/*void waiting_for_start(SOCKET ConnectSocket, char* recvbuf, int recvbuf_len) {
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
}*/

void process_morning_info(std::vector<std::pair<std::string, int>>& morning_status, char* recvbuf, int& my_role) {
    int len = 0;
    while (recvbuf[len]) len++;
    
    if (recvbuf[0] == 's')
    {
        my_role = recvbuf[4] - 48;
        switch (my_role)
        {
        case 0:
            std::cout << "You are villian\n";
            break;
        case 1:
            std::cout << "You are policeman\n";
            break;
        case 2:
            std::cout << "You are doctor\n";
            break;
        case 3:
            std::cout << "You are mafia\n";
            break;
        }
        int num_of_alife = recvbuf[2] - 48;
        morning_status.resize(num_of_alife);
        int cur_name = 0;
        morning_status[cur_name].first.clear();
        morning_status[cur_name].second = 0;
        for (int i = 8; i < len; i++)
        {
            if (recvbuf[i] == ',') {
                cur_name++;
                morning_status[cur_name].first.clear();
                morning_status[cur_name].second = 0;
                std::cout << '\n';
            }
            else
            {
                morning_status[cur_name].first.push_back(recvbuf[i]);
                std::cout << recvbuf[i];
            }
        }

        std::cout << "Start message received!\n";
        return;
        
        if ((len < 9) || (recvbuf[1] != 'a') || (recvbuf[3] != 'r') || (recvbuf[5] != 'i') || (recvbuf[7] != 'n'))
        {
            std::cout << "Incorrect start message!\n";
            return;
        }
    }

    else
    {
        if (recvbuf[0] != 'm')
        {
            std::cout << "Incorrect morning message!\n";
            return;
        }
        
        int status;
        std::vector<std::pair<std::string, int>> prev_status = morning_status;
        for (int i = 3; i < len; i = i + 3)
        {
            status = recvbuf[i + 2] - 48;

            morning_status[i - 48].second = status;
        }
    }
}

void make_choise(SOCKET ConnectSocket, std::vector<std::pair<std::string, int>>& morning_status, char* recvbuf, int recvbuf_len, int& my_role) {
    switch (my_role)
    {
    case 0:
        std::cout << "((((((((((\n";
        break;
    case 1:
        std::cout << "Arest anybody, policeman\n";
        break;
    case 2:
        std::cout << "Care anybody, doctor\n";
        break;
    case 3:
        std::cout << "Kill anybody, mafia\n";
        break;
    }
    std::string name = " ";

    bool cor_name = false;
    auto iter = morning_status.begin();
    while (!cor_name)
    {
        std::cout << "Enter correct name from the list\n";
        std::cin >> name;
        iter = std::find_if(morning_status.begin(), morning_status.end(), [name](std::pair<std::string, int> pr)
            {
                return ((pr.first == name) && ((pr.second == 0) || (pr.second == 1) || (pr.second == 3) || (pr.second == 4)));
            }
        );
        cor_name = iter != morning_status.end();
    }
    int index = std::distance(morning_status.begin(), iter);
    char* buf = new char[2];
    buf[0] = 'd';
    buf[1] = index + 48;
    int iResult = send(ConnectSocket, buf, 2, 0);
    check_send_warning(iResult);
    delete[] buf;

}

void vote(SOCKET ConnectSocket, std::vector<std::pair<std::string, int>>& morning_status, char* recvbuf, int recvbuf_len, int& my_role) {
    
    std::string name = " ";

    bool cor_name = false;
    auto iter = morning_status.begin();
    while (!cor_name)
    {
        std::cout << "Enter correct name from the list\n";
        std::cin >> name;
        iter = std::find_if(morning_status.begin(), morning_status.end(), [name](std::pair<std::string, int> pr)
            {
                return ((pr.first == name) && ((pr.second == 0) || (pr.second == 1) || (pr.second == 3) || (pr.second == 4)));
            }
        );
        cor_name = (iter != morning_status.end());
    }
    int index = std::distance(morning_status.begin(), iter);
    char* buf = new char[2];
    buf[0] = 'v';
    buf[1] = index + 48;
    int iResult = send(ConnectSocket, buf, 2, 0);
    check_send_warning(iResult);
    delete[] buf;

}

void waiting_for_message(SOCKET ConnectSocket, std::vector<std::pair<std::string, int>>& morning_status, char* recvbuf, int recvbuf_len, int& my_role) {

    int iResult;
    do {
        iResult = recv(ConnectSocket, recvbuf, recvbuf_len, 0);
        if (iResult > 0) {
            printf("Bytes received: %d\n", iResult);
            std::cout << recvbuf;
        }
        else if (iResult == 0)
            printf("Connection closed\n");
        else
            printf("recv failed with error: %d\n", WSAGetLastError());

    } while (iResult <= 0);

    if ((recvbuf[0] == 's') || (recvbuf[0] == 'm'))
    {
        process_morning_info(morning_status, recvbuf, my_role);
    }
    else if (recvbuf[0] == 'D')
    {
        make_choise(ConnectSocket, morning_status, recvbuf, recvbuf_len, my_role);

        do {
            iResult = recv(ConnectSocket, recvbuf, recvbuf_len, 0);
            if (iResult > 0) {
                printf("Bytes received: %d\n", iResult);
                std::cout << recvbuf;
            }
            else if (iResult == 0)
                printf("Connection closed\n");
            else
                printf("recv failed with error: %d\n", WSAGetLastError());

        } while (iResult <= 0);

        if (recvbuf[0] == '1')
        {
            std::cout << "You catched mafia!!! Congradulations!!!\n";
        }
        else
        {
            std::cout << "You'll be lucky another time(((\n";
        }
    }
    else if (recvbuf[0] == 'V')
    {
        vote(ConnectSocket, morning_status, recvbuf, recvbuf_len, my_role);

        do {
            iResult = recv(ConnectSocket, recvbuf, recvbuf_len, 0);
            if (iResult > 0) {
                printf("Bytes received: %d\n", iResult);
                std::cout << recvbuf;
            }
            else if (iResult == 0)
                printf("Connection closed\n");
            else
                printf("recv failed with error: %d\n", WSAGetLastError());

        } while ((iResult <= 0) || (((recvbuf[0]) < '0') && (recvbuf[0]) > '9'));

        
        std::cout << "During the voting we desided to kill\n" << morning_status[recvbuf[0] - 48].first;
        
    }
}