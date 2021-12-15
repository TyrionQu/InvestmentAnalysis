// CTransactionSummaries.cpp : implementation file
//

#include "pch.h"
#include "InvestmentAnalysis.h"
#include "TransactionSummariesView.h"

#include "ChildFrm.h"


// CTransactionSummariesView

IMPLEMENT_DYNCREATE(CTransactionSummariesView, CView)

CTransactionSummariesView::CTransactionSummariesView()
{

}

CTransactionSummariesView::~CTransactionSummariesView()
{
}

BEGIN_MESSAGE_MAP(CTransactionSummariesView, CView)
	ON_WM_PAINT()
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


void CTransactionSummariesView::OnPaint()
{
	CFont font;
	VERIFY(font.CreateFont(
		24,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Arial")));                 // lpszFacename

	CPaintDC dc(this); // device context for painting
	CFont* def_font = dc.SelectObject(&font);

	CChildFrame* pParent = (CChildFrame*)GetParentFrame();

	CString strOutput;
	strOutput.Format(L"�������ڣ�%s", pParent->m_ClosingDate.Format(L"%Y��%m��%d��").GetString());
	dc.TextOut(10, 10, strOutput);
	strOutput.Format(L"�˻���%13.2lf", pParent->m_nBalance);
	dc.TextOut(50, 40, strOutput);

	strOutput.Format(L"�����%12.2lf", pParent->m_nDeposit);
	dc.TextOut(50, 80,  strOutput);
	strOutput.Format(L"ȡ����%12.2lf", -pParent->m_nWithdraw);
	dc.TextOut(50, 100, strOutput);
	strOutput.Format(L"�˻���%13.2lf", pParent->m_nDeposit + pParent->m_nWithdraw);
	dc.TextOut(50, 120, strOutput);

	strOutput.Format(L"��Ϣ�ܶ%13.2lf", pParent->m_nInterest);
	dc.TextOut(50, 160, strOutput);
	strOutput.Format(L"��Ϣ˰�%15.2lf", -pParent->m_nInterestTax);
	dc.TextOut(50, 180, strOutput);
	strOutput.Format(L"��Ϣ���ã�%13.2lf", pParent->m_nInterest + pParent->m_nInterestTax);
	dc.TextOut(50, 200, strOutput);

	strOutput.Format(L"�ܼ�����%13.2lf", pParent->m_nProfit);
	dc.TextOut(50, 240, strOutput);
	strOutput.Format(L"����ɱ���");
	dc.TextOut(50, 260, strOutput);
	strOutput.Format(L"�����ɱ���");
	dc.TextOut(50, 280, strOutput);

	dc.SelectObject(def_font);
}
