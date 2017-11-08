/*
 *  StudioPersist.c
 *
 *  Created on: 2016年6月16日
 *  Author: 04153010_姜文昊
 */

#include "Studio_Persist.h"
#include "../Service/studio.h"
#include "../common/list.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>


static const char STUDIO_DATA_FILE[] = "Studio.dat";
static const char STUDIO_DATA_TEMP_FILE[] = "StudioTmp.dat";

/*向文件中插入新演出厅*/ 
int Studio_Perst_Insert(const studio_t *data)
{
	assert(NULL!=data);
	FILE *fp=fopen(STUDIO_DATA_FILE,"ab");
	int rtn=0;
	if (NULL==fp) 
	{
		printf("无法打开文件 %s!\n", STUDIO_DATA_FILE);
		return 0;
	}
	rtn=fwrite(data,sizeof(studio_t),1,fp);
	fclose(fp);
	return rtn;
}

/*在文件中更新演出厅*/ 
int Studio_Perst_Update(const studio_t * data) 
{
	assert(NULL!=data);
	FILE *fp=fopen(STUDIO_DATA_FILE, "rb+");
	if (NULL==fp) 
	{
		printf("无法打开文件 %s!\n", STUDIO_DATA_FILE);
		return 0;
	}
	studio_t buf;
	int found = 0;
	while (!feof(fp)) 
	{
		if (fread(&buf, sizeof(studio_t),1,fp)) 
		{
			if (buf.id==data->id) 
			{
				fseek(fp,-sizeof(studio_t),SEEK_CUR);
				fwrite(data,sizeof(studio_t),1,fp);
				found=1;
				break;
			}

		}
	}
	fclose(fp);
	return found;
}

/*根据ID在文件中删除演出厅*/ 
int Studio_Perst_DeleteByID(int ID)
{
	char cmd[100];
	sprintf(cmd, "重命名  %s %s", STUDIO_DATA_FILE, STUDIO_DATA_TEMP_FILE);
	system(cmd);

	if(rename(STUDIO_DATA_FILE, STUDIO_DATA_TEMP_FILE)<0)
	{
		printf("无法打开文件 %s!\n", STUDIO_DATA_FILE);
		return 0;
	}
	FILE *fpSour, *fpTarg;
	fpSour = fopen(STUDIO_DATA_TEMP_FILE, "rb");
	if (NULL == fpSour )
	{
		printf("无法打开文件 %s!\n", STUDIO_DATA_FILE);
		return 0;
	}
	fpTarg = fopen(STUDIO_DATA_FILE, "wb");
	if ( NULL == fpTarg )
	{
		printf("无法打开文件 %s!\n", STUDIO_DATA_TEMP_FILE);
		return 0;
	}

	studio_t buf;
	int found = 0;
	while (!feof(fpSour)) 
	{
		if (fread(&buf,sizeof(studio_t),1,fpSour)) 
		{
			if (ID==buf.id) 
			{
				found=1;
				continue;
			}
			fwrite(&buf,sizeof(studio_t),1,fpTarg);
		}
	}
	fclose(fpTarg);
	fclose(fpSour);

	//删除临时文件
	remove(STUDIO_DATA_TEMP_FILE);
	return found;
}

/*根据ID从文件中载入演出厅*/ 
int Studio_Perst_SelectByID(int ID, studio_t *buf) {
	assert(NULL!=buf);
	FILE *fp = fopen(STUDIO_DATA_FILE, "rb");
	if (NULL == fp) {
		return 0;
	}
	studio_t data;
	int found = 0;
	while (!feof(fp)) 
	{
		if (fread(&data, sizeof(studio_t), 1, fp)) 
		{
			if (ID==data.id) {
				*buf=data;
				found=1;
				break;
			};
		}
	}
	fclose(fp);
	return found;
}

/*从文件中载入所有演出厅*/ 
int Studio_Perst_SelectAll(studio_list_t list) 
{
	studio_node_t *newNode;
	studio_t data;
	int recCount = 0;
	assert(NULL!=list);
	List_Free(list,studio_node_t);
	FILE *fp=fopen(STUDIO_DATA_FILE,"rb");
	if (NULL==fp)
	{
		printf("无法打开文件 %s!\n", STUDIO_DATA_FILE);
		return 0;
	}
	while (!feof(fp)) 
	{
		if (fread(&data,sizeof(studio_t),1,fp)) 
		{
			newNode=(studio_node_t*)malloc(sizeof(studio_node_t));
			if (!newNode) 
			{
				printf("警告，内存溢出!!!\n不能将更多的数据加载到内存中!!!\n");
				break;
			}
			newNode->data=data;
			List_AddTail(list,newNode);
			recCount++;
		}
	}
	fclose(fp);
	return recCount;
}
