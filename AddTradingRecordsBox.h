#pragma once
#include "afxdialogex.h"
#include "DigitalEdit.h"

// CAddTradingRecordsBox dialog

class CAddTradingRecordsBox : public CDialogEx
{
	DECLARE_DYNAMIC(CAddTradingRecordsBox)

public:
	CAddTradingRecordsBox(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CAddTradingRecordsBox();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TRADINGBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CComboBox m_comboExchangeType;
	CComboBox m_comboMarket;
	CDigitalEdit m_editStockCode;
	CDigitalEdit m_editStockPrice;
	CDigitalEdit m_editCommission;
	CDigitalEdit m_editStampTax;
	CDigitalEdit m_editStockAmount;
	CDigitalEdit m_editTradingAmount;
	UINT m_nStockCode;
	CString m_strStockName;
	double m_nStockPrice;
	double m_nCommission;
	double m_nStampTax;
	int  m_nStockAmount;
	double m_nMinSoldPrice;
	double m_nExchangeMoney;
	double m_nBalance;
	HRESULT InsertTradingRecord();
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboMarketname();
	afx_msg void OnCbnSelchangeComboExchangetype();
	afx_msg void OnEnKillfocusEditStockcode();
	afx_msg void OnClickedBtnNext();
	afx_msg void OnBnClickedOk();
	COleDateTime m_TradingDate;
	afx_msg void OnEnKillfocusEditSecurity();
};
