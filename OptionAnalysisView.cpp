// OptionAnalysisView.cpp : implementation file
//

#include "pch.h"
#include "framework.h"

#include <thread>
#include <fstream>
#include <ppl.h>

#include "InvestmentAnalysis.h"
#include "InvestmentAnalysisDoc.h"
#include "OptionAnalysisView.h"


// COptionAnalysisBaseView

IMPLEMENT_DYNCREATE(COptionAnalysisBaseView, CListView)

COptionAnalysisBaseView::COptionAnalysisBaseView()
{

}

COptionAnalysisBaseView::~COptionAnalysisBaseView()
{
}

BEGIN_MESSAGE_MAP(COptionAnalysisBaseView, CListView)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &COptionAnalysisBaseView::OnNMCustomdraw)
END_MESSAGE_MAP()


// COptionAnalysisBaseView diagnostics

#ifdef _DEBUG
void COptionAnalysisBaseView::AssertValid() const
{
	CListView::AssertValid();
}

#ifndef _WIN32_WCE
void COptionAnalysisBaseView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif
#endif //_DEBUG


// COptionAnalysisBaseView message handlers


void COptionAnalysisBaseView::OnNMCustomdraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);
	*pResult = CDRF_DODEFAULT;
	switch (pLVCD->nmcd.dwDrawStage) {
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT:
		if (1 == pLVCD->nmcd.lItemlParam % 2) {
			pLVCD->clrTextBk = RGB(192, 192, 192);
		}
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT | CDDS_SUBITEM:
		switch (pLVCD->iSubItem) {
		case 1:
		case 3:
		case 6:
		case 8:
			if (1 == pLVCD->nmcd.lItemlParam % 2) {
				pLVCD->clrText = RGB(128, 0, 0);
			}
			else {
				pLVCD->clrText = RGB(255, 0, 0);
			}
			break;
		default:
			if (1 == pLVCD->nmcd.lItemlParam % 2) {
				pLVCD->clrText = RGB(0, 0, 255);
			}
			else {
				pLVCD->clrText = RGB(0, 0, 0);
			}
			break;
		}
	default:
		break;
	}
}

bool COptionAnalysisBaseView::IsFourthMidweek(int nDate)
{
	int nYear = nDate / 10000;
	int nMonth = nDate / 100 - nYear * 100;
	int nDay = nDate - (nDate / 100) * 100;

	COleDateTime date(nYear, nMonth, nDay, 0, 0, 0);
	if (date.GetDayOfWeek() != 4 || nDay <= 21)
		return false;

	return true;
}

