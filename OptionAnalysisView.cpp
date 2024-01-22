// OptionAnalysisView.cpp : implementation file
//

#include "pch.h"
#include "framework.h"

#include <thread>
#include <fstream>

#include "InvestmentAnalysis.h"
#include "InvestmentAnalysisDoc.h"
#include "OptionAnalysisView.h"


// COptionAnalysisBaseView

IMPLEMENT_DYNCREATE(COptionAnalysisBaseView, CSortableListView)

COptionAnalysisBaseView::COptionAnalysisBaseView()
{

}

COptionAnalysisBaseView::~COptionAnalysisBaseView()
{
}

BEGIN_MESSAGE_MAP(COptionAnalysisBaseView, CSortableListView)
	ON_NOTIFY_REFLECT(LVN_GETINFOTIP, &COptionAnalysisBaseView::OnLvnGetInfoTip)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &COptionAnalysisBaseView::OnNMCustomdraw)
END_MESSAGE_MAP()


// COptionAnalysisBaseView diagnostics

#ifdef _DEBUG
void COptionAnalysisBaseView::AssertValid() const
{
	CSortableListView::AssertValid();
}

#ifndef _WIN32_WCE
void COptionAnalysisBaseView::Dump(CDumpContext& dc) const
{
	CSortableListView::Dump(dc);
}
#endif
#endif //_DEBUG


// COptionAnalysisBaseView message handlers


bool COptionAnalysisBaseView::IsFourthWednesday(const CStockPrice& current, int i)
{
	int nDate = current.m_nDate;
	int nYear = nDate / 10000;
	int nMonth = nDate / 100 - nYear * 100;
	int nDay = nDate - (nDate / 100) * 100;

	COleDateTime date(nYear, nMonth, nDay, 0, 0, 0);
	const int nWeek = date.GetDayOfWeek();
	if (m_nLastMonth != nMonth)
		m_MonthStockPrice.reset();
	m_MonthStockPrice.update(current);
	m_PeriodStockPrice.update(current);
	bool bThisOne = false;
	if (m_nLastMonth != nMonth)
	{
//		if (m_nLastMonth != 0 && nWeek != 4)
			m_nWednesday = 0;
		m_nLastMonth = nMonth;
	}
	if (nWeek == 4)
	{
		m_nWednesday++;
#if 0
		if (m_nWednesday == 3 && (i + 5) < m_vecStock.size())
		{
			const CStockPrice nextWed = m_vecStock[i + 5];
			if ((nextWed.m_nDate / 100) % 100 != nMonth)
				bThisOne = true;
		}
#else
		if (m_nWednesday == 3)
		{
			// For October
			COleDateTime nextWed(date.m_dt + 7);
			byte nNextWedMonth = nextWed.GetMonth();
			if (nNextWedMonth != nMonth)
				bThisOne = true;
			// For Spring Festival
			if ((i + 5) < m_vecStock.size())
			{
				const CStockPrice nextWedStock = m_vecStock[i + 5];
				if ((nextWedStock.m_nDate / 100) % 100 != nMonth && nDay > 20)
					bThisOne = true;
			}
		}
#endif
	}
	if (m_nWednesday == 3)
	{
		if (nDate == 20230130 || nDate == 20230524)
			bThisOne = true;
	}
	if (m_nWednesday == 4 || bThisOne)
	{
		m_nLastMonth = nMonth;
		m_nWednesday = 0;
		return true;
	}

	return false;
}

