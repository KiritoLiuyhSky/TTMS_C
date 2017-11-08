	/*
 *  Seat_UI.c
 *
 *  Created on: 2015��5��23��
 *  Author: lc
 */
#include "Seat_UI.h"

#include "../Service/Seat.h"
#include "../Service/Studio.h"
#include "../Service/EntityKey.h"
#include "../Common/list.h"
#include <stdio.h>

//����״̬���ض�Ӧ��ʾ״̬����
inline char Seat_UI_Status2Char(seat_status_t status) {

	char statusChar;
	switch (status) {
	case SEAT_GOOD:		//����λ
		statusChar = '#';
		break;
	case SEAT_BROKEN:	//�𻵵���λ
		statusChar = '~';
		break;
	case SEAT_NONE:
	default:
		statusChar = ' ';
		break;
	}
	return statusChar;
}

//����״̬���ŷ�����λ״̬
inline seat_status_t Seat_UI_Char2Status(char statusChar) {
	seat_status_t status;
	switch (statusChar) {
	case '#':		//����λ
		status = SEAT_GOOD;
		break;
	case '~':	//�𻵵���λ
		status = SEAT_BROKEN;
		break;
	default:
		status = SEAT_NONE;
		break;
	}
	return status;
}

void Seat_UI_MgtEntry(int roomID) {
	int i, j;
	char choice;
	int seatCount;
	int changedCount = 0;
	studio_t studioRec;
	system("cls");
	if (!Studio_Srv_FetchByID(roomID, &studioRec)) {  //��ö�Ӧid��ӳ������Ϣ
		printf("\t\t\tû�иüƻ�\n���س�����\n");
		getchar();
		return;
	}

	seat_list_t list;
	seat_node_t *p;

	List_Init(list, seat_node_t);
	//ѡ���ӳ����������λ
	seatCount = Seat_Srv_FetchByRoomID(list, roomID);

	if (!seatCount) {		//����ӳ����û��������λ�����Զ�������λ
		seatCount = Seat_Srv_RoomInit(list, roomID, studioRec.rowsCount,
				studioRec.colsCount);
		//�޸��ݳ��������λ����
		studioRec.seatsCount = seatCount;
		Studio_Srv_Modify(&studioRec);
	}
	do {

		//system("cls");
		printf("\n\t\t\t==================================================================\n");
		printf("\t\t\t%*s",28,"��ӳ����λ\n");

		printf("\t\t\t%5c", ' ');
		for (i = 1; i <= studioRec.colsCount; i++) {
			printf("%3d", i);
		}
		printf(
				"\n\t\t\t------------------------------------------------------------------\n");
		//��ʾ����
		for (i = 1; i <= studioRec.rowsCount; i++) {
			j = 1;
			printf("\t\t\t%2d��:%c", i, ' ');

			List_ForEach(list,p)
			{
				if (p->data.row == i) {
					while (p->data.column != j) {
						printf("%3c", ' ');
						j++;
					}
					printf("%3c", Seat_UI_Status2Char(p->data.status));
					j++;
				}
			}
			printf("\n");
		}

		printf("\t\t\t[A]���  |  [D]ɾ��  |  [U]����  |  [R]����\n");
		printf("\t\t\t******************************************************************\n");
		printf("��ѡ����");
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);

		switch (choice) {
		case 'a':
		case 'A':
			changedCount = Seat_UI_Add(list, roomID, studioRec.rowsCount,
					studioRec.colsCount);
			if (changedCount > 0) {
				seatCount += changedCount;
				//�޸��ݳ��������λ����
				studioRec.seatsCount = seatCount;
				Studio_Srv_Modify(&studioRec);
			}
			break;
		case 'd':
		case 'D':
			changedCount = Seat_UI_Delete(list, studioRec.rowsCount,
					studioRec.colsCount);
			if (changedCount > 0) {
				seatCount -= changedCount;
				//�޸��ݳ��������λ����
				studioRec.seatsCount = seatCount;
				Studio_Srv_Modify(&studioRec);
			}
			break;
		case 'u':
		case 'U':
			Seat_UI_Modify(list, studioRec.rowsCount, studioRec.colsCount);
			break;
		}
	} while (choice != 'r' && choice != 'R');

	//�ͷ�����ռ�
	List_Destroy(list, seat_node_t);
}

