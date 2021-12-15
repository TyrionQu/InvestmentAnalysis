#pragma once


// CTransactionSummariesView view

class CTransactionSummariesView : public CView
{
	DECLARE_DYNCREATE(CTransactionSummariesView)

protected:
	CTransactionSummariesView();           // protected constructor used by dynamic creation
	virtual ~CTransactionSummariesView();

public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};


