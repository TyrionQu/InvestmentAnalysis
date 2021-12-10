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
	DDV_MinMaxUInt(pDX, m_nStockAmount, 1, 1000000);
	DDX_Text(pDX, IDC_EDIT_MINSOLDPRICE, m_nMinSoldPrice);
	DDX_Text(pDX, IDC_EDIT_EXCHANGEMONEY, m_nExchangeMoney);
	DDX_Text(pDX, IDC_EDIT_ACCOUNTMONEY, m_nBalance);
}


BEGIN_MESSAGE_MAP(CAddTradingRecordsBox, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_MARKETNAME, &CAddTradingRecordsBox::OnCbnSelchangeComboMarketname)
	ON_CBN_SELCHANGE(IDC_COMBO_EXCHANGETYPE, &CAddTradingRecordsBox::OnCbnSelchangeComboExchangetype)
	ON_EN_KILLFOCUS(IDC_EDIT_STOCKCODE, &CAddTradingRecordsBox::OnEnKillfocusEditStockcode)
	ON_BN_CLICKED(IDC_BTN_NEXT, &CAddTradingRecordsBox::OnClickedBtnNext)
	ON_BN_CLICKED(IDOK, &CAddTradingRecordsBox::OnBnClickedOk)
END_MESSAGE_MAP()


// CAddTradingRecordsBox message handlers


BOOL CAddTradingRecordsBox::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	std::function<void(int n, CCommand<CAccessor<CSecuritiesTypeTable> >& DBTable)> callback = [&](int n, CCommand<CAccessor<CSecuritiesTypeTable> >& DBTable)
	{
		m_comboMarket.InsertString(n, DBTable.m_SecuritiesType);
	};

	std::function<void()> final_callback = [&]()
	{
		m_comboMarket.SetCurSel(0);
	};

	std::function<void(int n, CCommand<CAccessor<CTradingTypeTable> >& DBTable)> trading_callback = [&](int n, CCommand<CAccessor<CTradingTypeTable> >& DBTable)
	{
		m_comboExchangeType.InsertString(n, DBTable.m_TradingName);
	};

	std::function<void()> trading_final_callback = [&]()
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
	m_strStockName = "�ַ�����";
	m_nStockPrice = 8.68;
	m_nCommission = 0.00075;
	m_nStampTax = 0.001;
	m_nStockAmount = 100;
	m_nMinSoldPrice = 0.0;
	m_nExchangeMoney = 868;

	UpdateData(0);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CAddTradingRecordsBox::OnCbnSelchangeComboMarketname()
{
	UINT nSel = m_comboMarket.GetCurSel();
	switch (nSel)
	{
	case 0:
		m_nStockCode = 600000;
		m_strStockName = "�ַ�����";
		m_nStockPrice = 8.68;
		m_nStampTax = 0.001;
		break;
	case 1:
		m_nStockCode = 101619;
		m_strStockName = "��ծ1619";
		m_nStockPrice = 98.98;
		m_nStampTax = 0.0;
		break;
	case 2:
		m_nStockCode = 510300;
		m_strStockName = "����300ETF";
		m_nStockPrice = 5.018;
		m_nStampTax = 0.0;
		break;
	case 3:
		m_nStockCode = 700;
		m_strStockName = "��Ѷ�ع�";
		m_nStockPrice = 458.60;
		break;
	case 4:
		m_nStockCode = 9;
		m_strStockName = "�޷�����";
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
		m_editTradingAmount.EnableWindow(FALSE);
	}
	else
	{
		m_editStockCode.EnableWindow(TRUE);
		m_editStockPrice.EnableWindow(TRUE);
		m_editStampTax.EnableWindow(TRUE);
		m_editCommission.EnableWindow(TRUE);
		m_editTradingAmount.EnableWindow(TRUE);
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
		securitiesTableName = L"HKStockTable";
		break;
	case 4:
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
	CDialogEx::OnOK();
}


HRESULT CAddTradingRecordsBox::InsertTradingRecord()
{
	// TODO: Add your implementation code here.
	return S_OK;
}
