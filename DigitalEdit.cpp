// DigitalEdit.cpp : implementation file
//

#include "pch.h"
#include "InvestmentAnalysis.h"
#include "DigitalEdit.h"


// CDigitalEdit

IMPLEMENT_DYNAMIC(CDigitalEdit, CEdit)

CDigitalEdit::CDigitalEdit()
	: m_nX(0), m_nY(0)
{
}

CDigitalEdit::~CDigitalEdit()
{
}


BEGIN_MESSAGE_MAP(CDigitalEdit, CEdit)
	ON_WM_CHAR()
	ON_WM_WINDOWPOSCHANGING()
END_MESSAGE_MAP()



// CDigitalEdit message handlers



void CDigitalEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == '.')
	{
		CString strContent;
		GetWindowText(strContent);

		if (strContent.Find('.') >= 0)
			return;
	}
	else if ((nChar - '0' < 0 || nChar - '0' > 9) && nChar != VK_BACK)
		return;

	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

void CDigitalEdit::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	lpwndpos->x = m_nX;
	lpwndpos->y = m_nY;

	CEdit::OnWindowPosChanging(lpwndpos);
}
