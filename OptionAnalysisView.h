#pragma once

#include "SortableListView.h"

// COptionAnalysisBaseView view

const unsigned int nMaxTitle = 32;

class COptionAnalysisBaseView : public CSortableListView
{
	DECLARE_DYNCREATE(COptionAnalysisBaseView)

protected:
	COptionAnalysisBaseView();           // protected constructor used by dynamic creation
	virtual ~COptionAnalysisBaseView();

	TCHAR m_strViewTitle[nMaxTitle] = { 0 };
	std::string m_strIndexFileName{ "" };
	std::vector<CStockPrice> m_vecSocket;

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

private:
	bool IsFourthMidweek(int nDay);

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnLvnGetInfoTip(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdraw(NMHDR* pNMHDR, LRESULT* pResult);
};

class C510050OptionView : public COptionAnalysisBaseView
{
	DECLARE_DYNCREATE(C510050OptionView)
protected:
	C510050OptionView()
	{
		_tcscpy_s(m_strViewTitle, nMaxTitle, _T("50ETF期权分析"));
		m_strIndexFileName = std::string("sh510050.day");
	}

	DECLARE_MESSAGE_MAP()
};

class C510300OptionView : public COptionAnalysisBaseView
{
	DECLARE_DYNCREATE(C510300OptionView)
protected:
	C510300OptionView()
	{
		_tcscpy_s(m_strViewTitle, nMaxTitle, _T("300ETF期权分析"));
		m_strIndexFileName = std::string("sh510300.day");
	}

	DECLARE_MESSAGE_MAP()
};
