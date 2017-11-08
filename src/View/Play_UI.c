#include "Play_UI.h"
#include "Schedule_UI.h"

#include "../Common/List.h"
#include "../Service/Play.h"
#include "../Service/EntityKey.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Function:    Play_UI_ShowList
 * Function ID:	TTMS_SCU_Play_UI_Show
 * Description: ���б�ģʽ��ʾ��Ŀ��Ϣ
 * Input:       list��Ŀ��Ϣ�����ͷָ�룬paging��ҳ���ò���
 * Output:      ���о�Ŀ����Ϣ
 * Return:      ���ز��ҵ��ļ�¼��Ŀ
 */
void Play_UI_ShowList(play_list_t list,Pagination_t paging)
{
	int i,j;
	char choice;
	play_node_t *pos;
	List_Init(list, play_node_t);
	paging.offset = 0;
	paging.pageSize = 2;


	//��������
	paging.totalRecords = Play_Srv_FetchAll(list);
	Paging_Locate_FirstPage(list, paging);
	do{
		system("cls");
		printf("\n\t\t\t==================================================================\n");
		printf("\t\t\t%*s",32,"��Ŀ��Ϣ\n");
        printf("\t\t\t------------------------------------------------------------------\n");
		//��ʾ����
		Paging_ViewPage_ForEach(list, paging, play_node_t, pos, i){
			printf("\t\t\t��ĿID[%d]\n", pos->data.id);
			printf("\t\t\t��Ŀ����[%s]\n",pos->data.name);
			printf("\t\t\t��Ŀ����[%s]\n",pos->data.area);
			j=pos->data.type;
		switch(j){
			case 1:printf("\t\t\t��Ŀ����[%s]\n","��Ӱ����");break;
			case 2:printf("\t\t\t��Ŀ����[%s]\n","�������");break;
			case 3:printf("\t\t\t��Ŀ����[%s]\n","���ֻ�����");break;
		}
		j=pos->data.rating;
		switch(j){
			case 1:printf("\t\t\t��Ŀ�ȼ�[%s]\n","��ͯ��");break;
			case 2:printf("\t\t\t��Ŀ�ȼ�[%s]\n","�����");break;
			case 3:printf("\t\t\t��Ŀ�ȼ�[%s]\n","���˾�");break;
		}
		printf("\t\t\t��Ŀʱ��[%d]\n",pos->data.duration);
		printf("\t\t\t��Ŀ��ӳ����[%d.%d.%d]\n",pos->data.start_date.year,pos->data.start_date.month,pos->data.start_date.day);
		printf("\t\t\t������ӳ����[%d.%d.%d]\n",pos->data.end_date.year,pos->data.end_date.month,pos->data.end_date.day);
		printf("\t\t\tƱ��[%d]\n",pos->data.price);
		//printf("\n");
		}

		printf("\t\t\t------- �� %2d �� ----------------------- �� %2d/%2d ҳ ----\n",paging.totalRecords, Pageing_CurPage(paging),Pageing_TotalPages(paging));

		printf("\t\t\t[P]��һҳ    [N]��һҳ    [R]����");
		printf("\n\t\t\t=======================================================\n");
		printf("\t\t\t��ѡ��:");
		fflush(stdin);
		scanf("%c", &choice);
		getchar();
		switch (choice)
		{
			case 'p':
			case 'P':
				if (!Pageing_IsFirstPage(paging))
				{
					paging.totalRecords = Play_Srv_FetchAll(list);
					Paging_Locate_OffsetPage(list, paging, -1, play_node_t);
				}
				break;
			case 'n':
			case 'N':
				if (!Pageing_IsLastPage(paging))
				{
					paging.totalRecords = Play_Srv_FetchAll(list);
					Paging_Locate_OffsetPage(list, paging, 1, play_node_t);
				}
				break;
		}
	} while (choice != 'r' && choice != 'R');
	//}
	List_Destroy(list, play_node_t);
}

/*
 * Function:    Play_UI_MgtEntry
 * Function ID:	TTMS_SCU_Play_UI_MgtEnt
 * Description: ��Ŀ��Ϣ�������
 * Input:       flagΪ0���������ҳ�棬flagΪ1�����ѯ����
 * Output:      ��Ŀ�������
 * Return:      ��
 */
