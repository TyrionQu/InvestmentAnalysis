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

// InvestmentAnalysisView.cpp : implementation of the CInvestmentAnalysisView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "InvestmentAnalysis.h"
#endif

#include "InvestmentAnalysisDoc.h"
#include "InvestmentAnalysisView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CInvestmentAnalysisView

IMPLEMENT_DYNCREATE(CInvestmentAnalysisView, CView)

BEGIN_MESSAGE_MAP(CInvestmentAnalysisView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CInvestmentAnalysisView construction/destruction

CInvestmentAnalysisView::CInvestmentAnalysisView() noexcept
{
	// TODO: add construction code here

}

CInvestmentAnalysisView::~CInvestmentAnalysisView()
{
}

BOOL CInvestmentAnalysisView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CInvestmentAnalysisView drawing

void CInvestmentAnalysisView::OnDraw(CDC* /*pDC*/)
{
	CInvestmentAnalysisDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

void CInvestmentAnalysisView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CInvestmentAnalysisView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CInvestmentAnalysisView diagnostics

#ifdef _DEBUG
void CInvestmentAnalysisView::AssertValid() const
{
	CView::AssertValid();
}

void CInvestmentAnalysisView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CInvestmentAnalysisDoc* CInvestmentAnalysisView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CInvestmentAnalysisDoc)));
	return (CInvestmentAnalysisDoc*)m_pDocument;
}
#endif //_DEBUG


// CInvestmentAnalysisView message handlers
