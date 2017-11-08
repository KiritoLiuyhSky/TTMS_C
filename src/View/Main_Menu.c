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
		printf("\t\t\t      \t\t\t剧院票务管理系统\n");
		printf("\t\t\t------------------------------------------------------------------\n");
		printf("\t\t\t[R]退票              \t\t\t\t[P]演出管理\n");
		printf("\t\t\t------------------------------------------------------------------\n");
		printf("\t\t\t[T]售票               \t\t\t\t[S]演出厅管理 \n");
		printf("\t\t\t------------------------------------------------------------------\n");
		printf("\t\t\t[Q]查询               \t\t\t\t[N]排名统计\n");
		printf("\t\t\t------------------------------------------------------------------\n");
		printf("\t\t\t[L]注销               \t\t\t\t[A]用户管理 \n");
		printf("\t\t\t******************************************************************\n" );
		printf("\t\t\t[E]退出\n");
		printf("\n\t\t\t==================================================================\n");
		printf("\t\t\t选择功能:");
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
				printf("\n\t\t\t注销成功，请按任意键返回!");
				getchar();
				getchar();
				main();
			}
			break;
		}
	} while ('E' != choice && 'e' != choice);
}
