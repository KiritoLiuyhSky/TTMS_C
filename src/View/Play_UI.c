#include "Play_UI.h"
#include "Schedule_UI.h"

#include "../Common/List.h"
#include "../Service/Play.h"
#include "../Service/EntityKey.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Function:    Play_UI_ShowList
 * Function ID:	TTMS_SCU_Play_UI_Show
 * Description: 以列表模式显示剧目信息
 * Input:       list剧目信息链表的头指针，paging分页设置参数
 * Output:      所有剧目的信息
 * Return:      返回查找到的记录数目
 */
void Play_UI_ShowList(play_list_t list,Pagination_t paging)
{
	int i,j;
	char choice;
	play_node_t *pos;
	List_Init(list, play_node_t);
	paging.offset = 0;
	paging.pageSize = 2;


	//载入数据
	paging.totalRecords = Play_Srv_FetchAll(list);
	Paging_Locate_FirstPage(list, paging);
	do{
		system("cls");
		printf("\n\t\t\t==================================================================\n");
		printf("\t\t\t%*s",32,"剧目信息\n");
        printf("\t\t\t------------------------------------------------------------------\n");
		//显示数据
		Paging_ViewPage_ForEach(list, paging, play_node_t, pos, i){
			printf("\t\t\t剧目ID[%d]\n", pos->data.id);
			printf("\t\t\t剧目名称[%s]\n",pos->data.name);
			printf("\t\t\t剧目产地[%s]\n",pos->data.area);
			j=pos->data.type;
		switch(j){
			case 1:printf("\t\t\t剧目类型[%s]\n","电影类型");break;
			case 2:printf("\t\t\t剧目类型[%s]\n","歌剧类型");break;
			case 3:printf("\t\t\t剧目类型[%s]\n","音乐会类型");break;
		}
		j=pos->data.rating;
		switch(j){
			case 1:printf("\t\t\t剧目等级[%s]\n","儿童剧");break;
			case 2:printf("\t\t\t剧目等级[%s]\n","少年剧");break;
			case 3:printf("\t\t\t剧目等级[%s]\n","成人剧");break;
		}
		printf("\t\t\t剧目时长[%d]\n",pos->data.duration);
		printf("\t\t\t剧目上映日期[%d.%d.%d]\n",pos->data.start_date.year,pos->data.start_date.month,pos->data.start_date.day);
		printf("\t\t\t结束放映日期[%d.%d.%d]\n",pos->data.end_date.year,pos->data.end_date.month,pos->data.end_date.day);
		printf("\t\t\t票价[%d]\n",pos->data.price);
		//printf("\n");
		}

		printf("\t\t\t------- 共 %2d 项 ----------------------- 第 %2d/%2d 页 ----\n",paging.totalRecords, Pageing_CurPage(paging),Pageing_TotalPages(paging));

		printf("\t\t\t[P]上一页    [N]下一页    [R]返回");
		printf("\n\t\t\t=======================================================\n");
		printf("\t\t\t请选择:");
		fflush(stdin);
		scanf("%c", &choice);
		getchar();
		switch (choice)
		{
			case 'p':
			case 'P':
				if (!Pageing_IsFirstPage(paging))
				{
					paging.totalRecords = Play_Srv_FetchAll(list);
					Paging_Locate_OffsetPage(list, paging, -1, play_node_t);
				}
				break;
			case 'n':
			case 'N':
				if (!Pageing_IsLastPage(paging))
				{
					paging.totalRecords = Play_Srv_FetchAll(list);
					Paging_Locate_OffsetPage(list, paging, 1, play_node_t);
				}
				break;
		}
	} while (choice != 'r' && choice != 'R');
	//}
	List_Destroy(list, play_node_t);
}

/*
 * Function:    Play_UI_MgtEntry
 * Function ID:	TTMS_SCU_Play_UI_MgtEnt
 * Description: 剧目信息管理界面
 * Input:       flag为0，进入管理页面，flag为1进入查询界面
 * Output:      剧目管理界面
 * Return:      无
 */
