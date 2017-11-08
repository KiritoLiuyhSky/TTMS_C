#include "Ticket_UI.h"

#include "../Common/list.h"
#include "../Service/ticket.h"
#include "../Service/schedule.h"
#include "../Service/play.h"
#include "../Service/studio.h"

#include <stdio.h>
#include <stdlib.h>

//根据状态返回对应表示状态符号
inline char Ticket_UI_Status2Char(ticket_status_t status) {

	char statusChar;
	switch (status) {
	case TICKET_AVL:		//有座位
		statusChar = '#';
		break;
	case TICKET_SOLD:	//损坏的座位
		statusChar = '@';
		break;
	case TICKET_RESV:
	default:
		statusChar = '~';
		break;
	}
	return statusChar;
}

void Ticket_UI_Print(int ID){
	//根据ID载入票
	ticket_t ticket;
	if(!Ticket_Srv_FetchByID(ID, &ticket)){
		printf("\t\t\t票ID:%d 不存在! \n 按回车返回\n ", ID);
		return ;
	}

	//载入演技计划及剧目
	schedule_t sch;
	play_t play;
	Schedule_Srv_FetchByID(ticket.schedule_id, &sch);
	Play_Srv_FetchByID(sch.play_id, &play);

	//载入座位及演出厅
	seat_t seat;
	studio_t room;
	Seat_Srv_FetchByID(ticket.seat_id, &seat);
	Studio_Srv_FetchByID(seat.roomID, &room);

	//显示票信息
	printf("\t\t\t------------------------------------------------------------------\n");
	printf("\t\t\t|%9s%-10d%15s%-5d%5s%-4d|\n", "票ID:", ticket.id,
			"行:", seat.row, "列:", seat.column );
	printf("\t\t\t|%9s%-39s|\n", "演出名称:", play.name);
	printf("\t\t\t|%9s%-11d %16d-%2d-%2d %2d:%2d|\n", "价格:", play.price,
				sch.date.year, sch.date.month, sch.date.day,
				sch.time.hour, sch.time.minute);
	printf("\t\t\t------------------------------------------------------------------\n");
}

//根据计划ID显示所有票
void Ticket_UI_ListBySch(const schedule_t *sch,	ticket_list_t tickList, seat_list_t seatList) {
	assert(NULL!=sch && NULL!=tickList && NULL!=seatList);
	int i, j;

	studio_t studioRec;
	play_t   playRec;
	ticket_node_t *pTicket;
	seat_node_t *pSeat;

	if (!Studio_Srv_FetchByID(sch->studio_id, &studioRec)) {  //获得对应id放映厅的信息
		printf("\t\t\t放映厅不存在\n按回车键返回\n");
		getchar();
		return;
	}

	if (!Play_Srv_FetchByID(sch->play_id, &playRec)) {  //获得对应id剧目的信息
		printf("\t\t\t演出不存在\n按回车键返回\n");
		getchar();
		return;
	}

	printf( "\t\t\t**********************  %s 的票务信息 ***********************\n", playRec.name);
	printf("\t\t\t%5c", ' ');
	for (i = 1; i <= studioRec.colsCount; i++) {
		printf("%3d", i);
	}

	printf("\n\t\t\t------------------------------------------------------------------\n");
	//显示数据
	for (i = 1; i <= studioRec.rowsCount; i++) {
		j = 1;
		printf("\t\t\t%2d:%c", i, '|');
		List_ForEach(tickList, pTicket)
		{
			pSeat=Seat_Srv_FindByID(seatList, pTicket->data.seat_id);
			if(NULL!=pSeat && pSeat->data.row == i) {
				while (pSeat->data.column != j) {
					printf("\t\t\t%3c", ' ');
					j++;
				}
				printf("\t\t\t%3c", Ticket_UI_Status2Char(pTicket->data.status));
				j++;
			}
		}
		printf("\n");
	}
	printf("\t\t\t******************************************************************\n");
	getchar();
}

