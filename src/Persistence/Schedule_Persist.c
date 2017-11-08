/*
 *  Schedule_Persist.c
 *
 *  Created on: 2016��6��23��
 *  Author: 04153010_�����
 */


#include "Schedule_Persist.h"
#include "../Service/schedule.h"
#include "../Common/list.h"
#include "../Service/ticket.h"
//#include "../Persistence/Common_Persist.h"
#include "../View/Ticket_UI.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>


static const char SCHEDULE_DATA_FILE[] = "Schedule.dat";//�����ݳ��ƻ���Ϣ
static const char SCHEDULE_DATA_TEMP_FILE[] = "ScheduleTmp.dat";////ɾ�������ʱ����ʱ�����ݳ��ƻ���Ϣ

/*
 * Function:    Schedule_Perst_Insert
 * Function ID:	TTMS_SCU_Schedule_Perst_Insert
 * Description: ���ݳ��ƻ���Ϣ�ļ�ĩβд��һ���ݳ��ƻ���Ϣ
 * Input:       �������ļ����ݳ��ƻ���Ϣ����
 * Output:      ���ļ�ʧ�ܵ���Ϣ
 * Return:      д���ļ��ļ�¼��
 */
int Schedule_Perst_Insert(const schedule_t *data)
{
	assert(NULL!=data);
	FILE *fp=fopen(SCHEDULE_DATA_FILE,"ab");
	int rtn=0;
	if (NULL==fp)
	{
		printf("�޷����ļ� %s!\n", SCHEDULE_DATA_FILE);
		return 0;
	}
	rtn=fwrite(data,sizeof(schedule_t),1,fp);
	fclose(fp);
	return rtn;
}

/*
 * Function:    Schedule_Perst_Update
 * Function ID:	TTMS_SCU_Schedule_Perst_Mod
 * Description: �����ݳ��ƻ�ID�Ÿ����ļ��е��ݳ��ƻ���Ϣ
 * Input:       �����ļ��и��µ��ݳ��ƻ���Ϣ����
 * Output:      ���ļ�ʧ�ܵ���Ϣ
 * Return:      ���µ��ݳ��ƻ���Ϣ����0��ʾδ�ҵ���1��ʾ�ҵ�������
 */
int Schedule_Perst_Update(const schedule_t *data)
{
	assert(NULL!=data);
	FILE *fp = fopen(SCHEDULE_DATA_FILE, "rb+");
	if (NULL == fp)
	{
		printf("�޷����ļ� %s!\n", SCHEDULE_DATA_FILE);
		return 0;
	}

	schedule_t buf;
	int found = 0;

	while (!feof(fp)) {
		if (fread(&buf, sizeof(schedule_t), 1, fp))
		{
			if (buf.id == data->id)
			{
				fseek(fp, -sizeof(schedule_t), SEEK_CUR);
				fwrite(data, sizeof(schedule_t), 1, fp);
				found = 1;
				break;
			}
		}
	}
	fclose(fp);

	return found;
}

/*
 * Function:    Schedule_Perst_DeleteByID
 * Function ID:	TTMS_SCU_Schedule_Perst_DelByID
 * Description: �����ݳ��ƻ�ID��ɾ���ݳ��ƻ�����Ϣ
 * Input:       ��ɾ�����ݳ��ƻ�ID��
 * Output:      ���ļ�ʧ�ܵ���Ϣ
 * Return:      0��ʾɾ��ʧ�ܣ�1��ʾɾ���ɹ�
 */
int Schedule_Perst_DeleteByID(int ID){
	char cmd[100];
	sprintf(cmd, "������  %s %s", SCHEDULE_DATA_FILE, SCHEDULE_DATA_TEMP_FILE);
	system(cmd);

	if(rename(SCHEDULE_DATA_FILE, SCHEDULE_DATA_TEMP_FILE)<0)
	{
		printf("�޷����ļ� %s!\n", SCHEDULE_DATA_FILE);
		return 0;
	}

	FILE *fpSour, *fpTarg;

	fpSour = fopen(SCHEDULE_DATA_TEMP_FILE, "rb");
	if (NULL == fpSour )
	{
		printf("�޷����ļ� %s!\n", SCHEDULE_DATA_FILE);
		return 0;
	}
	fpTarg = fopen(SCHEDULE_DATA_FILE, "wb");

	if ( NULL == fpTarg )
	{
		printf("�޷����ļ� %s!\n", SCHEDULE_DATA_TEMP_FILE);
		return 0;
	}

	schedule_t buf;
	int found = 0;
	while (!feof(fpSour))
	{
		if (fread(&buf,sizeof(schedule_t),1,fpSour))
		{
			if (ID==buf.id)
			{
				found=1;
				continue;
			}
			fwrite(&buf,sizeof(schedule_t),1,fpTarg);
		}
	}
	fclose(fpTarg);
	fclose(fpSour);

	//ɾ����ʱ�ļ�
	remove(SCHEDULE_DATA_TEMP_FILE);

	return found;
}