void Play_UI_MgtEntry(int flag)
{
	int id/*,play_id*/;
	char choice;
	system("cls");
	play_list_t head;
	Pagination_t paging;

	List_Init(head, play_node_t);
	paging.offset = 0;
	paging.pageSize = 2;

	//载入数据
	paging.totalRecords = Play_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);

	if(flag == 0)
	{
		do
		{
			Play_UI_ShowList(head,paging);
			printf("\t\t\t ------- 共%2d 项----------------------- %2d/%2d 页 ----\n",paging.totalRecords, Pageing_CurPage(paging),Pageing_TotalPages(paging));

            printf("\n\t\t\t******************************************************************\n" );
			printf("\t\t\t[S]进入演出计划  [A]添加  [Q]查找  [D]删除   [U]修改   [R]返回  [p]上一页  [n]下一页");
            printf("\n\t\t\t==================================================================\n");
			printf("\t\t\t请选择:");
			//fflush(stdin);
			choice = getchar();
			switch (choice)
			{
			case 's':
			case 'S':
				Schedule_UI_MgtEntry(1);
				break;
			case 'a':
			case 'A':
				if (Play_UI_Add()) //新添加成功，跳到最后一页显示
				{
					paging.totalRecords = Play_Srv_FetchAll(head);
					Paging_Locate_LastPage(head, paging, play_node_t);
				}
				break;
			case 'd':
			case 'D':
				printf("\t\t\t请输入剧目ID:");
				scanf("%d", &id);
				getchar();
				if (Play_UI_Delete(id))
				{	//从新载入数据
					paging.totalRecords = Play_Srv_FetchAll(head);
					List_Paging(head, paging, play_node_t);
				}
				break;
			case 'u':
			case 'U':
				printf("\t\t\t请输入剧目ID:");
				scanf("%d", &id);
				if (Play_UI_Modify(id))
				{	//从新载入数据
					paging.totalRecords = Play_Srv_FetchAll(head);
					List_Paging(head, paging, play_node_t);
				}
				break;
			case 'p':
			case 'P':
				if (!Pageing_IsFirstPage(paging))
				{
					paging.totalRecords = Play_Srv_FetchAll(head);
					Paging_Locate_OffsetPage(head, paging, -1, play_node_t);
				}
				break;
			case 'n':
			case 'N':
				if (!Pageing_IsLastPage(paging))
				{
					paging.totalRecords = Play_Srv_FetchAll(head);
					Paging_Locate_OffsetPage(head, paging, 1, play_node_t);
				}
				break;
			case 'Q':
			case 'q':
				printf("\t\t\t请输入剧目ID:");
				scanf("%d", &id);
				if (Play_UI_Query(id))
				{	//从新载入数据
					paging.totalRecords = Play_Srv_FetchAll(head);
					List_Paging(head, paging, play_node_t);
				}
				break;
			/*case 's':
			case 'S':

				//翻页输出已有剧目信息
				Play_UI_ShowList(head,paging);
				break;*/
			}
		}while(choice != 'r' && choice != 'R');
		List_Destroy(head, play_node_t);
	}
	else{
		do{
			printf("\t\t\t------- 共 %2d 项 ----------------------- 第 %2d/%2d 页 ----\n",paging.totalRecords, Pageing_CurPage(paging),Pageing_TotalPages(paging));
			printf("\n\t\t\t******************************************************************\n");
			printf("\t\t\t[Q]查找  [R]返回   [S]查看");
			printf("\n\t\t\t==================================================================\n");
			printf("\t\t\t请选择:");
			fflush(stdin);
			scanf("%c", &choice);
			fflush(stdin);
			switch (choice)
			{
				case 'Q':
				case 'q':
					printf("\t\t\t请输入剧目ID:");
					scanf("%d", &id);
					if (Play_UI_Query(id))
					{	//从新载入数据
						paging.totalRecords = Play_Srv_FetchAll(head);
						List_Paging(head, paging, play_node_t);
					}
					break;
				case 's':
				case 'S':

					//翻页输出已有剧目信息
					Play_UI_ShowList(head,paging);
					break;
			}
		}while(choice != 'r' && choice != 'R');
	List_Destroy(head, play_node_t);
}
}

