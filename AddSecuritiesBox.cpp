// CAddSecuritiesBox.cpp : implementation file
//

#include "pch.h"
#include "InvestmentAnalysis.h"
#include "afxdialogex.h"
#include "AddSecuritiesBox.h"


// CAddSecuritiesBox dialog

const int default_stock_code = 605000;

IMPLEMENT_DYNAMIC(CAddSecuritiesBox, CDialogEx)

CAddSecuritiesBox::CAddSecuritiesBox(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_BASICINFOBOX, pParent)
	, m_nStockCode(0)
	, m_strStockName(_T(""))
{

}

CAddSecuritiesBox::~CAddSecuritiesBox()
{
}

void CAddSecuritiesBox::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_STOCKTYPE, m_comboSecuritiesType);
	DDX_Text(pDX, IDC_EDIT_STOCK_CODE, m_nStockCode);
	DDV_MinMaxUInt(pDX, m_nStockCode, 0, 999999);
	DDX_Text(pDX, IDC_EDIT_STOCK_NAME, m_strStockName);
	DDV_MaxChars(pDX, m_strStockName, 16);
	DDX_Control(pDX, IDC_EDIT_STOCK_CODE, m_editStockCode);
}


BEGIN_MESSAGE_MAP(CAddSecuritiesBox, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_NEXT, &CAddSecuritiesBox::OnBnClickedBtnNext)
	ON_BN_CLICKED(IDOK, &CAddSecuritiesBox::OnBnClickedOk)
END_MESSAGE_MAP()


// CAddSecuritiesBox message handlers


BOOL CAddSecuritiesBox::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	std::function<void(int n, CCommand<CAccessor<CSecuritiesTypeTable> >& DBTable)> callback = [&](int n, CCommand<CAccessor<CSecuritiesTypeTable> >& DBTable)
	{
		m_comboSecuritiesType.InsertString(n, DBTable.m_SecuritiesType);
	};

	std::function<void(int n)> final_callback = [&](int n)
	{
		m_comboSecuritiesType.SetCurSel(0);
	};

	CString strCmd(L"SELECT SerialNo, TypeName FROM SecuritiesTypeTable ORDER BY SerialNo");
	concurrency::task_group taskgroup;

	taskgroup.run_and_wait([&] {
		RetrieveData<CSecuritiesTypeTable>(strCmd, callback, final_callback);
		}
	);

	m_nStockCode = default_stock_code;
	UpdateData(0);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CAddSecuritiesBox::OnBnClickedBtnNext()
{
	CString strMsg;
	strMsg.Format(L"Insert security %s %06d %s!",
		m_strStockName.GetString(), m_nStockCode,
		FAILED(AddNewSecurity()) ? L"FAILED" : L"succeed");
	MessageBox(strMsg);
}


void CAddSecuritiesBox::OnBnClickedOk()
{
	if (FAILED(AddNewSecurity()))
	{
		MessageBox(L"insert FAILED!");
		return;
	}
	CDialogEx::OnOK();
}


HRESULT CAddSecuritiesBox::AddNewSecurity()
{
	UpdateData(1);
	int nSel = m_comboSecuritiesType.GetCurSel();
	CString securitiesTableName;
	switch (nSel)
	{
	case 0:
		securitiesTableName = "CNStockTable";
		break;
	case 1:
		securitiesTableName = "CNBondTable";
		break;
	case 2:
		securitiesTableName = "CNFundTable";
		break;
	case 3:
		securitiesTableName = "CNOptionTable";
		break;
	case 4:
		securitiesTableName = "HKStockTable";
		break;
	case 5:
		securitiesTableName = "TerminatedSecuritiesTable";
		break;
	default:
		break;
	}

	CString strCmd;
	strCmd.Format(L"INSERT INTO %s (SecurityCode, SecurityName) VALUES (%d, \"%s\")",
		securitiesTableName.GetString(), m_nStockCode, m_strStockName.GetString());
	HRESULT hr = UpdateDatabaseTable<CSecuritiesTable>(strCmd);

	return hr;
}
