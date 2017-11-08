/*
* File name:  Schedule_UI.c
* File ID:	  TTMS_SSF_Schedule_UI_Sour
* Author:     04153007_������ 
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
	 * Description: ��ʾ���е��ݳ��ƻ���Ϣ,���в�ѯ��
	 * Input:       ��
	 * Output:      ���о�Ŀ����Ϣ
	 * Return:      ��
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

	//��������
	paging.totalRecords = Schedule_Srv_FetchAll(head);

	Paging_Locate_FirstPage(head, paging);

	do {
		system("cls");
		printf("\n\t\t\t==================================================================\n");
		printf("\t\t\t      \t\t\t�ݳ��ƻ� �б�\n");
		printf("\t\t\t------------------------------------------------------------------\n");
		printf("\t\t\t%5s%5s%8s%10s%10s%10s\n", "�ƻ�", "��Ŀ", "�ݳ���",
					 "��ӳ����","��ӳʱ��","��λ��");
		//��ʾ����
		Paging_ViewPage_ForEach(head, paging, schedule_node_t, p, i){
			printf("\t\t\t%2d %2d %2d", p->data.id, p->data.play_id,p->data.studio_id);

			printf("%4d-%02d-%02d ",p->data.date.year, p->data.date.month, p->data.date.day);
			printf("%2d:%02d:%02d ", p->data.time.hour, p->data.time.minute, p->data.time.second);
			printf("%5d\n", p->data.seat_count);
		}


			printf("\n\t\t\t-------- �� %2d �� --------------------------- %2d/%2d ҳ --------\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
			printf("\t\t\t******************************************************************\n" );
			printf("\t\t\t       [P]��ҳ      |      [N]��ҳ      |        [R]����\n\n");



			printf("\n\t\t\t==================================================================\n");
			printf("\t\t\t��ѡ��:");

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
		//�ͷ�����ռ�
		List_Destroy(head, schedule_node_t);
}

/*
 * Function:    Schedule_UI_Add
 * Function ID:	TTMS_SCU_Schedule_UI_Add
 * Description: ���һ���ݳ��ƻ���Ϣ
 * Input:       ��ؾ�Ŀ��ID��
 * Output:      ������Ϣʱ�ĸ�����ʾ
 * Return:      ��ӵļ�¼��
 */
int Schedule_UI_Add(int play_id) {
	schedule_t rec;
	int newRecCount = 0;
	char choice;

	do {
		system("cls");
        printf("\n\t\t\t==================================================================\n");
    	printf("\t\t\t      \t\t\t��ӷ�ӳ��\n");
        printf("\t\t\t------------------------------------------------------------------\n");
		fflush(stdin);
		rec.id = EntKey_Srv_CompNewKey("schedule");
		printf("\t\t\t��ĿID:");
		scanf("%d",&(rec.play_id));
		printf("\t\t\t�ݳ���ID:");
		scanf("%d", &(rec.studio_id));
		printf("\t\t\t��ӳ����:");
		scanf("%d%d%d", &(rec.date.year),&(rec.date.month),&(rec.date.day));
		printf("\t\t\t��ӳʱ��:");
		getchar();
		scanf("%d%d",&(rec.time.hour),&(rec.time.minute)/*,&(rec.time.second)*/);
		rec.seat_count=0;
        printf("\n\t\t\t==================================================================\n");

		if (Schedule_Srv_Add(&rec)) {
			rec.seat_count += 1;
			newRecCount += 1;
			printf("\t\t\t��ӳɹ���\n");
		} else
			printf("\t\t\t���ʧ��!\n");
        printf("\t\t\t****************************************************************\n" );
		printf("\t\t\t[A]�������       [R]����");
		fflush(stdin);
		scanf("%c", &choice);
	} while ('a' == choice || 'A' == choice);
	return newRecCount;
}

/*
 * Function:    Schedule_UI_Modify
 * Function ID:	TTMS_SCU_Schedule_UI_Mod
 * Description: ����ID�Ÿ����ݳ��ƻ���Ϣ
 * Input:       �����µľ�ĿID��
 * Output:      ������Ϣʱ�ĸ�����ʾ
 * Return:      ���µľ�Ŀ��Ϣ����0��ʾδ�ҵ���1��ʾ�ҵ�������
 */
