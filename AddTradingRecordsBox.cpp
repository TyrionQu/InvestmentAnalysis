// AddTradingRecordsBox.cpp : implementation file
//

#include "pch.h"
#include "InvestmentAnalysis.h"
#include "afxdialogex.h"
#include "AddTradingRecordsBox.h"


// CAddTradingRecordsBox dialog

IMPLEMENT_DYNAMIC(CAddTradingRecordsBox, CDialogEx)

CAddTradingRecordsBox::CAddTradingRecordsBox(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TRADINGBOX, pParent)
	, m_nStockCode(0)
	, m_strStockName(_T(""))
	, m_nStockPrice(0)
	, m_nCommission(0)
	, m_nStampTax(0)
	, m_nStockAmount(0)
	, m_nMinSoldPrice(0)
	, m_nExchangeMoney(0)
	, m_nBalance(0)
	, m_TradingDate(COleDateTime::GetCurrentTime())
{

}

CAddTradingRecordsBox::~CAddTradingRecordsBox()
{
}

void CAddTradingRecordsBox::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_EXCHANGETYPE, m_comboExchangeType);
	DDX_Control(pDX, IDC_COMBO_MARKETNAME, m_comboMarket);
	DDX_Control(pDX, IDC_EDIT_STOCKCODE, m_editStockCode);
	DDX_Control(pDX, IDC_EDIT_STOCKPRICE, m_editStockPrice);
	DDX_Control(pDX, IDC_EDIT_COMMISSION, m_editCommission);
	DDX_Control(pDX, IDC_EDIT_STAMPTAX, m_editStampTax);
	DDX_Control(pDX, IDC_EDIT_STOCKAMOUNT, m_editStockAmount);
	DDX_Control(pDX, IDC_EDIT_EXCHANGEMONEY, m_editTradingAmount);
	DDX_Text(pDX, IDC_EDIT_STOCKCODE, m_nStockCode);
	DDV_MinMaxUInt(pDX, m_nStockCode, 1, 999999);
	DDX_Text(pDX, IDC_EDIT_STOCKNAME, m_strStockName);
	DDV_MaxChars(pDX, m_strStockName, 32);
	DDX_Text(pDX, IDC_EDIT_STOCKPRICE, m_nStockPrice);
	DDV_MinMaxDouble(pDX, m_nStockPrice, -10, DBL_MAX);
	DDX_Text(pDX, IDC_EDIT_COMMISSION, m_nCommission);
	DDV_MinMaxDouble(pDX, m_nCommission, 0.0, 0.1);
	DDX_Text(pDX, IDC_EDIT_STAMPTAX, m_nStampTax);
	DDV_MinMaxDouble(pDX, m_nStampTax, 0, 0.1);
	DDX_Text(pDX, IDC_EDIT_STOCKAMOUNT, m_nStockAmount);
	DDV_MinMaxUInt(pDX, m_nStockAmount, 0, 1000000);
	DDX_Text(pDX, IDC_EDIT_MINSOLDPRICE, m_nMinSoldPrice);
	DDX_Text(pDX, IDC_EDIT_EXCHANGEMONEY, m_nExchangeMoney);
	DDX_Text(pDX, IDC_EDIT_ACCOUNTMONEY, m_nBalance);
	DDX_DateTimeCtrl(pDX, IDC_EXCHANGE_DATE, m_TradingDate);
}


BEGIN_MESSAGE_MAP(CAddTradingRecordsBox, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_MARKETNAME, &CAddTradingRecordsBox::OnCbnSelchangeComboMarketname)
	ON_CBN_SELCHANGE(IDC_COMBO_EXCHANGETYPE, &CAddTradingRecordsBox::OnCbnSelchangeComboExchangetype)
	ON_EN_KILLFOCUS(IDC_EDIT_STOCKCODE, &CAddTradingRecordsBox::OnEnKillfocusEditStockcode)
	ON_BN_CLICKED(IDC_BTN_NEXT, &CAddTradingRecordsBox::OnClickedBtnNext)
	ON_BN_CLICKED(IDOK, &CAddTradingRecordsBox::OnBnClickedOk)
	ON_EN_KILLFOCUS(IDC_EDIT_STOCKPRICE, &CAddTradingRecordsBox::OnEnKillfocusEditSecurity)
	ON_EN_KILLFOCUS(IDC_EDIT_STOCKAMOUNT, &CAddTradingRecordsBox::OnEnKillfocusEditSecurity)
