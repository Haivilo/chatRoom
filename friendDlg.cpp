

#include "stdafx.h"
#include "chatRoom.h"
#include "friendDlg.h"
#include "afxdialogex.h"
#include "md5.h"


IMPLEMENT_DYNAMIC(friendDlg, CDialogEx)

friendDlg::friendDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(friendDlg::IDD, pParent)
	, promptEdit(_T(""))
{

}

friendDlg::~friendDlg()
{
}

void friendDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, treedlg);
	DDX_Text(pDX, IDC_EDIT1, promptEdit);
}


BEGIN_MESSAGE_MAP(friendDlg, CDialogEx)
	ON_MESSAGE(pythonmessage, &friendDlg::OnPythonmessage)
	ON_BN_CLICKED(IDC_BUTTON3, &friendDlg::OnBnClickedButton3)
	ON_MESSAGE(WM_login, &friendDlg::OnLogin)
	ON_MESSAGE(WM_register, &friendDlg::OnRegister)
	ON_MESSAGE(WM_chat, &friendDlg::OnChat)
	ON_MESSAGE(WM_groupchat, &friendDlg::OnGroupchat)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE1, &friendDlg::OnDblclkTree1)
	ON_WM_CLOSE()
	ON_MESSAGE(sendFriend, &friendDlg::OnSendfriend)
	ON_MESSAGE(receiveFriend, &friendDlg::OnReceivefriend)
	ON_MESSAGE(addGroup, &friendDlg::OnAddgroup)
	ON_BN_CLICKED(IDC_BUTTON4, &friendDlg::OnBnClickedButton4)
	ON_MESSAGE(joinGroup, &friendDlg::OnJoingroup)
	ON_BN_CLICKED(IDC_BUTTON5, &friendDlg::OnBnClickedButton5)
END_MESSAGE_MAP()




afx_msg LRESULT friendDlg::OnPythonmessage(WPARAM wParam, LPARAM lParam)
{
	return 0;
}


void friendDlg::OnBnClickedButton3()
{//create grp
	_Message mMes = {};

	formatMessage(mMes, 6);//6create grp
	if (strlen(mMes.UserName) == 0 || strlen(mMes.TargetName) == 0)
	{//non empty
		MessageBox(L"target can't be empty");
	}
	else
	{//send
		sock->dosend(&mMes);
	}
}


afx_msg LRESULT friendDlg::OnLogin(WPARAM wParam, LPARAM lParam)
{
	
	pMessage mess = (pMessage)wParam;
	if (strcmp(mess->MessageBuf, "already logged in") && strcmp(mess->MessageBuf, "wrong password") && strcmp(mess->MessageBuf, "no existing account")){
		CStringA temp = mess->MessageBuf;//if success log in
		
		
		int index = temp.Find('!');
		CStringA friends = temp.Left(index);//friend list
		CStringA groups = temp.Right(temp.GetLength() - index - 1);//grp list

		while (friends.GetLength())
		{//split the buffer, add to friend ls
			index = friends.Find('|');
			CStringA fr = friends.Left(index);
			friends = friends.Right(friends.GetLength() - index - 1);
			treedlg.InsertItem(CA2W(fr), HtreeFriend);
		}
		while (groups.GetLength())
		{//split the buffer, add to grp ls
			index = groups.Find('|');
			CStringA grp = groups.Left(index);
			treedlg.InsertItem(CA2W(grp), HtreeGroup);
			//grpvc.push_back(grp);
			groups = groups.Right(groups.GetLength() - index - 1);
		}
		SetWindowText(CA2W(mess->UserName));//put on self name
		ShowWindow(5);//show自己
		::ShowWindow(AfxGetMainWnd()->m_hWnd, 0);//hide log on pg
	}
	else
	{
		MessageBox(CA2W(mess->MessageBuf));//non success
	}
	delete mess;
	return 0;
}


afx_msg LRESULT friendDlg::OnRegister(WPARAM wParam, LPARAM lParam)
{
	pMessage mess = (pMessage)wParam;
	MessageBox(CA2W(mess->MessageBuf));//send msg back directly
	delete mess;
	return 0;
}

CDialogEx* friendDlg::describeDlg(TCHAR myself[20], TCHAR target[20], CString content, DWORD id, bool frdlg){
	//used while creating chat window
	if (frdlg)
	{
		dmchat* obj = new dmchat;
		obj->Create(id, this);
		obj->SetWindowText(target);
		obj->up += content + L"\r\n";//refresh msg
		obj->user = myself;//refresh vars
		obj->target = target;
		obj->UpdateData(0);
		obj->ShowWindow(5);

		return obj;
	}
	else
	{
		groupChatDlg* obj = new groupChatDlg;
		obj->Create(id, this);
		obj->SetWindowText(target);
		obj->up += content + L"\r\n";
		obj->user = myself;
		obj->grpname = target;
		obj->UpdateData(0);
		obj->ShowWindow(5);
		return obj;
	}
}

