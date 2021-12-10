#pragma once
#include "afxdialogex.h"
#include "DigitalEdit.h"


// CAddSecuritiesBox dialog

class CAddSecuritiesBox : public CDialogEx
{
	DECLARE_DYNAMIC(CAddSecuritiesBox)

public:
	CAddSecuritiesBox(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CAddSecuritiesBox();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BASICINFOBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox m_comboSecuritiesType;
	afx_msg void OnBnClickedBtnNext();
	afx_msg void OnBnClickedOk();
private:
	HRESULT AddNewSecurity();
	UINT m_nStockCode;
	CString m_strStockName;
	CDigitalEdit m_editStockCode;
};
