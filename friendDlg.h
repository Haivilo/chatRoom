#pragma once

#include"mySocket.h"
#include "afxcmn.h"
#include "dmchat.h"
#include"groupChatDlg.h"
// friendDlg 
#include <vector>
using namespace std;
typedef struct INFO
{
	char TargetName[20];
	CDialogEx * Dlgobj;
};
class friendDlg : public CDialogEx
{
	DECLARE_DYNAMIC(friendDlg)

public:
	friendDlg(CWnd* pParent = NULL);  
	virtual ~friendDlg();
	mySocket* sock;
	HTREEITEM HtreeFriend;//tree for contacts
	HTREEITEM HtreeGroup;//tree for grp chat
	int FindChatWindows(char*UserName);
// 对话框数据
	enum { IDD = IDD_DIALOG1,
			pythonmessage =WM_USER + 1,//enumerate every msg
			WM_register,
			WM_login,
			WM_chat,
			WM_groupchat,
			sendFriend,
			addGroup,
			receiveFriend,
			joinGroup
	};
	vector<INFO> DlgVec;
protected:
	CDialogEx* describeDlg(TCHAR myself[20], TCHAR target[20], CString content, DWORD id, bool frdlg = 1);
	//new dlg
	void formatMessage(_Message &mMes, int type);//used for adding new friend and grp
	virtual void DoDataExchange(CDataExchange* pDX);   

	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnPythonmessage(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedButton3();
protected:
	afx_msg LRESULT OnLogin(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRegister(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnChat(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGroupchat(WPARAM wParam, LPARAM lParam);
public:
	CTreeCtrl treedlg;//tree
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClose();
protected:
	afx_msg LRESULT OnSendfriend(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnReceivefriend(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAddgroup(WPARAM wParam, LPARAM lParam);
public:
	CString promptEdit;//add friends or grp string
	afx_msg void OnBnClickedButton4();
protected:
	afx_msg LRESULT OnJoingroup(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedButton5();
};
