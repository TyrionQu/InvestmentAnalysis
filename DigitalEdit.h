#pragma once


// CDigitalEdit

class CDigitalEdit : public CEdit
{
	DECLARE_DYNAMIC(CDigitalEdit)

public:
	CDigitalEdit();
	virtual ~CDigitalEdit();

protected:
	DECLARE_MESSAGE_MAP()
public:
	int m_nX;
	int m_nY;
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
};


