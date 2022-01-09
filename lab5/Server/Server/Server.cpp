/*
Реализовать простейший почтовый сервер и протокол 
взаимодействия с ним. Предусмотреть команды  чтения входящих 
писем и удаления отправленных.
*/

#pragma warning (disable: 4996)
#include <iostream>
#include <WinSock2.h>
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#pragma comment (lib, "ws2_32.lib")

using namespace std;

int main()
{
	setlocale(LC_ALL, "rus");
	//для загрузки библиотеки
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD(2, 2);//версия библиотеки(у нас 2)
	//проверка загрузки библиотеки:
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) return 0;
	struct sockaddr_in peer;
	peer.sin_family = AF_INET;
	peer.sin_port = htons(1280);
	peer.sin_addr.s_addr = inet_addr("127.0.0.1");

	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
	connect(s, (struct sockaddr*)&peer, sizeof(peer));
	char b[34];
	b[0] = '\0';
	recv(s, b, sizeof(b), 0);
	cout << b << endl;
	b[0] = '\0';
	while (true) {
		cout << endl << "Для отправки смс введите - 1" << endl;
		cout << "Для чтения смс введите - 2" << endl;
		cout << "Для удаления отправленных смс введите - 3" << endl;
		cout << "Выход - 0 " << endl;
		cin.getline(b, 34, '\n');
		send(s, b, sizeof(b), 0);
		switch (atoi(b)) {
		case 1:
		{
			b[0] = '\0';
			cout << "Для отправки смс введите номер - сообщение" << endl;
			cin.getline(b, 34, '\n');
			send(s, b, sizeof(b), 0);
			cout << "Ваше письмо отправлено!" << endl;
			break;
		}
		case 2:
		{
			b[0] = '\0';
			cout << "Для получения смс введите номер" << endl;
			cin.getline(b, 34, '\n');
			send(s, b, sizeof(b), 0);
			b[0] = '\0';
			recv(s, b, sizeof(b), 0);
			cout << "Ваше письмо:" << b << endl;
			break;
		}
		case 3:
		{
			int i = 0, c = 0;
			int n = 0;
			b[0] = '\0';
			cout << "Выберите № сообщения для удаления:" << endl;
			cout << "№ " << " номер " << " текст " << endl;
			recv(s, b, sizeof(b), 0);
			cout << n << " ";
			for (i; i < sizeof(b); i++) {
				cout << b[i];
				if (b[i] == ' ') {
					c++;
					if (c % 2 == 0) {
						cout << endl;
						cout << ++n << " ";
					}
				}
			}
			cin.getline(b, 34, '\n');
			send(s, b, sizeof(b), 0);
			b[0] = '\0';
			recv(s, b, sizeof(b), 0);
			cout << b << endl;
			break;
		}
		case 0: {
			break;
		}
		}
	}
	WSACleanup();
	return 0;
}