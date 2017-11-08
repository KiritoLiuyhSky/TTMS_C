/*
* File name:  Schedule.c
* File ID:	  TTMS_SSF_Schedule_Srv_Sour
* Author:     KiritoLiuSky 
* Version:    V1.0
* Date: 	  2016��6��18��20:42:10 
*/


#include "Schedule.h"
#include "../Common/list.h"
#include "../Persistence/Schedule_Persist.h"
#include "Ticket.h"
#include <stdio.h>

/*
 * Function:    Schedule_Srv_Add
 * Function ID:	TTMS_SCU_Schedule_Srv_Add
 * Description: ���һ���ݳ��ƻ���Ϣ
 * Input:       ����ӵ��ݳ��ƻ���Ϣ����
 * Output:      ��
 * Return:      ��ӵļ�¼��
 */
inline int Schedule_Srv_Add(const schedule_t *data) {
	return Schedule_Perst_Insert(data);
}

/*
 * Function:    Schedule_Srv_Modify
 * Function ID:	TTMS_SCU_Schedule_ Srv _Mod
 * Description: ����һ���ݳ��ƻ���Ϣ
 * Input:       �����µ��ݳ��ƻ���Ϣ����
 * Output:      ��
 * Return:      ���µ��ݳ��ƻ���Ϣ����0��ʾδ�ҵ���1��ʾ�ҵ�������
 */
inline int Schedule_Srv_Modify(const schedule_t *data) {
	return Schedule_Perst_Update(data);
}

/*
 * Function:    Schedule_Srv_DeleteByID
 * Function ID:	TTMS_SCU_Schedule_Srv_DelByID
 * Description: ����ID��ɾ���ݳ��ƻ���Ϣ
 * Input:       ��ɾ�����ݳ��ƻ�ID��
 * Output:      ��
 * Return:      0��ʾɾ��ʧ�ܣ�1��ʾɾ���ɹ�
 */
inline int Schedule_Srv_DeleteByID(int ID) {
	return Schedule_Perst_DeleteByID(ID);
}

/*
 * Function:    Schedule_Srv_FetchByID
 * Function ID:	TTMS_SCU_Schedule_Srv_FetchByID
 * Description: ����ID�Ų���һ���ݳ��ƻ���Ϣ������buf��ָ��Ԫ
 * Input:       �����ҵ��ݳ��ƻ�ID�ţ������ҵ�����Ϣ���浽buf��
 * Output:      ��
 * Return:      0��ʾδ�ҵ���1��ʾ�ҵ���
 */
inline int Schedule_Srv_FetchByID(int ID, schedule_t *buf) {
	return Schedule_Perst_SelectByID(ID, buf);
} 

/*
 * Function:    Schedule_Srv_FetchAll
 * Function ID:	TTMS_SCU_Schedule_Srv_FetchAll
 * Description: ȡ�������ݳ��ƻ���Ϣ������һ������
 * Input:       list�ݳ��ƻ���Ϣ�����ͷָ��
 * Output:      ��
 * Return:      ���ز��ҵ��ļ�¼��Ŀ
 */
inline int Schedule_Srv_FetchAll(schedule_list_t list) {
	return Schedule_Perst_SelectAll(list);
}

/*
 * Function:    Schedule_Srv_FetchByPlay
 * Function ID:	TTMS_SCU_Schedule_Srv_FetchByPlay
 * Description: ���վ�ĿID�ţ�������ݳ��ƻ���Ϣȡ��������һ������
 * Input:       listΪ���ҵ����ݳ��ƻ���Ϣ�����ͷָ�룬play_idΪ��Ŀ��ID��
 * Output:      ��
 * Return:      ���ز��ҵ��ļ�¼��Ŀ
 */
inline int Schedule_Srv_FetchByPlay(schedule_list_t list,int play_id) {
	return Schedule_Perst_SelectByPlay(list,play_id);
}

/*
 * Function:    Schedule_Srv_StatRevByPlay
 * Function ID:	TTMS_SCU_Schedule_Srv_StatByPlay
 * Description: ���ݾ�ĿID��ͳ�������ʼ�Ʊ��������Ʊ��������
 * Input:       play_idΪ��ͳ�ƾ�Ŀ��ID�ţ�soldCountΪ������Ʊ����totalCountΪ��Ʊ��
 * Output:      ��
 * Return:      ����Ʊ������
 */
int Schedule_Srv_StatRevByPlay(int play_id, int *soldCount, int *totalCount) {
	schedule_list_t list,curPos;	
	int sum=0 , sold_temp, total_temp;

	*soldCount = *totalCount = 0;

	Schedule_Perst_SelectByPlay(list, play_id);
	
	List_ForEach(list, curPos)
	{
		sum+=Ticket_Srv_StatRevBySchID(curPos->data.id, &sold_temp, &total_temp);
		(*soldCount) += sold_temp;
		(*totalCount)+= total_temp;
	}
	return sum;
}

