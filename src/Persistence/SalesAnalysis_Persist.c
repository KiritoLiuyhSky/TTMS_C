/*
 * salesanalysisPersist.c
 *
 *  Created on: 2015年5月8日
 *      Author: Administrator
 */

#include "../Persistence/EntityKey_Persist.h"
//#include "../Service/play.h"
#include "../Service/Sale.h"
#include "../Service/salesanalysis.h"
#include "../Common/list.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

//static const char PLAY_DATA_FILE[] = "play.dat";
static const char SALE_DATA_FILE[] = "sale.dat";
static const char SALESANALYSIS_DATA_FILE[] = "salesanalysis.dat";

//将一条salesanalysis记录（*data）写入salesanalysis.dat文件；成功return 1，否则return 0
int Salesanalysis_Perst_Insert(const salesanalysis_t *data) {
	assert(NULL!=data);
	FILE *fp=fopen(SALESANALYSIS_DATA_FILE,"ab");
	int rtn=0;
	if (NULL==fp)
	{
		printf("无法打开文件 %s!\n", SALESANALYSIS_DATA_FILE);
		return 0;
	}
	rtn=fwrite(data,sizeof(salesanalysis_t),1,fp);
	fclose(fp);
	return rtn;
}

//遍历读salesanalysis.dat文件建立销售分析（salesanalysis）链表
int SalesAnalysis_Perst_SelectAll(salesanalysis_list_t list) {
	salesanalysis_node_t *newNode;
	salesanalysis_t data;
	int recCount = 0;

	assert(NULL!=list);

	if (access(SALESANALYSIS_DATA_FILE, 0))
	{
		printf("无法打开文件 %s!\n", SALESANALYSIS_DATA_FILE);
		return 0;
	}

	List_Free(list, salesanalysis_node_t);

	FILE *fp = fopen(SALESANALYSIS_DATA_FILE, "rb");
	if (NULL == fp)
	{
		printf("无法打开文件 %s!\n", SALESANALYSIS_DATA_FILE);
		return 0;
	}

	while (!feof(fp))
	{
		if (fread(&data, sizeof(salesanalysis_t), 1, fp))
		{
			newNode = (salesanalysis_node_t*) malloc(sizeof(salesanalysis_node_t));
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

//遍历读Sale.dat文件建立list（sale_list_t） 链表，返回链表list长度
int Sale_Perst_SelectAll(sale_list_t list) {
	sale_node_t *newNode;
	sale_t data;
	int recCount = 0;

	assert(NULL!=list);

	if (access(SALE_DATA_FILE, 0))
	{
		printf("无法打开文件 %s!\n", SALE_DATA_FILE);
		return 0;
	}

	List_Free(list, sale_node_t);

	FILE *fp = fopen(SALE_DATA_FILE, "rb");
	if (NULL == fp)
	{
		printf("无法打开文件 %s!\n", SALE_DATA_FILE);
		return 0;
	}

	while (!feof(fp))
	{
		if (fread(&data, sizeof(sale_t), 1, fp))
		{
			newNode = (sale_node_t*) malloc(sizeof(sale_node_t));
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