/*
 * Function:    Play_UI_Add
 * Function ID:	TTMS_SCU_Play_UI_Add
 * Description: 添加一条剧目信息
 * Input:       无
 * Output:      输入信息时的各种提示
 * Return:      添加的记录数
 */
 int Play_UI_Add(void)
{
	play_t data;
	char choice;
	int newRecCount = 0,type,rating;
	do{
	system("cls");
    printf("\n\t\t\t==================================================================\n");
		printf("\t\t\t%*s",32,"添加新的剧目信息\n");
        printf("\t\t\t------------------------------------------------------------------\n");
		printf("\t\t\t请输入剧目名称:");
		scanf("%s",data.name);
		printf("\t\t\t请输入剧目产地:");
		scanf("%s",data.area);
		printf("\t\t\t请输入剧目时长(以分钟为单位,输入数字):");
		scanf("%d",&data.duration);
		printf("\t\t\t[电影类型]输入1,[歌剧类型]输入2,[音乐会类型]输入3:");
		scanf("%d",&type);
		if(type==1 || type==2 || type==3)
		{
			data.type=(play_type_t)type;
		}
		else
		{
			while(type!=1 && type!=2 && type!=3)
			{	printf("\t\t\t输入有误,请重新输入!\n");
				printf("\t\t\t[电影类型]输入1,[歌剧类型]输入2,[音乐会类型]输入3:");
				scanf("%d",&type);
			}
			data.type=(play_type_t)type;
		}
		printf("\t\t\t[儿童剧]输入1,[少年剧]输入2,[成人剧]输入3:");
		scanf("%d",&rating);
		if(rating==1 || rating==2 || rating==3)
		{
			data.rating=(play_rating_t)rating;
		}
		else
		{
			while(rating!=1 && rating!=2 && rating!=3)
			{	printf("\t\t\t输入有误,请重新输入!");
				printf("\t\t\t[儿童剧]输入1,[少年剧]输入2,[成人剧]输入3:");
				scanf("%d",&rating);
			}
			data.rating=(play_rating_t)rating;
		}
		printf("\t\t\t请输入开始放映日期(如2014 7 2):");
		scanf("%d %d %d",&data.start_date.year,&data.start_date.month,&data.start_date.day);
		printf("\t\t\t请输入放映结束日期(如2014 7 2):");
		scanf("%d %d %d",&data.end_date.year,&data.end_date.month,&data.end_date.day);
		printf("\t\t\t请输入票价:");
		scanf("%d",&data.price);
		getchar();
        printf("\n\t\t\t==================================================================\n");

		//获取主键
		data.id = EntKey_Srv_CompNewKey("Play");

		if (Play_Srv_Add(&data)) {
			newRecCount += 1;
			printf("\t\t\t新剧目添加成功!\n");
		} else
			printf("\t\t\t新剧目添加失败!\n");
        printf("\t\t\t------------------------------------------------------------------\n");
		printf("\t\t\t[A]添加      [R]返回\n");
		fflush(stdin);
		scanf("%c", &choice);
		getchar();
	} while ('a' == choice || 'A' == choice);
	return newRecCount;

}

