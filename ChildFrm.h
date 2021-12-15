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

// ChildFrm.h : interface of the CChildFrame class
//

#pragma once

#include <map>

class CChildFrame : public CMDIChildWndEx
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame() noexcept;

// Attributes
protected:
	CSplitterWndEx m_wndLeftSplitter;
	CSplitterWndEx m_wndRightSplitter;
public:
	double m_nWithdraw{ 0 };
	double m_nDeposit{ 0 };
	double m_nInterest{ 0 };
	double m_nInterestTax{ 0 };
	double m_nBalance{ 0 };
	double m_nProfit{ 0 };
	COleDateTime m_ClosingDate;

// Operations
public:

// Overrides
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	std::map<security_id, std::shared_ptr<CTradingRecord> > m_tradingRecords; // map<(code, type), CTradingRecord>

// Implementation
public:
	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
public:
	CWnd* GetSubWindow(int nRow, int nCol);
};
