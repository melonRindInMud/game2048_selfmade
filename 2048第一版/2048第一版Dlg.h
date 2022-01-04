
// 2048第一版Dlg.h : 头文件
//

#pragma once
#include "m_resource.h"

// CMy2048第一版Dlg 对话框
class CMy2048第一版Dlg : public CDialogEx
{
	friend class AI;
// 构造
public:
	CMy2048第一版Dlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MY2048_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;


public:
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
private:
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
	Game g; //2048游戏控制类对象
	AI ai;   //2048AI控制类对象
	CFont f;  //字体字体控制

	//多线程播放音乐

};