/*
 * Function:    Play_UI_Modify
 * Function ID:	TTMS_SCU_Play_UI_Mod
 * Description: 更新剧目信息
 * Input:       待更新的剧目ID号
 * Output:      输入信息时的各种提示
 * Return:      更新的剧目信息数，0表示未找到，1表示找到并更新
 */
 
 int Play_UI_Modify(int id)
{	int type,rating,i;
	play_t data;
	int rtn=0;
	char newname[31],newarea[9];
	int newduration,newprice,newSyear,newSmonth,newSday,newEyear,newEmonth,newEday,newrate,newtype;

	//Load record
	if (!Play_Srv_FetchByID(id, &data))
	{
		printf("\t\t\t剧目不存在,按确认键返回\n");
		getchar();
		return 0;
	}
    printf("\n\t\t\t==================================================================\n");
	printf("\t\t\t%*s",32,"修改剧目信息\n");
    printf("\t\t\t------------------------------------------------------------------\n");
	printf("\t\t\t剧目ID[%d]\n", data.id);
	printf("\t\t\t剧目名称[%s]\n", data.name);
	i=data.type;
	switch(i)
	{
		case 1:printf("\t\t\t剧目类型[%s]\n","电影类型");break;
		case 2:printf("\t\t\t剧目类型[%s]\n","歌剧类型");break;
		case 3:printf("\t\t\t剧目类型[%s]\n","音乐会类型");break;
	}
	printf("\t\t\t剧目出品地区[%s]\n",data.area);
	i=data.rating;
	switch(i)
	{
		case 1:printf("\t\t\t剧目等级[%s]\n","儿童剧");break;
		case 2:printf("\t\t\t目等级[%s]\n","少年剧");break;
		case 3:printf("\t\t\t剧目等级[%s]\n","成人剧");break;
	}
	printf("\t\t\t剧目时长[%d分钟]\n",data.duration);
	printf("\t\t\t剧目放映日期[%d.%d.%d]\n",data.start_date.year,data.start_date.month,data.start_date.day);
	printf("\t\t\t放映结束日期[%d.%d.%d]\n",data.end_date.year,data.end_date.month,data.end_date.day);
	printf("\t\t\t剧目票价[%d]\n",data.price);
    printf("\t\t\t------------------------------------------------------------------\n");
	fflush(stdin);

	if(Play_Srv_FetchByID(id,&data))
	{

		printf("\t\t\t请输入剧目名称:");
		scanf("%s",newname);
		printf("\t\t\t请输入剧目产地:");
		scanf("%s",newarea);
		printf("\t\t\t请输入剧目时长(以分钟为单位,输入数字):");
		scanf("%d",&newduration);
		printf("\t\t\t[电影类型]输入1,[歌剧类型]输入2,[音乐会类型]输入3:");
		scanf("%d",&newtype);
		if(newtype==1 || newtype==2 || newtype==3)
		{
			data.type=(play_type_t)newtype;
		}
		else
		{
			while(newtype!=1 && newtype!=2 && newtype!=3)
			{	printf("\t\t\t输入有误,请重新输入!:");
				printf("\t\t\t[电影类型]输入1,[歌剧类型]输入2,[音乐会类型]输入3:");
				scanf("%d",&newtype);
			}
			data.type=(play_type_t)newtype;
		}
		printf("\t\t\t[儿童剧]输入1,[少年剧]输入2,[成人剧]输入3:");
		scanf("%d",&newrate);
		if(newrate==1 || newrate==2 || newrate==3)
		{
			data.rating=(play_rating_t)newrate;
		}
		else
		{
			while(newrate!=1 && newrate!=2 && newrate!=3)
			{	printf("\t\t\t输入有误,请重新输入!\n");
				printf("\t\t\t[儿童剧]输入1,[少年剧]输入2,[成人剧]输入3:");
				scanf("%d",&newrate);
			}
			data.rating=(play_rating_t)newrate;
		}
		printf("\t\t\t请输入开始放映日期(如2014 7 2):");
		scanf("%d %d %d",&newSyear,&newSmonth,&newSday);
		printf("\t\t\t请输入放映结束日期(如2014 7 2):");
		scanf("%d %d %d",&newEyear,&newEmonth,&newEday);
		printf("\t\t\t请输入票价:");
		scanf("%d",&newprice);
		strcpy(data.name,newname);
		strcpy(data.area,newarea);
		data.duration=newduration;
		data.price=newprice;
		data.start_date.year=newSyear;
		data.start_date.month=newSmonth;
		data.start_date.day=newSday;
		data.end_date.year=newEyear;
		data.end_date.month=newEmonth;
		data.end_date.day=newEday;
	}
	else
	{
		//获取主键
		data.id = EntKey_Srv_CompNewKey("Play");

		printf("\t\t\t请输入剧目名称:");
		scanf("%s",newname);
		printf("\t\t\t请输入剧目产地:");
		scanf("%s",newarea);
		printf("\t\t\t请输入剧目时长(以分钟为单位,输入数字):");
		scanf("%d",&newduration);
		printf("\t\t\t电影类型输入1,歌剧类型输入2,音乐会类型输入3:");
		scanf("%d",&type);
		if(type==1 || type==2 || type==3)
		{
			data.type=type;
		}
		else
		{
			while(type!=1 && type!=2 && type!=3) {	
				printf("\t\t\t输入有误,请重新输入!\n");
				printf("\t\t\t电影类型输入1,歌剧类型输入2,音乐会类型输入3:");
				scanf("%d",&type);
			}
			data.type=type;
		}
		printf("\t\t\t儿童剧输入1,少年剧输入2,成人剧输入3:");
		scanf("%d",&rating);
		if(rating==1 || rating==2 || rating==3) {
			data.rating=rating;
		}
		else {
			while(rating!=1 && rating!=2 && rating!=3) {	
				printf("\t\t\t输入有误,请重新输入!\n");
				printf("\t\t\t儿童剧输入1,少年剧输入2,成人剧输入3:");
				scanf("%d",&rating);
			}
			data.rating=rating;
		}
		printf("\t\t\t请输入开始放映日期(如2014 7 2):");
		scanf("%d %d %d",&data.start_date.year,&data.start_date.month,&data.start_date.day);
		printf("\t\t\t请输入放映结束日期(如2014 7 2):");
		scanf("%d %d %d",&data.end_date.year,&data.end_date.month,&data.end_date.day);
		printf("\t\t\t请输入票价:");
		scanf("%d",&data.price);
        printf("\n\t\t\t==================================================================\n");
	}

    printf("\t\t\t------------------------------------------------------------------\n");

	if (Play_Srv_Modify(&data))
	{
		rtn = 1;
		printf("\t\t\t剧目修改成功!\n按确认键返回!\n");
	}
	else
	{
		printf("\t\t\t剧目修改失败!\n按确认键返回!\n");
	}
	getchar();
	return rtn;
}

