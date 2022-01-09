#pragma comment (lib, "ws2_32.lib")
#include <iostream>
#include <WinSock2.h>
#include <process.h> /* _beginthread, _endthread */
#include <string>
#include <vector>
#include <stdio.h>
#include <fstream>
#include <stdlib.h>
#pragma warning (disable: 4996)

using namespace std;

string file("D:\\уник\\server.txt");
struct Emails {
	string tel;
	string email;
}e;

vector<Emails> vec;
void readFileData(vector<Emails>& vec);
void writeFileData(vector<Emails> vec);

void sms(char* p, void* newS) {
	char number[34];
	number[0] = '\0';
	int j = 0;
	while (p[j] != '\0') {
		number[j] = p[j];
		j++;
	}
	number[j] = '\0';
	cout << "Номер телефона:" << (string)number << endl;
	for (int i = 0; i < vec.size(); i++) {
		if (vec.at(i).tel == number) {
			number[0] = '\0';
			strcat(number, vec.at(i).email.c_str());
			send((SOCKET)newS, number, sizeof(number), 0);
			break;
		}
		else if (i == vec.size() - 1) {
			number[0] = '\0';
			strcat(number, "не найдено!");
			send((SOCKET)newS, number, sizeof(number), 0);
		}
	}
}

void showAllSms(void* newS) {
	char buf[255] = "";
	string sms = "";
	for (int i = 0; i < vec.size(); i++) {
		sms += vec.at(i).tel + ' ' + vec.at(i).email + ' ';
	}
	sms += " ";
	buf[0] = '\0';
	strcpy(buf, sms.c_str());
	send((SOCKET)newS, buf, sizeof(buf), 0);
}

void sell(char* p) {
	Emails e;
	int k = 0, tel_n = 0, email_n = 0;
	char tel[34], email[34];
	tel[0] = '\0'; email[0] = '\0';
	for (int i = 0; i < strlen(p); i++) {
		if (p[i] == ' ') {
			k++;
		}
		if (k == 0) {
			tel[tel_n] = p[i];
			tel_n++;
		}
		if (k != 0) {
			email[email_n] = p[i];
			email_n++;
		}
	}
	tel[tel_n] = '\0';
	email[email_n] = '\0';
	vec.push_back(e);
	vec.at(0).tel = tel;
	vec.at(0).email = email;
	writeFileData(vec);
}

void SMSworking(void* newS) {
	readFileData(vec);
	char p[34], n[34];
	p[0] = '\0';
	n[0] = '\0';
	strcat(p, "SMS center connected \n");
	send((SOCKET)newS, p, sizeof(p), 0);
	while (recv((SOCKET)newS, n, sizeof(n), 0) != 0) {
		if (!strcmp(n, "1"))
		{
			p[0] = '\0';
			recv((SOCKET)newS, p, sizeof(p), 0);
			cout << "Полученны данные: " << p << endl;
			sell(p);
			n[0] = '\0';
		}
		if (!strcmp(n, "2"))
		{
			p[0] = '\0';
			recv((SOCKET)newS, p, sizeof(p), 0);
			sms(p, newS);
			n[0] = '\0';
		}
		if (!strcmp(n, "3"))
		{
			int ind;
			p[0] = '\0';
			showAllSms(newS);
			recv((SOCKET)newS, p, sizeof(p), 0);
			ind = atoi(p);
			if (ind >= 0 && ind < vec.size()) {
				cout << "Удалено сообщение:" << endl;
				string answ = "Сообщение удалено!";
				cout << vec.at(ind).tel << " " << vec.at(ind).email << endl;
				vec.erase(vec.begin() + ind);
				strcpy(p, answ.c_str());
				send((SOCKET)newS, p, sizeof(p), 0);
			}
			else {
				p[0] = '\0';
				string answ = "Такого сообщения не существует!";
				strcpy(p, answ.c_str());
				send((SOCKET)newS, p, sizeof(p), 0);
			}
			n[0] = '\0';
			writeFileData(vec);
		}
		if (!strcmp(n, "0")) {
			writeFileData(vec);
			closesocket((SOCKET)newS);
			exit(EXIT_SUCCESS);
			n[0] = '\0';
		}
	}
}

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
	sockaddr_in local; //структура для хранения адреса
	local.sin_family = AF_INET;
	local.sin_port = htons(1280);
	local.sin_addr.s_addr = htonl(INADDR_ANY);
	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);//семейство инет протоколов;указывает
	//на протокол, установ соединение; 0
		bind(s, (struct sockaddr*)&local, sizeof(local));//привязка адреса к сокету
	listen(s, 5);//установка соединения с клиентом
	cout << "Server receive ready" << endl;
	while (true) {
		sockaddr_in remote;
		int j = sizeof(remote);
		SOCKET newS = accept(s, (struct sockaddr*)&remote, &j);
		cout << "Новый пользователь IP - " << inet_ntoa(remote.sin_addr) << endl <<
			"порт - " << ntohs(remote.sin_port) << "\n";
		_beginthread(SMSworking, 0, (void*)newS);
	}
	WSACleanup();
	return 0;
}

void writeFileData(vector<Emails> vec) {
	ofstream fout(file, ios::out);
	for (int i = 0; i < vec.size(); i++)
	{
		fout << vec.at(i).tel
			<< vec.at(i).email;
		if (i < vec.size() - 1)
		{
			fout << endl;
		}
	}
	fout.close();
}

void readFileData(vector<Emails>& vec) {
	ifstream fin(file, ios::in);
	if (!fin.is_open()) cout << "Файл не существует!";
	else
	{
		vec.erase(vec.begin(), vec.end());
		while (!fin.eof())
		{
			fin >> e.tel
				>> e.email;
			vec.push_back(e);
		}
	}
	fin.close();
}
