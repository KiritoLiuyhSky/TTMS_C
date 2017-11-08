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
		printf("\t\t\t%*s",32,"信息查询\n");
        printf("\t\t\t演出相关查询:\n\n");
		printf("\t\t\t[S]演出相关                     [P]演出\n");
		printf("\t\t\t[C]时间表\n");
        printf("\n\t\t\t票务查询:\n\n");
		printf("\t\t\t[T]查票\n");
        printf("  \t\t\t*************************************************************\n" );
		printf("\t\t\t[R]返回\n");
        printf("\n\t\t\t==================================================================\n");
		printf("\t\t\t请选择功能 :");
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
