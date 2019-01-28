
// chatRoomDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "mySocket.h"
#include "friendDlg.h"
// CchatRoomDlg dialog
class CchatRoomDlg : public CDialogEx
{
// Construction
public:
	CchatRoomDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_CHATROOM_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	mySocket* sock;//sock class used to send msg
	CEdit passwd;//save psw
	CEdit acct;//save account
	friendDlg friendDialog;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnClose();
};
