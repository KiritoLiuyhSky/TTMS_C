/*
 * Studio_UI.c
 *
 *  Created on: 2015��4��24��
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

	//��������
	paging.totalRecords = Studio_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);

	do {
		system("cls");
		printf("\n\t\t\t==================================================================\n");
		printf("\t\t\t%*s",32,"�ݳ��б�\n");
		printf("\t\t\t%5s  %18s  %10s  %10s  %10s\n", "ID", "����", "��",
				"��", "��λ");
		printf("\t\t\t------------------------------------------------------------------\n");
		//��ʾ����
		for (i = 0, pos = (studio_node_t *) (paging.curPos);
				pos != head && i < paging.pageSize; i++) {
			printf("\t\t\t%5d  %18s  %10d  %10d  %10d\n", pos->data.id,
					pos->data.name, pos->data.rowsCount, pos->data.colsCount,
					pos->data.seatsCount);
			pos = pos->next;
		}
		printf("\t\t\t------- ��%2d �� ----------------------- �� %2d/%2d ----\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf("  \t\t\t*************************************************************\n" );
		printf(
				"\t\t\t[P]��һҳ | [N]��һҳ | [A]��� | [D]ɾ�� | [U]���� | [S]��λ | [R]����");
		printf("\n\t\t\t==================================================================\n");
		printf("\t\t\t��ѡ����:");
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);

		switch (choice) {
		case 'a':
		case 'A':
			if (Studio_UI_Add()) //����ӳɹ����������һҳ��ʾ
			{
				paging.totalRecords = Studio_Srv_FetchAll(head);
				Paging_Locate_LastPage(head, paging, studio_node_t);
			}
			break;
		case 'd':
		case 'D':
			printf("\t\t\t������ID:");
			scanf("%d", &id);
			if (Studio_UI_Delete(id)) {	//������������
				paging.totalRecords = Studio_Srv_FetchAll(head);
				List_Paging(head, paging, studio_node_t);
			}
			break;
		case 'u':
		case 'U':
			printf("\t\t\t������ID:");
			scanf("%d", &id);
			if (Studio_UI_Modify(id)) {	//������������
				paging.totalRecords = Studio_Srv_FetchAll(head);
				List_Paging(head, paging, studio_node_t);
			}
			break;
		case 's':
		case 'S':
			printf("\t\t\t������ID:");
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
	//�ͷ�����ռ�
	List_Destroy(head, studio_node_t);
}

int Studio_UI_Add(void) {
	studio_t rec;
	int newRecCount = 0;
	char choice;

	do {
		system("cls");
		printf("\n\t\t\t==================================================================\n");
		printf("\t\t\t%*s",28,"����µķ�ӳ��\n");
		printf("\t\t\t------------------------------------------------------------------\n");
		printf("\t\t\t����");
		fflush(stdin);
		gets(rec.name);
		printf("\t\t\t����������λ");
		scanf("%d", &(rec.rowsCount));
		printf("\t\t\t����������λ");
		scanf("%d", &(rec.colsCount));
		rec.seatsCount = 0;
		printf("\n\t\t\t==================================================================\n");

		//��ȡ����
		rec.id = EntKey_Srv_CompNewKey("Studio");
		
		rec.seatsCount = rec.rowsCount*rec.colsCount;

		if (Studio_Srv_Add(&rec)) {
			newRecCount += 1;
			printf("\t\t\t��ӳɹ�\n");
		} else
			printf("\t\t\t���ʧ��\n");
			printf("\t\t\t------------------------------------------------------------------\n");
		printf("\t\t\t[A]�������		[R]����");
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
		printf("\t\t\t���䲻����\n���س�����\n");
		getchar();
		return 0;
	}

	printf("\n\t\t\t==================================================================\n");
	printf("\t\t\t%*s",28,"�����ݳ���\n");
	printf("\t\t\t------------------------------------------------------------------\n");
	printf("\t\t\t�ݳ���ID:%d\n", rec.id);
	printf("\t\t\t�ݳ�������:[%s]:", rec.name);
	fflush(stdin);
	gets(rec.name);

	List_Init(list, seat_node_t);
	seatcount = Seat_Srv_FetchByRoomID(list, rec.id);
	if(seatcount){
		do{			//�����λ�ļ���������λ��Ϣ������µ����б������ǰ�󣬷����������
			printf("\t\t\t��λ����Ӧ���� >= [%d]:", rec.rowsCount);
			scanf("%d", &(newrow));
			printf("\t\t\t��λ����ӦС�� >= [%d]:", rec.colsCount);
			scanf("%d", &(newcolumn));
		}while(	newrow<rec.rowsCount||newcolumn<rec.colsCount);
		rec.rowsCount=newrow;
		rec.colsCount=newcolumn;
		rec.seatsCount=seatcount;
	}
	else{
		printf("\t\t\t����������λ: ");
		scanf("\t\t\t%d", &rec.rowsCount);
		printf("\t\t\t����������λ: ");
		scanf("%d", &rec.colsCount);
		rec.seatsCount=0;
	}

	printf("\t\t\t------------------------------------------------------------------\n");

	if (Studio_Srv_Modify(&rec)) {
		rtn = 1;
		printf(
				"\t\t\t���³ɹ�\n���س�����\n");
	} else
		printf("\t\t\t����ʧ��\n���س�����\n");

	getchar();
	return rtn;
}

int Studio_UI_Delete(int id) {

	int rtn = 0;

	if (Studio_Srv_DeleteByID(id)) {
		//��ɾ����ӳ��ʱ��ͬʱ���ݷ�ӳ��idɾ����λ�ļ��е���λ
		if(Seat_Srv_DeleteAllByRoomID(id))
			printf("\t\t\t��λɾ���ɹ�\n");
		printf(
				"\t\t\t��ӳ��ɾ���ɹ�\n���س�����\n");
		rtn = 1;
	} else {
		printf("\t\t\t��ӳ��������\n���س�����\n");
	}

	getchar();
	return rtn;
}
