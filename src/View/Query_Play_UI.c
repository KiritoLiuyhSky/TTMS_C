#include "Query_Play_UI.h"
#include "../Service/play.h"
#include "../Persistence/Query_Persist.h"
#include "../Service/schedule.h"
#include "../Persistence/Schedule_Persist.h"
#include "../Service/studio.h"
#include "../Persistence/Ticket_Persist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void DisplayQueryPlay(void)
{
	fflush(stdin);
	char fileName[30];
	play_t play;

	char choice;
	do {
		fflush(stdin);
		memset(fileName, 0, sizeof(fileName));
		printf("\t\t\t�������ݳ�����:");
		scanf("%s",fileName);
		printf("\n\t\t\t==================================================================\n");
		printf("\t\t\t%*s",32,"�ݳ���Ϣ");
		if (Query_PlayName(fileName,&play)) {
			printf("\t\t\tID\t\t����\t\t����\t �ȼ�\t\t��ʼ����\t ��������\t�۸�\n");
			printf("\t\t\t-------------------------------------------------------\n");
			printf("\t\t\t%d\t\t%s\t\t%s\t%s\t%d-%d-%d\t%d-%d-%d\t%d\n", play.id,
								play.name,	play.area,
								(play.rating==1?"child   ":(play.rating==2?"teenager":"adult   ")),
								play.start_date.year,play.start_date.month,
								play.start_date.day,play.end_date.year,
								play.end_date.month,play.end_date.day,play.price);
			schedule_t  sched;
			studio_t studio = {0};
	//		seat_list_t list, int studio_id
			if (Query_Schedule_By_PlayId(play.id, &sched)) {
				Query_Studio_By_Schedule(sched.studio_id, &studio);
				printf("\t\t\t�ݳ������� %d:%d:%d %d:%d  %s �ݳ��� ����\n", sched.date.year, sched.date.month,
																			sched.date.day,sched.time.hour,
																			sched.time.minute, studio.name);
				printf("\t\t\t�� %d ����λ, ���� %d, ��ʣ %d\n", Seat_Number_Count(sched.studio_id), Sold_Ticket_Count(sched.id),
																			Seat_Number_Count(sched.studio_id)-Sold_Ticket_Count(sched.id));
			} else {
				printf("\t\t\t���и��ݳ�(��'^'��)\n");
			}
			printf("  \t\t\t*************************************************************\n" );
			printf("\t\t\t������ǰ����ĸѡ����\n");
			printf("\t\t\t[R]����, [B]Ԥ��, [A]�ٴβ�ѯ:");
			fflush(stdin);
			scanf("%c", &choice);
			switch(choice) {
			case 'a':
			case 'A':
				break;
			case 'B':
			case 'b':
				//Sale the ticket
				break;
			}
		} else {
			printf("\t\t\t�ݳ�������\n");
			printf("\t\t\t[R]����, [B]Ԥ��, [A]�ٴβ�ѯ:");
			fflush(stdin);
			scanf("%c", &choice);
			switch(choice) {
			case 'a':
			case 'A':
				break;
			case 'B':
			case 'b':
				//Sale the ticke
				break;
			}
		}
	} while (choice != 'r' && choice != 'R');
}
