// SortableListView.cpp : implementation file
//

#include "pch.h"
#include "InvestmentAnalysis.h"
#include "SortableListView.h"


// CSortableListView

IMPLEMENT_DYNCREATE(CSortableListView, CListView)

CSortableListView::CSortableListView()
{

}

CSortableListView::~CSortableListView()
{
}

BEGIN_MESSAGE_MAP(CSortableListView, CListView)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, &CSortableListView::OnLvnColumnclick)
END_MESSAGE_MAP()


// CSortableListView diagnostics

#ifdef _DEBUG
void CSortableListView::AssertValid() const
{
	CListView::AssertValid();
}

#ifndef _WIN32_WCE
void CSortableListView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif
#endif //_DEBUG

struct KeySortParam
{
	CListCtrl* m_plc;
	int m_nSortType;
	int m_nColumn;
};

int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
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
	if (nCol == 0) {
		x1 = _ttoi(strItem1.GetString());
		x2 = _ttoi(strItem2.GetString());
	}
	else {
		x1 = int(_ttof(strItem1.GetString()) * 1000);
		x2 = int(_ttof(strItem2.GetString()) * 1000);
	}

	int result = 0;
	if (SortType == LVS_SORTASCENDING) {
		return x1 - x2;
	}
	else {
		return x2 - x1;
	}
}

// CSortableListView message handlers


void CSortableListView::OnLvnColumnclick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
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
	lc->SortItemsEx(CompareFunc, (LPARAM)&sortParams);

	*pResult = 0;
}
