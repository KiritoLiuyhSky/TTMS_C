/*
 * Studio_UI.c
 *
 *  Created on: 2015年4月24日
 *      Author: Administrator
 */
#include "Studio_UI.h"

#include "../Common/list.h"
#include "../Service/studio.h"
#include "../Service/seat.h"
#include "../Service/EntityKey.h"
#include "Seat_UI.h"

static const int STUDIO_PAGE_SIZE = 5;

#include <stdio.h>

void Studio_UI_MgtEntry(void) {
	int i, id;
	char choice;

	studio_list_t head;
	studio_node_t *pos;
	Pagination_t paging;

	List_Init(head, studio_node_t);
	paging.offset = 0;
	paging.pageSize = STUDIO_PAGE_SIZE;

	//载入数据
	paging.totalRecords = Studio_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);

	do {
		system("cls");
		printf("\n\t\t\t==================================================================\n");
		printf("\t\t\t%*s",32,"演出列表\n");
		printf("\t\t\t%5s  %18s  %10s  %10s  %10s\n", "ID", "名称", "行",
				"列", "座位");
		printf("\t\t\t------------------------------------------------------------------\n");
		//显示数据
		for (i = 0, pos = (studio_node_t *) (paging.curPos);
				pos != head && i < paging.pageSize; i++) {
			printf("\t\t\t%5d  %18s  %10d  %10d  %10d\n", pos->data.id,
					pos->data.name, pos->data.rowsCount, pos->data.colsCount,
					pos->data.seatsCount);
			pos = pos->next;
		}
		printf("\t\t\t------- 共%2d 项 ----------------------- 第 %2d/%2d ----\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf("  \t\t\t*************************************************************\n" );
		printf(
				"\t\t\t[P]上一页 | [N]下一页 | [A]添加 | [D]删除 | [U]更新 | [S]座位 | [R]返回");
		printf("\n\t\t\t==================================================================\n");
		printf("\t\t\t请选择功能:");
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);

		switch (choice) {
		case 'a':
		case 'A':
			if (Studio_UI_Add()) //新添加成功，跳到最后一页显示
			{
				paging.totalRecords = Studio_Srv_FetchAll(head);
				Paging_Locate_LastPage(head, paging, studio_node_t);
			}
			break;
		case 'd':
		case 'D':
			printf("\t\t\t请输入ID:");
			scanf("%d", &id);
			if (Studio_UI_Delete(id)) {	//从新载入数据
				paging.totalRecords = Studio_Srv_FetchAll(head);
				List_Paging(head, paging, studio_node_t);
			}
			break;
		case 'u':
		case 'U':
			printf("\t\t\t请输入ID:");
			scanf("%d", &id);
			if (Studio_UI_Modify(id)) {	//从新载入数据
				paging.totalRecords = Studio_Srv_FetchAll(head);
				List_Paging(head, paging, studio_node_t);
			}
			break;
		case 's':
		case 'S':
			printf("\t\t\t请输入ID:");
			scanf("%d", &id);
			Seat_UI_MgtEntry(id);
			paging.totalRecords = Studio_Srv_FetchAll(head);
			List_Paging(head, paging, studio_node_t);
			break;
		case 'p':
		case 'P':
			if (1 < Pageing_CurPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, -1, studio_node_t);
			}
			break;
		case 'n':
		case 'N':
			if (Pageing_TotalPages(paging) > Pageing_CurPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, 1, studio_node_t);
			}
			break;
		}
	} while (choice != 'r' && choice != 'R');
	//释放链表空间
	List_Destroy(head, studio_node_t);
}

int Studio_UI_Add(void) {
	studio_t rec;
	int newRecCount = 0;
	char choice;

	do {
		system("cls");
		printf("\n\t\t\t==================================================================\n");
		printf("\t\t\t%*s",28,"添加新的放映室\n");
		printf("\t\t\t------------------------------------------------------------------\n");
		printf("\t\t\t厅名");
		fflush(stdin);
		gets(rec.name);
		printf("\t\t\t共多少行座位");
		scanf("%d", &(rec.rowsCount));
		printf("\t\t\t共多少列座位");
		scanf("%d", &(rec.colsCount));
		rec.seatsCount = 0;
		printf("\n\t\t\t==================================================================\n");

		//获取主键
		rec.id = EntKey_Srv_CompNewKey("Studio");
		
		rec.seatsCount = rec.rowsCount*rec.colsCount;

		if (Studio_Srv_Add(&rec)) {
			newRecCount += 1;
			printf("\t\t\t添加成功\n");
		} else
			printf("\t\t\t添加失败\n");
			printf("\t\t\t------------------------------------------------------------------\n");
		printf("\t\t\t[A]继续添加		[R]返回");
		fflush(stdin);
		scanf("%c", &choice);
	} while ('a' == choice || 'A' == choice);
	return newRecCount;
}

int Studio_UI_Modify(int id) {
	studio_t rec;
	int rtn = 0;
	int newrow, newcolumn;
	seat_list_t list;
	int seatcount;

	/*Load record*/
	if (!Studio_Srv_FetchByID(id, &rec)) {
		printf("\t\t\t房间不存在\n按回车返回\n");
		getchar();
		return 0;
	}

	printf("\n\t\t\t==================================================================\n");
	printf("\t\t\t%*s",28,"更新演出厅\n");
	printf("\t\t\t------------------------------------------------------------------\n");
	printf("\t\t\t演出厅ID:%d\n", rec.id);
	printf("\t\t\t演出厅名字:[%s]:", rec.name);
	fflush(stdin);
	gets(rec.name);

	List_Init(list, seat_node_t);
	seatcount = Seat_Srv_FetchByRoomID(list, rec.id);
	if(seatcount){
		do{			//如果座位文件中已有座位信息，则更新的行列必须比以前大，否则不允许更改
			printf("\t\t\t座位行数应大于 >= [%d]:", rec.rowsCount);
			scanf("%d", &(newrow));
			printf("\t\t\t座位列数应小于 >= [%d]:", rec.colsCount);
			scanf("%d", &(newcolumn));
		}while(	newrow<rec.rowsCount||newcolumn<rec.colsCount);
		rec.rowsCount=newrow;
		rec.colsCount=newcolumn;
		rec.seatsCount=seatcount;
	}
	else{
		printf("\t\t\t共多少行座位: ");
		scanf("\t\t\t%d", &rec.rowsCount);
		printf("\t\t\t共多少列座位: ");
		scanf("%d", &rec.colsCount);
		rec.seatsCount=0;
	}

	printf("\t\t\t------------------------------------------------------------------\n");

	if (Studio_Srv_Modify(&rec)) {
		rtn = 1;
		printf(
				"\t\t\t更新成功\n按回车返回\n");
	} else
		printf("\t\t\t更新失败\n按回车返回\n");

	getchar();
	return rtn;
}

int Studio_UI_Delete(int id) {

	int rtn = 0;

	if (Studio_Srv_DeleteByID(id)) {
		//在删除放映厅时，同时根据放映厅id删除座位文件中的座位
		if(Seat_Srv_DeleteAllByRoomID(id))
			printf("\t\t\t座位删除成功\n");
		printf(
				"\t\t\t放映厅删除成功\n按回车返回\n");
		rtn = 1;
	} else {
		printf("\t\t\t放映厅不存在\n按回车返回\n");
	}

	getchar();
	return rtn;
}
