// CTransactionSummaries.cpp : implementation file
//

#include "pch.h"
#include "InvestmentAnalysis.h"
#include "TransactionSummariesView.h"


// CTransactionSummariesView

IMPLEMENT_DYNCREATE(CTransactionSummariesView, CView)

CTransactionSummariesView::CTransactionSummariesView()
{

}

CTransactionSummariesView::~CTransactionSummariesView()
{
}

BEGIN_MESSAGE_MAP(CTransactionSummariesView, CView)
END_MESSAGE_MAP()


// CTransactionSummariesView drawing

void CTransactionSummariesView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}


// CTransactionSummariesView diagnostics

#ifdef _DEBUG
void CTransactionSummariesView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CTransactionSummariesView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CTransactionSummariesView message handlers
