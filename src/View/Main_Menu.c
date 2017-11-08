#include <stdio.h>

#include "Main_Menu.h"
#include "Schedule_UI.h"
#include "Studio_UI.h"
#include "Play_UI.h"
#include "Queries_Menu.h"
#include "Account_UI.h"
#include "Sale_UI.h"
#include "SalesAnalysis_UI.h"

void Main_Menu(void) {
	char choice;
	do {
		system("cls");
		printf("\n\t\t\t==================================================================\n");
		printf("\t\t\t      \t\t\t��ԺƱ�����ϵͳ\n");
		printf("\t\t\t------------------------------------------------------------------\n");
		printf("\t\t\t[R]��Ʊ              \t\t\t\t[P]�ݳ�����\n");
		printf("\t\t\t------------------------------------------------------------------\n");
		printf("\t\t\t[T]��Ʊ               \t\t\t\t[S]�ݳ������� \n");
		printf("\t\t\t------------------------------------------------------------------\n");
		printf("\t\t\t[Q]��ѯ               \t\t\t\t[N]����ͳ��\n");
		printf("\t\t\t------------------------------------------------------------------\n");
		printf("\t\t\t[L]ע��               \t\t\t\t[A]�û����� \n");
		printf("\t\t\t******************************************************************\n" );
		printf("\t\t\t[E]�˳�\n");
		printf("\n\t\t\t==================================================================\n");
		printf("\t\t\tѡ����:");
		fflush(stdin);
		choice = getchar();
		switch (choice) {
		case 'S':
		case 's':
			Studio_UI_MgtEntry();
			break;
		case 'P':
		case 'p':
			Play_UI_MgtEntry(0);
			break;
		case 'Q':
		case 'q':
			Queries_Menu();
			break;
		case 'T':
		case 't':
			Sale_UI_MgtEntry();
			break;
		case 'R':
		case 'r':
			Sale_UI_ReturnTicket();
			break;
		case 'N':
		case 'n':
			SalesAanalysis_UI_MgtEntry();
			break;
		case 'A':
		case 'a':
			Account_UI_MgtEntry();
			break;
		case 'L':
		case 'l':
			if(Account_Srv_Logout()){
				printf("\n\t\t\tע���ɹ����밴���������!");
				getchar();
				getchar();
				main();
			}
			break;
		}
	} while ('E' != choice && 'e' != choice);
}