void Play_UI_MgtEntry(int flag)
{
	int id/*,play_id*/;
	char choice;
	system("cls");
	play_list_t head;
	Pagination_t paging;

	List_Init(head, play_node_t);
	paging.offset = 0;
	paging.pageSize = 2;

	//��������
	paging.totalRecords = Play_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);

	if(flag == 0)
	{
		do
		{
			Play_UI_ShowList(head,paging);
			printf("\t\t\t ------- ��%2d ��----------------------- %2d/%2d ҳ ----\n",paging.totalRecords, Pageing_CurPage(paging),Pageing_TotalPages(paging));

            printf("\n\t\t\t******************************************************************\n" );
			printf("\t\t\t[S]�����ݳ��ƻ�  [A]���  [Q]����  [D]ɾ��   [U]�޸�   [R]����  [p]��һҳ  [n]��һҳ");
            printf("\n\t\t\t==================================================================\n");
			printf("\t\t\t��ѡ��:");
			//fflush(stdin);
			choice = getchar();
			switch (choice)
			{
			case 's':
			case 'S':
				Schedule_UI_MgtEntry(1);
				break;
			case 'a':
			case 'A':
				if (Play_UI_Add()) //����ӳɹ����������һҳ��ʾ
				{
					paging.totalRecords = Play_Srv_FetchAll(head);
					Paging_Locate_LastPage(head, paging, play_node_t);
				}
				break;
			case 'd':
			case 'D':
				printf("\t\t\t�������ĿID:");
				scanf("%d", &id);
				getchar();
				if (Play_UI_Delete(id))
				{	//������������
					paging.totalRecords = Play_Srv_FetchAll(head);
					List_Paging(head, paging, play_node_t);
				}
				break;
			case 'u':
			case 'U':
				printf("\t\t\t�������ĿID:");
				scanf("%d", &id);
				if (Play_UI_Modify(id))
				{	//������������
					paging.totalRecords = Play_Srv_FetchAll(head);
					List_Paging(head, paging, play_node_t);
				}
				break;
			case 'p':
			case 'P':
				if (!Pageing_IsFirstPage(paging))
				{
					paging.totalRecords = Play_Srv_FetchAll(head);
					Paging_Locate_OffsetPage(head, paging, -1, play_node_t);
				}
				break;
			case 'n':
			case 'N':
				if (!Pageing_IsLastPage(paging))
				{
					paging.totalRecords = Play_Srv_FetchAll(head);
					Paging_Locate_OffsetPage(head, paging, 1, play_node_t);
				}
				break;
			case 'Q':
			case 'q':
				printf("\t\t\t�������ĿID:");
				scanf("%d", &id);
				if (Play_UI_Query(id))
				{	//������������
					paging.totalRecords = Play_Srv_FetchAll(head);
					List_Paging(head, paging, play_node_t);
				}
				break;
			/*case 's':
			case 'S':

				//��ҳ������о�Ŀ��Ϣ
				Play_UI_ShowList(head,paging);
				break;*/
			}
		}while(choice != 'r' && choice != 'R');
		List_Destroy(head, play_node_t);
	}
	else{
		do{
			printf("\t\t\t------- �� %2d �� ----------------------- �� %2d/%2d ҳ ----\n",paging.totalRecords, Pageing_CurPage(paging),Pageing_TotalPages(paging));
			printf("\n\t\t\t******************************************************************\n");
			printf("\t\t\t[Q]����  [R]����   [S]�鿴");
			printf("\n\t\t\t==================================================================\n");
			printf("\t\t\t��ѡ��:");
			fflush(stdin);
			scanf("%c", &choice);
			fflush(stdin);
			switch (choice)
			{
				case 'Q':
				case 'q':
					printf("\t\t\t�������ĿID:");
					scanf("%d", &id);
					if (Play_UI_Query(id))
					{	//������������
						paging.totalRecords = Play_Srv_FetchAll(head);
						List_Paging(head, paging, play_node_t);
					}
					break;
				case 's':
				case 'S':

					//��ҳ������о�Ŀ��Ϣ
					Play_UI_ShowList(head,paging);
					break;
			}
		}while(choice != 'r' && choice != 'R');
	List_Destroy(head, play_node_t);
}
}

