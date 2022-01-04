
// 2048��һ��Dlg.h : ͷ�ļ�
//

#pragma once
#include "m_resource.h"

// CMy2048��һ��Dlg �Ի���
class CMy2048��һ��Dlg : public CDialogEx
{
	friend class AI;
// ����
public:
	CMy2048��һ��Dlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MY2048_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;


public:
	// ���ɵ���Ϣӳ�亯��
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
	Game g; //2048��Ϸ���������
	AI ai;   //2048AI���������
	CFont f;  //�����������

	//���̲߳�������

};
