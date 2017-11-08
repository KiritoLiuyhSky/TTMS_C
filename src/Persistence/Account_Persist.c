/*
 * Account_Persist.c
 *
 *  Created on: 2015��5��8��
 *      Author: Administrator
 */

#include "Account_Persist.h"
#include "../Service/Account.h"
#include "../common/list.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

static const char ACCOUNT_DATA_FILE[] = "Account.dat";
static const char ACCOUNT_DATA_TEMP_FILE[] = "AccountTmp.dat";

//�ж��˺��ļ��Ƿ���ڣ�����1���ڣ�����0������
int Account_Perst_CheckAccFile() {
	if (access(ACCOUNT_DATA_FILE,0)==0){
		return 1;
	}
	else{
	    return 0;
	}
}

//�����û��������˺�,����ɹ� return 1������ return 0
int Account_Perst_SelByName(char usrName[], account_t *buf) {
	FILE *fp = fopen(ACCOUNT_DATA_FILE, "rb");
	if (NULL == fp) {
		printf("�޷����ļ� %s!\n", ACCOUNT_DATA_FILE);
		return 0;
	}

	account_t data;
	int found = 0;

	while (!feof(fp)) {
		if (fread(&data, sizeof(account_t), 1, fp)) {
			if (strcmp(usrName,data.username)==0) {
				*buf = data;
				found = 1;
				break;
			}
		}
	}
	fclose(fp);
    return found;
}

//���˺�д���˺��ļ��У�����ʵ��д������ݿ���Ŀ
int Account_Perst_Insert(const account_t *data) {
	assert(NULL!=data);
	FILE *fp=fopen(ACCOUNT_DATA_FILE,"ab");
	int rtn=0;
	if (NULL==fp)
	{
		printf("�޷����ļ� %s!\n", ACCOUNT_DATA_FILE);
		return 0;
	}
	rtn=fwrite(data,sizeof(account_t),1,fp);
	fclose(fp);
	return rtn;
}

//���˺��ļ��в���������˺�ƥ����˺ţ��ҵ� return 1������ return 0�������и�����д
int Account_Perst_Update(const account_t * data) {
	assert(NULL!=data);

	FILE *fp = fopen(ACCOUNT_DATA_FILE, "rb+");
	if (NULL == fp)
	{
		printf("�޷����ļ� %s!\n", ACCOUNT_DATA_FILE);
		return 0;
	}

	account_t buf;
	int found = 0;

	while (!feof(fp)) {
		if (fread(&buf, sizeof(account_t), 1, fp))
		{
			if (buf.id == data->id)
			{
				fseek(fp, -sizeof(account_t), SEEK_CUR);
				fwrite(data, sizeof(account_t), 1, fp);
				found = 1;
				break;
			}
		}
	}
	fclose(fp);

	return found;
}

//���˺��ļ���ɾ�������idƥ����˺ţ�ɾ���ɹ� return 1������ return 0��
int Account_Perst_DeleteByID(int id) {
	char cmd[100];
	sprintf(cmd, "������  %s %s", ACCOUNT_DATA_FILE, ACCOUNT_DATA_TEMP_FILE);
	system(cmd);

	if(rename(ACCOUNT_DATA_FILE, ACCOUNT_DATA_TEMP_FILE)<0)
	{
		printf("�޷����ļ� %s!\n", ACCOUNT_DATA_FILE);
		return 0;
	}
	FILE *fpSour, *fpTarg;
	fpSour = fopen(ACCOUNT_DATA_TEMP_FILE, "rb");
	if (NULL == fpSour )
	{
		printf("�޷����ļ� %s!\n", ACCOUNT_DATA_FILE);
		return 0;
	}
	fpTarg = fopen(ACCOUNT_DATA_FILE, "wb");
	if ( NULL == fpTarg )
	{
		printf("�޷����ļ� %s!\n", ACCOUNT_DATA_TEMP_FILE);
		return 0;
	}

	account_t buf;
	int found = 0;
	while (!feof(fpSour))
	{
		if (fread(&buf,sizeof(account_t),1,fpSour))
		{
			if (id==buf.id)
			{
				found=1;
				continue;
			}
			fwrite(&buf,sizeof(account_t),1,fpTarg);
		}
	}
	fclose(fpTarg);
	fclose(fpSour);

	//ɾ����ʱ�ļ�
	remove(ACCOUNT_DATA_TEMP_FILE);

	return found;
}

//���˺��ļ��в��������idƥ����˺ţ���ͨ��ָ��buf������ƥ��ɹ� return 1������ return 0��
int Account_Perst_SelectByID(int id, account_t *buf) {
	assert(NULL!=buf);

	FILE *fp = fopen(ACCOUNT_DATA_FILE, "rb");
	if (NULL == fp) {
		return 0;
	}

	account_t data;
	int found = 0;

	while (!feof(fp)) {
		if (fread(&data, sizeof(account_t), 1, fp))
		{
			if (id == data.id)
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

//������ACCOUNT_DATA_FILE�ļ�����̬�����û��˺�list����,list Ϊ����ͷָ�룬����list����
int Account_Perst_SelectAll(account_list_t list) {
	account_node_t *newNode;
	account_t data;
	int recCount = 0;

	assert(NULL!=list);

	if (access(ACCOUNT_DATA_FILE, 0))
	{
		printf("�޷����ļ� %s!\n", ACCOUNT_DATA_FILE);
		return 0;
	}

	List_Free(list, account_node_t);

	FILE *fp = fopen(ACCOUNT_DATA_FILE, "rb");
	if (NULL == fp)
	{
		printf("�޷����ļ� %s!\n", ACCOUNT_DATA_FILE);
		return 0;
	}

	while (!feof(fp))
	{
		if (fread(&data, sizeof(account_t), 1, fp))
		{
			newNode = (account_node_t*) malloc(sizeof(account_node_t));
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