/*
 * Function:    Play_UI_Add
 * Function ID:	TTMS_SCU_Play_UI_Add
 * Description: ���һ����Ŀ��Ϣ
 * Input:       ��
 * Output:      ������Ϣʱ�ĸ�����ʾ
 * Return:      ��ӵļ�¼��
 */
 int Play_UI_Add(void)
{
	play_t data;
	char choice;
	int newRecCount = 0,type,rating;
	do{
	system("cls");
    printf("\n\t\t\t==================================================================\n");
		printf("\t\t\t%*s",32,"����µľ�Ŀ��Ϣ\n");
        printf("\t\t\t------------------------------------------------------------------\n");
		printf("\t\t\t�������Ŀ����:");
		scanf("%s",data.name);
		printf("\t\t\t�������Ŀ����:");
		scanf("%s",data.area);
		printf("\t\t\t�������Ŀʱ��(�Է���Ϊ��λ,��������):");
		scanf("%d",&data.duration);
		printf("\t\t\t[��Ӱ����]����1,[�������]����2,[���ֻ�����]����3:");
		scanf("%d",&type);
		if(type==1 || type==2 || type==3)
		{
			data.type=(play_type_t)type;
		}
		else
		{
			while(type!=1 && type!=2 && type!=3)
			{	printf("\t\t\t��������,����������!\n");
				printf("\t\t\t[��Ӱ����]����1,[�������]����2,[���ֻ�����]����3:");
				scanf("%d",&type);
			}
			data.type=(play_type_t)type;
		}
		printf("\t\t\t[��ͯ��]����1,[�����]����2,[���˾�]����3:");
		scanf("%d",&rating);
		if(rating==1 || rating==2 || rating==3)
		{
			data.rating=(play_rating_t)rating;
		}
		else
		{
			while(rating!=1 && rating!=2 && rating!=3)
			{	printf("\t\t\t��������,����������!");
				printf("\t\t\t[��ͯ��]����1,[�����]����2,[���˾�]����3:");
				scanf("%d",&rating);
			}
			data.rating=(play_rating_t)rating;
		}
		printf("\t\t\t�����뿪ʼ��ӳ����(��2014 7 2):");
		scanf("%d %d %d",&data.start_date.year,&data.start_date.month,&data.start_date.day);
		printf("\t\t\t�������ӳ��������(��2014 7 2):");
		scanf("%d %d %d",&data.end_date.year,&data.end_date.month,&data.end_date.day);
		printf("\t\t\t������Ʊ��:");
		scanf("%d",&data.price);
		getchar();
        printf("\n\t\t\t==================================================================\n");

		//��ȡ����
		data.id = EntKey_Srv_CompNewKey("Play");

		if (Play_Srv_Add(&data)) {
			newRecCount += 1;
			printf("\t\t\t�¾�Ŀ��ӳɹ�!\n");
		} else
			printf("\t\t\t�¾�Ŀ���ʧ��!\n");
        printf("\t\t\t------------------------------------------------------------------\n");
		printf("\t\t\t[A]���      [R]����\n");
		fflush(stdin);
		scanf("%c", &choice);
		getchar();
	} while ('a' == choice || 'A' == choice);
	return newRecCount;

}