void COptionAnalysisBaseView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	CListCtrl& ListCtrl = GetListCtrl();
	ListCtrl.ModifyStyle(0, LVS_REPORT | LVS_SINGLESEL | LVS_EDITLABELS | LVS_SHOWSELALWAYS);
	DWORD dwStyle = ListCtrl.GetExtendedStyle();
	ListCtrl.SetExtendedStyle(dwStyle | LVS_EX_INFOTIP | LVS_EX_LABELTIP | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_LABELTIP);

	GetDocument()->SetTitle(m_strViewTitle);

	LV_COLUMN  lvc;
	TCHAR* arrTitle[] = { L"统计月份", L"当日收盘", L"T-1日收盘", L"T-2日收盘",
		L"T-3日收盘", L"T-4收盘", L"当日涨幅", L"二日涨幅",
		L"三日涨幅", L"四日涨幅", L"五日涨幅"
	};
	UCHAR      nWidth[] = { 120, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160 };
	USHORT     nFMT[] = { LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_CENTER,
		LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_CENTER,
		LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_CENTER
	};

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	for (int i = 0; i < sizeof(nWidth) / sizeof(UCHAR); i++) {
		lvc.iSubItem = i;
		lvc.pszText = arrTitle[i];
		lvc.cx = nWidth[i];
		lvc.fmt = nFMT[i];
		ListCtrl.InsertColumn(i, &lvc);
	}

	std::string strPathName{ "E:\\zd_zszq\\vipdoc\\sh\\lday\\" };
	strPathName += m_strIndexFileName;

	if (m_vecSocket.size() > 0)
		m_vecSocket.clear();

	std::ifstream file(strPathName.c_str(), std::ios::binary);
	if (file.bad())
	{
		CString strError = L"Can't read file " + CString(strPathName.c_str());
		MessageBox(strError);
		return;
	}

	UINT nBytesRead = 0;
	UINT nDataSize = sizeof(CStockPrice);
	while (!file.eof())
	{
		CStockPrice curIndexPrice;
		file.read((char*)&curIndexPrice, nDataSize);
		if (file.gcount() != nDataSize)
			break;
		m_vecSocket.push_back(curIndexPrice);
	}

	file.close();

	concurrency::task_group taskgroup;

	taskgroup.run_and_wait([this] {
		size_t i = m_vecSocket.size();
		CListCtrl& ListCtrl = GetListCtrl();
		UINT nIndex = 0;
		LV_ITEM lvItem;
		TCHAR   buff[32];
		while (i > 4)
		{
			i--;
			CStockPrice curPrice = m_vecSocket[i];
			if (!IsFourthMidweek(curPrice.m_nDate))
			{
				continue;
			}
			lvItem.lParam = nIndex;
			lvItem.iItem = nIndex++;
			lvItem.mask = LVIF_TEXT | LVIF_PARAM;
			swprintf(buff, 32, L"%d", curPrice.m_nDate);
			lvItem.pszText = buff;
			lvItem.iSubItem = 0;
			ListCtrl.InsertItem(&lvItem);

			lvItem.iSubItem = 1;
			lvItem.mask = LVIF_TEXT;
			swprintf(buff, 7, L"%0.3f", ((double)curPrice.m_nClose) / 1000.00);
			ListCtrl.SetItem(&lvItem);

			lvItem.iSubItem = 2;
			lvItem.mask = LVIF_TEXT;
			swprintf(buff, 7, L"%0.3f", ((double)m_vecSocket[i - 1].m_nClose) / 1000.00);
			ListCtrl.SetItem(&lvItem);

			lvItem.iSubItem = 3;
			lvItem.mask = LVIF_TEXT;
			swprintf(buff, 7, L"%0.3f", ((double)m_vecSocket[i - 2].m_nClose) / 1000.00);
			ListCtrl.SetItem(&lvItem);

			lvItem.iSubItem = 4;
			lvItem.mask = LVIF_TEXT;
			swprintf(buff, 7, L"%0.3f", ((double)m_vecSocket[i - 3].m_nClose) / 1000.00);
			ListCtrl.SetItem(&lvItem);

			lvItem.iSubItem = 5;
			lvItem.mask = LVIF_TEXT;
			swprintf(buff, 7, L"%0.3f", ((double)m_vecSocket[i - 4].m_nClose) / 1000.00);
			ListCtrl.SetItem(&lvItem);

			lvItem.iSubItem = 6;
			lvItem.mask = LVIF_TEXT;
			swprintf(buff, 7, L"%0.2f%%", (((double)curPrice.m_nClose) / m_vecSocket[i - 1].m_nClose - 1) * 100);
			ListCtrl.SetItem(&lvItem);

			lvItem.iSubItem = 7;
			lvItem.mask = LVIF_TEXT;
			swprintf(buff, 7, L"%0.2f%%", (((double)curPrice.m_nClose) / m_vecSocket[i - 2].m_nClose - 1) * 100);
			ListCtrl.SetItem(&lvItem);

			lvItem.iSubItem = 8;
			lvItem.mask = LVIF_TEXT;
			swprintf(buff, 7, L"%0.2f%%", (((double)curPrice.m_nClose) / m_vecSocket[i - 3].m_nClose - 1) * 100);
			ListCtrl.SetItem(&lvItem);

			lvItem.iSubItem = 9;
			lvItem.mask = LVIF_TEXT;
			swprintf(buff, 7, L"%0.2f%%", (((double)curPrice.m_nClose) / m_vecSocket[i - 4].m_nClose - 1) * 100);
			ListCtrl.SetItem(&lvItem);

			lvItem.iSubItem = 10;
			lvItem.mask = LVIF_TEXT;
			swprintf(buff, 7, L"%0.2f%%", (((double)curPrice.m_nClose) / m_vecSocket[i - 5].m_nClose - 1) * 100);
			ListCtrl.SetItem(&lvItem);

			i -= 4;
		}
	});
}

IMPLEMENT_DYNCREATE(C510050OptionView, COptionAnalysisBaseView)

BEGIN_MESSAGE_MAP(C510050OptionView, COptionAnalysisBaseView)
END_MESSAGE_MAP()

IMPLEMENT_DYNCREATE(C510300OptionView, COptionAnalysisBaseView)

BEGIN_MESSAGE_MAP(C510300OptionView, COptionAnalysisBaseView)
END_MESSAGE_MAP()
