// g++ -o a test.cpp -static -pthread -lws2_32
#include <iostream>
#include <string>
#include <thread>
#include <WinSock2.h> //windows.hより先にincludeらしい
#include <Windows.h>
#include <ws2tcpip.h>


#pragma comment(lib, "ws2_32.lib")

using namespace std;

void ThreadFunc(int &sock) {
    char recvbuff[30];
    memset(recvbuff, 0, sizeof(recvbuff));
    int len = recv(sock, recvbuff, sizeof(recvbuff), 0);
    cout << recvbuff << endl;
}

int main() {
    struct sockaddr_in server;
    int sock;
    char buf[32];
    int n;

    WSADATA data;
    WSAStartup(MAKEWORD(2,0), &data);

    // ソケットの作成
    sock = socket(AF_INET, SOCK_STREAM, 0);

    //接続先指定用構造体の準備
    server.sin_family = AF_INET;
    server.sin_port = htons(12345);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    // サーバに接続
    connect(sock, (struct sockaddr *)&server, sizeof(server));

    std::thread th1(ThreadFunc, sock);
    th1.join();

    Sleep(10);

    // サーバからデータを受信 
    memset(buf, 0, sizeof(buf));
    n = recv(sock, buf, sizeof(buf), 0);

    printf("%d, %s\n", n, buf);

    /* socketの終了 */
    closesocket(sock);
    return 0;
}
