/*
 *  Seat_Persist.c
 *
 *  Created on: 2016年6月17日
 *  Author: 04153010_姜文昊
 */

#include "Seat_Persist.h"
#include "../Service/Seat.h"
#include "../common/list.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <io.h>

static const char SEAT_DATA_FILE[] = "Seat.dat";
static const char SEAT_DATA_TEMP_FILE[] = "SeatTmp.dat";

/*向文件中插入新座位*/
int Seat_Perst_Insert(const seat_t *data)
{
	assert(NULL!=data);
	FILE *fp = fopen(SEAT_DATA_FILE, "ab");
	int rtn = 0;

	if (NULL == fp)
	{
		printf("无法打开文件 %s!\n", SEAT_DATA_FILE);
		return 0;
	}

	rtn = fwrite(data, sizeof(seat_t), 1, fp);

	fclose(fp);
	return rtn;
}

/*向文件中批量插入座位*/
int Seat_Perst_InsertBatch(seat_list_t list)
{
	seat_node_t *p;
	assert(NULL!=list);
	FILE *fp = fopen(SEAT_DATA_FILE, "ab");
	int rtn = 0;

	if (NULL == fp)
	{
		printf("无法打开文件 %s!\n", SEAT_DATA_FILE);
		return 0;
	}

	List_ForEach(list,p)
	{
		fwrite(&(p->data), sizeof(seat_t), 1, fp);
		rtn++;
	}

	fclose(fp);
	return rtn;
}

/*向文件中更新座位*/
int Seat_Perst_Update(const seat_t *seatdata)
{
	assert(NULL!=seatdata);

	FILE *fp = fopen(SEAT_DATA_FILE, "rb+");
	if (NULL == fp)
	{
		printf("无法打开文件 %s!\n", SEAT_DATA_FILE);
		return 0;
	}

	seat_t buf;
	int found = 0;

	while (!feof(fp)) {
		if (fread(&buf, sizeof(seat_t), 1, fp))
		{
			if (buf.id == seatdata->id)
			{
				fseek(fp, -sizeof(seat_t), SEEK_CUR);
				fwrite(seatdata, sizeof(seat_t), 1, fp);
				found = 1;
				break;
			}
		}
	}
	fclose(fp);

	return found;
}

/*根据座位ID在文件中删除其所有座位*/
int Seat_Perst_DeleteByID(int ID)
{
	char cmd[100];
	//sprintf(cmd, "重命名  %s %s", SEAT_DATA_FILE, SEAT_DATA_TEMP_FILE);
	system(cmd);
	FILE *fpSour, *fpTarg;

	if (rename(SEAT_DATA_FILE, SEAT_DATA_TEMP_FILE) < 0)
	{
		printf("无法打开文件 %s!\n", SEAT_DATA_FILE);
		return 0;
	}

	fpSour = fopen(SEAT_DATA_TEMP_FILE, "rb");
	fpTarg = fopen(SEAT_DATA_FILE, "wb");

	if (NULL == fpTarg)
	{
		printf("无法打开文件 %s!\n", SEAT_DATA_FILE);
		return 0;
	}
	if (NULL == fpSour)
	{
		printf("无法打开文件 %s!\n", SEAT_DATA_TEMP_FILE);
		return 0;
	}

	seat_t buf;

	int found = 0;
	while (!feof(fpSour)) {
		if (fread(&buf, sizeof(seat_t), 1, fpSour))
		{
			if (ID == buf.id)
			{
				found = 1;
				continue;
			}
			fwrite(&buf, sizeof(seat_t), 1, fpTarg);
		}
	}

	fclose(fpTarg);
	fclose(fpSour);

	//删除临时文件
	remove(SEAT_DATA_TEMP_FILE);

	return found;
}