/*
 * Function:    Schedule_Perst_SelectByID
 * Function ID:	TTMS_SCU_Schedule_Perst_SelByID
 * Description: �����ݳ��ƻ�ID��ɾ���ݳ��ƻ�����Ϣ
 * Input:       �����ҵ��ݳ��ƻ�ID�ţ�������ҽ�����ڴ�ĵ�ַ
 * Output:      ��
 * Return:      0��ʾδ�ҵ���1��ʾ�ҵ���
 */
int Schedule_Perst_SelectByID(int ID, schedule_t *buf){
	assert(NULL!=buf);

	FILE *fp = fopen(SCHEDULE_DATA_FILE, "rb");
	if (NULL == fp) {
		return 0;
	}

	schedule_t data;
	int found = 0;

	while (!feof(fp)) {
		if (fread(&data, sizeof(schedule_t), 1, fp))
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

/*
 * Function:    Schedule_Perst_SelectAll
 * Function ID:	TTMS_SCU_Schedule_Perst_SelAll
 * Description: �������ݳ��ƻ���Ϣ������һ������
 * Input:       listΪ�ݳ��ƻ���Ϣ�����ͷָ��
 * Output:      ��ʾ��������ʱ������ռ�ʧ��
 * Return:      ���ز��ҵ��ļ�¼��Ŀ
 */
int Schedule_Perst_SelectAll(schedule_list_t list){
	schedule_node_t *newNode;
	schedule_t data;
	int recCount = 0;

	assert(NULL!=list);

	if (access(SCHEDULE_DATA_FILE, 0))
	{
		printf("�޷����ļ� %s!\n", SCHEDULE_DATA_FILE);
		return 0;
	}

	List_Free(list, schedule_node_t);

	FILE *fp = fopen(SCHEDULE_DATA_FILE, "rb");
	if (NULL == fp)
	{
		printf("�޷����ļ� %s!\n", SCHEDULE_DATA_FILE);
		return 0;
	}

	while (!feof(fp))
	{
		if (fread(&data, sizeof(schedule_t), 1, fp))
		{
			newNode = (schedule_node_t*) malloc(sizeof(schedule_node_t));
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

/*
 * Function:    Schedule_Perst_SelectByPlay
 * Function ID:	TTMS_SCU_Schedule_Perst_SelByPlay
 * Description: ��ͬ���Ŀ���ݳ��ƻ���Ϣ��������������һ������
 * Input:       listΪ�����������ݳ��ƻ���Ϣ�����ͷָ�룬��Ŀ��ID��
 * Output:      ��ʾ��������ʱ������ռ�ʧ��
 * Return:      ���ز��ҵ��ļ�¼��Ŀ
 */
int Schedule_Perst_SelectByPlay(schedule_list_t list, int play_id){
	schedule_node_t *newNode;
	schedule_t data;
	int recCount = 0;

	assert(NULL!=list);

	if (access(SCHEDULE_DATA_FILE, 0))
	{
		printf("�޷����ļ� %s!\n", SCHEDULE_DATA_FILE);
		return 0;
	}

	List_Free(list, schedule_node_t);

	FILE *fp = fopen(SCHEDULE_DATA_FILE, "rb");
	if (NULL == fp)
	{
		printf("�޷����ļ� %s!\n", SCHEDULE_DATA_FILE);
		return 0;
	}

	while (!feof(fp))
	{
		if (fread(&data, sizeof(schedule_t), 1, fp)
				&&data.play_id==play_id){
			newNode = (schedule_node_t*) malloc(sizeof(schedule_node_t));
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
