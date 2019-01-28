#include "stdafx.h"
#include "mySocket.h"


mySocket::mySocket()
{
}
SOCKET mySocket::m_sendSock;
DWORD WINAPI RecvMessageThreadProc(
	_In_ LPVOID lpParameter
	){
	while (1){
		mySocket* Sock = (mySocket*)lpParameter;//get self socket
		pMessage pMess = (pMessage) new char[sizeof(_Message)]{};
		recv(Sock->m_sendSock, (char*)pMess, sizeof(_Message), 0);//receive
		switch (pMess->MessageType)
		{//根据不同消息传
		case 1:SendMessage((HWND)Sock->Dlghwnd, WM_register, (DWORD)pMess, 0); break;//register
		case 2:SendMessage((HWND)Sock->Dlghwnd, WM_login, (DWORD)pMess, 0); break;//log in
		case 3:SendMessage((HWND)Sock->Dlghwnd, WM_chat, (DWORD)pMess, 0); break;//dm
		case 4:SendMessage((HWND)Sock->Dlghwnd, WM_groupchat, (DWORD)pMess, 0); break;//grp chat
		case 5:SendMessage((HWND)Sock->Dlghwnd, sendFriend, (DWORD)pMess, 0); break;//add friend
		case 6:SendMessage((HWND)Sock->Dlghwnd, addGroup, (DWORD)pMess, 0); break;//add grp
		case 7:SendMessage((HWND)Sock->Dlghwnd, receiveFriend, (DWORD)pMess, 0); break;//receive add msg
		case 8:SendMessage((HWND)Sock->Dlghwnd, joinGroup, (DWORD)pMess, 0); break;//create grp

		default:
			break;
		}
		//SendMessage((HWND)Sock->Dlghwnd, 0x410, (DWORD)pMess, 0);
		//cout << pMess->MessageType << endl << pMess->UserName << endl << pMess->TargetName << endl << pMess->MessageBuf << endl;
	}
}
void mySocket::doconnet(int hand){
	//connect, trigger multithread
	Dlghwnd = hand;
	WSAData WsaData = { 0 };
	int RetValue = WSAStartup(MAKEWORD(2, 2), &WsaData);
	m_sendSock = socket(
		AF_INET,			
		SOCK_STREAM,		// (SOCK_DGRAM)
		IPPROTO_TCP			// (IPPROTO_UDP)
		);
	if (m_sendSock == INVALID_SOCKET)
	{
		printf("socket failed!\n");
		system("pause");
	}
	sockaddr_in ServerAddr = { 0 };			// server
	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(12345);	
	// ServerAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	inet_pton(AF_INET, "127.0.0.1", (LPVOID)&ServerAddr.sin_addr.S_un);
	RetValue = connect(
		m_sendSock,			// 
		(sockaddr*)&ServerAddr,	// 
		sizeof(sockaddr_in)		// size of struct
		);
	CreateThread(0, 0, RecvMessageThreadProc, this, 0, 0);
}

void mySocket::dosend(pMessage pMes){//send
	send(m_sendSock, (char*)pMes, sizeof(_Message), 0);
}

mySocket::~mySocket()
{
}
