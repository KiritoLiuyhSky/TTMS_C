/*
 * salesanalysis.c
 *
 *  Created on: 2016年6月20日
 *      Author: 程龙 
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

//计算员工usrID在给定时间区间的销售额，返回销售额
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

//结合剧目Play.dat,统计销售数据，构建销售分析salesanalysis_list_t list链表，返回list链表长度
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


//对剧院电影票房排序
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


