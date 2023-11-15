// CCalculateRealRate.cpp : implementation file
//

#include "pch.h"
#include "InvestmentAnalysis.h"
#include "afxdialogex.h"
#include "CalculateRealRate.h"


// CCalculateRealRate dialog

IMPLEMENT_DYNAMIC(CCalculateRealRate, CDialogEx)

CCalculateRealRate::CCalculateRealRate(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_INTEREST_BOX, pParent)
	, m_nMaturityDate(COleDateTime::GetCurrentTime())
	, m_nCurrentValue(0)
	, m_nCouponRate(0)
	, m_nTaxRate(0)
	, m_nRatesByYear(0)
	, m_strRealRates("")
{

}

CCalculateRealRate::~CCalculateRealRate()
{
}

void CCalculateRealRate::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_CURRENT_VALUE, m_editCurrentValue);
	DDX_Control(pDX, IDC_EDIT_COUPON_RATE, m_editCouponRate);
	DDX_Control(pDX, IDC_EDIT_TAX_RATE, m_editTaxRate);
	DDX_Control(pDX, IDC_EDIT_YEAR_RATES, m_editRatesByYear);
	DDX_DateTimeCtrl(pDX, IDC_MATURITY_DATE, m_nMaturityDate);
	DDX_Text(pDX, IDC_EDIT_CURRENT_VALUE, m_nCurrentValue);
	DDV_MinMaxDouble(pDX, m_nCurrentValue, 0, 1000.0);
	DDX_Text(pDX, IDC_EDIT_COUPON_RATE, m_nCouponRate);
	DDV_MinMaxDouble(pDX, m_nCouponRate, 0, 100.0);
	DDX_Text(pDX, IDC_EDIT_TAX_RATE, m_nTaxRate);
	DDV_MinMaxDouble(pDX, m_nTaxRate, 0, 50.0);
	DDX_Text(pDX, IDC_EDIT_YEAR_RATES, m_nRatesByYear);
	DDV_MinMaxUInt(pDX, m_nRatesByYear, 0, 12);
	DDX_Text(pDX, IDC_EDIT_REAL_RATE, m_strRealRates);
}


BEGIN_MESSAGE_MAP(CCalculateRealRate, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_CALCULATE_RATE, &CCalculateRealRate::OnBnClickedBtnCalculateRate)
END_MESSAGE_MAP()


// CCalculateRealRate message handlers
const int MAX_INTEREST = 40;


void CCalculateRealRate::OnBnClickedBtnCalculateRate()
{
	UpdateData(1);

	COleDateTime MaturityDate, today, nextInterestDay;
	today.m_dt = (LONG)COleDateTime::GetTickCount();
	int nYear  = m_nMaturityDate.GetYear();
	int nMonth = m_nMaturityDate.GetMonth();
	int nDay  = m_nMaturityDate.GetDay();
	MaturityDate.SetDate(nYear, nMonth, nDay);

	double nPrice = m_nCurrentValue;
	double nTaxRate = m_nTaxRate;
	double nInterestRate = m_nCouponRate;
	int nInterestTimes = m_nRatesByYear;

	double nUpperInterestRate = MAX_INTEREST / 100.0;
	double nLowerInterestRate = 0;
	double nCurrentRate = nUpperInterestRate / 2;
	double wResult = 0;
	double nError = 0;
	int    i = 0;

	int nMaturityYear = 0;
	nMaturityYear = nYear - today.GetYear();
	nextInterestDay.SetDate(today.GetYear(), nMonth, nDay);
	if (nextInterestDay.m_dt < today.m_dt)
	{
		nMaturityYear--;
		nextInterestDay.SetDate(today.GetYear() + 1, nMonth, nDay);
	}
	int nInterestDay = (int)(nextInterestDay.m_dt - today.m_dt);

    nPrice += nInterestRate * (365 - nInterestDay) / 365;

	do {
		double nDayInterestRate = pow((1 + nCurrentRate / nInterestTimes), (nInterestTimes / 365.0));
		int n = nMaturityYear * nInterestTimes;
		double y = nInterestRate * (1 - nTaxRate / 100) / nInterestTimes;
		wResult = y * (1 - 1 / pow((1 + nCurrentRate), n)) / nCurrentRate;
		wResult += 100 / pow((1 + nCurrentRate), n);

		// Calculate the interest of this year
		while (nInterestDay > 365 / nInterestTimes)
		{
			wResult = (wResult + y) / (1 + nCurrentRate);
			nInterestDay -= 365 / nInterestTimes;
		}
		wResult += y;

		wResult /= pow(nDayInterestRate, nInterestDay);
		nError = wResult - nPrice;
		if (nError < 0)
		{
			nUpperInterestRate = nCurrentRate;
			nCurrentRate -= (nCurrentRate - nLowerInterestRate) / 2;
		}
		else
		{
			nLowerInterestRate = nCurrentRate;
			nCurrentRate += (nUpperInterestRate - nCurrentRate) / 2;
		}
		i++;
		if (i > 1000)
		{
			break;
		}
	} while (fabs(nError) > 0.0001);

	m_strRealRates.Format(L"%.3lf/%.3lf", (pow((1 + nCurrentRate), nInterestTimes) - 1) * 100, nCurrentRate * nInterestTimes * 100);

	UpdateData(0);
}


BOOL CCalculateRealRate::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_nCurrentValue = 100;
	m_nCouponRate = 4.5;
	m_strRealRates.AppendFormat(L"%0.2f/%0.2f", m_nCouponRate, m_nCouponRate);
	m_nTaxRate = 20.0;
	m_nRatesByYear = 1;

	UpdateData(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
