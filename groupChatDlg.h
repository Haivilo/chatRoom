#pragma once
#include "afxwin.h"

#include"mySocket.h"



class groupChatDlg : public CDialogEx
{
	DECLARE_DYNAMIC(groupChatDlg)

public:
	CString user;
	CString grpname;
	groupChatDlg(CWnd* pParent = NULL);  
	virtual ~groupChatDlg();
	mySocket* sock;

	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX); 

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	CString up;
	CString down;
};