END_MESSAGE_MAP()


// CAddTradingRecordsBox message handlers


BOOL CAddTradingRecordsBox::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	std::function<void(int n, CCommand<CAccessor<CSecuritiesTypeTable> >& DBTable)> callback = [&](int n, CCommand<CAccessor<CSecuritiesTypeTable> >& DBTable)
	{
		m_comboMarket.InsertString(n, DBTable.m_SecuritiesType);
	};

	std::function<void(int n)> final_callback = [&](int n)
	{
		m_comboMarket.SetCurSel(0);
	};

	std::function<void(int n, CCommand<CAccessor<CTradingTypeTable> >& DBTable)> trading_callback = [&](int n, CCommand<CAccessor<CTradingTypeTable> >& DBTable)
	{
		m_comboExchangeType.InsertString(n, DBTable.m_TradingName);
	};

	std::function<void(int n)> trading_final_callback = [&](int n)
	{
		m_comboExchangeType.SetCurSel(7);
	};

	concurrency::task_group taskgroup;

	taskgroup.run_and_wait([&] {
		CString strCmd;
		strCmd.Format(L"SELECT AccountBalance FROM TradingRecordTable ORDER BY SerialNo DESC");
		GetTableTopLine<double>(strCmd, &m_nBalance);
		strCmd.Format(L"SELECT SerialNo, TypeName FROM SecuritiesTypeTable ORDER BY SerialNo");
		RetrieveData<CSecuritiesTypeTable>(strCmd, callback, final_callback);
		strCmd.Format(L"SELECT SerialNo, TradingName FROM TradingTypeTable ORDER BY SerialNo");
		RetrieveData<CTradingTypeTable>(strCmd, trading_callback, trading_final_callback);
	});

	m_nStockCode = 600000;
	m_strStockName = "浦发银行";
	m_nStockPrice = 8.68;
	m_nCommission = 0.0005;
	m_nStampTax = 0.001;
	m_nStockAmount = 100;
	m_nMinSoldPrice = 0.0;

	UpdateData(0);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CAddTradingRecordsBox::OnCbnSelchangeComboMarketname()
{
	UpdateData(1);
	UINT nSel = m_comboMarket.GetCurSel();
	switch (nSel)
	{
	case 0:
		m_nStockCode = 600000;
		m_strStockName = "浦发银行";
		m_nStockPrice = 8.68;
		m_nStampTax = 0.001;
		m_nCommission = 0.0005;
		break;
	case 1:
		m_nStockCode = 101619;
		m_strStockName = "国债1619";
		m_nStockPrice = 98.98;
		m_nStampTax = 0.0;
		break;
	case 2:
		m_nStockCode = 510300;
		m_strStockName = "沪深300ETF";
		m_nStockPrice = 5.018;
		m_nStampTax = 0.0;
		m_nCommission = 0.0003;
		break;
	case 3:
		m_nStockCode = 510050;
		m_strStockName = "上证50ETF";
		m_nStockPrice = 1.00;
		m_nStampTax = 0.0;
		m_nCommission = 0.0;
		break;
	case 4:
		m_nStockCode = 700;
		m_strStockName = "腾讯控股";
		m_nStockPrice = 458.60;
		break;
	case 5:
		m_nStockCode = 9;
		m_strStockName = "无法交易";
		m_nStockPrice = 0.0;
		break;
	default:
		break;
	}
	m_nExchangeMoney = m_nStockPrice * m_nStockAmount;
	UpdateData(0);
}


