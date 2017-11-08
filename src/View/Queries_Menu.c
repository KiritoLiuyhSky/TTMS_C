#include <stdio.h>
#include <stdlib.h>

#include "Queries_Menu.h"
#include "Schedule_UI.h"
#include "Studio_UI.h"
#include "Play_UI.h"

void Queries_Menu(void){
	char choice;
	system("cls");
	do {
        printf("\n\t\t\t==================================================================\n");
		printf("\t\t\t%*s",32,"��Ϣ��ѯ\n");
        printf("\t\t\t�ݳ���ز�ѯ:\n\n");
		printf("\t\t\t[S]�ݳ����                     [P]�ݳ�\n");
		printf("\t\t\t[C]ʱ���\n");
        printf("\n\t\t\tƱ���ѯ:\n\n");
		printf("\t\t\t[T]��Ʊ\n");
        printf("  \t\t\t*************************************************************\n" );
		printf("\t\t\t[R]����\n");
        printf("\n\t\t\t==================================================================\n");
		printf("\t\t\t��ѡ���� :");
		fflush(stdin);
		choice = getchar();
		switch (choice) {
		case 'S':
		case 's'://Studio_query;
			break;
		case 'P':
		case 'p':
			Play_UI_MgtEntry(1);
			break;
		case 'T':
		case 't': //Ticket_query;
			break;
		case 'C':
		case 'c':
			Schedule_UI_ListAll();
			break;
		}
	} while ('R' != choice && 'r' != choice);
	}
