#include <sys/types.h>
#include<sys/socket.h>
#include<sys/signal.h>
#include<sys/wait.h>
#include<sys/resource.h>
#include<netinet/in.h>

#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

struct Film {
	char name[100];
	char cost[100];
	char time_to_start[100];
	char longing[100];
} em[10];
int amount = 5;
// процедура для обслуживания соединения
int Func(int newS) {
	long int i, num, t, mon, doh, nal;
	float sum;
	int m;
	char p, p1, s;
	char buf[256], b[256];
	while (true) {
		recv(newS, buf, sizeof(buf), 0);
		p = buf[0];
		switch (p) {
		case '1':
		{
			//Добавить фильм
			buf[0] = '\0';
			recv(newS, buf, sizeof(buf), 0);
			strcpy(em[amount].name, buf);
			recv(newS, buf, sizeof(buf), 0);
			strcpy(em[amount].cost, buf);
			recv(newS, buf, sizeof(buf), 0);
			strcpy(em[amount].time_to_start, buf);
			recv(newS, buf, sizeof(buf), 0);
			strcpy(em[amount].longing, buf);
			amount++;
			break;
		}
		case '2':
		{
			//Удалить фильм
			buf[0] = '\0';
			recv(newS, buf, sizeof(buf), 0);  //Номер
			num = atoi(buf);
			for (int i = num - 1; i < amount - 1; i++)
			{
				Film temp = em[i];
				em[i] = em[i + 1];
				em[i + 1] = temp;
			}
			for (int i = 0; i < 100; i++)
			{
				em[amount].name[i] = 0;
				em[amount].cost[i] = 0;
				em[amount].time_to_start[i] = 0;
				em[amount].longing[i] = 0;
			}
			amount--;
			break;
		}
		
		case '3':
		{
			//Список фильмов
			for (i = 0; i < amount; i++) {
				buf[0] = '\0';
				strcat(buf, "Название фильма: ");
				strcat(buf, em[i].name);
				strcat(buf, "  Стоимость билета: ");
				strcat(buf, em[i].cost);
				strcat(buf, "  Время до начала: ");
				strcat(buf, em[i].time_to_start);
				strcat(buf, "  Продолжительность сеанса: ");
				strcat(buf, em[i].longing);
				send(newS, buf, sizeof(buf), 0);
			}
			break;
		}
		case '4':
		{
			//Отсортировать фильмы
			recv(newS, buf, sizeof(buf), 0);
			char p1 = buf[0];
			switch (p1)
			{
			case '1':
			{
				for (int i = 0; i < amount; i++)
				{
					for (int j = i + 1; j < amount; j++)
					{
						if (strcmp(em[i].name, em[j].name) > 0)
						{
							Film temp = em[i];
							em[i] = em[j];
							em[j] = temp;
						}
					}
				}
				break;
			}
			case '2':
			{
				for (int i = 0; i < amount; i++)
				{
					for (int j = i+1; j < amount; j++)
					{
						if (atoi(em[i].cost) > atoi(em[j].cost))
						{
							Film temp = em[i];
							em[i] = em[j];
							em[j] = temp;
						}
					}
				}
				break;
			}
			case '3':
			{
				for (int i = 0; i < amount; i++)
				{
					for (int j = i+1; j < amount; j++)
					{
						if (atoi(em[i].time_to_start)> atoi(em[j].time_to_start))
						{
							Film temp = em[i];
							em[i] = em[j];
							em[j] = temp;
						}
					}
				}
				break;
			}
			case '4':
			{
				for (int i = 0; i < amount; i++)
				{
					for (int j = i+1; j < amount; j++)
					{
						if (atoi(em[i].longing) > atoi(em[j].longing))
						{
							Film temp = em[i];
							em[i] = em[j];
							em[j] = temp;
						}
					}
				}
				break;
			}
			}
			break;
		}
		case '5':
		{
			//Поиск фильма
            buf[0]='\0';
			recv(newS, buf, sizeof(buf), 0);
			char p1 = buf[0];
			recv(newS, buf, sizeof(buf), 0);
			char buf2[10][256];
				for(int i = 0; i < 10; i++)
					{
						buf2[i][0]='\0';
					}
			int c2 = 0;
			switch (p1)
			{
			case '1':
				{
				
				for (int i = 0; i < amount; i++)
				{
					int counter = 0;
					bool b_counter = false;
					if (strcmp(em[i].name, buf) == 0)
					{
						ln:
						strcat(buf2[c2], "Название фильма: ");
						strcat(buf2[c2], em[i].name);
						strcat(buf2[c2], "  Стоимость билета: ");
						strcat(buf2[c2], em[i].cost);
						strcat(buf2[c2], "  Время до начала: ");
						strcat(buf2[c2], em[i].time_to_start);
						strcat(buf2[c2], "  Продолжительность сеанса: ");
						strcat(buf2[c2], em[i].longing);
						
						c2++;
						continue;
					}
					for (int j = 0; em[i].name[j] != '\0' || buf[j] != '\0'; j++)
					{
						if (counter == 3)
						{
							b_counter = true;
							break;
						}
						if (em[i].name[j] == buf[j])
						{
							counter++;
						}
					}
					if (b_counter == true)
					{
						goto ln;
					}
				}
					break;
				}
			case '2':
			{
				for (int i = 0; i < amount; i++)
				{
					if (atoi(em[i].cost) == atoi(buf))
					{

						strcat(buf2[c2], "Название фильма: ");
						strcat(buf2[c2], em[i].name);
						strcat(buf2[c2], "  Стоимость билета: ");
						strcat(buf2[c2], em[i].cost);
						strcat(buf2[c2], "  Время до начала: ");
						strcat(buf2[c2], em[i].time_to_start);
						strcat(buf2[c2], "  Продолжительность сеанса: ");
						strcat(buf2[c2], em[i].longing);
						c2++;
					}
				}
				break;
			}
			case '3':
			{
				for (int i = 0; i < amount; i++)
				{
					if (atoi(em[i].time_to_start) == atoi(buf))
					{
						
						strcat(buf2[c2], "Название фильма: ");
						strcat(buf2[c2], em[i].name);
						strcat(buf2[c2], "  Стоимость билета: ");
						strcat(buf2[c2], em[i].cost);
						strcat(buf2[c2], "  Время до начала: ");
						strcat(buf2[c2], em[i].time_to_start);
						strcat(buf2[c2], "  Продолжительность сеанса: ");
						strcat(buf2[c2], em[i].longing);
						c2++;
					}
				}
				break;
			}
			case '4':
			{
				for (int i = 0; i < amount; i++)
				{
					if (atoi(em[i].longing) == atoi(buf))
					{
						
						strcat(buf2[c2], "Название фильма: ");
						strcat(buf2[c2], em[i].name);
						strcat(buf2[c2], "  Стоимость билета: ");
						strcat(buf2[c2], em[i].cost);
						strcat(buf2[c2], "  Время до начала: ");
						strcat(buf2[c2], em[i].time_to_start);
						strcat(buf2[c2], "  Продолжительность сеанса: ");
						strcat(buf2[c2], em[i].longing);
						c2++;
					}
				}
				break;
			}
			}
                        char buf3[256];
                        buf3[0]=c2+'0';
                        puts(buf3);
			send(newS, buf3, sizeof(buf3), 0);
			for (int i = 0; i < c2; i++)
			{
                                puts(buf2[i]);
				send(newS, buf2[i], sizeof(buf2[i]), 0);
			}
			break;
		}
		case '6':
		{
			exit(0);
		}

		}
	}
}
void reaper(int sig) {
	int status;
	while (wait3(&status, WNOHANG, (struct rusage*)0) >= 0);
}

