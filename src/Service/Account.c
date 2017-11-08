/*
 * account.c
 *
 *  Created on: 2016��6��16��
 *      Author: 04153009_���� 
 */
#include "Account.h"
#include "EntityKey.h"
#include "../Common/list.h"
#include "../Persistence/Account_Persist.h"
#include <string.h>
#include <stdlib.h>

extern account_t gl_CurUser;
int Account_Srv_Logout(){
	account_t data={0, USR_ANOMY, "Anonymous",""};
	gl_CurUser= data;
	if(gl_CurUser.type==USR_ANOMY){
		return 1;
	}
	else{
		return 0;
	}
}

//����ϵͳ��ʼ���˺�admin
void Account_Srv_InitSys()
{
	if(!Account_Perst_CheckAccFile() ){
		account_t data={ 0 , USR_ADMIN , "admin" ,"123456" };	
		data.id=EntKey_Srv_CompNewKey("user");	
		Account_Perst_Insert(&data);
	}
}
//��֤��¼�˺��Ƿ��Ѵ��ڣ����ڣ������¼�û���Ϣ��ȫ�ֱ���gl_CurUser��return 1������return 0
inline int Account_Srv_Verify(char usrName[], char pwd[])
{
	account_t buf;
	if(!Account_Perst_SelByName(usrName, &buf))
	{
		return 0;
	}
	if(strcmp(pwd,buf.password)!=0)
	{
		return 0;
	}
	return 1;
}
//��֤�û����Ƿ���ڣ�����list�������ڣ����ض�Ӧ���ָ�룻���򣬷���null
account_node_t * Account_Srv_FindByUsrName(account_list_t list, char usrName[]) 
{
	account_list_t curPos;
	List_ForEach(list,curPos)
	{
		if(strcmp(curPos->data.username,usrName)==0)
		{
			return curPos;
		}
	}
	return NULL ;																								
}

//���һ���û��˺ţ�ͨ������Account_Perst_Insert(data)����ʵ��
inline int Account_Srv_Add(const account_t *data)
{
	return Account_Perst_Insert(data);
}

//�޸�һ���û��˺ţ�ͨ������Account_Perst_Update(data)����ʵ��
inline int Account_Srv_Modify(const account_t *data)
{
	return Account_Perst_Update(data);
}

//ɾ��һ���û��˺ţ�ͨ������Account_Perst_DeleteByID(usrID)����ʵ��
inline int Account_Srv_DeleteByID(int usrID)
{
	return Account_Perst_DeleteByID(usrID);
}

//��ȡusrID��Ӧ���û��˺���Ϣ��ͨ������Account_Perst_SelectByID(usrID, buf)����ʵ��
inline int Account_Srv_FetchByID(int usrID, account_t *buf)
{
	return Account_Perst_SelectByID(usrID, buf);
}

//��ȡusrName��Ӧ���û��˺���Ϣ��ͨ������Account_Perst_SelByName(usrName, buf)����ʵ��
inline int Account_Srv_FetchByName(char usrName[], account_t *buf)
{
	return  Account_Perst_SelByName(usrName, buf);
}

//��ȡ�����û��˺���Ϣ�����浽list�����У�ͨ������Account_Perst_SelectAll(list)����ʵ��
inline int Account_Srv_FetchAll(account_list_t list)
{
	return Account_Perst_SelectAll(list);
}