int Schedule_UI_Modify(int id){
	/*schedule_t rec;
	int rtn = 0,seatcount;

	if (!Schedule_Srv_FetchByID(id, &rec)) {
		printf("\t\t\t��ӳ�Ҳ�����\n���س�������\n");
		getchar();
		getchar();
		return 0;
	}

	printf("\n\t\t\t==================================================================\n");
	printf("\t\t\t      \t\t\t���·�ӳ�ƻ�\n");
	printf("\t\t\t------------------------------------------------------------------\n");
	printf("\t\t\t�ݳ��ƻ�ID:%d\n", rec.id);
	printf("\t\t\t��ĿID:%d\n",rec.play_id);
	printf("\t\t\t�ݳ���ID:%d\n", rec.studio_id);
	printf("\t\t\t��ӳ����:%d��%2d��%2d��\n",rec.date.year,rec.date.month,rec.date.day);
	printf("\t\t\t��ӳʱ��:%d:%d\n",rec.time.hour,rec.time.minute);
	printf("\t\t\t��λ��:%d\n", rec.seat_count);
	seatcount =Schedule_Perst_SelectByID(rec.id);
	if (!seatcount) {
		printf("\t\t\t�ݳ���ID:");
		scanf("%d", &(rec.studio_id));
		printf("\t\t\t��ĿID:");
		scanf("%d",&(rec.play_id));
		printf("\t\t\t��ӳ����:");
		scanf("%d%d%d", &(rec.date.year),&(rec.date.month),&(rec.date.day));
		printf("\t\t\t��ӳʱ��:");
		scanf("%d%d",&(rec.time.hour),&(rec.time.minute));
	}

    printf("------------------------------------------------------------------\n");
	Schedule_Perst_DeleteByID(id);
	Schedule_Perst_Insert(&rec);

	if (Schedule_Srv_Modify(&rec)) {
		rtn = 1;
		printf(
				"\t\t\t���³ɹ�\n���س�������\n");
	} else
		printf("\t\t\t����ʧ��\n���س�������\n");

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
				printf("\t\t\t      \t\t\t�޸� �ݳ��ƻ�\n");
       			printf("\n\t\t\t==================================================================\n");
       			printf("\t\t\t%5s \t %5s\t%8s \t %10s \t %10s \t %10s \n", "ID", "��ĿID", "�ݳ���ID",
				 "��ӳ����","��ӳʱ��","��λ��");
				printf("\t\t\t------------------------------------------------------------------\n");
		
		
		
				printf("\t\t\t%5d %5d %5d ", data.id, data.play_id,data.studio_id);
			
				printf("\t%4d-%02d-%02d ",data.date.year, data.date.month, data.date.day);
				printf("\t%2d:%02d:%02d ", data.time.hour, data.time.minute, data.time.second);
				printf("\t%5d\n", data.seat_count);
				
				printf("\n\t\t\t==================================================================\n");
				printf("\t\t\t[A]�޸��ݳ���        [B]�޸ķ�ӳʱ��          [R]����\n");
				printf("\n\t\t\t==================================================================\n");
				printf("\t\t\t��ѡ��:");
				choice=l_getc();
				if('r'==choice || 'R'==choice) break;	
				switch(choice){
					
					case 'a':
					case 'A':
				
						printf("\t\t\t�������ݲ�����ID��");
				
						while(1){
							if(scanf("%d",&sid)==1){
							 cl_stdin(); 
							 
							 if(!Studio_Srv_FetchByID(sid, &sdata)){
							 	printf("\n\t\t\tIDΪ%d���ݳ��������ڣ����������룺",sid);
							 	continue;
							 }
							 
							data.studio_id=sdata.id;
							data.seat_count=Seat_Number_Count(data.studio_id);
							 
							 break;
							}
							else{ cl_stdin(); printf("\t\t\t����������������������:"); }
						}
						break;
		
					case 'b':
					case 'B':
				
						printf("\t\t\t�������ӳ����:(�� �� ��) ");
						while(1){
							if(scanf("%d%d%d",&(data.date.year), &(data.date.month), &(data.date.day))==3){
								 cl_stdin();
								 if(data.date.month>12 || data.date.day>31){
								 	printf("\t\t\t������� �� �� �� ��������������: ");
								 	continue;
								 }
								 if(DateCmp(data.date, DateNow())==-1){
								 	printf("\t\t\t��������������ڽ��գ�����������: ");
								 	continue;
								 
								 }
								 
								 break;
							}
							else{ cl_stdin(); printf("\t\t\t����������������������: "); }
						}
						
				
				
						printf("\t\t\t�������ӳʱ��:(ʱ ��) ");
						while(1){ 
							if(scanf("%d%d",&(data.time.hour), &(data.time.minute))==2){
								data.time.second=0;
								cl_stdin();
								if(data.time.hour>24 || data.time.minute>60){
								 	printf("\t\t\t������� ʱ �� �� ��������������:");
								 	continue;
								 }
						
								 break;
							}
							else{ cl_stdin(); printf("\t\t\t����������������������:"); }
						}
						
						break;
				
				
				}//switch

				if(Schedule_Srv_Modify(&data)){
					
					printf("\t\t\t�޸ĳɹ���\n");
					ret=1;
				}else{
					printf("\t\t\t�޸�ʧ�ܣ�\n");
				}
				
				printf("\t\t\t������������޸ġ�");
		
				getchar();
		
		}//while
	}else{
		printf("\t\t\tδ�ҵ�IDΪ%d���ݳ��ƻ���\n",id);
		printf("\t\t\t����������أ�\n");
		//cl_stdin();
		getchar();
		return 0;
	}
       return ret;
}

/*
 * Function:    Schedule_UI_Delete
 * Function ID:	TTMS_SCU_Schedule_UI_Del
 * Description: ����ID��ɾ���ݳ��ƻ���Ϣ
 * Input:       ��ɾ���ľ�ĿID��
 * Output:      ��ʾɾ���Ƿ�ɹ�
 * Return:      0��ʾɾ��ʧ�ܣ�1��ʾɾ���ɹ�
 */
int Schedule_UI_Delete(int id){

	int rtn = 0;

	if (Schedule_Srv_DeleteByID(id)) {
		if (Ticket_Perst_Delete(id))
			printf("\t\t\t��λɾ���ɹ�\n");
			printf("\t\t\t�ݳ�ɾ���ɹ�!\n���س�������\n");
		rtn = 1;
	} else {
		printf("\t\t\t��ӳ�Ҳ�����\n���س�������\n");
	}

	getchar();
	return rtn;

}

/*
 * Function:    Schedule_UI_Query
 * Function ID:	TTMS_SCU_Schedule_UI_Qry
 * Description: ����ID�Ų�ѯ�ݳ��ƻ���Ϣ
 * Input:       �����ҵľ�ĿID��
 * Output:      ���ҵ��ľ�Ŀ��Ϣ
 * Return:      0��ʾδ�ҵ���1��ʾ�ҵ���
 */
int Schedule_UI_Query(int id){

	schedule_t rec;
	/*int rtn = 0,seatcount;*/

	/*Load record*/
	if (!Schedule_Srv_FetchByID(id, &rec)) {
		printf("\t\t\t��ӳ�ƻ�������\n���س�������\n");
		getchar();
		return 0;
	}

    printf("\n\t\t\t==================================================================\n");
	printf("\t\t\t      \t\t\t���·�ӳ�ƻ�\n");
    printf("\t\t\t------------------------------------------------------------------\n");
    printf("\t\t\t�ݳ��ƻ�ID:%d\n", rec.id);
    printf("\t\t\t��ĿID:%d\n",rec.play_id);
    printf("\t\t\t�ݳ���ID:%d\n", rec.studio_id);
    printf("\t\t\t��ӳ����:%d��%2d��%2d��\n",rec.date.year,rec.date.month,rec.date.day);
   	printf("\t\t\t��ӳʱ��:%2d:%2d\n",rec.time.hour,rec.time.minute/*,rec.time.second*/);
   	printf("\t\t\t��λ��:%d\n", rec.seat_count);
	return 0;
}

/*
 * Function:    Schedule_UI_ListByPlay
 * Function ID:	TTMS_SCU_Schedule_UI_ListByPlay
 * Description: ���б�ģʽ��ʾ������Ŀ���ݳ��ƻ���Ϣ
 * Input:       listΪ���ҵ����ݳ��ƻ���Ϣ�����ͷָ�룬playΪ��Ŀ��Ϣ��pagingΪ��ʾ��ҳ���ò���
 * Output:      ��
 * Return:      ���ز��ҵ��ļ�¼��Ŀ
 */
void Schedule_UI_ListByPlay(const play_t *play, schedule_list_t list, Pagination_t paging){
	Schedule_Srv_FetchAll(list);
	schedule_node_t *rec=list->next;
	while (rec != NULL){
		printf("\n\t\t\t==================================================================\n");
		printf("\t\t\t      \t\t\t���·�ӳ�ƻ�\n");
		printf("\n\t\t\t------------------------------------------------------------------\n");
		printf("\t\t\t�ݳ���ID%d\n", rec->data.studio_id);
		printf("\t\t\t��ӳ����:%d��%2d��%2d��\n",rec->data.date.year,rec->data.date.month,rec->data.date.day);
		printf("\t\t\t��ӳʱ��:%2d:%2d\n",rec->data.time.hour,rec->data.time.minute/*,rec->data.time.second*/);
		printf("\t\t\t��λ��:%d\n", rec->data.seat_count);
		rec=rec->next;
	}

}

/*
 * Function:    Schedule_UI_MgtEntry
 * Function ID:	TTMS_SCU_Schedule_UI_MgtEnt
 * Description: �ݳ��ƻ�������棬�ݳ��ƻ����վ�Ŀ���й����Ծ�ĿID��Ϊ����
 * Input:       ��ؾ�ĿID��
 * Output:      �ݳ��ƻ���Ϣ
 * Return:      ��
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

//��������
paging.totalRecords = Schedule_Srv_FetchAll(head);
Paging_Locate_FirstPage(head, paging);

do {
	system("cls");
	printf("\n\t\t\t==================================================================\n");
	printf("\t\t\t      \t\t\t�ݳ��ƻ�\n");
	printf("\n\t\t\t==================================================================\n");
	printf("\n\t\t\t");
	printf("%-10s %-10s %-15s %-15s %-15s %-15s \n", "�ƻ�", "��Ŀ", "�ݳ���","��ӳ����","��ӳʱ��","��λ��");
	printf("\t\t\t------------------------------------------------------------------\n");
	//��ʾ����
	Paging_ViewPage_ForEach(head, paging, schedule_node_t, pos, i){
		printf("\t\t\t%-10d  %-10d  %-10d    %d��%2d��%2d��    %d:%d    %10d\n", pos->data.id,
				pos->data.play_id, pos->data.studio_id, pos->data.date.year,pos->data.date.month,pos->data.date.day,pos->data.time.hour,pos->data.time.minute/*,pos->data.time.second*/,pos->data.seat_count);
				}
	printf("\n\t\t\t------- �� %2d �� ----------------------- �� %2d/%2d ҳ ----------\n",
			paging.totalRecords, Pageing_CurPage(paging),
			Pageing_TotalPages(paging));
	printf("\n\t\t\t******************************************************************\n" );
	printf("\t\t\t[P]��һҳ  |  [N]��һҳ  |  [A]���  |  [D]ɾ��  |  [U]����  |  [R]����");
	printf("\n\t\t\t==================================================================\n");
	printf("\t\t\t����ĸѡ����:");
	//fflush(stdin);
	scanf("%c", &choice);

	switch (choice) {
	case 'a':
	case 'A':
		if (Schedule_UI_Add(play_id)) //����ӳɹ����������һҳ��ʾ
		{
			paging.totalRecords = Schedule_Srv_FetchAll(head);
			Paging_Locate_LastPage(head, paging, schedule_node_t);
		}
		break;
	case 'd':
	case 'D': {
		printf("\n\t\t\t==================================================================\n");
		printf("\t\t\t      \t\t\t�����ݳ��ƻ�\n");
		printf("\n\t\t\t------------------------------------------------------------------\n");
		printf("\t\t\t������Ҫɾ�����ݳ��ƻ�ID:");
		scanf("%d",&ID);
		if (Schedule_UI_Delete(ID)) {	//������������
			paging.totalRecords = Schedule_Srv_FetchAll(head);
			List_Paging(head, paging, schedule_node_t);
		}
		break;
	}
	case 'u':
	case 'U': {
		printf("\n\t\t\t==================================================================\n");
		printf("\t\t\t      \t\t\t�����ݳ��ƻ�\n");
		printf("\t\t\t------------------------------------------------------------------\n");
		printf("\t\t\t������Ҫ�޸ĵ��ݳ��ƻ�ID:");
		scanf("%d",&ID);
		if (Schedule_UI_Modify(ID)) {	//������������
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
	//�ͷ�����ռ�
	List_Destroy(head, schedule_node_t);
}
