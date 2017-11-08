/*
* File name:  Schedule_UI.c
* File ID:	  TTMS_SSF_Schedule_UI_Sour
* Author:     04153007_梁嘉龙 
* Version:    V1.5
* Date: 	  2016-06-21
*/


#include "Schedule_UI.h"
#include "Play_UI.h"

#include "../Common/list.h"
#include "../Service/Schedule.h"
#include "../Service/Play.h"
#include "../Service/Studio.h"
#include "../Service/EntityKey.h"

#include <stdio.h>
#include <stdlib.h>

	/*
	 * Function:    Schedule_UI_ListAll
	 * Function ID:	TTMS_SCU_Schedule_UI_List
	 * Description: 显示所有的演出计划信息,进行查询。
	 * Input:       无
	 * Output:      所有剧目的信息
	 * Return:      无
	 */
void Schedule_UI_ListAll(void) {
	int i, id;
	char choice;

	schedule_list_t head;
	schedule_node_t *p;
	Pagination_t paging;

	List_Init(head, schedule_node_t);

	paging.offset = 0;
	paging.pageSize = SCHEDULE_PAGE_SIZE;

	//载入数据
	paging.totalRecords = Schedule_Srv_FetchAll(head);

	Paging_Locate_FirstPage(head, paging);

	do {
		system("cls");
		printf("\n\t\t\t==================================================================\n");
		printf("\t\t\t      \t\t\t演出计划 列表\n");
		printf("\t\t\t------------------------------------------------------------------\n");
		printf("\t\t\t%5s%5s%8s%10s%10s%10s\n", "计划", "剧目", "演出厅",
					 "放映日期","放映时间","座位数");
		//显示数据
		Paging_ViewPage_ForEach(head, paging, schedule_node_t, p, i){
			printf("\t\t\t%2d %2d %2d", p->data.id, p->data.play_id,p->data.studio_id);

			printf("%4d-%02d-%02d ",p->data.date.year, p->data.date.month, p->data.date.day);
			printf("%2d:%02d:%02d ", p->data.time.hour, p->data.time.minute, p->data.time.second);
			printf("%5d\n", p->data.seat_count);
		}


			printf("\n\t\t\t-------- 共 %2d 项 --------------------------- %2d/%2d 页 --------\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
			printf("\t\t\t******************************************************************\n" );
			printf("\t\t\t       [P]上页      |      [N]下页      |        [R]返回\n\n");



			printf("\n\t\t\t==================================================================\n");
			printf("\t\t\t请选择:");

			choice=getchar();


			switch (choice) {


			case 'p':
			case 'P':
				if (!Pageing_IsFirstPage(paging)) {
					Paging_Locate_OffsetPage(head, paging, -1, schedule_node_t);
				}
				break;
			case 'n':
			case 'N':
				if (!Pageing_IsLastPage(paging)) {
					Paging_Locate_OffsetPage(head, paging, 1, schedule_node_t);
				}
				break;
			}
		} while (choice != 'r' && choice != 'R');
		//释放链表空间
		List_Destroy(head, schedule_node_t);
}

/*
 * Function:    Schedule_UI_Add
 * Function ID:	TTMS_SCU_Schedule_UI_Add
 * Description: 添加一条演出计划信息
 * Input:       相关剧目的ID号
 * Output:      输入信息时的各种提示
 * Return:      添加的记录数
 */
int Schedule_UI_Add(int play_id) {
	schedule_t rec;
	int newRecCount = 0;
	char choice;

	do {
		system("cls");
        printf("\n\t\t\t==================================================================\n");
    	printf("\t\t\t      \t\t\t添加放映室\n");
        printf("\t\t\t------------------------------------------------------------------\n");
		fflush(stdin);
		rec.id = EntKey_Srv_CompNewKey("schedule");
		printf("\t\t\t剧目ID:");
		scanf("%d",&(rec.play_id));
		printf("\t\t\t演出厅ID:");
		scanf("%d", &(rec.studio_id));
		printf("\t\t\t放映日期:");
		scanf("%d%d%d", &(rec.date.year),&(rec.date.month),&(rec.date.day));
		printf("\t\t\t放映时间:");
		getchar();
		scanf("%d%d",&(rec.time.hour),&(rec.time.minute)/*,&(rec.time.second)*/);
		rec.seat_count=0;
        printf("\n\t\t\t==================================================================\n");

		if (Schedule_Srv_Add(&rec)) {
			rec.seat_count += 1;
			newRecCount += 1;
			printf("\t\t\t添加成功！\n");
		} else
			printf("\t\t\t添加失败!\n");
        printf("\t\t\t****************************************************************\n" );
		printf("\t\t\t[A]继续添加       [R]返回");
		fflush(stdin);
		scanf("%c", &choice);
	} while ('a' == choice || 'A' == choice);
	return newRecCount;
}

/*
 * Function:    Schedule_UI_Modify
 * Function ID:	TTMS_SCU_Schedule_UI_Mod
 * Description: 按照ID号更新演出计划信息
 * Input:       待更新的剧目ID号
 * Output:      输入信息时的各种提示
 * Return:      更新的剧目信息数，0表示未找到，1表示找到并更新
 */
int Schedule_UI_Modify(int id){
	/*schedule_t rec;
	int rtn = 0,seatcount;

	if (!Schedule_Srv_FetchByID(id, &rec)) {
		printf("\t\t\t放映室不存在\n按回车键结束\n");
		getchar();
		getchar();
		return 0;
	}

	printf("\n\t\t\t==================================================================\n");
	printf("\t\t\t      \t\t\t更新放映计划\n");
	printf("\t\t\t------------------------------------------------------------------\n");
	printf("\t\t\t演出计划ID:%d\n", rec.id);
	printf("\t\t\t剧目ID:%d\n",rec.play_id);
	printf("\t\t\t演出厅ID:%d\n", rec.studio_id);
	printf("\t\t\t放映日期:%d年%2d月%2d日\n",rec.date.year,rec.date.month,rec.date.day);
	printf("\t\t\t放映时间:%d:%d\n",rec.time.hour,rec.time.minute);
	printf("\t\t\t座位数:%d\n", rec.seat_count);
	seatcount =Schedule_Perst_SelectByID(rec.id);
	if (!seatcount) {
		printf("\t\t\t演出厅ID:");
		scanf("%d", &(rec.studio_id));
		printf("\t\t\t剧目ID:");
		scanf("%d",&(rec.play_id));
		printf("\t\t\t放映日期:");
		scanf("%d%d%d", &(rec.date.year),&(rec.date.month),&(rec.date.day));
		printf("\t\t\t放映时间:");
		scanf("%d%d",&(rec.time.hour),&(rec.time.minute));
	}

    printf("------------------------------------------------------------------\n");
	Schedule_Perst_DeleteByID(id);
	Schedule_Perst_Insert(&rec);

	if (Schedule_Srv_Modify(&rec)) {
		rtn = 1;
		printf(
				"\t\t\t更新成功\n按回车键返回\n");
	} else
		printf("\t\t\t更新失败\n按回车键返回\n");

	getchar();
	return rtn;*/
	int ret=0,sid;
       	char choice;
	schedule_t data;
	studio_t sdata;	
	
	if(Schedule_Perst_SelectByID(id, &data)){
		
		while(1){ 

				system("cls");
				printf("\n\t\t\t==================================================================\n");
				printf("\t\t\t      \t\t\t修改 演出计划\n");
       			printf("\n\t\t\t==================================================================\n");
       			printf("\t\t\t%5s \t %5s\t%8s \t %10s \t %10s \t %10s \n", "ID", "剧目ID", "演出厅ID",
				 "放映日期","放映时间","座位数");
				printf("\t\t\t------------------------------------------------------------------\n");
		
		
		
				printf("\t\t\t%5d %5d %5d ", data.id, data.play_id,data.studio_id);
			
				printf("\t%4d-%02d-%02d ",data.date.year, data.date.month, data.date.day);
				printf("\t%2d:%02d:%02d ", data.time.hour, data.time.minute, data.time.second);
				printf("\t%5d\n", data.seat_count);
				
				printf("\n\t\t\t==================================================================\n");
				printf("\t\t\t[A]修改演出厅        [B]修改放映时间          [R]返回\n");
				printf("\n\t\t\t==================================================================\n");
				printf("\t\t\t请选择:");
				choice=l_getc();
				if('r'==choice || 'R'==choice) break;	
				switch(choice){
					
					case 'a':
					case 'A':
				
						printf("\t\t\t请输入演播厅的ID：");
				
						while(1){
							if(scanf("%d",&sid)==1){
							 cl_stdin(); 
							 
							 if(!Studio_Srv_FetchByID(sid, &sdata)){
							 	printf("\n\t\t\tID为%d的演出厅不存在，请重新输入：",sid);
							 	continue;
							 }
							 
							data.studio_id=sdata.id;
							data.seat_count=Seat_Number_Count(data.studio_id);
							 
							 break;
							}
							else{ cl_stdin(); printf("\t\t\t您的输入有误！请重新输入:"); }
						}
						break;
		
					case 'b':
					case 'B':
				
						printf("\t\t\t请输入放映日期:(年 月 日) ");
						while(1){
							if(scanf("%d%d%d",&(data.date.year), &(data.date.month), &(data.date.day))==3){
								 cl_stdin();
								 if(data.date.month>12 || data.date.day>31){
								 	printf("\t\t\t您输入的 月 或 日 有误！请重新输入: ");
								 	continue;
								 }
								 if(DateCmp(data.date, DateNow())==-1){
								 	printf("\t\t\t您输入的日期早于今日！请重新输入: ");
								 	continue;
								 
								 }
								 
								 break;
							}
							else{ cl_stdin(); printf("\t\t\t您的输入有误！请重新输入: "); }
						}
						
				
				
						printf("\t\t\t请输入放映时间:(时 分) ");
						while(1){ 
							if(scanf("%d%d",&(data.time.hour), &(data.time.minute))==2){
								data.time.second=0;
								cl_stdin();
								if(data.time.hour>24 || data.time.minute>60){
								 	printf("\t\t\t您输入的 时 或 分 有误！请重新输入:");
								 	continue;
								 }
						
								 break;
							}
							else{ cl_stdin(); printf("\t\t\t您的输入有误！请重新输入:"); }
						}
						
						break;
				
				
				}//switch

				if(Schedule_Srv_Modify(&data)){
					
					printf("\t\t\t修改成功！\n");
					ret=1;
				}else{
					printf("\t\t\t修改失败！\n");
				}
				
				printf("\t\t\t按任意键继续修改。");
		
				getchar();
		
		}//while
	}else{
		printf("\t\t\t未找到ID为%d的演出计划！\n",id);
		printf("\t\t\t按任意键返回！\n");
		//cl_stdin();
		getchar();
		return 0;
	}
       return ret;
}

/*
 * Function:    Schedule_UI_Delete
 * Function ID:	TTMS_SCU_Schedule_UI_Del
 * Description: 按照ID号删除演出计划信息
 * Input:       待删除的剧目ID号
 * Output:      提示删除是否成功
 * Return:      0表示删除失败，1表示删除成功
 */
int Schedule_UI_Delete(int id){

	int rtn = 0;

	if (Schedule_Srv_DeleteByID(id)) {
		if (Ticket_Perst_Delete(id))
			printf("\t\t\t座位删除成功\n");
			printf("\t\t\t演出删除成功!\n按回车键返回\n");
		rtn = 1;
	} else {
		printf("\t\t\t放映室不存在\n按回车键返回\n");
	}

	getchar();
	return rtn;

}

/*
 * Function:    Schedule_UI_Query
 * Function ID:	TTMS_SCU_Schedule_UI_Qry
 * Description: 按照ID号查询演出计划信息
 * Input:       待查找的剧目ID号
 * Output:      查找到的剧目信息
 * Return:      0表示未找到，1表示找到了
 */
int Schedule_UI_Query(int id){

	schedule_t rec;
	/*int rtn = 0,seatcount;*/

	/*Load record*/
	if (!Schedule_Srv_FetchByID(id, &rec)) {
		printf("\t\t\t放映计划不存在\n按回车键返回\n");
		getchar();
		return 0;
	}

    printf("\n\t\t\t==================================================================\n");
	printf("\t\t\t      \t\t\t更新放映计划\n");
    printf("\t\t\t------------------------------------------------------------------\n");
    printf("\t\t\t演出计划ID:%d\n", rec.id);
    printf("\t\t\t剧目ID:%d\n",rec.play_id);
    printf("\t\t\t演出厅ID:%d\n", rec.studio_id);
    printf("\t\t\t放映日期:%d年%2d月%2d日\n",rec.date.year,rec.date.month,rec.date.day);
   	printf("\t\t\t放映时间:%2d:%2d\n",rec.time.hour,rec.time.minute/*,rec.time.second*/);
   	printf("\t\t\t座位数:%d\n", rec.seat_count);
	return 0;
}

/*
 * Function:    Schedule_UI_ListByPlay
 * Function ID:	TTMS_SCU_Schedule_UI_ListByPlay
 * Description: 以列表模式显示给定剧目的演出计划信息
 * Input:       list为查找到的演出计划信息链表的头指针，play为剧目信息，paging为显示分页设置参数
 * Output:      无
 * Return:      返回查找到的记录数目
 */
void Schedule_UI_ListByPlay(const play_t *play, schedule_list_t list, Pagination_t paging){
	Schedule_Srv_FetchAll(list);
	schedule_node_t *rec=list->next;
	while (rec != NULL){
		printf("\n\t\t\t==================================================================\n");
		printf("\t\t\t      \t\t\t更新放映计划\n");
		printf("\n\t\t\t------------------------------------------------------------------\n");
		printf("\t\t\t演出厅ID%d\n", rec->data.studio_id);
		printf("\t\t\t放映日期:%d年%2d月%2d日\n",rec->data.date.year,rec->data.date.month,rec->data.date.day);
		printf("\t\t\t放映时间:%2d:%2d\n",rec->data.time.hour,rec->data.time.minute/*,rec->data.time.second*/);
		printf("\t\t\t座位数:%d\n", rec->data.seat_count);
		rec=rec->next;
	}

}

/*
 * Function:    Schedule_UI_MgtEntry
 * Function ID:	TTMS_SCU_Schedule_UI_MgtEnt
 * Description: 演出计划管理界面，演出计划按照剧目进行管理，以剧目ID号为输入
 * Input:       相关剧目ID号
 * Output:      演出计划信息
 * Return:      无
 */
void Schedule_UI_MgtEntry(int play_id){
	int i,ID;
char choice;

schedule_list_t head;
schedule_node_t *pos;
Pagination_t paging;

List_Init(head, schedule_node_t);
paging.offset = 0;
paging.pageSize = SCHEDULE_PAGE_SIZE;

//载入数据
paging.totalRecords = Schedule_Srv_FetchAll(head);
Paging_Locate_FirstPage(head, paging);

do {
	system("cls");
	printf("\n\t\t\t==================================================================\n");
	printf("\t\t\t      \t\t\t演出计划\n");
	printf("\n\t\t\t==================================================================\n");
	printf("\n\t\t\t");
	printf("%-10s %-10s %-15s %-15s %-15s %-15s \n", "计划", "剧目", "演出厅","放映日期","放映时间","座位数");
	printf("\t\t\t------------------------------------------------------------------\n");
	//显示数据
	Paging_ViewPage_ForEach(head, paging, schedule_node_t, pos, i){
		printf("\t\t\t%-10d  %-10d  %-10d    %d年%2d月%2d日    %d:%d    %10d\n", pos->data.id,
				pos->data.play_id, pos->data.studio_id, pos->data.date.year,pos->data.date.month,pos->data.date.day,pos->data.time.hour,pos->data.time.minute/*,pos->data.time.second*/,pos->data.seat_count);
				}
	printf("\n\t\t\t------- 共 %2d 项 ----------------------- 第 %2d/%2d 页 ----------\n",
			paging.totalRecords, Pageing_CurPage(paging),
			Pageing_TotalPages(paging));
	printf("\n\t\t\t******************************************************************\n" );
	printf("\t\t\t[P]上一页  |  [N]下一页  |  [A]添加  |  [D]删除  |  [U]更新  |  [R]返回");
	printf("\n\t\t\t==================================================================\n");
	printf("\t\t\t按字母选择功能:");
	//fflush(stdin);
	scanf("%c", &choice);

	switch (choice) {
	case 'a':
	case 'A':
		if (Schedule_UI_Add(play_id)) //新添加成功，跳到最后一页显示
		{
			paging.totalRecords = Schedule_Srv_FetchAll(head);
			Paging_Locate_LastPage(head, paging, schedule_node_t);
		}
		break;
	case 'd':
	case 'D': {
		printf("\n\t\t\t==================================================================\n");
		printf("\t\t\t      \t\t\t更新演出计划\n");
		printf("\n\t\t\t------------------------------------------------------------------\n");
		printf("\t\t\t请输入要删除的演出计划ID:");
		scanf("%d",&ID);
		if (Schedule_UI_Delete(ID)) {	//从新载入数据
			paging.totalRecords = Schedule_Srv_FetchAll(head);
			List_Paging(head, paging, schedule_node_t);
		}
		break;
	}
	case 'u':
	case 'U': {
		printf("\n\t\t\t==================================================================\n");
		printf("\t\t\t      \t\t\t更新演出计划\n");
		printf("\t\t\t------------------------------------------------------------------\n");
		printf("\t\t\t请输入要修改的演出计划ID:");
		scanf("%d",&ID);
		if (Schedule_UI_Modify(ID)) {	//从新载入数据
			paging.totalRecords = Schedule_Srv_FetchAll(head);
			List_Paging(head, paging, schedule_node_t);
		}
		break;
	}
	case 'p':
	case 'P':
		if (!Pageing_IsFirstPage(paging)) {
			Paging_Locate_OffsetPage(head, paging, -1, schedule_node_t);
		}
		break;
	case 'n':
	case 'N':
			if (!Pageing_IsLastPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, 1, schedule_node_t);
			}
			break;
		}
	} while (choice != 'r' && choice != 'R');
	//释放链表空间
	List_Destroy(head, schedule_node_t);
}
