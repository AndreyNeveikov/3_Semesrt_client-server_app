#pragma comment(lib, "ws2_32.lib")
#include <stdio.h>
#include <iostream>
#include <winsock2.h>
#include <cstdlib>
#include <conio.h>
#pragma warning(disable: 4996)
using namespace std;
struct help
{
	int search;
};
struct help h;
void main()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD(2, 2);
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) { return; }
	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
	// указание адреса и порта сервера 
	sockaddr_in dest_addr;
	dest_addr.sin_family = AF_INET;
	dest_addr.sin_port = htons(1280);
	dest_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	connect(s, (sockaddr*)&dest_addr, sizeof(dest_addr));
	while (true)
	{
		char buf[200];
		cout << "Enter workers' salary:" << endl;
		cin >> h.search;
		send(s, (char*)&h, sizeof(h), 0);
		recv(s, buf, sizeof(buf), 0);
		cout << buf << endl;
		break;
	}
	closesocket(s);
	WSACleanup();
}
