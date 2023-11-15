#pragma once
#include "afxdialogex.h"
#include "DigitalEdit.h"


// CCalculateRealRate dialog

class CCalculateRealRate : public CDialogEx
{
	DECLARE_DYNAMIC(CCalculateRealRate)

public:
	CCalculateRealRate(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CCalculateRealRate();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INTEREST_BOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnCalculateRate();
	CDigitalEdit m_editCurrentValue;
	CDigitalEdit m_editCouponRate;
	CDigitalEdit m_editTaxRate;
	CDigitalEdit m_editRatesByYear;
	COleDateTime m_nMaturityDate;
	double m_nCurrentValue;
	double m_nCouponRate;
	double m_nTaxRate;
	UINT m_nRatesByYear;
	CString m_strRealRates;
	virtual BOOL OnInitDialog();
};
