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

#include "ChildFrm.h"
#include "DetailedTradingView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTransactionRecordView

IMPLEMENT_DYNCREATE(CTransactionRecordView, CListView)

BEGIN_MESSAGE_MAP(CTransactionRecordView, CListView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CTransactionRecordView::OnNMCustomdraw)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, &CTransactionRecordView::OnLvnColumnclick)
	ON_NOTIFY_REFLECT(NM_CLICK, &CTransactionRecordView::OnNMClick)
	ON_NOTIFY_REFLECT(LVN_GETINFOTIP, &CTransactionRecordView::OnLvnGetInfoTip)
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
	TCHAR* arrTitle[] = { L"股票名称", L"股票代码", L"买入均价", L"买入数目", L"卖出均价", L"卖出数目", L"买卖次数", L"买卖盈亏", L""};
	UCHAR      nWidth[] = { 160, 100, 100, 100, 100, 100, 100, 140, 0 };
	USHORT     nFMT[] = { LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_RIGHT, LVCFMT_RIGHT, LVCFMT_RIGHT, LVCFMT_RIGHT, LVCFMT_CENTER, LVCFMT_RIGHT, LVCFMT_CENTER };

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	for (int i = 0; i < sizeof(nWidth) / sizeof(UCHAR); i++)
	{
		lvc.iSubItem = i;
		lvc.pszText = arrTitle[i];
		lvc.cx = nWidth[i];
		lvc.fmt = nFMT[i];
		ListCtrl.InsertColumn(i, &lvc);
	}

	CChildFrame* pChild = (CChildFrame*)GetParentFrame();

	LV_ITEM  lvItem;
	int      nIndex = 0;
	TCHAR    buff[32];
	for (auto record : pChild->m_tradingRecords)
	{
		lvItem.mask = LVIF_TEXT | LVIF_PARAM;
		lvItem.lParam = nIndex;
		lvItem.iItem = nIndex++;
		_stprintf_s(buff, 32, L"%s", record.second->m_strStockName);
		lvItem.pszText = buff;
		lvItem.iSubItem = 0;
		ListCtrl.InsertItem(&lvItem);

		lvItem.iSubItem = 1;
		lvItem.mask = LVIF_TEXT;
		_stprintf_s(buff, 32, L"%06d%s", record.first.m_nSecurityCode, record.first.m_nSecurityType == 4 ? m_nOptionMark.GetString() : L"");
		ListCtrl.SetItem(&lvItem);

		lvItem.iSubItem = 2;
		_stprintf_s(buff, 32, L"%6.3lf", (record.second->m_nBuyAmount) ? (-1) * record.second->m_nBuyAmount / record.second->m_nBuyVolume : 0);
		ListCtrl.SetItem(&lvItem);

		lvItem.iSubItem = 3;
		_stprintf_s(buff, 32, L"%8d", record.second->m_nBuyVolume);
		ListCtrl.SetItem(&lvItem);

		lvItem.iSubItem = 4;
		_stprintf_s(buff, 32, L"%6.3lf", (record.second->m_nSoldAmount) ? ((-1) * record.second->m_nSoldAmount / record.second->m_nSoldVolume) : 0);
		ListCtrl.SetItem(&lvItem);

		lvItem.iSubItem = 5;
		_stprintf_s(buff, 32, L"%8d", (-1) * record.second->m_nSoldVolume);
		ListCtrl.SetItem(&lvItem);

		lvItem.iSubItem = 6;
		_stprintf_s(buff, 32, L"%d/%d", record.second->m_nBuyCount, record.second->m_nSellCount);
		ListCtrl.SetItem(&lvItem);

		double nProfit = record.second->m_nBuyAmount + record.second->m_nSoldAmount;
		lvItem.iSubItem = 7;
		_stprintf_s(buff, 32, L"%12.2lf", nProfit);
		ListCtrl.SetItem(&lvItem);

		lvItem.iSubItem = 8;
		_stprintf_s(buff, 32, L"%02X", record.first.m_nSecurityType);
		ListCtrl.SetItem(&lvItem);

		if (record.second->m_nSoldVolume + record.second->m_nBuyVolume == 0)
			pChild->m_nProfit += nProfit;
	}
}