void ListTickets(void){
	int i, id, schedule_id;
	schedule_t schedule_rec;
	play_t play_rec;
	seat_t seat_rec;
	char choice;

	ticket_list_t head;
	ticket_node_t *pos;
	Pagination_t paging;

	List_Init(head, ticket_node_t);
	paging.offset = 0;
	paging.pageSize = TICKET_PAGE_SIZE;

	printf("\t\t\t请输入演出计划ID\n");
	scanf("%d",&schedule_id);

	//载入数据
	paging.totalRecords = Ticket_Srv_FetchBySchID(head,schedule_id);
	Paging_Locate_FirstPage(head, paging);
	//需要增加查找座位信息

	Schedule_Srv_FetchByID(schedule_id,&schedule_rec);
	Play_Srv_FetchByID(schedule_rec.play_id,&play_rec);
	do {
		printf("\n\t\t\t==================================================================\n");
		printf("\t\t\t%*s",32,"票列表\n");
		printf("\t\t\tID\t\t演出名称\t\t行\t列\t 日期\t时间\t\t价格\t状态\n");
		printf("\t\t\t------------------------------------------------------------------\n");
		//显示数据
		for (i = 0, pos = (ticket_node_t *) (paging.curPos);
				pos != head && i < paging.pageSize; i++) {
			printf("\t\t\t%d\t%s\t%d-%d-%d\t%d:%d\t%d\t%s\n"/*t%d\t%d*/, pos->data.id, play_rec.name,
					seat_rec.row, seat_rec.column,
					schedule_rec.date.year,schedule_rec.date.month,
					schedule_rec.date.day,schedule_rec.time.hour,
					schedule_rec.time.minute,pos->data.price,
					pos->data.status==0?"to sell":"sold");
			pos = pos->next;
		}
		printf(
				"\t\t\t== 共 %d 项=========================== 第 %d/%d 页 ==\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf("\t\t\t[P]上一页 | [N]下一页 | [U]更新 | [R]返回 ");
		fflush(stdin);
		scanf("%c", &choice);
		switch (choice) {
		case 'u':
		case 'U':
			printf("\t\t\tInput the ID:");
			scanf("%d", &id);
			if (UpdateTicket(id)) {	//从新载入数据
				paging.totalRecords = Ticket_Srv_FetchBySchID(head,schedule_rec.id);
				List_Paging(head, paging, ticket_node_t);
			}
			break;
		case 'p':
		case 'P':
			if (1 < Pageing_CurPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, -1, ticket_node_t);
			}
			break;
		case 'n':
		case 'N':
			if (Pageing_TotalPages(paging) > Pageing_CurPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, 1, ticket_node_t);
			}
			break;
		}
	} while (choice != 'r' && choice != 'R');
	//释放链表空间
	List_Destroy(head, ticket_node_t);
	}


int UpdateTicket(int id){
	ticket_t rec;
	schedule_t schedule_rec;
	play_t play_rec;
	int rtn = 0;


	/*Load record*/
	if (!Ticket_Srv_FetchByID(id, &rec)) {
		printf("\t\t\tThe play does not exist!\nPress [Enter] key to return!\n");
		getchar();
		return 0;
	}
	Schedule_Srv_FetchByID(rec.schedule_id,&schedule_rec);
	Play_Srv_FetchByID(schedule_rec.play_id,&play_rec);
	//需要增加查找座位信息

	printf("\n\t\t\t==================================================================\n");
	printf("\t\t\t%*s",28,"更新票务信息\n");
	printf("\t\t\t------------------------------------------------------------------\n");
	printf("\t\t\t票ID:%d\n", rec.id);
	printf("\t\t\t演出名称[%s]:", play_rec.name);
	//需要输出座位的行列号
	printf("\t\t\t计划日期(yyyy-mm-dd)[%d-%d-%d]:",schedule_rec.date.year,
			schedule_rec.date.month,schedule_rec.date.day);
	printf("\t\t\t计划时间[%d:%d]:",schedule_rec.time.hour,schedule_rec.time.minute);
	printf("\t\t\t票价[%d]:",rec.price);
	scanf("%d",&(rec.price));
	printf("\t\t\t售票状态[%s](0.to sell,1.sold):",rec.status==1?"未售":"已售");
	scanf("%d",&rec.status);
	printf("\t\t\t------------------------------------------------------------------\n");

	if (Ticket_Srv_Modify(&rec)) {
		rtn = 1;
		printf(
				"\t\t\t票务信息更新成功！\n按回车键返回\n");
	} else
		printf("\t\t\t票务信息更新失败！\n按回车键返回\n");

	getchar();
	return rtn;
}


int QueryTicket(int id){

	int rtn=0;
	schedule_t schedule_rec;
	play_t play_rec;
	ticket_t rec;
	if (Ticket_Srv_FetchByID(id,&rec)) {
		printf("\n\t\t\t==================================================================\n");
		printf("\t\t\t%*s",3,"票务信息\n");
		printf("\t\t\tID\t\t演出名称t\t行号\t列号\t 日期\t时间\t\t票价\t状态\n");
		printf("\t\t\t------------------------------------------------------------------\n");
		Schedule_Srv_FetchByID(id,&schedule_rec);
		Play_Srv_FetchByID(schedule_rec.play_id,&play_rec);
		//添加查找座位行列信息的函数
		printf("\t\t\t%d\t%s\\t%d-%d-%d\t%d:%d\t%d\t%d\n"/*t%d\t%d*/, rec.id, play_rec.name,
//					seat_rec.row, seat_rec.column,
				schedule_rec.date.year,schedule_rec.date.month,
				schedule_rec.date.day,schedule_rec.time.hour,
				schedule_rec.time.minute,rec.price,rec.status);
		rtn=1;
	}
	else
	{
		printf("\t\t\t此票不存在\n按回车退出\n");
	}
	printf("\t\t\t------------------------------------------------------------------\n");
	system("pause");
	return rtn;
}