int Seat_UI_Add(seat_list_t list, int roomID, int row, int column) {  //����һ����λ
	seat_t rec;
	seat_node_t *p;
	int newRecCount = 0;
	char choice;
	do {
		system("cls");
		printf("\n\t\t\t==================================================================\n");
		printf("\t\t\t%*s",32,"�������λ\n");
		printf(
				"\t\t\t------------------------------------------------------------------\n");

		do {
			printf(
					"\t\t\t�в��ɳ� %d and �в��ɳ� %d!\n",
					row, column);
			printf("\t\t\t�ڼ��У� ");
			scanf("%d", &(rec.row));
			printf("\t\t\t�ڼ��У� ");
			scanf("%d", &(rec.column));
			fflush(stdin);
		} while (rec.row > row || rec.column > column);

		p = Seat_Srv_FindByRowCol(list, rec.row, rec.column);
		if (p) {						//����������к�����Ӧ����λ�Ѵ��ڣ����ܲ���
			printf("\t\t\t��λ�Ѵ���\n");
			continue;
		}

		rec.id = EntKey_Srv_CompNewKey("Seat");		//������λid
		rec.roomID = roomID;
		rec.status = SEAT_GOOD;    //���������λ��״̬Ĭ��Ϊ����λ
		printf("\n\t\t\t==================================================================\n");

		if (Seat_Srv_Add(&rec)) {
			newRecCount++;
			printf("\t\t\t����λ��ӳɹ�\n");
			p = (seat_node_t*) malloc(sizeof(seat_node_t));
			p->data = rec;
			Seat_Srv_AddToSoftedList(list, p); //������������λ�������list
		} else
			printf("\t\t\t����λ���ʧ��\n");
		printf("\t\t\t------------------------------------------------------------------\n");
		printf("\t\t\t[A]�������, [R]����");
		fflush(stdin);
		scanf("%c", &choice);
	} while ('a' == choice || 'A' == choice);
	return newRecCount;
}

int Seat_UI_Modify(seat_list_t list, int row, int column) {
	int rtn = 0;
	int newrow, newcolumn;
	char choice;
	seat_node_t *p;

		printf("\n\t\t\t==================================================================\n");
	printf("\t\t\t%*s",32,"������λ\n");
	printf("\t\t\t------------------------------------------------------------------\n");

	do {
		do {				//���µ���λ������Ϣ���ܳ�����ӳ����������
			printf(
					"\t\t\t�в��ɳ� %d and �в��ɳ� %d!\n",
					row, column);
			printf("\t\t\t��λ�кţ� ");
			scanf("%d", &newrow);
			printf("\t\t\t��λ�кţ� ");
			scanf("%d", &newcolumn);
		} while (newrow > row || newcolumn > column);

		p = Seat_Srv_FindByRowCol(list, newrow, newcolumn);
		if (p) {
			printf("\t\t\t��λ��Ϣ [%d,%d]: [%c]:", newrow, newcolumn,
					Seat_UI_Status2Char(p->data.status));
			fflush(stdin);
			p->data.status = Seat_UI_Char2Status(getchar());
		printf("\t\t\t------------------------------------------------------------------\n");
			if (Seat_Srv_Modify(&(p->data))) {
				rtn = 1;
				printf("\t\t\t��λ���³ɹ�\n");
			} else
				printf("\t\t\t��λ����ʧ��\n");
		} else
			printf("\t\t\t��λ������\n");
		printf("\t\t\t------------------------------------------------------------------\n");
		printf("\t\t\t[U]�������, [R]����");
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);
	} while ('u' == choice || 'U' == choice);
	return rtn;
}

int Seat_UI_Delete(seat_list_t list, int row, int column) {

	int delSeatCount = 0;
	int newrow, newcolumn;
	seat_node_t *p;
	char choice;

	do {
		system("cls");
		printf("\n\t\t\t==================================================================\n");
		printf("\t\t\t%*s",32,"ɾ����λ\n");
		printf("\t\t\t------------------------------------------------------------------\n");
		do {
			fflush(stdin);
			printf("\t\t\t�в��ɳ� %d �в��ɳ� %d!\n",
					row, column);
			printf("\t\t\t��λ�кţ� ");
			scanf("%d", &(newrow));
			printf("\t\t\t��λ�кţ� ");
			scanf("%d", &(newcolumn));
			fflush(stdin);
		} while (newrow > row || newcolumn > column);

		p = Seat_Srv_FindByRowCol(list, newrow, newcolumn);
		if (p) {
			printf("\n\t\t\t==================================================================\n");
			if (Seat_Srv_DeleteByID(p->data.id)) {
				printf("\t\t\t��λɾ���ɹ�\n");

				delSeatCount++;
				List_FreeNode(p);	//�ͷŽ����λ���p
			}
		} else {
			printf("\t\t\t��λ������\n");
		}

		printf("\t\t\t------------------------------------------------------------------\n");
		printf("\t\t\t[D]����ɾ��, [R]����");
		fflush(stdin);
		scanf("%c", &choice);
		fflush(stdin);
	} while ('d' == choice || 'D' == choice);
	return delSeatCount;
}
