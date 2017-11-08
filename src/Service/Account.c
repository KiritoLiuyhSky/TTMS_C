/*
 * account.c
 *
 *  Created on: 2016年6月16日
 *      Author: 04153009_程龙 
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

//创建系统初始化账号admin
void Account_Srv_InitSys()
{
	if(!Account_Perst_CheckAccFile() ){
		account_t data={ 0 , USR_ADMIN , "admin" ,"123456" };	
		data.id=EntKey_Srv_CompNewKey("user");	
		Account_Perst_Insert(&data);
	}
}
//验证登录账号是否已存在，存在，保存登录用户信息到全局变量gl_CurUser，return 1；否则return 0
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
//验证用户名是否存在，遍历list，若存在，返回对应结点指针；否则，返回null
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

//添加一个用户账号，通过调用Account_Perst_Insert(data)函数实现
inline int Account_Srv_Add(const account_t *data)
{
	return Account_Perst_Insert(data);
}

//修改一个用户账号，通过调用Account_Perst_Update(data)函数实现
inline int Account_Srv_Modify(const account_t *data)
{
	return Account_Perst_Update(data);
}

//删除一个用户账号，通过调用Account_Perst_DeleteByID(usrID)函数实现
inline int Account_Srv_DeleteByID(int usrID)
{
	return Account_Perst_DeleteByID(usrID);
}

//提取usrID对应的用户账号信息，通过调用Account_Perst_SelectByID(usrID, buf)函数实现
inline int Account_Srv_FetchByID(int usrID, account_t *buf)
{
	return Account_Perst_SelectByID(usrID, buf);
}

//提取usrName对应的用户账号信息，通过调用Account_Perst_SelByName(usrName, buf)函数实现
inline int Account_Srv_FetchByName(char usrName[], account_t *buf)
{
	return  Account_Perst_SelByName(usrName, buf);
}

//提取所有用户账号信息，保存到list链表中，通过调用Account_Perst_SelectAll(list)函数实现
inline int Account_Srv_FetchAll(account_list_t list)
{
	return Account_Perst_SelectAll(list);
}

