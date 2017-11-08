/*
 * salesanalysis.c
 *
 *  Created on: 2016��6��20��
 *      Author: ���� 
 */
#include <string.h>
#include <stdio.h>
#include "SalesAnalysis.h"
#include "../Persistence/Sale_Persist.h"
#include "../Persistence/SalesAnalysis_Persist.h"
#include "Sale.h"
#include "Ticket.h"
#include "Play.h"
#include "Schedule.h"
#include "../Common/list.h"

//����Ա��usrID�ڸ���ʱ����������۶�������۶�
inline int Salesanalysis_Srv_CompSaleVal(int usrID, user_date_t stDate, user_date_t endDate)
{
	int amount=0;
	sale_list_t list,pSale;
	Sale_Perst_SelectByUsrID(list,usrID,stDate,endDate);
	pSale=list->next;
	while(pSale!=list)
	{
		amount+=pSale->data.value;
		pSale=pSale->next;
	}
	return amount;
}

//��Ͼ�ĿPlay.dat,ͳ���������ݣ��������۷���salesanalysis_list_t list��������list������
int Salesanalysis_Srv_StaticSale(salesanalysis_list_t list) 
{
	salesanalysis_list_t pre;
	pre=list->next;
	while(pre!=NULL)
	{
		printf("succeed");
		pre=pre->next;
	}
	return 1;
}


//�Ծ�Ժ��ӰƱ������
void SalesAnalysis_Srv_SortBySale(salesanalysis_list_t list)
{
	salesanalysis_list_t temp=NULL ,buf=list->next,flag;
	List_Init(temp,salesanalysis_node_t);
	while(buf!=list)
	{
		if(temp->next == temp)
		{
			List_AddHead(temp,buf);
			flag=temp->next;
		
		}
		else
		{
			while(flag->data.sales > buf->data.sales)
			{
				flag=flag->next;
			}
			List_InsertBefore(flag,buf);
		}
		buf=list->next;
	}
	list->next = temp->next;
	temp->next->prev = list;
}


