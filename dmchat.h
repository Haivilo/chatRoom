#pragma once


// dmchat 
#include"mySocket.h"
class dmchat : public CDialogEx
{
	DECLARE_DYNAMIC(dmchat)

public:
	dmchat(CWnd* pParent = NULL); 
	virtual ~dmchat();
	mySocket* sock;//used to send msg


	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);   

	DECLARE_MESSAGE_MAP()
public:
	CString user;
	CString target;
	CString up;
	CString down;
	afx_msg void OnBnClickedButton1();
};
