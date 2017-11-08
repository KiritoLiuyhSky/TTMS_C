#include "Play_UI.h"
#include "Schedule_UI.h"
#include "Sale_UI.h"
#include "Ticket_UI.h"
#include "../Service/Ticket.h"
#include "../Service/Seat.h"
#include "../Service/Studio.h"
#include "../Service/EntityKey.h"
#include "../Service/Sale.h"
#include "../Common/ComFunction.h"
#include "../Common/Common.h"
#include "Main_Menu.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//���õ�½�û���ȫ�����ݽṹ
extern account_t gl_CurUser;

//���ݼƻ�ID����ʾ�ݳ�Ʊ
void Sale_UI_ShowTicket(int schID){
	// �벹������
	
}

inline int Sale_UI_SellTicket(ticket_list_t tickList, seat_list_t seatList){
	
	int id;
	ticket_list_t head;
	schedule_t sch;
	
	
	printf("\t\t\t�������ݳ��ƻ���ID:");
			
	while(1){
					if(scanf("%d",&id)==1){ cl_stdin(); break;}
					else{ cl_stdin(); printf("\t\t\t����������������������:"); }
	}
	
	if(!Schedule_Srv_FetchByID(id, &sch)){
			printf("\n\t\t\tδ�ҵ�IDΪ%d���ݳ��ƻ�������������� ������",id);
			getchar();
			getchar();
			return 0;
	}
	
	Ticket_Srv_FetchBySchID(tickList, sch.id);
			
	Seat_Srv_FetchValidByRoomID(seatList, sch.studio_id);
	
	
	Ticket_UI_ListBySch(&sch,tickList,seatList);
	
	
    return 1;
}



