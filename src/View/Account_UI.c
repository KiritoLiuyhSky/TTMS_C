/*
*Copyright(C), 2016, Group 2 SE1501
*File name:Account
*File ID: TTMS_SCU_Account
*Author: 04153007_梁嘉龙
*Version: 0.1
*Date: 2016.06.17
*Modify log:
	2016.06.17 Add code
*/
#include "Account_UI.h"
#include "../Common/list.h"
#include "../Service/Account.h"
#include "../Service/EntityKey.h"
#include "Account_UI.h"
#include<windows.h>
#include <stdio.h>
#include<assert.h>
#include<string.h>
#include <unistd.h>
#include <sys/time.h>

/*
*Function: SysLogin
*Function ID: TTMS_SCU_Login
*Description:登录函数，提示用户输入用户名和密码，登录成功return 1，否则提示重新登录，超过3次，登录失败
*Input: usrName, pwd
*Output:
*Return: 1 or 0
*/

/*输入密码，回显*键*/
void inputPasswd(char passwd[], int min, int max)
{
    char s[20];
    char temp[2];
    int n;
    strcpy(s," ");
    while(1)
    {
        while(1)
        {
            n=getch();
            if(n == 13)
            {
                break;
            }
            printf("*");
            memset(temp,0,sizeof(temp));
            sprintf(temp,"%c",n);
            strcat(s,temp);
        }
        if(strlen(s) > max+1)
        {
            memset(s, 0, sizeof(s));
            printf("\n\t\t\t密码过长，请重新输入:");
            strcpy(s," ");
            continue;
        }
        if(strlen(s) < min+1)
        {
            memset(s, 0, sizeof(s));
            printf("\n\t\t\t密码过短，请重新输入:");
            strcpy(s," ");
            continue;
        }
        break;
    }
    printf("\n");
    strcpy(passwd, s+1);
}


int SysLogin() {
	char user[30],pass[30];
	printf("\t\t\t========================\n");
	printf("\n");
	printf("\t\t\t   请输入账号：");
	scanf("%s",user);
	//printf(" ==\n");
	printf("\t\t\t   请输入密码：");
	inputPasswd(pass,0,20);
	printf("\n");
	//printf(" ==\n");
	printf("\t\t\t========================\n");
	getchar();
	if(Account_Srv_Verify(user,pass))
	{
		return 1;
	}
    return 0;
}

//管理系统用户功能入口函数，显示用户账号管理菜单
void Account_UI_MgtEntry() {
	int i;
	char userName[30];
	char choice;

	account_list_t head;
	account_node_t *pos;
	Pagination_t paging;

	List_Init(head, account_node_t);
	paging.offset = 0;
	paging.pageSize = ACCOUNT_PAGE_SIZE;

	//载入数据
	paging.totalRecords = Account_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);

	do {
		system("cls");
		printf("\n\t\t\t==================================================================\n");
		printf("\t\t\t%*s",32,"账号列表\n");
		printf("\t\t\t%-10s  %-18s  %-18s\n", "ID", "用户名", "密码");
		printf("\t\t\t------------------------------------------------------------------\n");
		//显示数据
		Paging_ViewPage_ForEach(head, paging, account_node_t, pos, i){
			printf("\t\t\t%-10d  %-18s  %-18s\n", pos->data.id,pos->data.username, pos->data.password);
		}

		printf("-\t\t\t------ 共 %2d 项----------------------- 第 %2d/%2d 页----\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf("\t\t\t******************************************************************\n");
		printf("\t\t\t[P]上一页 | [N]下一页 | [A]添加 | [D]删除 | [U]更新 | [R]返回");
		printf("\n\t\t\t==================================================================\n");
		printf("\t\t\t输入功能前的字母，选择功能:" );
		
		choice = getchar();
 		//scanf("%*c");
		//fflush(stdin);

		switch (choice) {
		case 'a':
		case 'A':
			if (Account_UI_Add(NULL)) //新添加成功，跳到最后一页显示
			{
				paging.totalRecords = Account_Srv_FetchAll(head);
				Paging_Locate_LastPage(head, paging, account_node_t);
			}
			break;
		case 'u':
		case 'U':
			printf("\t\t\t输入要修改的用户名:");
			scanf("%s",userName);
			if (Account_UI_Modify(NULL, userName))//从新载入数据
			{
				paging.totalRecords = Account_Srv_FetchAll(head);
				List_Paging(head, paging, account_node_t);
			}
			break;
		case 'd':
		case 'D':
			printf("\t\t\t输入要删除的用户名:");
			scanf("%s",userName);
			getchar();
			if (Account_UI_Delete(NULL, userName))//从新载入数据
			{
				paging.totalRecords = Account_Srv_FetchAll(head);
				List_Paging(head, paging, account_node_t);
			}
			break;
		case 's':
		case 'S':
			/*printf("Input the room ID:");
			scanf("%d", &id);
			getchar();
			Seat_UI_MgtEntry(id);
			paging.totalRecords = Account_Srv_FetchAll(head);
			List_Paging(head, paging, account_node_t);
			*/break;
		case 'p':
		case 'P':
			if (!Pageing_IsFirstPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, -1, account_node_t);
			}
			break;
		case 'n':
		case 'N':
			if (!Pageing_IsLastPage(paging)) {
				Paging_Locate_OffsetPage(head, paging, 1, account_node_t);
			}
			break;
		}
	} while (choice != 'r' && choice != 'R');
	//释放链表空间
	List_Destroy(head, account_node_t);}