void CTransactionRecordView::OnNMCustomdraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);
	*pResult = CDRF_DODEFAULT;
	switch (pLVCD->nmcd.dwDrawStage) {
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT:
		if (1 == pLVCD->nmcd.dwItemSpec % 2) {
			pLVCD->clrTextBk = RGB(192, 192, 192);
		}
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT | CDDS_SUBITEM:
		if (pLVCD->iSubItem == 7) {
			CListCtrl* lc = &GetListCtrl();

			CString strItem = lc->GetItemText((int)pLVCD->nmcd.dwItemSpec, 7);
			double nAmount = _ttof(strItem);
			unsigned char nGreen = (nAmount >= 0) ? 192 : 0;
			unsigned char nRed = (nAmount < 0) ? 255 : 0;
			pLVCD->clrText = RGB(nRed, nGreen, 0);
		}
		else {
			if (pLVCD->iSubItem % 2) {
				if (1 == pLVCD->nmcd.dwItemSpec % 2) {
					pLVCD->clrText = RGB(128, 0, 0);
				}
				else {
					pLVCD->clrText = RGB(255, 0, 0);
				}
			}
			else {
				if (1 == pLVCD->nmcd.dwItemSpec % 2) {
					pLVCD->clrText = RGB(0, 0, 255);
				}
				else {
					pLVCD->clrText = RGB(0, 0, 0);
				}
			}
		}
		break;
	default:
		break;
	}
}

struct KeySortParam
{
	CListCtrl* m_plc;
	int m_nSortType;
	int m_nColumn;
};

int CALLBACK compare_cb(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	KeySortParam* pSortParam = (KeySortParam*)lParamSort;
	CListCtrl* pListCtrl = pSortParam->m_plc;
	int nCol = pSortParam->m_nColumn;
	int SortType = pSortParam->m_nSortType;
	CString strItem1 = pListCtrl->GetItemText(static_cast<int>(lParam1), nCol);
	CString strItem2 = pListCtrl->GetItemText(static_cast<int>(lParam2), nCol);
	strItem1.TrimRight('%');
	strItem2.TrimRight('%');
	int x1, x2;
	if (nCol == 1 || nCol == 3 || nCol ==5) {
		x1 = _ttoi(strItem1.GetString());
		x2 = _ttoi(strItem2.GetString());
	}
	else {
		x1 = (int)(_ttof(strItem1.GetString()) * 1000);
		x2 = (int)(_ttof(strItem2.GetString()) * 1000);
	}

	int result = 0;
	if (SortType == LVS_SORTASCENDING) {
		return x1 - x2;
	}
	else {
		return x2 - x1;
	}
};

void CTransactionRecordView::OnLvnColumnclick(NMHDR* pNMHDR, LRESULT* pResult)
{

	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if (pNMLV->iSubItem == 0)
		return;

	CListCtrl* lc = &GetListCtrl();

	if (m_nCurrentSortedColumn == pNMLV->iSubItem) {
		if (m_nCurrtentSortedType == LVS_SORTASCENDING) {
			m_nCurrtentSortedType = LVS_SORTDESCENDING;
		}
		else {
			m_nCurrtentSortedType = LVS_SORTASCENDING;
		}
	}
	else {
		m_nCurrentSortedColumn = pNMLV->iSubItem;
		m_nCurrtentSortedType = LVS_SORTASCENDING;
	}

	KeySortParam sortParams;
	sortParams.m_plc = lc;
	sortParams.m_nColumn = m_nCurrentSortedColumn;
	sortParams.m_nSortType = m_nCurrtentSortedType;
	lc->SortItemsEx(compare_cb, (LPARAM)&sortParams);
}


void CTransactionRecordView::OnNMClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	CListCtrl& ListCtrl = GetListCtrl();
	CString strCode = ListCtrl.GetItemText(pNMItemActivate->iItem, 1);
	if (strCode == m_strSecurityCode)
		return;

	CString strType = ListCtrl.GetItemText(pNMItemActivate->iItem, 8);
	int nSecurityCode = _ttoi(strCode.GetString());
	int nSecurityType = _tcstol(strType.GetString(), NULL, 16);
	CChildFrame* pChild = (CChildFrame*)GetParentFrame();

	CDetailedTradingView *pView = (CDetailedTradingView*)pChild->GetSubWindow(0, 2);

	security_id sid(nSecurityCode, (BYTE)nSecurityType);
	pView->Refresh(sid);

	m_strSecurityCode = strCode;

	*pResult = 0;
}


void CTransactionRecordView::OnLvnGetInfoTip(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLVGETINFOTIP pGetInfoTip = reinterpret_cast<LPNMLVGETINFOTIP>(pNMHDR);

	_stprintf_s(pGetInfoTip->pszText, 16, L"%d", pGetInfoTip->iItem + 1);

	*pResult = 0;
}