afx_msg LRESULT friendDlg::OnChat(WPARAM wParam, LPARAM lParam)
{//dm chat
	pMessage mess = (pMessage)wParam;
	int index = FindChatWindows(mess->UserName);
	string str = mess->MessageBuf;
	str = base64_decode(str);//decrypt
	CString hh = CA2W(str.c_str());
	if (index != -1)
	{//window already exist
		if (hh.GetLength() == 0){
			MessageBox(L"friend is not online");
		}//if null, not online
		else
		{
			((dmchat *)DlgVec[index].Dlgobj)->up += hh + L"\r\n";
			DlgVec[index].Dlgobj->UpdateData(0);
			DlgVec[index].Dlgobj->ShowWindow(5);
		//on specified window
		}
		
	}
	else
	{

		//new window
		INFO info = {};
		strcpy_s(info.TargetName, strlen(mess->UserName) + 1, mess->UserName);
		info.Dlgobj = describeDlg(CA2W(mess->TargetName), CA2W(mess->UserName), hh, IDD_DIALOG2);
		DlgVec.push_back(info);//push into vector，save name

	}
	delete mess;
	return 0;

}


afx_msg LRESULT friendDlg::OnGroupchat(WPARAM wParam, LPARAM lParam)
{//grp msg
	pMessage mess = (pMessage)wParam;
	int index = FindChatWindows(mess->TargetName);
	string str = mess->MessageBuf;
	str = base64_decode(str);
	CString hh = CA2W(str.c_str());//decrypt
	if (index != -1)
	{//window already exist
		((groupChatDlg *)DlgVec[index].Dlgobj)->up += hh + L"\r\n";
		DlgVec[index].Dlgobj->UpdateData(0);
		DlgVec[index].Dlgobj->ShowWindow(5);

	
	}
	else
	{//create

		//groupChatDlg* obj = new groupChatDlg;
		//obj->Create(IDD_DIALOG2, this);
		//obj->SetWindowText(CA2W(mess->TargetName));
		//obj->up += hh + L"\r\n";
		//obj->user = CA2W(mess->UserName);
		//obj->grpname = CA2W(mess->TargetName);
		//obj->UpdateData(0);
		//obj->ShowWindow(5);
		INFO info = {};
		strcpy_s(info.TargetName, strlen(mess->TargetName) + 1, mess->TargetName);
		info.Dlgobj = info.Dlgobj = describeDlg(CA2W(mess->UserName), CA2W(mess->TargetName), hh, IDD_DIALOG3,0);
		DlgVec.push_back(info);
	}
	delete mess;
	return 0;
	
}


BOOL friendDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	HtreeFriend = treedlg.InsertItem(L"friend", TVI_ROOT);
	HtreeGroup = treedlg.InsertItem(L"groups", TVI_ROOT);


	return TRUE;  // return TRUE unless you set the focus to a control

}


void friendDlg::OnDblclkTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	
	HTREEITEM  htree = treedlg.GetSelectedItem();
	CString str = treedlg.GetItemText(htree);

	if (str.Compare(L"friend") && str.Compare(L"groups"))
	{//not parent
		int index = FindChatWindows(CW2A(str));
		if (index != -1)
		{
			DlgVec[index].Dlgobj->ShowWindow(5);
		}
		else if ((int)treedlg.GetParentItem(htree) == (int)HtreeGroup)
		{//parent is group
			groupChatDlg* obj = new groupChatDlg;
			obj->Create(IDD_DIALOG3, this);
			GetWindowTextW(obj->user);
			obj->SetWindowTextW(str);//used for sending msg afterwars, get a place  to store string
			obj->grpname = str;
			obj->ShowWindow(5);
			//create and display 
			INFO info = {};
			strcpy_s(info.TargetName, strlen(CW2A(str)) + 1, CW2A(str));
			info.Dlgobj = obj;
			DlgVec.push_back(info);
			//push to vector
			//below is same 
		}
		else
		{//parent is chat
			dmchat* obj = new dmchat;
			obj->Create(IDD_DIALOG2, this);
			GetWindowTextW(obj->user);
			obj->SetWindowTextW(str);
			obj->ShowWindow(5);
			obj->target = str;
			INFO info = {};
			strcpy_s(info.TargetName, strlen(CW2A(str)) + 1, CW2A(str));
			info.Dlgobj = obj;
			DlgVec.push_back(info);
		}
	}
	*pResult = 0;
}
int friendDlg::FindChatWindows(char*UserName)
{//if found : return index，else  -1
	int index = 0;
	for (int i = 0; i < DlgVec.size(); i++)
	{
		if (strcmp(UserName, DlgVec[i].TargetName) == 0)
		{//compare
			return i;
		}
	}
	return -1;
}


