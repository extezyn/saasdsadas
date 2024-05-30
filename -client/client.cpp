// ConsoleApplication3.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
#define WIN32_LEAN_AND_MEAN // Исключить некоторые малоиспользуемые компоненты из заголовочных файлов Windows
#include  <Windows.h>
#include <WS2tcpip.h>// Заголовочный файл для функций TCP/IP
#include <Winsock2.h>// Заголовочный файл для функций сокетов
#include <iostream>

using namespace std;
int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    WSADATA wsaData; // Структура для хранения информации о версии Winsock
    ADDRINFO hints;  // Структура для хранения информации о сетевом адресе
    ADDRINFO* addrResult; // Указатель на структуру ADDRINFO для хранения результатов поиска сетевого адреса
    SOCKET ConnectSocket = INVALID_SOCKET; // Сокет для подключения клиента
    const char* sendBuffer = "Hello from Client";
    const char* sendBuffer2 = "Hello from Client2";

    char recvBuffer[512]; //набор битов для сообщения

    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        cout << "беда" << endl;
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints)); // Обнулить память для структуры hints
    hints.ai_family = AF_INET; // Установить тип сетевого адреса - IPv4
    hints.ai_socktype = SOCK_STREAM; // Установить тип сокета - потоковый
    hints.ai_protocol = IPPROTO_TCP;  // Установить протокол - TCP



    result = getaddrinfo("localhost", "666", &hints, &addrResult); // Получение сетевого адреса для прослушивания
    if (result != 0) {
        cout << "беда 2" << endl;
        freeaddrinfo(addrResult);
        WSACleanup(); // Очистка библиотеки Winsock
        return 1;
    }
    ConnectSocket = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol);  // Инициализация сокета
    if (ConnectSocket == INVALID_SOCKET) {
        cout << "Сокет не создан" << endl;
        freeaddrinfo(addrResult); // Освобождение памяти для структуры ADDRINFO
        WSACleanup(); 
        return 1;
    }
    result = connect(ConnectSocket, addrResult->ai_addr, addrResult->ai_addrlen); //Эта строка кода пытается установить соединение с сервером по заданному IP-адресу и порту.
    if (result == SOCKET_ERROR) {
        cout << "нет, сервера нема" << endl;
        closesocket(ConnectSocket); //закрытие сокета
        ConnectSocket = INVALID_SOCKET;
        freeaddrinfo(addrResult); //критует если не сделать
        return 1;
    }

    result = send(ConnectSocket, sendBuffer, (int)strlen(sendBuffer), 0); //отпрвка сообщения на сервак
    if (result == SOCKET_ERROR) {
        cout << "send failde error" << endl;
        freeaddrinfo(addrResult);
        WSACleanup();
        return 1;
    }
    cout << "send" << result << endl;

    result = send(ConnectSocket, sendBuffer2, (int)strlen(sendBuffer2), 0); //отправка второго сообщения на сервак
    if (result == SOCKET_ERROR) {
        cout << "send failde error" << endl;
        freeaddrinfo(addrResult);
        WSACleanup();
        return 1;
    }
    cout << "send" << result << endl;

    result = shutdown(ConnectSocket, SD_SEND); //откоючение клиента после всех сообщений
    if (result == SOCKET_ERROR) {
        cout << "shutdown failde error" << endl;
        closesocket(ConnectSocket);
        freeaddrinfo(addrResult);
        WSACleanup();
        return 1;
    }

    do {
        ZeroMemory(recvBuffer, 512, 0); // Обнуление буфера для получения сообщений
        result = recv(ConnectSocket, recvBuffer, 512, 0); // Получение сообщения от клиента
        if (result > 0) { //при усепхе
            cout << "received" << result << "bytes" << endl;
            cout << "received data" << recvBuffer << endl;
        }
        else if (result == 0) { //закрытое соединение
            cout << "Connection closed" << endl;
        }
        else { //ошибка
            cout << "received with ERRROR" << endl;
        }

    } while (result > 0);
    freeaddrinfo(addrResult);
    WSACleanup();
    return 1;
}