//���ݾ�ĿID��ʾ�ݳ��ƻ�
void Sale_UI_ShowScheduler(int playID) {
	int i, id;
	char choice;
	
	play_t pdata;
	ticket_list_t tickList;
	seat_list_t   seatList;
	
	schedule_list_t head;
	schedule_node_t *p;
	Pagination_t paging;
	
	Play_Srv_FetchByID(playID, &pdata);
	
	List_Init(head, schedule_node_t);
	
	paging.offset = 0;
	paging.pageSize = SCHEDULE_PAGE_SIZE;

	//��������
	paging.totalRecords = Schedule_Srv_FetchByPlay(head,playID);
	
	Paging_Locate_FirstPage(head, paging);

	do {
		system("cls");
		printf("\n\t\t\t==================================================================\n");
		printf("\t\t\t      \t\t\t��Ŀ��%10s ���ݳ��ƻ� �б�\n",pdata.name);

		printf("\t\t\t%5s \t %-5s\t%-8s \t %-10s \t %-10s \t %-10s \n", "�ƻ�", "��Ŀ", "�ݳ���",
				 "��ӳ����","��ӳʱ��","��λ��");
		printf("\t\t\t------------------------------------------------------------------\n");
		
		if(!List_IsEmpty(head)){
			//��ʾ����
			Paging_ViewPage_ForEach(head, paging, schedule_node_t, p, i){
				printf("\t\t\t%5d %5d %5d ", p->data.id, p->data.play_id,p->data.studio_id);
			
				printf("\t%4d-%02d-%02d ",p->data.date.year, p->data.date.month, p->data.date.day);
				printf("\t%2d:%02d ", p->data.time.hour, p->data.time.minute);
				printf("\t%5d\n", p->data.seat_count);
			}
		}else{
		
			printf("\t\t\t�������ݣ�\n");
		}

		printf("\n\t\t\t------------ �� %2d ��-------------------- %2d/%2d ҳ---------------\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf("\t\t\t******************************************************************\n");
		printf("\t\t\t[P]��ҳ             |    [N]��ҳ             |     [R]����\n");
		printf("\t\t\t[S]������Ʊϵͳ\n");
		printf("\n\t\t\t==================================================================\n");
		printf("\t\t\tѡ����:");
		
		choice=l_getc();
	

		switch (choice) {
		
		case 'u':
		case 'U':
			
				printf("\t\t\t������Ҫ�޸��ݳ��ƻ���ID:");
			
				while(1){
					if(scanf("%d",&id)==1){ cl_stdin(); break;}
					else{ cl_stdin(); printf("\t\t\t����������������������:"); }
				}
			
				
			break;
			
		case 's':
		case 'S':
			{
				
				
				
				List_Init(tickList, ticket_node_t);
				List_Init(seatList,   seat_node_t);
				
				
			
			
				Sale_UI_SellTicket(tickList,seatList);
				
				List_Destroy(tickList, ticket_node_t);
				List_Destroy(seatList,   seat_node_t);
			}
			break;
			
		case 't':
		case 'T':
			//();
			break;
		
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


void Sale_UI_MgtEntry() {
	int i, id;
	char choice;

	play_list_t head;
	play_node_t *p;
	Pagination_t paging;

	List_Init(head, play_node_t);
	
	paging.offset = 0;
	paging.pageSize = PLAY_PAGE_SIZE;

	//��������
	paging.totalRecords = Play_Srv_FetchAll(head);
	
	Paging_Locate_FirstPage(head, paging);

	do {
		system("cls");
		printf("\n\t\t\t==================================================================\n");
		printf("\t\t\t      \t\t\t��Ŀ �б�\n");
		printf("\t\t\t%2s  %-10s\t%-8s  %-5s %-10s \t %-10s \t %-5s \n", "ID", "����", "����",
				"ʱ��", "��ӳ����","��������","Ʊ��");
		printf("\t\t\t------------------------------------------------------------------\n");
		
		
		//��ʾ����
		Paging_ViewPage_ForEach(head, paging, play_node_t, p, i){
			printf("\t\t\t%2d  %-10s %-10s %-5d ", p->data.id, p->data.name,p->data.area, p->data.duration);
			printf("%4d-%02d-%02d ", p->data.start_date.year, p->data.start_date.month, p->data.start_date.day);
			printf("\t%4d-%02d-%02d ",p->data.end_date.year, p->data.end_date.month, p->data.end_date.day);
			printf("\t%-5d\n", p->data.price);
		}

		printf("\n\t\t\t----------- �� %2d �� ------------------- %2d/%2d ҳ  ----------------\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf("\t\t\t ******************************************************************\n");
		printf("\t\t\t        [P]��ҳ        |            [N]��ҳ\n\n");
		printf("\n\t\t\t[A]���ݾ�ĿID�г��ݳ��ƻ�                                 [R]����\n");
		printf("\n\t\t\t==================================================================\n");
		printf("\t\t\t��ѡ��:");
		
		choice=l_getc();
	

		switch (choice) {
		case 'a':
		case 'A':
				
				printf("\t\t\t������Ҫ����ݳ��ƻ��� ��ĿID:");
				while(1){
					if(scanf("%d",&id)==1){ cl_stdin(); break;}
					else{ cl_stdin(); printf("\t\t����������������������:"); }
				}
				Sale_UI_ShowScheduler(id);
			
				break;
		/*
		case 'u':
		case 'U':
			
			
			break;
			
		
		*/
		case 'p':
		case 'P':
			if (!Pageing_IsFirstPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, -1, play_node_t);
			}
			break;
		case 'n':
		case 'N':
			if (!Pageing_IsLastPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, 1, play_node_t);
			}
			break;
		}
	} while (choice != 'r' && choice != 'R');
	//�ͷ�����ռ�
	List_Destroy(head, play_node_t);
}

//��Ʊ
void Sale_UI_ReturnTicket(){
	char choice;
	//int id,t=0;
     //while(1){
		system("cls");
       	//printf("\n\n\n\n");
       	printf("\n\t\t\t==================================================================\n");
       	printf("\t\t\t      \t\t\t��   Ʊ\n");
       	printf("\n\t\t\t==================================================================\n");
       	printf("\t\t\t\t\t��ӰԺ��Ʊһ���۳����ܲ��˻�");
       	printf("\t\t\t\t��[R]����");
       	getchar();
       	getchar();
       	Main_Menu();
       	//printf("\t\t\t������Ʊ��ID:");
		/*
		while(1){
				if(scanf("%d",&id)==1){ cl_stdin(); break;}
				else{ cl_stdin(); printf("\t\t\t����������������������:"); }
		}
		
		
		ticket_t buf;
		schedule_t scd;
			
	   	Schedule_Srv_FetchByID(buf.schedule_id, &scd);
		if( Ticket_Srv_FetchByID(id,&buf) ){
			if(buf.status==TICKET_SOLD){
       				Ticket_UI_Print(id);
       				user_time_t nowtime=TimeNow();
       				if(DateCmp(DateNow(), scd.date)==-1 || (DateCmp(DateNow(), scd.date)==0 && scd.time.hour<nowtime.hour && scd.time.minute<nowtime.minute ) ){
       				t=1;
       				Ticket_UI_Print(id);
       				}else{
       					printf("\n\t\t\t��Ʊ�ѹ���Ч�ڣ��޷���Ʊ");
       					printf("\n\n\t\t\t�������������������\n");
					getchar();
					break;
					
       				}
       			}else{
       			
       				printf("\n\t\t\t��Ʊ�����ڻ�δ�۳����޷���Ʊ");
       				printf("\n\n\t\t\t�������������������\n");
       				
				getchar();
				break;
       			}
       		}
		
		
		if(t){
			printf("\t\t\t������ Q ȷ����Ʊ  ���� R ���أ�");
			choice=l_getc();
			if('r'==choice || 'R'==choice) break;	
			if('q'==choice || 'Q'==choice) {
			
				
	       			buf.status=TICKET_AVL;
	       			sale_t data;
	       					
	       			data.id=EntKey_Srv_CompNewKey("sale");
	       			data.user_id=gl_CurUser.id;
	       			data.ticket_id=id;
	       			data.date=DateNow();
	       			data.time=TimeNow();
	       			data.value=buf.price;
	       			data.type=SALE_RETURN;
	       					
	       			Sale_Srv_Add(&data);
	       					
	       			Ticket_Srv_Modify(&buf);
       					
       				printf("\n\t\t\t��Ʊ�ɹ���");
       				printf("\n\n\t\t\t�������������������\n");
				cl_stdin();
				getchar();
											
				break;
       			}
       					
       		}
     			
		
	*/
    //}
}

