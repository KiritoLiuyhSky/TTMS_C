/*
 *  Ticket_Persist.c
 *
 *  Created on: 2016��6��20��
 *  Author: 04153010_�����
 */

#include "Ticket_Persist.h"
//#include "../Persistence/Common_Persist.h"
#include "../Service/ticket.h"
#include "../Service/seat.h"
#include "../common/list.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <io.h>

static const char TICKET_DATA_FILE[]="Ticket.dat";
static const char TICKET_DATA_TEMP_FILE[]="TicketTemp.dat";

/*���ļ�������������Ʊ*/
int Ticket_Perst_Insert(ticket_list_t list)
{
	ticket_node_t *pos;

	int rtn = 0;
	ticket_t data;
	FILE *fp = fopen(TICKET_DATA_FILE, "ab");
	if (NULL == fp)
	{
		printf("�޷����ļ� %s!\n", TICKET_DATA_FILE);
		return 0;
	}
	List_ForEach(list,pos)
	{
		data=pos->data;
		fwrite(&data,sizeof(ticket_t),1,fp);
		rtn++;
	}
	fclose(fp);
	return rtn;
}

/*�����ݳ��ƻ�ID����ɾ��Ʊ����*/
int Ticket_Perst_Delete(int schedule_id)
{

	FILE *fpSour, *fpTarg;
	fpSour = fopen(TICKET_DATA_TEMP_FILE, "rb");
	fpTarg = fopen(TICKET_DATA_FILE, "wb");

	if (rename(TICKET_DATA_FILE, TICKET_DATA_TEMP_FILE) < 0)
	{
		printf("�޷����ļ� %s!\n", TICKET_DATA_FILE);
		return 0;
	}

	if (NULL == fpTarg || NULL == fpSour)
	{
		printf("�޷����ļ� %s!\n", TICKET_DATA_FILE);
		return 0;
	}

	ticket_t buf;

	int found = 0;
	while (!feof(fpSour))
	{
		if (fread(&buf, sizeof(ticket_t), 1, fpSour))
		{
			if (schedule_id == buf.schedule_id)
			{
				found =1;
				continue;
			}
			fwrite(&buf, sizeof(ticket_t), 1, fpTarg);
		}
	}

	fclose(fpTarg);
	fclose(fpSour);

	remove(TICKET_DATA_TEMP_FILE);

	return found;

}

/*���ļ��и���Ʊ��Ϣ*/
int Ticket_Perst_Update(const ticket_t * data)
{
	assert(NULL!=data);

	FILE *fp = fopen(TICKET_DATA_FILE, "rb+");
	if (NULL == fp)
	{
		printf("�޷����ļ� %s!\n", TICKET_DATA_FILE);
		return 0;
	}

	ticket_t buf;
	int found = 0;

	while (!feof(fp)) {
		if (fread(&buf, sizeof(ticket_t), 1, fp))
		{
			if (buf.id == data->id)
			{
				fseek(fp, -sizeof(ticket_t), SEEK_CUR);
				fwrite(data, sizeof(ticket_t), 1, fp);
				found = 1;
				break;
			}

		}
	}
	fclose(fp);

	return found;
}

/*����ƱID���ļ�������Ʊ*/
int Ticket_Perst_SelectByID(int ID, ticket_t *buf)
{
	assert(NULL!=buf);

	FILE *fp = fopen(TICKET_DATA_FILE, "rb");
	if (NULL == fp)
	{
		printf("�޷����ļ� %s!\n", TICKET_DATA_FILE);
		return 0;
	}

	ticket_t data;
	int found = 0;

	while (!feof(fp))
	{
		if (fread(&data, sizeof(ticket_t), 1, fp))
		{
			if (ID == data.id)
			{
				*buf = data;
				found = 1;
				break;
			};

		}
	}
	fclose(fp);

	return found;
}

/*���ļ�����������Ʊ*/
int Ticket_Perst_SelectAll(ticket_list_t list)
{
	ticket_node_t *newNode;
	ticket_t data;
	int recCount = 0;

	assert(NULL!=list);

	printf("�޷����ļ� %s!\n", TICKET_DATA_FILE);
	if (access(TICKET_DATA_FILE, 0))
		return 0;

	List_Free(list, ticket_node_t);

	FILE *fp = fopen(TICKET_DATA_FILE, "rb");
	if (NULL == fp)
	{
		printf("�޷����ļ� %s!\n", TICKET_DATA_FILE);
		return 0;
	}

	while (!feof(fp))
	{
		if (fread(&data, sizeof(ticket_t), 1, fp)){
			newNode = (ticket_node_t*) malloc(sizeof(ticket_node_t));
			if (!newNode) {
				printf("���棬�ڴ����!!!\n���ܽ���������ݼ��ص��ڴ���!!!\n");
				break;
			}
			newNode->data = data;
			List_AddTail(list, newNode);
			recCount++;
		}
	}
	fclose(fp);
	return recCount;
}

/*�����ݳ��ƻ�ID���ļ�������Ʊ*/
int Ticket_Perst_SelectBySchID(ticket_list_t list, int schedule_id)
{
	ticket_node_t *newNode;
	ticket_t data;
	int recCount = 0;

	assert(NULL!=list);

	if (access(TICKET_DATA_FILE, 0))
	{
		printf("�޷����ļ� %s!\n", TICKET_DATA_FILE);
		return 0;
	}

	List_Free(list, ticket_node_t);

	FILE *fp = fopen(TICKET_DATA_FILE, "rb");
	if (NULL == fp)
	{
		printf("�޷����ļ� %s!\n", TICKET_DATA_FILE);
		return 0;
	}

	while (!feof(fp))
	{
		if (fread(&data, sizeof(ticket_t), 1, fp)
				&&data.schedule_id==schedule_id){
			newNode = (ticket_node_t*) malloc(sizeof(ticket_node_t));
			if (!newNode)
			{
				printf("���棬�ڴ����!!!\n���ܽ���������ݼ��ص��ڴ���!!!\n");
				break;
			}
			newNode->data = data;
			List_AddTail(list, newNode);
			recCount++;
		}
	}

	fclose(fp);
	return recCount;
}

/*����Ʊ�����б�����Ʊ*/
int Ticket_Perst_SelectByKeyList(ticket_list_t list, entkey_list_t keyList)
{
	ticket_node_t *newNode;
	ticket_t data;
	int recCount = 0;

	assert(NULL!=list && keyList);


	List_Free(list, ticket_node_t);

	FILE *fp = fopen(TICKET_DATA_FILE, "rb");
	if (NULL == fp)
	{
		printf("�޷����ļ� %s!\n", TICKET_DATA_FILE);
		return 0;
	}

	while (!feof(fp))
	{
		if (fread(&data, sizeof(ticket_t), 1, fp)&& EntKey_Srv_CheckExist(keyList, data.id))
		{
			newNode = (ticket_node_t*) malloc(sizeof(ticket_node_t));
			if (!newNode)
			{
				printf("���棬�ڴ����!!!\n���ܽ���������ݼ��ص��ڴ���!!!\n");
				break;
			}
			newNode->data = data;
			List_AddTail(list, newNode);
			recCount++;
		}
	}

	fclose(fp);
	return recCount;
}
