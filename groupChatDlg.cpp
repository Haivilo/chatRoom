// groupChatDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "chatRoom.h"
#include "groupChatDlg.h"
#include "afxdialogex.h"
#include "md5.h"



IMPLEMENT_DYNAMIC(groupChatDlg, CDialogEx)

groupChatDlg::groupChatDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(groupChatDlg::IDD, pParent)
	, up(_T(""))
	, down(_T(""))
{

}

groupChatDlg::~groupChatDlg()
{
}

void groupChatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, up);
	DDX_Text(pDX, IDC_EDIT2, down);
}


BEGIN_MESSAGE_MAP(groupChatDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &groupChatDlg::OnBnClickedButton1)
END_MESSAGE_MAP()




void groupChatDlg::OnBnClickedButton1()
{
	//std::string msg = CW2A(user) + " : ";
	//msg += CW2A(down);
	//unsigned char* codedmsg = (unsigned char*)msg.c_str();
 	_Message mMes = {};

	mMes.MessageType = 4;//4 is grp chat
	std::string msg = CW2A(user) + " : ";
	msg += CW2A(down);//add name and msg
	unsigned char* codedmsg = (unsigned char*)msg.c_str();
	std::string haha = base64_encode(codedmsg, strlen((char*)codedmsg));//encrypt
	strcpy_s(mMes.UserName, strlen(CW2A(user)) + 1, CW2A(user));//
	strcpy_s(mMes.TargetName, strlen(CW2A(grpname)) + 1, CW2A(grpname));
	strcpy_s(mMes.MessageBuf, strlen(haha.c_str()) + 1, haha.c_str());
	//copy msg
	if (strlen(mMes.UserName) == 0 || strlen(mMes.TargetName) == 0 || down.GetLength()== 0)
	{//not null 
		MessageBox(0, L"can't be empty");
		return;
	}

	sock->dosend(&mMes);
	// TODO:  在此添加控件通知处理程序代码
}