void friendDlg::OnClose()
{
	::SendMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);
	CDialogEx::OnClose();
}


afx_msg LRESULT friendDlg::OnSendfriend(WPARAM wParam, LPARAM lParam)
{
	//add friend 
	pMessage mess = (pMessage)wParam;
	if (!strcmp(mess->MessageBuf,"succ"))//if succ
	{
		treedlg.InsertItem(CA2W(mess->TargetName), HtreeFriend);//add tree unit
		UpdateData(0);
	}
	MessageBox(CA2W(mess->MessageBuf));
	delete mess;
	return 0;
}


afx_msg LRESULT friendDlg::OnReceivefriend(WPARAM wParam, LPARAM lParam)
{
	//收到被加消息
	pMessage mess = (pMessage)wParam;
	//加树
	treedlg.InsertItem(CA2W(mess->UserName), HtreeFriend);
	UpdateData(0);
	//messagebox通知
	CString str = CA2W(mess->UserName) + L" has added you as a friend";
	MessageBox(str);
	delete mess;
	return 0;
}


afx_msg LRESULT friendDlg::OnAddgroup(WPARAM wParam, LPARAM lParam)
{
	//加群的返回消息
	pMessage mess = (pMessage)wParam;
	if (!strcmp(mess->MessageBuf, "succ"))//如果成功
	{
		treedlg.InsertItem(CA2W(mess->TargetName), HtreeGroup);//添加树控件
		UpdateData(0);
	}
	MessageBox(CA2W(mess->MessageBuf));
	delete mess;
	return 0;
	//加树
}


void friendDlg::OnBnClickedButton4()
{//加好友

	//username is the client
	//target is the one being added
	_Message mMes = {};
	//UpdateData();
	//memset(&mMes, 0, sizeof(_Message));
	//CString user;
	//GetWindowText(user);
	//mMes.MessageType = 5;//5是加好友
	//strcpy_s(mMes.UserName, strlen(CW2A(user)) + 1, CW2A(user));
	//strcpy_s(mMes.TargetName, strlen(CW2A(promptEdit)) + 1, CW2A(promptEdit));
	//不能为空，不能一样
	formatMessage(mMes, 5);
	if (strlen(mMes.UserName) == 0 || strlen(mMes.TargetName) == 0 || strcmp(mMes.UserName, mMes.TargetName)==0)
	{
		MessageBox(L"target can't be empty or ur username");// 不可以加
	}
	else
	{
		sock->dosend(&mMes);// send过去
	}	// TODO:  在此添加控件通知处理程序代码
}


afx_msg LRESULT friendDlg::OnJoingroup(WPARAM wParam, LPARAM lParam)
{
	pMessage mess = (pMessage)wParam;
	if (!strcmp(mess->MessageBuf, "succ"))//如果成功
	{
		treedlg.InsertItem(CA2W(mess->TargetName), HtreeGroup);//添加树控件
		//UpdateData(0);
	}
	MessageBox(CA2W(mess->MessageBuf));
	delete mess;
	return 0;
}

void friendDlg::formatMessage(_Message &mMes, int type){
	//用于输出message，在加好友和加group时候
	UpdateData();
	memset(&mMes, 0, sizeof(_Message));
	CString user;
	GetWindowText(user);
	mMes.MessageType = type;//5是加好友,8加group
	strcpy_s(mMes.UserName, strlen(CW2A(user)) + 1, CW2A(user));
	strcpy_s(mMes.TargetName, strlen(CW2A(promptEdit)) + 1, CW2A(promptEdit));
}

void friendDlg::OnBnClickedButton5()
{//加群组
	_Message mMes = {};
	//UpdateData();
	//memset(&mMes, 0, sizeof(_Message));
	//CString user;
	//GetWindowText(user);
	//mMes.MessageType = 8;//5是加好友
	//strcpy_s(mMes.UserName, strlen(CW2A(user)) + 1, CW2A(user));
	//strcpy_s(mMes.TargetName, strlen(CW2A(promptEdit)) + 1, CW2A(promptEdit));
	//不能为空，不能一样
	formatMessage(mMes, 8);//先创建消息
	if (strlen(mMes.UserName) == 0 || strlen(mMes.TargetName) == 0)
	{//不可为空
		MessageBox(L"target can't be empty");
	}
	else//发送
	{
		sock->dosend(&mMes);
	}	
	// TODO:  在此添加控件通知处理程序代码
	// TODO:  在此添加控件通知处理程序代码
}
