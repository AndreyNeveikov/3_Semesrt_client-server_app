#include <sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale>
int main() {
	setlocale(LC_ALL, "rus");
	struct sockaddr_in peer;
	int s, t, t1, amount=5;
	int rc;
	char buf[256], p, p1, b[256];;

	peer.sin_family = AF_INET;
	peer.sin_port = htons(7500);
	peer.sin_addr.s_addr = inet_addr("127.0.0.1");

	s = socket(AF_INET, SOCK_STREAM, 0);
	rc = connect(s, (struct sockaddr*)&peer, sizeof(peer));

	puts("Сервер хранит информацию о наличии билетов к кассе кинотеатра.\nКлиент имеет возможность просмотра, добавления, удаления информации,\nсор-тировки по разным полям, а также поиска по заданному условию\n (например, по фильму, по стоимости билета, по времени начала сеанса). \nПрограмма клиента должна содержать меню, позволяющее осуществлять\nуказанные действия на сервере. \n");

	while (true) {
		//Выбор пункта меню и отправка его серверу
		puts("Выберите действие:");
		puts("\t1 - Добавить фильм");
		puts("\t2 - Удалить фильм");
		puts("\t3 - Список фильмов");
		puts("\t4 - Отсортировать фильмы");
		puts("\t5 - Поиск фильма");
		puts("\t6 - Выход");
		scanf("%s", buf);
		buf[1] = '\0';
		send(s, buf, sizeof(buf), 0);
		p = buf[0];

		switch (p) {
		case '1':
		{
			buf[0] = '\0';
			if (amount >= 10)
			{
				puts("Достигнут максимум элементов");
				break;
			}
			puts("Введите название фильма:"); scanf("%s", buf);
			send(s, buf, sizeof(buf), 0);
			puts("Введите стоимость билета: "); scanf("%s", buf);
			send(s, buf, sizeof(buf), 0);
			puts("Введите время до начала сеанса: "); scanf("%s", buf);
			send(s, buf, sizeof(buf), 0);
			puts("Введите длительность фильма: "); scanf("%s", buf);
			send(s, buf, sizeof(buf), 0);
			amount++;
			break;
		}
		case '2': //Подредактировать
		{
			buf[0] = '\0';
			puts("Введите номер фильма");
			scanf("%s", buf);
			int num = atoi(buf);
			if (num > amount || num < 1)
			{
				puts("Ошибка ввода");
				break;
			}//Какой номер будем редактировать			send(s,buf,sizeof(buf),0);
			send(s, buf, sizeof(buf), 0);
			amount--;
			break;
		}
		case '3'://Просмотреть 5 записей
		{
			buf[0] = '\0';
                        int count = 1;
			for (int i = 0; i < amount; i++)
			{
				recv(s, buf, sizeof(buf), 0);
                                printf("%d. %s", count, buf);
				puts("\n");
                                count++;
			}
			break;
		}
		case '4':
		{
			buf[0] = '\0';
			puts("Выберите вариант сортировки:\n1.По названию фильму\n2.По стоимости билета\n3.По времени до начала\n4.По продолжительности");
			scanf("%s", buf);
			if (atoi(buf) > 4 || atoi(buf) < 1)
			{
				puts("Ошибка ввода");
				break;
			}
			send(s, buf, sizeof(buf), 0);
			puts("Успешно отсортированно");
			break;
		}
		case '5':
		{
			buf[0] = '\0';
			while (true)
			{
				puts("Выберите пункт, по которому желаете осуществить поиск:\n1.Название фильма\n2.Стоимость билета\n3.Время до начала\n4.Продолжительность фильма");
				scanf("%s", buf);
				if (atoi(buf) > 4 || atoi(buf) < 1)
				{
					puts("Ошибка ввода");
					continue;
				}
				send(s, buf, sizeof(buf), 0);
				puts("Введите данные поиска");
				scanf("%s", buf);
				send(s, buf, sizeof(buf), 0);
				int counter = 0;
				puts("Результат:");
				
				//puts(buf);
			    recv(s, buf, sizeof(buf), 0);
				counter = atoi(buf);
				if (counter == 0)
				{
					puts("Отсутствует");
					break;
				}
				for (int i = 0; i < counter; i++)
				{
                                        
					recv(s, buf, sizeof(buf), 0);
					puts(buf);
				}
				break;
			}
			break;
		}
		case '6':
		{//Выход
			exit(0);
		}
		}
	}
}
