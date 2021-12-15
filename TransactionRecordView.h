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

// TransactionRecordView.h : interface of the CTransactionRecordView class
//

#pragma once

#include "SortableListView.h"


class CTransactionRecordView : public CListView
{
protected: // create from serialization only
	CTransactionRecordView() noexcept;
	DECLARE_DYNCREATE(CTransactionRecordView)

// Attributes
public:
	const CString m_nOptionMark = L"*";
	CInvestmentAnalysisDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	CString m_strSecurityCode;
	LONG    m_nCurrentSortedColumn{ -1 };
	LONG    m_nCurrtentSortedType{ 0 };

// Implementation
public:
	virtual ~CTransactionRecordView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnNMCustomdraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLvnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLvnGetInfoTip(NMHDR* pNMHDR, LRESULT* pResult);
};

#ifndef _DEBUG  // debug version in InvestmentAnalysisView.cpp
inline CInvestmentAnalysisDoc* CTransactionRecordView::GetDocument() const
   { return reinterpret_cast<CInvestmentAnalysisDoc*>(m_pDocument); }
#endif