void COptionAnalysisBaseView::OnInitialUpdate()
{
	CSortableListView::OnInitialUpdate();

	CListCtrl& ListCtrl = GetListCtrl();
	ListCtrl.ModifyStyle(0, LVS_REPORT | LVS_SINGLESEL | LVS_EDITLABELS | LVS_SHOWSELALWAYS);
	DWORD dwStyle = ListCtrl.GetExtendedStyle();
	ListCtrl.SetExtendedStyle(dwStyle | LVS_EX_INFOTIP | LVS_EX_LABELTIP | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_LABELTIP);

	GetDocument()->SetTitle(m_strViewTitle);

	LV_COLUMN  lvc;
	TCHAR* arrTitle[] = { L"统计月份", L"T日收盘", L"涨幅", L"T-1日收盘", L"涨幅", L"T-2日收盘", L"涨幅",
		L"T-3日收盘", L"涨幅", L"T-4收盘", L"涨幅", L"当日涨幅", L"二日涨幅", L"三日涨幅", L"四日涨幅",
		L"五日涨幅", L"周期最大涨幅", L"最大跌幅", L"收盘涨幅", L"当月最大涨幅", L"最大跌幅", L"收盘涨幅"
	};
	USHORT nWidth[] = { 120, 160, 90, 160, 90, 160, 90, 160, 90, 160, 90, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120, 120 };
	USHORT nFMT[] = { LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_CENTER,
		LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_CENTER,
		LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_CENTER,
		LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_CENTER,
		LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_CENTER,
		LVCFMT_CENTER, LVCFMT_CENTER
	};

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	for (int i = 0; i < sizeof(nWidth) / sizeof(USHORT); i++) {
		lvc.iSubItem = i;
		lvc.pszText = arrTitle[i];
		lvc.cx = nWidth[i];
		lvc.fmt = nFMT[i];
		ListCtrl.InsertColumn(i, &lvc);
	}

	concurrency::task_group taskgroup;

	taskgroup.run_and_wait([&] {
		std::string strPathName{ "E:\\zd_zszq\\vipdoc\\" };
		strPathName += m_strIndexFileName;

		if (m_vecStock.size() > 0)
			m_vecStock.clear();

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
			if (m_vecStock.empty())
				curIndexPrice.x = (float)curIndexPrice.m_nOpen;
			else
			{
				curIndexPrice.x = (float)m_vecStock.back().m_nClose;
			}
			m_vecStock.push_back(curIndexPrice);
		}

		file.close();

		UINT nIndex = 0;
		int i = -1;
		LV_ITEM lvItem;
		TCHAR   buff[32];
		bool unfilled = true;
		auto FillListView = [&](const CStockPrice& curPrice) {
			lvItem.lParam = nIndex;
			lvItem.iItem = nIndex++;
			lvItem.mask = LVIF_TEXT | LVIF_PARAM;
			_stprintf_s(buff, 32, L"%d", curPrice.m_nDate);
			lvItem.pszText = buff;
			lvItem.iSubItem = 0;
			ListCtrl.InsertItem(&lvItem);

			double nFillData[22] = { 0 };
			nFillData[1] = ((double)curPrice.m_nClose) / 1000.00;
			nFillData[2] = (((double)m_vecStock[i].m_nClose) / m_vecStock[i - 1].m_nClose - 1) * 100;
			nFillData[3] = ((double)m_vecStock[i - 1].m_nClose) / 1000.00;
			nFillData[4] = (((double)m_vecStock[i - 1].m_nClose) / m_vecStock[i - 2].m_nClose - 1) * 100;
			nFillData[5] = ((double)m_vecStock[i - 2].m_nClose) / 1000.00;
			nFillData[6] = (((double)m_vecStock[i - 2].m_nClose) / m_vecStock[i - 3].m_nClose - 1) * 100;
			nFillData[7] = ((double)m_vecStock[i - 3].m_nClose) / 1000.00;
			nFillData[8] = (((double)m_vecStock[i - 3].m_nClose) / m_vecStock[i - 4].m_nClose - 1) * 100;
			nFillData[9] = ((double)m_vecStock[i - 4].m_nClose) / 1000.00;
			nFillData[10] = (((double)m_vecStock[i - 4].m_nClose) / m_vecStock[i - 5].m_nClose - 1) * 100;
			nFillData[11] = (((double)curPrice.m_nClose) / m_vecStock[i - 1].m_nClose - 1) * 100;
			nFillData[12] = (((double)curPrice.m_nClose) / m_vecStock[i - 2].m_nClose - 1) * 100;
			nFillData[13] = (((double)curPrice.m_nClose) / m_vecStock[i - 3].m_nClose - 1) * 100;
			nFillData[14] = (((double)curPrice.m_nClose) / m_vecStock[i - 4].m_nClose - 1) * 100;
			nFillData[15] = (((double)curPrice.m_nClose) / m_vecStock[i - 5].m_nClose - 1) * 100;
			nFillData[16] = (((double)m_PeriodStockPrice.m_nHigh) / m_PeriodStockPrice.x - 1) * 100;
			nFillData[17] = (((double)m_PeriodStockPrice.m_nLow) / m_PeriodStockPrice.x - 1) * 100;
			nFillData[18] = (((double)m_PeriodStockPrice.m_nClose) / m_PeriodStockPrice.x - 1) * 100;
			nFillData[19] = (((double)m_MonthStockPrice.m_nHigh) / m_MonthStockPrice.x - 1) * 100;
			nFillData[20] = (((double)m_MonthStockPrice.m_nLow) / m_MonthStockPrice.x - 1) * 100;
			nFillData[21] = (((double)m_MonthStockPrice.m_nClose) / m_MonthStockPrice.x - 1) * 100;

			for (int j = 1; j < 22; j++)
			{
				lvItem.iSubItem = j;
				lvItem.mask = LVIF_TEXT;
				if (j % 2 == 1 && j < 10)
				{
					if (m_bIndex == true)
						_stprintf_s(buff, 12, L"%0.2f", nFillData[j] * 10);
					else
						_stprintf_s(buff, 12, L"%0.3f", nFillData[j]);
				}
				else
					_stprintf_s(buff, 12, L"%0.2f%%", nFillData[j]);
				ListCtrl.SetItem(&lvItem);
			}
			m_PeriodStockPrice.reset();
		};
		for (const CStockPrice& curPrice : m_vecStock)
		{
			i++;
			if (!IsFourthWednesday(curPrice, i))
			{
				unfilled = true;
				continue;
			}
			FillListView(curPrice);

			unfilled = false;
		}

		if (unfilled && m_vecStock.size() > 0)
			FillListView(m_vecStock.back());
	});
}

