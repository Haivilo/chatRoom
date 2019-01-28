

#include "stdafx.h"
#include "chatRoom.h"
#include "dmchat.h"
#include "afxdialogex.h"
#include"md5.h"


IMPLEMENT_DYNAMIC(dmchat, CDialogEx)

dmchat::dmchat(CWnd* pParent /*=NULL*/)
	: CDialogEx(dmchat::IDD, pParent)
	, up(_T(""))
	, down(_T(""))
{

}

dmchat::~dmchat()
{
}

void dmchat::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, up);
	DDX_Text(pDX, IDC_EDIT2, down);
}


BEGIN_MESSAGE_MAP(dmchat, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &dmchat::OnBnClickedButton1)
END_MESSAGE_MAP()


// dmchat 消息处理程序


void dmchat::OnBnClickedButton1()
{
	UpdateData(1);
	_Message mMes = {};
	mMes.MessageType = 3;//3 dm
	std::string msg = CW2A(user) + " : ";//add its user name
	msg += CW2A(down);//add message
	unsigned char* codedmsg = (unsigned char*)msg.c_str();
	std::string haha = base64_encode(codedmsg, strlen((char*)codedmsg));//encrypt
	strcpy_s(mMes.UserName, strlen(CW2A(user)) + 1, CW2A(user));
	strcpy_s(mMes.TargetName, strlen(CW2A(target)) + 1, CW2A(target));
	strcpy_s(mMes.MessageBuf, strlen(haha.c_str()) + 1, (haha.c_str()));
	//send to message
	if (strlen(mMes.UserName) == 0 || strlen(mMes.TargetName) == 0 || strlen(mMes.MessageBuf) == 0)
	{//non empty
		MessageBox(0, L"cannot be left empty");
		return;
	}

	sock->dosend(&mMes);

}
