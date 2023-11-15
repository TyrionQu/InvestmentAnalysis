// DetailedTradingView.cpp : implementation file
//

#include "pch.h"
#include "InvestmentAnalysis.h"
#include "DetailedTradingView.h"


// CDetailedTradingView

IMPLEMENT_DYNCREATE(CDetailedTradingView, CSortableListView)

CDetailedTradingView::CDetailedTradingView()
{

}

CDetailedTradingView::~CDetailedTradingView()
{
}

BEGIN_MESSAGE_MAP(CDetailedTradingView, CSortableListView)
END_MESSAGE_MAP()


// CDetailedTradingView diagnostics

#ifdef _DEBUG
void CDetailedTradingView::AssertValid() const
{
	CSortableListView::AssertValid();
}

#ifndef _WIN32_WCE
void CDetailedTradingView::Dump(CDumpContext& dc) const
{
	CSortableListView::Dump(dc);
}
#endif
#endif //_DEBUG


// CDetailedTradingView message handlers


void CDetailedTradingView::OnInitialUpdate()
{
	CSortableListView::OnInitialUpdate();

	CListCtrl& ListCtrl = GetListCtrl();
	ListCtrl.ModifyStyle(0, LVS_REPORT | LVS_SINGLESEL | LVS_EDITLABELS | LVS_SHOWSELALWAYS);
	DWORD dwStyle = ListCtrl.GetExtendedStyle();
	ListCtrl.SetExtendedStyle(dwStyle | LVS_EX_INFOTIP | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_LABELTIP);

	LV_COLUMN  lvc;
	TCHAR* arrTitle[] = { L"交易日期", L"交易价格", L"交易数目", L"成交金额", L"备注信息"};
	UCHAR  nWidth[] = { 120, 160, 160, 160, 250 };
	USHORT nFMT[] = { LVCFMT_LEFT, LVCFMT_RIGHT, LVCFMT_RIGHT, LVCFMT_RIGHT, LVCFMT_CENTER };

	ListCtrl.DeleteAllItems();

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


void CDetailedTradingView::Refresh(const security_id& sid)
{
	CListCtrl& ListCtrl = GetListCtrl();
	double nSum = 0.0;
	long nVolume = 0;

	std::function<void(int n, CCommand<CAccessor<CTradingRecordTable> >& DBTable)> callback = [&](int n, CCommand<CAccessor<CTradingRecordTable> >& DBTable)
	{
		LV_ITEM  lvItem;
		TCHAR    buff[32];
		
		COleDateTime tradingDate;
		tradingDate.m_dt = DBTable.m_TradingDate;

		lvItem.mask = LVIF_TEXT | LVIF_PARAM;
		lvItem.iItem = n;
		_stprintf_s(buff, 32, L"%s", tradingDate.Format(L"%Y-%m-%d").GetString());
		lvItem.pszText = buff;
		lvItem.lParam = n;
		lvItem.iSubItem = 0;
		ListCtrl.InsertItem(&lvItem);

		lvItem.iSubItem = 1;
		lvItem.mask = LVIF_TEXT;
		_stprintf_s(buff, 32, L"%6.3lf", DBTable.m_Price);
		ListCtrl.SetItem(&lvItem);

		lvItem.iSubItem = 2;
		lvItem.mask = LVIF_TEXT;
		_stprintf_s(buff, 32, L"%6d", DBTable.m_Volume);
		ListCtrl.SetItem(&lvItem);

		lvItem.iSubItem = 3;
		lvItem.mask = LVIF_TEXT;
		_stprintf_s(buff, 32, L"%8.2lf", DBTable.m_Amount);
		ListCtrl.SetItem(&lvItem);

		lvItem.iSubItem = 4;
		lvItem.mask = LVIF_TEXT;
		_stprintf_s(buff, 32, L"交易类型 - %d 市场类型 - %d", DBTable.m_TradingType, DBTable.m_SecurityType);
		ListCtrl.SetItem(&lvItem);

		nSum += DBTable.m_Amount;
		nVolume += DBTable.m_Volume;
	};

	std::function<void(int n)> final_callback = [&](int n)
	{
		LV_ITEM  lvItem;
		TCHAR    buff[32];

		lvItem.mask = LVIF_TEXT | LVIF_PARAM;
		lvItem.iItem = n;
		_stprintf_s(buff, 32, L"%14s", L"合计：");
		lvItem.pszText = buff;
		lvItem.lParam = n;
		lvItem.iSubItem = 0;
		ListCtrl.InsertItem(&lvItem);

		lvItem.iSubItem = 1;
		lvItem.mask = LVIF_TEXT;
		_stprintf_s(buff, 32, L"买卖%06d", sid.m_nSecurityCode);
		ListCtrl.SetItem(&lvItem);

		lvItem.iSubItem = 2;
		lvItem.mask = LVIF_TEXT;
		_stprintf_s(buff, 32, L"%d次 剩余%d股", n, nVolume);
		ListCtrl.SetItem(&lvItem);

		lvItem.iSubItem = 3;
		lvItem.mask = LVIF_TEXT;
		_stprintf_s(buff, 32, L"%8.2lf", nSum);
		ListCtrl.SetItem(&lvItem);
	};

	ListCtrl.DeleteAllItems();

	concurrency::task_group taskgroup;

	taskgroup.run_and_wait([&] {
		CString strCmd;
		strCmd.Format(L"SELECT SerialNo, TradingDate, TradingType, AccountBalance, SecurityCode, SecurityType, Price, Volume, Amount FROM TradingRecordTable WHERE (SecurityCode = %d) AND (SecurityType = %d) ORDER BY SerialNo",
			sid.m_nSecurityCode, sid.m_nSecurityType);
		RetrieveData<CTradingRecordTable>(strCmd, callback, final_callback);
		});
}
