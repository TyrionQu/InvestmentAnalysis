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

// TransactionRecordView.cpp : implementation of the CTransactionRecordView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "InvestmentAnalysis.h"
#endif

#include "InvestmentAnalysisDoc.h"
#include "TransactionRecordView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTransactionRecordView

IMPLEMENT_DYNCREATE(CTransactionRecordView, CListView)

BEGIN_MESSAGE_MAP(CTransactionRecordView, CListView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CTransactionRecordView construction/destruction

CTransactionRecordView::CTransactionRecordView() noexcept
{
	// TODO: add construction code here

}

CTransactionRecordView::~CTransactionRecordView()
{
}

BOOL CTransactionRecordView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CListView::PreCreateWindow(cs);
}

// CTransactionRecordView drawing

void CTransactionRecordView::OnDraw(CDC* /*pDC*/)
{
	CInvestmentAnalysisDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

void CTransactionRecordView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CTransactionRecordView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CTransactionRecordView diagnostics

#ifdef _DEBUG
void CTransactionRecordView::AssertValid() const
{
	CListView::AssertValid();
}

void CTransactionRecordView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CInvestmentAnalysisDoc* CTransactionRecordView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CInvestmentAnalysisDoc)));
	return (CInvestmentAnalysisDoc*)m_pDocument;
}
#endif //_DEBUG


// CTransactionRecordView message handlers


void CTransactionRecordView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	CListCtrl& ListCtrl = GetListCtrl();
	ListCtrl.ModifyStyle(0, LVS_REPORT | LVS_SINGLESEL | LVS_EDITLABELS | LVS_SHOWSELALWAYS);
	DWORD dwStyle = ListCtrl.GetExtendedStyle();
	ListCtrl.SetExtendedStyle(dwStyle | LVS_EX_INFOTIP | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_LABELTIP);

	GetDocument()->SetTitle(L"交易记录");

	LV_COLUMN  lvc;
	TCHAR* arrTitle[] = { L"股票名称", L"股票代码", L"买入均价", L"买入数目", L"卖出均价", L"卖出数目", L"买卖次数", L"买卖盈亏" };
	UCHAR      nWidth[] = { 100, 100, 100, 100, 100, 100, 100, 120 };
	USHORT     nFMT[] = { LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_RIGHT, LVCFMT_RIGHT, LVCFMT_RIGHT, LVCFMT_RIGHT, LVCFMT_CENTER, LVCFMT_RIGHT };

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	for (int i = 0; i < sizeof(nWidth) / sizeof(UCHAR); i++)
	{
		lvc.iSubItem = i;
		lvc.pszText = arrTitle[i];
		lvc.cx = nWidth[i];
		lvc.fmt = nFMT[i];
		ListCtrl.InsertColumn(i, &lvc);
	}
}
