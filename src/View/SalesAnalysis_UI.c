/*
 * SalesAnalysis_UI.c
 *
 *  Created on: 2015��5��8��
 *      Author: Administrator
 */
#include "SalesAnalysis_UI.h"
#include "../Common/list.h"
#include "../Common/common.h"
#include "../Service/SalesAnalysis.h"
#include <conio.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

//�ⲿ����gl_CurUser ���ڴ洢��¼�û���Ϣ
extern account_t gl_CurUser;

//�����ҳ������ҳ���СΪ5
static const int SALESANALYSIS_PAGE_SIZE = 5;

//��Ժ�������а���,������ʾ��ֹĿǰ��Ժ��ӰƱ�����а�
void SalesAnalysis_UI_BoxOffice() {
	int i;
	char choice;

	salesanalysis_list_t head;
	salesanalysis_node_t *pos;
	Pagination_t paging;

	List_Init(head, salesanalysis_node_t);
	paging.offset = 0;
	paging.pageSize = SALESANALYSIS_PAGE_SIZE;

	//��������
	paging.totalRecords = Salesanalysis_Srv_StaticSale(head);
	SalesAnalysis_Srv_SortBySale(head);   //����Ʊ������

	Paging_Locate_FirstPage(head, paging);
	do{
		printf("\t\t\t=========================================================================\n");
		printf("\t\t\t%28s""Ʊ��ͼ��\n");
		printf("\t\t\t-------------------------------------------------------------------------\n");
		printf("\t\t\t��Ŀ��\t\t����\t\t��Ʊ��\tƱ��\t��ӳʱ��\t\t��ӳʱ��\n");

			for (i = 0, pos = (salesanalysis_node_t *) (paging.curPos);
				pos != head && i < paging.pageSize; i++) {
			printf("\t\t\t%-10s\t%-10s\t%-5ld\t%-5ld\t%d-%d-%d\t%d-%d-%d\t\n",
					pos->data.name, pos->data.area, pos->data.totaltickets,pos->data.sales,
					pos->data.start_date.year,pos->data.start_date.month,pos->data.start_date.day,
					pos->data.end_date.year,pos->data.end_date.month,pos->data.end_date.day);
			pos = pos->next;
		}
		printf("\t\t\t---------- �� %2d �� --------------------------- �� %2d/%2d ҳ --------\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf("\t\t\t*************************************************************************\n");
		printf("\t\t\t[P]��һҳ	|	[N]��һҳ	|	[R]����\n");
		printf("\n\t\t\t=========================================================================\n");
		fflush(stdin);
		printf("\t\t\t��ѡ���ܣ�");
		scanf("%c",&choice);

		switch(choice)
		{
			case 'P':
			case 'p':
					if (1 < Pageing_CurPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, -1, salesanalysis_node_t);
					}
				break;
			case 'N':
			case 'n':
					if (Pageing_TotalPages(paging) > Pageing_CurPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, 1, salesanalysis_node_t);
					}
				break;
		}
	} while(choice!='R' && choice!='r');
		List_Destroy(head, salesanalysis_node_t);
}

//��ʾ��ƱԱ�ڸ��������������Ʊ���
void SalesAnalysis_UI_StatSale(int usrID, user_date_t stDate, user_date_t endDate){    //���ڲ������û����뻹�ǣ�
	account_t usr;
	int sale_count=0;
	sale_count=Salesanalysis_Srv_CompSaleVal(usrID,stDate,endDate);    //��ȡ����Ա�����۶�

	if(!Account_Srv_FetchByID(usrID,&usr)) {
		printf("\t\t\tID������!\n�밴�س�����\n");
		getchar();
		return ;
	}

	printf("\t\t\t================��ƱԱ%d���������===================\n",usrID);
	printf("\t\t\t���ۿ�ʼ����:     %d-%d-%d\n",stDate.year,stDate.month,stDate.day);
	printf("\t\t\t���۽�������:     %d-%d-%d\n",endDate.year,endDate.month,endDate.day);
	printf("\t\t\t���۶%d\n",sale_count);
	printf("\t\t\t======================================================\n");

}

//���۷�����ں�������ʾ�˵����˵�����"������ʾ��ֹĿǰ��Ժ��ӰƱ�����а�"������ʾ���ѯ������ƱԱ��Ʊ�������
//����ѯ������������ĳ��ƱԱ��Ʊ�����
void SalesAanalysis_UI_MgtEntry() {

	user_date_t stDate,endDate;
	account_t user;
	char choice;
	do{
//		printf("���������������Ӧ�ַ���\n\n");
		printf("\t\t\t===============================================\n");
		printf("      \t\t\t[B]��ĿƱ�����а�\n");
		printf("      \t\t\t[D]��ѯԱ���ڸ��������������۶����\n");
		printf("      \t\t\t[S]��ѯԱ���ڸ�������������Ʊ���\n");
		printf("\n\t\t\t***********************************************\n");
		printf("\t\t\t[R]����\n");
		printf("\t\t\t===============================================\n");
		printf("\t\t\t��ѡ���ܣ�");
		fflush(stdin);
		scanf("%c",&choice);
		switch(choice)
		{
			case 'B':
			case 'b':
					SalesAnalysis_UI_BoxOffice();
				break;
			case 'D':
			case 'd':
				if(gl_CurUser.type==USR_CLERK)
					SalesAnalysis_UI_StatSale(gl_CurUser.id,stDate,endDate);//����
				else
 					{
 						printf("\t\t\t��������ƱԱ������\n");
 						fflush(stdin);
 						char UsrName[100];
						scanf("%s",UsrName);
						if(!Account_Srv_FetchByName(UsrName,&user))
							printf("\t\t\t�����ڸ���ƱԱ��Ϣ\n");
						else
							SalesAnalysis_UI_StatSale(user.id,stDate,endDate);//����
					}
				break;
			case 'S':
			case 's':
				if(gl_CurUser.type==USR_MANG)
				{
					char UsrName[100];
					printf("\t\t\t��������ƱԱ������\n");
					scanf("%s",UsrName);
						fflush(stdin);
					if(!Account_Srv_FetchByName(UsrName,&user))
						printf("�����ڸ���ƱԱ��Ϣ\n");
					else
						{
							printf("\t\t\t�������Ŀ��ӳʱ��:\n");	fflush(stdin);
							scanf("%d-%d-%d",&stDate.year,&stDate.month,&stDate.day);
							printf("\t\t\t�������Ŀ��ӳʱ��:\n");	fflush(stdin);
							scanf("%d-%d-%d",&endDate.year,&endDate.month,&endDate.day);
							SalesAnalysis_UI_StatSale(user.id,stDate,endDate);
						}
				}
				else
					{
						printf("\t\t\tû��Ȩ��\n");
						break;
					}
		 }
	}while(choice!='R' && choice!='r' );
}