void CAddTradingRecordsBox::OnCbnSelchangeComboExchangetype()
{
	UINT nSel = m_comboExchangeType.GetCurSel();

	// The following options have nothing with the specific stock code.
	if (nSel == 1 || nSel == 2 || nSel == 5 || nSel == 6)
	{
		m_editStockCode.EnableWindow(FALSE);
		m_editStockPrice.EnableWindow(FALSE);
		m_editStampTax.EnableWindow(FALSE);
		m_editCommission.EnableWindow(FALSE);
		m_editStockAmount.EnableWindow(FALSE);
	}
	else
	{
		m_editStockCode.EnableWindow(TRUE);
		m_editStockPrice.EnableWindow(TRUE);
		m_editStampTax.EnableWindow(TRUE);
		m_editCommission.EnableWindow(TRUE);
		m_editStockAmount.EnableWindow(TRUE);
	}
}


void CAddTradingRecordsBox::OnEnKillfocusEditStockcode()
{
	UpdateData(TRUE);
	int nSel = m_comboMarket.GetCurSel();
	CString securitiesTableName;
	switch (nSel)
	{
	case 0:
		securitiesTableName = L"CNStockTable";
		break;
	case 1:
		securitiesTableName = L"CNBondTable";
		break;
	case 2:
		securitiesTableName = L"CNFundTable";
		break;
	case 3:
		securitiesTableName = L"CNOptionTable";
		break;
	case 4:
		securitiesTableName = L"HKStockTable";
		break;
	case 5:
		securitiesTableName = L"TerminatedSecuritiesTable";
		break;
	default:
		break;
	}

	CString strCmd;
	strCmd.Format(L"SELECT SecurityName FROM %s WHERE (SecurityCode = %d)", securitiesTableName.GetString(), m_nStockCode);

	TCHAR buffer[32] = {0};
	HRESULT hr = GetTableTopLine<TCHAR>(strCmd, buffer);
	if (hr == DB_S_ENDOFROWSET)
	{
		MessageBox(L"Stock code don't exist!");
		// TODO: Here should do something to get focus on this edit
		return;
	}
	m_strStockName = buffer;
	UpdateData(0);
}


void CAddTradingRecordsBox::OnClickedBtnNext()
{
	HRESULT hr = InsertTradingRecord();
	if (FAILED(hr))
	{
		MessageBox(L"Failed in writing database!");
		return;
	}

	if (hr != S_OK)
		return;

	concurrency::task_group taskgroup;

	taskgroup.run_and_wait([&] {
		CString strCmd;
		strCmd.Format(L"SELECT AccountBalance FROM TradingRecordTable ORDER BY SerialNo DESC");
		GetTableTopLine<double>(strCmd, &m_nBalance);
		});
	UpdateData(0);
}


void CAddTradingRecordsBox::OnBnClickedOk()
{
	HRESULT hr = InsertTradingRecord();
	if (FAILED(hr))
	{
		MessageBox(L"Failed in writing database!");
		return;
	}
	if (hr == S_OK)
		CDialogEx::OnOK();
}


