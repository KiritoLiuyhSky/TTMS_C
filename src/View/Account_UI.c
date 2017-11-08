/*
*Copyright(C), 2016, Group 2 SE1501
*File name:Account
*File ID: TTMS_SCU_Account
*Author: 04153007_������
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
*Description:��¼��������ʾ�û������û��������룬��¼�ɹ�return 1��������ʾ���µ�¼������3�Σ���¼ʧ��
*Input: usrName, pwd
*Output:
*Return: 1 or 0
*/

/*�������룬����*��*/
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
            printf("\n\t\t\t�������������������:");
            strcpy(s," ");
            continue;
        }
        if(strlen(s) < min+1)
        {
            memset(s, 0, sizeof(s));
            printf("\n\t\t\t������̣�����������:");
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
	printf("\t\t\t   �������˺ţ�");
	scanf("%s",user);
	//printf(" ==\n");
	printf("\t\t\t   ���������룺");
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

//����ϵͳ�û�������ں�������ʾ�û��˺Ź���˵�
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

	//��������
	paging.totalRecords = Account_Srv_FetchAll(head);
	Paging_Locate_FirstPage(head, paging);

	do {
		system("cls");
		printf("\n\t\t\t==================================================================\n");
		printf("\t\t\t%*s",32,"�˺��б�\n");
		printf("\t\t\t%-10s  %-18s  %-18s\n", "ID", "�û���", "����");
		printf("\t\t\t------------------------------------------------------------------\n");
		//��ʾ����
		Paging_ViewPage_ForEach(head, paging, account_node_t, pos, i){
			printf("\t\t\t%-10d  %-18s  %-18s\n", pos->data.id,pos->data.username, pos->data.password);
		}

		printf("-\t\t\t------ �� %2d ��----------------------- �� %2d/%2d ҳ----\n",
				paging.totalRecords, Pageing_CurPage(paging),
				Pageing_TotalPages(paging));
		printf("\t\t\t******************************************************************\n");
		printf("\t\t\t[P]��һҳ | [N]��һҳ | [A]��� | [D]ɾ�� | [U]���� | [R]����");
		printf("\n\t\t\t==================================================================\n");
		printf("\t\t\t���빦��ǰ����ĸ��ѡ����:" );
		
		choice = getchar();
 		//scanf("%*c");
		//fflush(stdin);

		switch (choice) {
		case 'a':
		case 'A':
			if (Account_UI_Add(NULL)) //����ӳɹ����������һҳ��ʾ
			{
				paging.totalRecords = Account_Srv_FetchAll(head);
				Paging_Locate_LastPage(head, paging, account_node_t);
			}
			break;
		case 'u':
		case 'U':
			printf("\t\t\t����Ҫ�޸ĵ��û���:");
			scanf("%s",userName);
			if (Account_UI_Modify(NULL, userName))//������������
			{
				paging.totalRecords = Account_Srv_FetchAll(head);
				List_Paging(head, paging, account_node_t);
			}
			break;
		case 'd':
		case 'D':
			printf("\t\t\t����Ҫɾ�����û���:");
			scanf("%s",userName);
			getchar();
			if (Account_UI_Delete(NULL, userName))//������������
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
	//�ͷ�����ռ�
	List_Destroy(head, account_node_t);}


//���һ���û��˺���Ϣ������˺������ڣ���ʾ������Ϣ
int Account_UI_Add(account_list_t list ) {
	int type;

	account_t data;

	do {
		printf("\n\t\t\t==================================================================\n");
		printf("\t\t\t%32s\n","������û�");
		printf("\t\t\t------------------------------------------------------------------\n");
		fflush(stdin);
		printf("\t\t\t[0]����       [1]����Ա       [2]����        [9]ϵͳ����Ա\n");
		printf("\t\t\t==================================================================\n");
		printf("\t\t\t��ѡ���˺����ͣ�");
		scanf("%d",&type);
		getchar();
	}
	while(type!=0 && type!=1 && type!=2 && type!=9);
	data.type = type;
	data.id = EntKey_Srv_CompNewKey("Account");
	printf("\t\t\t�������˺ţ�");
	scanf("%s",data.username);
	getchar();
	printf("\t\t\t���������룺");
	scanf("%s",data.password);
	getchar();
	if(Account_Srv_Add(&data))
	{
		printf("\t\t\t��ӳɹ�!\n");
		getchar();
		return 1;
	}
	else {
		printf("\t\t\t���ʧ��!\n");
		getchar();
	}
	return 0;
}

//�����û��˺����޸��û��˺����룬����������û��˺�������ʾ������Ϣ
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
		printf("\t\t\t���˺Ų�����\n");
		return 0;
	}
	printf("\t\t\t�����������룺");
	scanf("%s",data->data.password);
	getchar();
	Account_Srv_Modify(&(data->data));
	printf("\t\t\t�޸ĳɹ�\n");

	return 1;
}

//�����û��˺���ɾ��һ���Ѿ����ڵ��û��˺���Ϣ���������������û��˺�������ʾ������Ϣ
int Account_UI_Delete(account_list_t list ,char usrName[]) {
	account_t buf;
	if(Account_Srv_FetchByName(usrName, &buf))
	{
		Account_Srv_DeleteByID(buf.id);
		printf("\t\t\tɾ���ɹ�\n");
		return 1;
	}
	printf("\t\t\t���˺Ų�����\n");
       	return  0;
}

//�����û��˺������Ҹ��û��˺��Ƿ���ڣ����ڷ���1�����򷵻�0������ʾ������Ϣ
int Account_UI_Query(account_list_t  list ,char usrName[]) {
	account_t buf;
	if(Account_Srv_FetchByName(usrName, &buf)){
		return 1;
	}
	printf("\t\t\t���˺Ų�����\n");

	return  0;
}