//添加一个用户账号信息，如果账号名存在，提示出错信息
int Account_UI_Add(account_list_t list ) {
	int type;

	account_t data;

	do {
		printf("\n\t\t\t==================================================================\n");
		printf("\t\t\t%32s\n","添加新用户");
		printf("\t\t\t------------------------------------------------------------------\n");
		fflush(stdin);
		printf("\t\t\t[0]匿名       [1]销售员       [2]经理        [9]系统管理员\n");
		printf("\t\t\t==================================================================\n");
		printf("\t\t\t请选择账号类型：");
		scanf("%d",&type);
		getchar();
	}
	while(type!=0 && type!=1 && type!=2 && type!=9);
	data.type = type;
	data.id = EntKey_Srv_CompNewKey("Account");
	printf("\t\t\t请输入账号：");
	scanf("%s",data.username);
	getchar();
	printf("\t\t\t请输入密码：");
	scanf("%s",data.password);
	getchar();
	if(Account_Srv_Add(&data))
	{
		printf("\t\t\t添加成功!\n");
		getchar();
		return 1;
	}
	else {
		printf("\t\t\t添加失败!\n");
		getchar();
	}
	return 0;
}

//根据用户账号名修改用户账号密码，不存在这个用户账号名，提示出错信息
int Account_UI_Modify(account_list_t list ,char usrName[]) {
	account_list_t head;
	account_list_t data = (account_list_t)malloc(sizeof(account_node_t));
	//puts("123");


	List_Init(head, account_node_t);
	Account_Srv_FetchAll(head);
	data=Account_Srv_FindByUsrName(head, usrName);
	//puts("456");
	if(data == NULL)
	{
		printf("\t\t\t该账号不存在\n");
		return 0;
	}
	printf("\t\t\t请输入新密码：");
	scanf("%s",data->data.password);
	getchar();
	Account_Srv_Modify(&(data->data));
	printf("\t\t\t修改成功\n");

	return 1;
}

//根据用户账号名删除一个已经存在的用户账号信息，如果不存在这个用户账号名，提示出错信息
int Account_UI_Delete(account_list_t list ,char usrName[]) {
	account_t buf;
	if(Account_Srv_FetchByName(usrName, &buf))
	{
		Account_Srv_DeleteByID(buf.id);
		printf("\t\t\t删除成功\n");
		return 1;
	}
	printf("\t\t\t该账号不存在\n");
       	return  0;
}

//根据用户账号名查找该用户账号是否存在，存在返回1，否则返回0，并提示错误信息
int Account_UI_Query(account_list_t  list ,char usrName[]) {
	account_t buf;
	if(Account_Srv_FetchByName(usrName, &buf)){
		return 1;
	}
	printf("\t\t\t该账号不存在\n");

	return  0;
}
