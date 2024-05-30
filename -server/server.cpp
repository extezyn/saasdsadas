// ConsoleApplication3.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#define WIN32_LEAN_AND_MEAN
#include  <Windows.h>
#include <WS2tcpip.h>
#include <Winsock2.h>
#include <iostream>

using namespace std;
int main()

{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    WSADATA wsaData; // Структура для хранения информации о версии Winsock
    ADDRINFO hints;   // Структура для хранения информации о сетевом адресе
    ADDRINFO* addrResult; // Указатель на структуру ADDRINFO для хранения результатов поиска сетевого адреса
    const char* sendBuffer = "Hello from Server"; // то что отправит сервер клиенту в случае успеха(буфет сообщения)

    SOCKET ConnectSocket = INVALID_SOCKET;  // Сокет для подключения клиента
    SOCKET ListenSocket = INVALID_SOCKET;  // Сокет для прослушивания входящих подключений

    // Обнуляем структуру hints и заполняем необходимые поля для создания сокета
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;


    // Буфер с битами для получения сообщений от клиента
    char recvBuffer[512];

    // Инициализируем библиотеку Winsock
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        cout << "беда" << endl;
        return 1;
    }

    // Получаем информацию о сетевом адресе и протоколе на порте 666
    result = getaddrinfo(NULL, "666", &hints, &addrResult);
    if (result != 0) {
        cout << "беда 2" << endl;
        freeaddrinfo(addrResult);
        WSACleanup();
        return 1;
    }
    // Создаем сокет для прослушивания входящих подключений
    ListenSocket = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        cout << "Сокет не создан" << endl;
        freeaddrinfo(addrResult);
        WSACleanup();
        return 1;
    }
    // Связываем сокет с сетевым адресом и протоколом
    result = bind(ListenSocket, addrResult->ai_addr, addrResult->ai_addrlen);
    if (result == SOCKET_ERROR) {
        cout << "binding error to server <3" << endl;
        closesocket(ListenSocket);
        ListenSocket = INVALID_SOCKET;
        freeaddrinfo(addrResult);
        return 1;
    }

    // Начинаем прослушивание входящих подключений
    result = listen(ListenSocket, SOMAXCONN);
    if (result == SOCKET_ERROR) {
        cout << "Error to listen <3" << endl;
        closesocket(ListenSocket);
        ListenSocket = INVALID_SOCKET;
        freeaddrinfo(addrResult);
        return 1;
    }

    // Принимаем входящее подключение от клиента
    ConnectSocket = accept(ListenSocket, NULL, NULL);
    if (result == SOCKET_ERROR) {
        cout << "accepting socket failed <3" << endl;
        WSACleanup();
        return 1;
    }

    //закрытие сокета
    closesocket(ListenSocket);


    // Цикл обработки сообщений от клиента
    do {
        ZeroMemory(recvBuffer, 512, 0);  // Обнуление буфера для получения сообщений
        result = recv(ConnectSocket, recvBuffer, 512, 0); // Получение сообщения от клиента
        if (result > 0) {

            cout << "received" << result << "bytes" << endl;
            cout << "received data" << recvBuffer << endl;

            // Отправляем ответное сообщение клиенту
            result = send(ConnectSocket, sendBuffer, (int)strlen(sendBuffer), 0);

            if (result == SOCKET_ERROR) {
                cout << "send failde error" << endl;
                freeaddrinfo(addrResult);
                WSACleanup();
                return 1;
            }

        }
        else if (result == 0) {
            cout << "Connection closed" << endl;
        }
        else {
            cout << "received with ERRROR" << endl;
        }

    } while (result > 0);

    // Закрываем соединение с клиентом псоле сообщений
    result = shutdown(ConnectSocket, SD_SEND);
    if (result == SOCKET_ERROR) {
        cout << "shutdown failde error" << endl;
        freeaddrinfo(addrResult);
        WSACleanup();
        return 1;
    }
    closesocket(ListenSocket);
    freeaddrinfo(addrResult);
    WSACleanup();
    return 1;
}

