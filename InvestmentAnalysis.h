// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface
// (the "Fluent UI") and is provided only as referential material to supplement the
// Microsoft Foundation Classes Reference and related electronic documentation
// included with the MFC C++ library software.
// License terms to copy, use or distribute the Fluent UI are available separately.
// To learn more about our Fluent UI licensing program, please visit
// https://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// InvestmentAnalysis.h : main header file for the InvestmentAnalysis application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CInvestmentAnalysisApp:
// See InvestmentAnalysis.cpp for the implementation of this class
//

class CInvestmentAnalysisApp : public CWinAppEx
{
public:
	CInvestmentAnalysisApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	UINT  m_nAppLook;
	CMultiDocTemplate* m_pDocTemplate510050{ nullptr };
	CMultiDocTemplate* m_pDocTemplate510300{ nullptr };
	CMultiDocTemplate* m_pDocTemplateTrading{ nullptr };
	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnFileNew();
};

extern CInvestmentAnalysisApp theApp;