/*
 * Function:    Play_UI_Delete
 * Function ID:	TTMS_SCU_Play_UI_Del
 * Description: 按照ID号删除剧目信息
 * Input:       待删除的剧目ID号
 * Output:      提示删除是否成功
 * Return:      0表示删除失败，1表示删除成功
 */
 
int Play_UI_Delete(int id)
{
	int rtn = 0;

	if (Play_Srv_DeleteByID(id))
	{
		printf("\t\t\t剧目成功删除!\n按确认键返回!\n");
		rtn=1;
	}
	else{
		printf("\t\t\t剧目删除失败!\n按确认键返回!\n");
	}
	getchar();
	return rtn;
}

/*
 * Function:    Play_UI_Query
 * Function ID:	TTMS_SCU_Play_UI_Qry
 * Description: 按照ID号查询剧目信息
 * Input:       待查找的剧目ID号
 * Output:      查找到的剧目信息
 * Return:      0表示未找到，1表示找到了
 */
 
 int Play_UI_Query(int id)
{
	play_t data;
	int rtn=0,i;
	if(Play_Srv_FetchByID(id,&data))
	{
		printf("\t\t\t剧目ID [%d]\n", data.id);
		printf("\t\t\t剧目名称 [%s]\n", data.name);
		i=data.type;
		switch(i)
		{
			case 1:printf("\t\t\t剧目类型[%s]\n","电影类型");break;
			case 2:printf("\t\t\t剧目类型[%s]\n","歌剧类型");break;
			case 3:printf("\t\t\t剧目类型[%s]\n","音乐会类型");break;
		}
		printf("\t\t\t剧目出品地区 [%s]\n",data.area);
		i=data.rating;
		switch(i)
		{
			case 1:printf("\t\t\t剧目等级[%s]\n","儿童剧");break;
			case 2:printf("\t\t\t剧目等级[%s]\n","少年剧");break;
			case 3:printf("\t\t\t剧目等级[%s]\n","成人剧");break;
		}
		printf("\t\t\t剧目时长 [%d]\n",data.duration);
		printf("\t\t\t剧目放映日期 [%d.%d.%d]\n",data.start_date.year,data.start_date.month,data.start_date.day);
		printf("\t\t\t放映结束日期 [%d.%d.%d]\n",data.end_date.year,data.end_date.month,data.end_date.day);
		rtn=1;
	}
	else{
		printf("\t\t\t没有找到该剧目的信息!\n");
	}
	printf("\t\t\t按任意键返回!");
	getchar();
	getchar();
	return rtn;
}
