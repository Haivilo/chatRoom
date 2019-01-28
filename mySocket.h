#pragma once
#include<Ws2tcpip.h>

enum {
	pythonmessage = WM_USER + 1,
	WM_register,
	WM_login,
	WM_chat,
	WM_groupchat,
	sendFriend,
	addGroup,
	receiveFriend,
	joinGroup
};
typedef struct _Message
{
	//#消息类型   
	//#register=1 
	//#log in=2  
	//#dm=3 
	//#grp chat =4 
	//#add contact= 5
	//#add grp chat=6
	int  MessageType;
	char UserName[20];
	char TargetName[33];
	//if #register  targetname psw
	//if #log in  targetname psw
	//if #dm  targetname friend's name 
	//if #grp chat  targetname chatbox name 
	//if #add contact  targetname : target name


	char MessageBuf[1024];
}*pMessage;

class mySocket
{
public:
	static SOCKET m_sendSock;
	int  Dlghwnd = 0;//save the handle to send
	void doconnet(int hand);
	void dosend(pMessage pMes);
	mySocket();
	~mySocket();
};

