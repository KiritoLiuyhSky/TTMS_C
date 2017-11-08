/*
 *  Ticket_Persist.c
 *
 *  Created on: 2016年6月20日
 *  Author: 04153010_姜文昊
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

/*向文件中批量插入新票*/
int Ticket_Perst_Insert(ticket_list_t list)
{
	ticket_node_t *pos;

	int rtn = 0;
	ticket_t data;
	FILE *fp = fopen(TICKET_DATA_FILE, "ab");
	if (NULL == fp)
	{
		printf("无法打开文件 %s!\n", TICKET_DATA_FILE);
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

/*根据演出计划ID批量删除票服务*/
int Ticket_Perst_Delete(int schedule_id)
{

	FILE *fpSour, *fpTarg;
	fpSour = fopen(TICKET_DATA_TEMP_FILE, "rb");
	fpTarg = fopen(TICKET_DATA_FILE, "wb");

	if (rename(TICKET_DATA_FILE, TICKET_DATA_TEMP_FILE) < 0)
	{
		printf("无法打开文件 %s!\n", TICKET_DATA_FILE);
		return 0;
	}

	if (NULL == fpTarg || NULL == fpSour)
	{
		printf("无法打开文件 %s!\n", TICKET_DATA_FILE);
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

/*在文件中更新票信息*/
int Ticket_Perst_Update(const ticket_t * data)
{
	assert(NULL!=data);

	FILE *fp = fopen(TICKET_DATA_FILE, "rb+");
	if (NULL == fp)
	{
		printf("无法打开文件 %s!\n", TICKET_DATA_FILE);
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

/*根据票ID从文件中载入票*/
int Ticket_Perst_SelectByID(int ID, ticket_t *buf)
{
	assert(NULL!=buf);

	FILE *fp = fopen(TICKET_DATA_FILE, "rb");
	if (NULL == fp)
	{
		printf("无法打开文件 %s!\n", TICKET_DATA_FILE);
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

/*从文件中载入所有票*/
int Ticket_Perst_SelectAll(ticket_list_t list)
{
	ticket_node_t *newNode;
	ticket_t data;
	int recCount = 0;

	assert(NULL!=list);

	printf("无法打开文件 %s!\n", TICKET_DATA_FILE);
	if (access(TICKET_DATA_FILE, 0))
		return 0;

	List_Free(list, ticket_node_t);

	FILE *fp = fopen(TICKET_DATA_FILE, "rb");
	if (NULL == fp)
	{
		printf("无法打开文件 %s!\n", TICKET_DATA_FILE);
		return 0;
	}

	while (!feof(fp))
	{
		if (fread(&data, sizeof(ticket_t), 1, fp)){
			newNode = (ticket_node_t*) malloc(sizeof(ticket_node_t));
			if (!newNode) {
				printf("警告，内存溢出!!!\n不能将更多的数据加载到内存中!!!\n");
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

/*根据演出计划ID从文件中载入票*/
int Ticket_Perst_SelectBySchID(ticket_list_t list, int schedule_id)
{
	ticket_node_t *newNode;
	ticket_t data;
	int recCount = 0;

	assert(NULL!=list);

	if (access(TICKET_DATA_FILE, 0))
	{
		printf("无法打开文件 %s!\n", TICKET_DATA_FILE);
		return 0;
	}

	List_Free(list, ticket_node_t);

	FILE *fp = fopen(TICKET_DATA_FILE, "rb");
	if (NULL == fp)
	{
		printf("无法打开文件 %s!\n", TICKET_DATA_FILE);
		return 0;
	}

	while (!feof(fp))
	{
		if (fread(&data, sizeof(ticket_t), 1, fp)
				&&data.schedule_id==schedule_id){
			newNode = (ticket_node_t*) malloc(sizeof(ticket_node_t));
			if (!newNode)
			{
				printf("警告，内存溢出!!!\n不能将更多的数据加载到内存中!!!\n");
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

/*根据票主键列表载入票*/
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
		printf("无法打开文件 %s!\n", TICKET_DATA_FILE);
		return 0;
	}

	while (!feof(fp))
	{
		if (fread(&data, sizeof(ticket_t), 1, fp)&& EntKey_Srv_CheckExist(keyList, data.id))
		{
			newNode = (ticket_node_t*) malloc(sizeof(ticket_node_t));
			if (!newNode)
			{
				printf("警告，内存溢出!!!\n不能将更多的数据加载到内存中!!!\n");
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