/*
 * Function:    Play_UI_Modify
 * Function ID:	TTMS_SCU_Play_UI_Mod
 * Description: ���¾�Ŀ��Ϣ
 * Input:       �����µľ�ĿID��
 * Output:      ������Ϣʱ�ĸ�����ʾ
 * Return:      ���µľ�Ŀ��Ϣ����0��ʾδ�ҵ���1��ʾ�ҵ�������
 */
 
 int Play_UI_Modify(int id)
{	int type,rating,i;
	play_t data;
	int rtn=0;
	char newname[31],newarea[9];
	int newduration,newprice,newSyear,newSmonth,newSday,newEyear,newEmonth,newEday,newrate,newtype;

	//Load record
	if (!Play_Srv_FetchByID(id, &data))
	{
		printf("\t\t\t��Ŀ������,��ȷ�ϼ�����\n");
		getchar();
		return 0;
	}
    printf("\n\t\t\t==================================================================\n");
	printf("\t\t\t%*s",32,"�޸ľ�Ŀ��Ϣ\n");
    printf("\t\t\t------------------------------------------------------------------\n");
	printf("\t\t\t��ĿID[%d]\n", data.id);
	printf("\t\t\t��Ŀ����[%s]\n", data.name);
	i=data.type;
	switch(i)
	{
		case 1:printf("\t\t\t��Ŀ����[%s]\n","��Ӱ����");break;
		case 2:printf("\t\t\t��Ŀ����[%s]\n","�������");break;
		case 3:printf("\t\t\t��Ŀ����[%s]\n","���ֻ�����");break;
	}
	printf("\t\t\t��Ŀ��Ʒ����[%s]\n",data.area);
	i=data.rating;
	switch(i)
	{
		case 1:printf("\t\t\t��Ŀ�ȼ�[%s]\n","��ͯ��");break;
		case 2:printf("\t\t\tĿ�ȼ�[%s]\n","�����");break;
		case 3:printf("\t\t\t��Ŀ�ȼ�[%s]\n","���˾�");break;
	}
	printf("\t\t\t��Ŀʱ��[%d����]\n",data.duration);
	printf("\t\t\t��Ŀ��ӳ����[%d.%d.%d]\n",data.start_date.year,data.start_date.month,data.start_date.day);
	printf("\t\t\t��ӳ��������[%d.%d.%d]\n",data.end_date.year,data.end_date.month,data.end_date.day);
	printf("\t\t\t��ĿƱ��[%d]\n",data.price);
    printf("\t\t\t------------------------------------------------------------------\n");
	fflush(stdin);

	if(Play_Srv_FetchByID(id,&data))
	{

		printf("\t\t\t�������Ŀ����:");
		scanf("%s",newname);
		printf("\t\t\t�������Ŀ����:");
		scanf("%s",newarea);
		printf("\t\t\t�������Ŀʱ��(�Է���Ϊ��λ,��������):");
		scanf("%d",&newduration);
		printf("\t\t\t[��Ӱ����]����1,[�������]����2,[���ֻ�����]����3:");
		scanf("%d",&newtype);
		if(newtype==1 || newtype==2 || newtype==3)
		{
			data.type=(play_type_t)newtype;
		}
		else
		{
			while(newtype!=1 && newtype!=2 && newtype!=3)
			{	printf("\t\t\t��������,����������!:");
				printf("\t\t\t[��Ӱ����]����1,[�������]����2,[���ֻ�����]����3:");
				scanf("%d",&newtype);
			}
			data.type=(play_type_t)newtype;
		}
		printf("\t\t\t[��ͯ��]����1,[�����]����2,[���˾�]����3:");
		scanf("%d",&newrate);
		if(newrate==1 || newrate==2 || newrate==3)
		{
			data.rating=(play_rating_t)newrate;
		}
		else
		{
			while(newrate!=1 && newrate!=2 && newrate!=3)
			{	printf("\t\t\t��������,����������!\n");
				printf("\t\t\t[��ͯ��]����1,[�����]����2,[���˾�]����3:");
				scanf("%d",&newrate);
			}
			data.rating=(play_rating_t)newrate;
		}
		printf("\t\t\t�����뿪ʼ��ӳ����(��2014 7 2):");
		scanf("%d %d %d",&newSyear,&newSmonth,&newSday);
		printf("\t\t\t�������ӳ��������(��2014 7 2):");
		scanf("%d %d %d",&newEyear,&newEmonth,&newEday);
		printf("\t\t\t������Ʊ��:");
		scanf("%d",&newprice);
		strcpy(data.name,newname);
		strcpy(data.area,newarea);
		data.duration=newduration;
		data.price=newprice;
		data.start_date.year=newSyear;
		data.start_date.month=newSmonth;
		data.start_date.day=newSday;
		data.end_date.year=newEyear;
		data.end_date.month=newEmonth;
		data.end_date.day=newEday;
	}
	else
	{
		//��ȡ����
		data.id = EntKey_Srv_CompNewKey("Play");

		printf("\t\t\t�������Ŀ����:");
		scanf("%s",newname);
		printf("\t\t\t�������Ŀ����:");
		scanf("%s",newarea);
		printf("\t\t\t�������Ŀʱ��(�Է���Ϊ��λ,��������):");
		scanf("%d",&newduration);
		printf("\t\t\t��Ӱ��������1,�����������2,���ֻ���������3:");
		scanf("%d",&type);
		if(type==1 || type==2 || type==3)
		{
			data.type=type;
		}
		else
		{
			while(type!=1 && type!=2 && type!=3) {	
				printf("\t\t\t��������,����������!\n");
				printf("\t\t\t��Ӱ��������1,�����������2,���ֻ���������3:");
				scanf("%d",&type);
			}
			data.type=type;
		}
		printf("\t\t\t��ͯ������1,���������2,���˾�����3:");
		scanf("%d",&rating);
		if(rating==1 || rating==2 || rating==3) {
			data.rating=rating;
		}
		else {
			while(rating!=1 && rating!=2 && rating!=3) {	
				printf("\t\t\t��������,����������!\n");
				printf("\t\t\t��ͯ������1,���������2,���˾�����3:");
				scanf("%d",&rating);
			}
			data.rating=rating;
		}
		printf("\t\t\t�����뿪ʼ��ӳ����(��2014 7 2):");
		scanf("%d %d %d",&data.start_date.year,&data.start_date.month,&data.start_date.day);
		printf("\t\t\t�������ӳ��������(��2014 7 2):");
		scanf("%d %d %d",&data.end_date.year,&data.end_date.month,&data.end_date.day);
		printf("\t\t\t������Ʊ��:");
		scanf("%d",&data.price);
        printf("\n\t\t\t==================================================================\n");
	}

    printf("\t\t\t------------------------------------------------------------------\n");

	if (Play_Srv_Modify(&data))
	{
		rtn = 1;
		printf("\t\t\t��Ŀ�޸ĳɹ�!\n��ȷ�ϼ�����!\n");
	}
	else
	{
		printf("\t\t\t��Ŀ�޸�ʧ��!\n��ȷ�ϼ�����!\n");
	}
	getchar();
	return rtn;
}

