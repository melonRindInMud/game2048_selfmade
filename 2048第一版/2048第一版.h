
// 2048��һ��.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMy2048��һ��App:
// �йش����ʵ�֣������ 2048��һ��.cpp
//

class CMy2048��һ��App : public CWinApp
{
public:
	CMy2048��һ��App();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMy2048��һ��App theApp;