/*根据演出厅ID在文件中删除座位*/
int Seat_Perst_DeleteAllByRoomID(int roomID)
{
	char cmd[100];
	sprintf(cmd, "重命名  %s %s", SEAT_DATA_FILE, SEAT_DATA_TEMP_FILE);
	system(cmd);

	FILE *fpSour, *fpTarg;
	fpSour = fopen(SEAT_DATA_TEMP_FILE, "rb");
	fpTarg = fopen(SEAT_DATA_FILE, "wb");

	if (rename(SEAT_DATA_FILE, SEAT_DATA_TEMP_FILE) < 0)
	{
		printf("无法打开文件 %s!\n", SEAT_DATA_FILE);
		return 0;
	}

	fpSour = fopen(SEAT_DATA_TEMP_FILE, "rb");
	fpTarg = fopen(SEAT_DATA_FILE, "wb");

	if (NULL == fpTarg)
	{
		printf("无法打开文件 %s!\n", SEAT_DATA_FILE);
		return 0;
	}
	if (NULL == fpSour)
	{
		printf("无法打开文件 %s!\n", SEAT_DATA_TEMP_FILE);
		return 0;
	}

	seat_t buf;

	int found = 0;
	while (!feof(fpSour))
	{
		if (fread(&buf, sizeof(seat_t), 1, fpSour))
		{
			if (roomID == buf.roomID)
			{
				found += 1;
				continue;
			}
			fwrite(&buf, sizeof(seat_t), 1, fpTarg);
		}
	}

	fclose(fpTarg);
	fclose(fpSour);

	//删除临时文件
	remove(SEAT_DATA_TEMP_FILE);

	return found;
}

/*根据座位ID从文件中载入ID*/
int Seat_Perst_SelectByID(int ID, seat_t *buf)
{
	assert(NULL!=buf);

	FILE *fp = fopen(SEAT_DATA_FILE, "rb");
	if (NULL == fp) {
		return 0;
	}

	seat_t data;
	int found = 0;

	while (!feof(fp)) {
		if (fread(&data, sizeof(seat_t), 1, fp))
		{
			if (ID == data.id)
			{
				*buf = data;
				found = 1;
				break;
			}
		}
	}
	fclose(fp);

	return found;
}

/*从文件中载入所有座位*/
int Seat_Perst_SelectAll(seat_list_t list)
{
	seat_node_t *newNode;
	seat_t data;
	int recCount = 0;

	assert(NULL!=list);

	if (access(SEAT_DATA_FILE, 0))
	{
		printf("无法打开文件 %s!\n", SEAT_DATA_FILE);
		return 0;
	}

	List_Free(list, seat_node_t);

	FILE *fp = fopen(SEAT_DATA_FILE, "rb");
	if (NULL == fp)
	{
		printf("无法打开文件 %s!\n", SEAT_DATA_FILE);
		return 0;
	}

	while (!feof(fp))
	{
		if (fread(&data, sizeof(seat_t), 1, fp))
		{
			newNode = (seat_node_t*) malloc(sizeof(seat_node_t));
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

/*根据演出厅ID从文件中载入其所有座位*/
int Seat_Perst_SelectByRoomID(seat_list_t list, int roomID)
{
	seat_node_t *newNode;
	seat_t data;
	int recCount = 0;

	assert(NULL!=list);


	if (access(SEAT_DATA_FILE, 0))
	{
		printf("无法打开文件 %s!\n", SEAT_DATA_FILE);
		return 0;
	}

	List_Free(list, seat_node_t);

	FILE *fp = fopen(SEAT_DATA_FILE, "rb");
	if (NULL == fp)
	{
		printf("无法打开文件 %s!\n", SEAT_DATA_FILE);
		return 0;
	}

	while (!feof(fp))
	{
		if (fread(&data, sizeof(seat_t), 1, fp))
			if (data.roomID == roomID)  //若座位是本放映厅的座位，则读出
			{
				newNode = (seat_node_t*) malloc(sizeof(seat_node_t));
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
