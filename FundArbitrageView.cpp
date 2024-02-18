#include "pch.h"
#include "FundArbitrageView.h"

IMPLEMENT_DYNCREATE(CFundArbitrageView, CSortableListView)


void CFundArbitrageView::OnInitialUpdate()
{
	CSortableListView::OnInitialUpdate();

	CListCtrl& ListCtrl = GetListCtrl();
	ListCtrl.ModifyStyle(0, LVS_REPORT | LVS_SINGLESEL | LVS_EDITLABELS | LVS_SHOWSELALWAYS);
	DWORD dwStyle = ListCtrl.GetExtendedStyle();
	ListCtrl.SetExtendedStyle(dwStyle | LVS_EX_INFOTIP | LVS_EX_LABELTIP | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_LABELTIP);

	GetDocument()->SetTitle(L"场外基金套利分析");
	LV_COLUMN  lvc;
	TCHAR* arrTitle[] = { L"基金代码", L"       基金名称", L"收盘价格", L"申购价格", L"潜在收益"};
	USHORT nWidth[] = { 90, 160, 90, 90, 90};
	USHORT nFMT[] = { LVCFMT_CENTER, LVCFMT_LEFT, LVCFMT_CENTER, LVCFMT_CENTER, LVCFMT_CENTER };

	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	for (int i = 0; i < sizeof(nWidth) / sizeof(USHORT); i++) {
		lvc.iSubItem = i;
		lvc.pszText = arrTitle[i];
		lvc.cx = nWidth[i];
		lvc.fmt = nFMT[i];
		ListCtrl.InsertColumn(i, &lvc);
	}

	std::function<void(int n)> final_callback = [&](int n)
	{
	};
	std::function<void(int n, CCommand<CAccessor<CSecuritiesTable> >& DBTable)> callback = [&](int n, CCommand<CAccessor<CSecuritiesTable> >& DBTable)
	{
		LV_ITEM  lvItem;
		int      nIndex = 0;
		TCHAR    buff[32];
	
		lvItem.mask = LVIF_TEXT | LVIF_PARAM;
		lvItem.lParam = nIndex;
		lvItem.iItem = nIndex++;
		_stprintf_s(buff, 32, L"%d", DBTable.m_SecurityCode);
		lvItem.pszText = buff;
		lvItem.iSubItem = 0;
		ListCtrl.InsertItem(&lvItem);

		lvItem.iSubItem = 1;
		lvItem.mask = LVIF_TEXT;
		_stprintf_s(buff, 32, L"%s", DBTable.m_SecurityName);
		ListCtrl.SetItem(&lvItem);
	};

	CString strCmd;
	strCmd.Format(L"SELECT SecurityCode, SecurityName FROM CNFundTable WHERE CrossMarket = true ORDER BY SecurityCode");
	RetrieveData<CSecuritiesTable>(strCmd, callback, final_callback);
}
