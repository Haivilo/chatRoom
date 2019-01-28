
// chatRoomDlg.cpp : implementation file
//

#include "stdafx.h"
#include "chatRoom.h"
#include "chatRoomDlg.h"
#include "afxdialogex.h"
#include"md5.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
//	afx_msg LRESULT OnSqlmessage(WPARAM wParam, LPARAM lParam);
	//afx_msg LRESULT OnAddgroup(WPARAM wParam, LPARAM lParam);
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
//	ON_MESSAGE(SQLMESSAGE, &CAboutDlg::OnSqlmessage)
	//ON_MESSAGE(addGroup, &CAboutDlg::OnAddgroup)
END_MESSAGE_MAP()


// CchatRoomDlg dialog



CchatRoomDlg::CchatRoomDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CchatRoomDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CchatRoomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, passwd);
	DDX_Control(pDX, IDC_EDIT2, acct);
}

BEGIN_MESSAGE_MAP(CchatRoomDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON2, &CchatRoomDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CchatRoomDlg::OnBnClickedButton1)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CchatRoomDlg message handlers

BOOL CchatRoomDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	friendDialog.Create(IDD_DIALOG1, this);
	friendDialog.ShowWindow(0);//init main menu
	sock = new mySocket;// init socket
	sock->doconnet(int(friendDialog.GetSafeHwnd()));//connect
	friendDialog.sock = sock;//save sock ptr
	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CchatRoomDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CchatRoomDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CchatRoomDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CchatRoomDlg::OnBnClickedButton2()//register
{
	_Message text;
	memset(&text, 0, sizeof(_Message));
	text.MessageType = 1;//register
	CString acctinfo; 
	CString passwdinfo;
	acct.GetWindowTextW(acctinfo);
	passwd.GetWindowTextW(passwdinfo);
	strcpy_s(text.UserName, strlen(CW2A(acctinfo)) + 1, CW2A(acctinfo));//copy
	strcpy_s(text.TargetName, strlen(md5(string(CW2A(passwdinfo)))) + 1, md5(string(CW2A(passwdinfo))));
	//copy£¬encrypt
	sock->dosend(&text); 

}


void CchatRoomDlg::OnBnClickedButton1()//log in
{
	_Message text;
	memset(&text, 0, sizeof(_Message));
	text.MessageType = 2;
	CString acctinfo;
	CString passwdinfo;
	acct.GetWindowTextW(acctinfo);
	passwd.GetWindowTextW(passwdinfo);
	//copy£¬encrypt
	strcpy_s(text.UserName, strlen(CW2A(acctinfo)) + 1, CW2A(acctinfo));
	strcpy_s(text.TargetName, strlen(md5(string(CW2A(passwdinfo)))) + 1, md5(string(CW2A(passwdinfo))));
	sock->dosend(&text);
}


//afx_msg LRESULT CAboutDlg::OnSqlmessage(WPARAM wParam, LPARAM lParam)
//{
//	return 0;
//}


void CchatRoomDlg::OnClose()
{

	EndDialog(0);
	CDialogEx::OnClose();
}


//afx_msg LRESULT CAboutDlg::OnAddgroup(WPARAM wParam, LPARAM lParam)
//{
//	return 0;
//}