int main() {
	strcpy(em[0].name, "Duna");
	strcpy(em[0].cost, "7");
	strcpy(em[0].time_to_start, "30");
	strcpy(em[0].longing, "130");

	strcpy(em[1].name, "Forsag");
	strcpy(em[1].cost, "10");
	strcpy(em[1].time_to_start, "10");
	strcpy(em[1].longing, "110");

	strcpy(em[2].name, "Batman");
	strcpy(em[2].cost, "6");
	strcpy(em[2].time_to_start, "40");
	strcpy(em[2].longing, "90");

	strcpy(em[3].name, "SquidGames");
	strcpy(em[3].cost, "8");
	strcpy(em[3].time_to_start, "30");
	strcpy(em[3].longing, "150");

	strcpy(em[4].name, "Lucifer");
	strcpy(em[4].cost, "7");
	strcpy(em[4].time_to_start, "10");
	strcpy(em[4].longing, "120");

	struct sockaddr_in local;
	int s, newS, rc;

	local.sin_family = AF_INET;
	local.sin_port = htons(7500);
	local.sin_addr.s_addr = htonl(INADDR_ANY);

	s = socket(AF_INET, SOCK_STREAM, 0);
	rc = bind(s, (struct sockaddr*)&local, sizeof(local));
	rc = listen(s, 5);
	(void)signal(SIGCHLD, reaper);
	while (true) {
		newS = accept(s, NULL, NULL);
		switch (fork()) {
		case 0:
			(void)close(s);
			exit(Func(newS));
		default:
			(void)close(newS);
		}
	}

	return 0;
}