HRESULT CAddTradingRecordsBox::InsertTradingRecord()
{
	UpdateData(1);
	int nIndex = m_comboExchangeType.GetCurSel() + 1;
	double  nExchangeMoney = m_nExchangeMoney;
	int     nStockAmount = m_nStockAmount;
	CString strCmd, strType;

	int nMarketType = m_comboMarket.GetCurSel();
	int nSecurityType = 0;
	switch (nMarketType)
	{
	case 0: // 沪深京股票
		nSecurityType = 1;
		break;
	case 1: // 沪深债券
		nSecurityType = 2;
		break;
	case 2: // 沪深基金
		nSecurityType = 3;
		break;
	case 3: // 沪深期权
		nSecurityType = 4;
		break;
	case 4: // 香港股票
		nSecurityType = 99;
		break;
	case 5: // 退市证券
		nSecurityType = 255;
		break;
	}

	switch (nIndex)
	{
	case 1: // 股息入帐
		m_nStockPrice = 0;
		strType.Format(L"股息入帐");
		break;
	case 2: // 利税代扣
		strType.Format(L"利税代扣");
		nExchangeMoney = -m_nExchangeMoney;
		m_nStockCode = 0;
		m_nStockPrice = 0;
		nStockAmount = 0;
		nSecurityType = 0;
		break;
	case 3: // 利息归本
		strType.Format(L"利息归本");
		m_nStockCode = 0;
		m_nStockPrice = 0;
		nStockAmount = 0;
		nSecurityType = 0;
		break;
	case 4: // 申购返款
		strType.Format(L"申购返款");
		break;
	case 5: // 新股申购
		strType.Format(L"新股申购");
		nExchangeMoney = -m_nExchangeMoney;
		break;
	case 6: // 银行转存
		strType.Format(L"银行转存");
		m_nStockCode = 0;
		m_nStockPrice = 0;
		nStockAmount = 0;
		nSecurityType = 0;
		break;
	case 7: // 银行转取
		strType.Format(L"银行转取");
		nExchangeMoney = -m_nExchangeMoney;
		m_nStockCode = 0;
		m_nStockPrice = 0;
		nStockAmount = 0;
		nSecurityType = 0;
		break;
	case 8: // 证券买入
		strType.Format(L"证券买入");
		nExchangeMoney = -m_nExchangeMoney;
		break;
	case 9: // 证券卖出
		strType.Format(L"证券卖出");
		nStockAmount = -m_nStockAmount;
		break;
	default:
		break;
	}

	double nNewBalance = nExchangeMoney + m_nBalance;
	m_TradingDate.m_dt = int(m_TradingDate.m_dt);
	strCmd.Format(L"交易日期：%d年%d月%d日\n交易类型：%d(%s)\n股票代码：%06d\n账户余额：%0.2lf\n股票价格：%0.3lf\n买卖数量：%d\n交易金额：%0.2lf",
		m_TradingDate.GetYear(), m_TradingDate.GetMonth(), m_TradingDate.GetDay(), nIndex, strType.GetString(), m_nStockCode, nNewBalance, m_nStockPrice, nStockAmount, nExchangeMoney);

	int nRet = MessageBox(strCmd, L"待插入的交易信息", MB_YESNO);
	if (nRet == IDNO)
	{
		MessageBox(L"Reenter your trading information ...");
		return IDNO;
	}
	strCmd.Empty();

	strCmd.Format(L"INSERT INTO TradingRecordTable (TradingDate, TradingType, SecurityCode, SecurityType, AccountBalance, Price, Volume, Amount) VALUES (%f, %d, %d, %d, %lf, %lf, %d, %lf)",
		m_TradingDate.m_dt, nIndex, m_nStockCode, nSecurityType, nNewBalance, m_nStockPrice, nStockAmount, nExchangeMoney);

	HRESULT hr = UpdateDatabaseTable<CTradingRecordTable>(strCmd);

	return hr;
}


void CAddTradingRecordsBox::OnEnKillfocusEditSecurity()
{
	UpdateData(1);
	int nIndex = m_comboExchangeType.GetCurSel() + 1;
	if (nIndex != 8 && nIndex != 9)
		return;

	double nTradingAmount = m_nStockPrice * m_nStockAmount;
	double nCommission = nTradingAmount * m_nCommission;
	nCommission = nCommission > 5 ? nCommission : 5;
	double nTax = nTradingAmount * m_nStampTax;

	if (nIndex == 8)
	{
		m_nExchangeMoney = nTradingAmount + nCommission;
		double nMinSoldMoney = m_nExchangeMoney / (1 - m_nStampTax - m_nCommission);
		m_nMinSoldPrice = double(int((nMinSoldMoney * 1000) / m_nStockAmount + 0.5)) / 1000;
	}
	else
	{
		m_nExchangeMoney = nTradingAmount - nCommission - nTax;
	}
	m_nExchangeMoney = double(int(m_nExchangeMoney * 100 + 0.5)) / 100;

	UpdateData(0);
}
