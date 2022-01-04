
// 2048第一版Dlg.cpp : 实现文件
//

#include "stdafx.h"
#include "2048第一版.h"
#include "2048第一版Dlg.h"
#include "afxdialogex.h"


#include "m_resource.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMy2048第一版Dlg 对话框



CMy2048第一版Dlg::CMy2048第一版Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMy2048第一版Dlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMy2048第一版Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMy2048第一版Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMy2048第一版Dlg::OnBnClickedButton1)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CMy2048第一版Dlg 消息处理程序

BOOL CMy2048第一版Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//g.New_Turn();
	//加载图片
#ifdef REMOD_1

	g.bmp[1].LoadBitmapA(IDB_RM_2);
	g.bmp[2].LoadBitmapA(IDB_RM_4);
	g.bmp[3].LoadBitmapA(IDB_RM_8);
	g.bmp[4].LoadBitmapA(IDB_RM_16);
	g.bmp[5].LoadBitmapA(IDB_RM_32);
	g.bmp[6].LoadBitmapA(IDB_RM_64);
	g.bmp[7].LoadBitmapA(IDB_RM_128);
	g.bmp[8].LoadBitmapA(IDB_RM_256);
	g.bmp[9].LoadBitmapA(IDB_RM_512);
	g.bmp[10].LoadBitmapA(IDB_RM_1024);
	
#endif


	f.CreatePointFont(300, "宋体");
	/*
	::mciSendString("open 123.mp3 alias start", NULL, 0, NULL);
	::mciSendString("play start repeat", NULL, 0, NULL); 
	*/

	srand(time(NULL));

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMy2048第一版Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMy2048第一版Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMy2048第一版Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



BOOL CMy2048第一版Dlg::PreTranslateMessage(MSG* pMsg)
{
	static int w = 0;
	static int c[4][4] = {0};
	// TODO: 在此添加专用代码和/或调用基类
	if(pMsg->message == WM_KEYDOWN)//待定
	{
		CDC* pDC = GetDC();
		int nkeyDown = pMsg->wParam;
		if(nkeyDown == VK_UP && w)
		{
			if( FALSE == g.pro_Move(UP, c))
			{
				PlaySound((LPTSTR)(IDR_BLOCK),NULL,SND_RESOURCE | SND_ASYNC);
				/*
				SetTimer(2, 500, NULL);
				AfxMessageBox("向此处移动不会改变当前格局 不能移动");
				*/
				return 0;
			}
			g.Move(UP);
			g.New_Turn(pDC, &f);
			g.show_game(pDC, &f);
		}
		else if(nkeyDown == VK_DOWN && w)
		{
			if( FALSE == g.pro_Move(DOWN, c))
			{
				PlaySound((LPTSTR)(IDR_BLOCK),NULL,SND_RESOURCE | SND_ASYNC);
				/*
				SetTimer(2, 500, NULL);
				AfxMessageBox("向此处移动不会改变当前格局 不能移动");
				*/
				return 0;
			}
			g.Move(DOWN);
			g.New_Turn(pDC, &f);
			g.show_game(pDC, &f);
		}
		else if(nkeyDown == VK_LEFT && w)
		{
			if( FALSE == g.pro_Move(LEFT, c))
			{
				PlaySound((LPTSTR)(IDR_BLOCK),NULL,SND_RESOURCE | SND_ASYNC);
				/*
				SetTimer(2, 500, NULL);
				AfxMessageBox("向此处移动不会改变当前格局 不能移动");
				*/
				return 0;
			}
			g.Move(LEFT);
			g.New_Turn(pDC, &f);
			g.show_game(pDC, &f);
		}
		else if(nkeyDown == VK_RIGHT && w)
		{
			if( FALSE == g.pro_Move(RIGHT, c))
			{
				PlaySound((LPTSTR)(IDR_BLOCK),NULL,SND_RESOURCE | SND_ASYNC);
				/*
				SetTimer(2, 500, NULL);
				AfxMessageBox("向此处移动不会改变当前格局 不能移动");
				*/
				return 0;
			}
			g.Move(RIGHT);
			g.New_Turn(pDC, &f);
			g.show_game(pDC, &f);
		}
		else if(nkeyDown == VK_SHIFT)
		{
			if(!w)
			{
				GetDlgItem(IDC_BUTTON1)->ShowWindow(SW_HIDE);
				g.New_Turn(this->GetDC(), &f);
				g.show_game(this->GetDC(), &f);


				//开始AI判定
#ifdef USE_AI
				SetTimer(1, INTERVAL, NULL);
#endif
				w = 1;
			}
			else
				;
		}

	}
	return CDialogEx::PreTranslateMessage(pMsg);
}



void CMy2048第一版Dlg::OnBnClickedButton1()
{
	PostMessageA(WM_KEYDOWN, VK_SHIFT);
	// TODO: 在此添加控件通知处理程序代码
}


void CMy2048第一版Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(nIDEvent == 1)
	{
		if(g.gg)
		{
			KillTimer(1);
			return ;
		}
		else
		{
			ai.One_Turn(g);
		}
	}
	else if(nIDEvent == 2)
	{
		KillTimer(2);
		keybd_event(VK_RETURN, 0, 0, 0 );//模拟按下某个键(调整VK_KEYNAME)
		Sleep(20);
		keybd_event (VK_RETURN, 0, KEYEVENTF_KEYUP, 0);//模拟抬起某个键
	}


	CDialogEx::OnTimer(nIDEvent);
}