/*
 * Function:    Play_UI_Delete
 * Function ID:	TTMS_SCU_Play_UI_Del
 * Description: ����ID��ɾ����Ŀ��Ϣ
 * Input:       ��ɾ���ľ�ĿID��
 * Output:      ��ʾɾ���Ƿ�ɹ�
 * Return:      0��ʾɾ��ʧ�ܣ�1��ʾɾ���ɹ�
 */
 
int Play_UI_Delete(int id)
{
	int rtn = 0;

	if (Play_Srv_DeleteByID(id))
	{
		printf("\t\t\t��Ŀ�ɹ�ɾ��!\n��ȷ�ϼ�����!\n");
		rtn=1;
	}
	else{
		printf("\t\t\t��Ŀɾ��ʧ��!\n��ȷ�ϼ�����!\n");
	}
	getchar();
	return rtn;
}

/*
 * Function:    Play_UI_Query
 * Function ID:	TTMS_SCU_Play_UI_Qry
 * Description: ����ID�Ų�ѯ��Ŀ��Ϣ
 * Input:       �����ҵľ�ĿID��
 * Output:      ���ҵ��ľ�Ŀ��Ϣ
 * Return:      0��ʾδ�ҵ���1��ʾ�ҵ���
 */
 
 int Play_UI_Query(int id)
{
	play_t data;
	int rtn=0,i;
	if(Play_Srv_FetchByID(id,&data))
	{
		printf("\t\t\t��ĿID [%d]\n", data.id);
		printf("\t\t\t��Ŀ���� [%s]\n", data.name);
		i=data.type;
		switch(i)
		{
			case 1:printf("\t\t\t��Ŀ����[%s]\n","��Ӱ����");break;
			case 2:printf("\t\t\t��Ŀ����[%s]\n","�������");break;
			case 3:printf("\t\t\t��Ŀ����[%s]\n","���ֻ�����");break;
		}
		printf("\t\t\t��Ŀ��Ʒ���� [%s]\n",data.area);
		i=data.rating;
		switch(i)
		{
			case 1:printf("\t\t\t��Ŀ�ȼ�[%s]\n","��ͯ��");break;
			case 2:printf("\t\t\t��Ŀ�ȼ�[%s]\n","�����");break;
			case 3:printf("\t\t\t��Ŀ�ȼ�[%s]\n","���˾�");break;
		}
		printf("\t\t\t��Ŀʱ�� [%d]\n",data.duration);
		printf("\t\t\t��Ŀ��ӳ���� [%d.%d.%d]\n",data.start_date.year,data.start_date.month,data.start_date.day);
		printf("\t\t\t��ӳ�������� [%d.%d.%d]\n",data.end_date.year,data.end_date.month,data.end_date.day);
		rtn=1;
	}
	else{
		printf("\t\t\tû���ҵ��þ�Ŀ����Ϣ!\n");
	}
	printf("\t\t\t�����������!");
	getchar();
	getchar();
	return rtn;
}