IMPLEMENT_DYNCREATE(C510050OptionView, COptionAnalysisBaseView)

BEGIN_MESSAGE_MAP(C510050OptionView, COptionAnalysisBaseView)
END_MESSAGE_MAP()

IMPLEMENT_DYNCREATE(C510300OptionView, COptionAnalysisBaseView)

BEGIN_MESSAGE_MAP(C510300OptionView, COptionAnalysisBaseView)
END_MESSAGE_MAP()

IMPLEMENT_DYNCREATE(C159915OptionView, COptionAnalysisBaseView)

BEGIN_MESSAGE_MAP(C159915OptionView, COptionAnalysisBaseView)
END_MESSAGE_MAP()

IMPLEMENT_DYNCREATE(C399905OptionView, COptionAnalysisBaseView)

BEGIN_MESSAGE_MAP(C399905OptionView, COptionAnalysisBaseView)
END_MESSAGE_MAP()

void COptionAnalysisBaseView::OnLvnGetInfoTip(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLVGETINFOTIP pGetInfoTip = reinterpret_cast<LPNMLVGETINFOTIP>(pNMHDR);

	CListCtrl& lc = GetListCtrl();
	int nAll = lc.GetItemCount();

	_stprintf_s(pGetInfoTip->pszText, 16, L"%d/%d", pGetInfoTip->iItem + 1, nAll);

	*pResult = 0;
}


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
		if (1 == pLVCD->nmcd.dwItemSpec % 2) {
			pLVCD->clrTextBk = RGB(192, 192, 192);
		}
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT | CDDS_SUBITEM:
		if (pLVCD->iSubItem > 10) {
			if (pLVCD->iSubItem > 15) {
				if (1 == pLVCD->nmcd.dwItemSpec % 2) {
					pLVCD->clrText = RGB(0, 0, 192);
				}
				else {
					pLVCD->clrText = RGB(192, 0, 192);
				}
			}
			else {
				if (1 == pLVCD->nmcd.dwItemSpec % 2) {
					pLVCD->clrText = RGB(255, 0, 255);
				}
				else {
					pLVCD->clrText = RGB(0, 0, 255);
				}
			}
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
