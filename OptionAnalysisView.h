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
	std::vector<CStockPrice> m_vecStock;
	CStockPrice m_MonthStockPrice;
	CStockPrice m_PeriodStockPrice;
	bool m_bIndex{ false };
	char m_nLastMonth{ 0 };
	char m_nWednesday{ 0 };

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

private:
	bool IsFourthWednesday(const CStockPrice& current, int i);

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
		m_strIndexFileName = std::string("sh\\lday\\sh510050.day");
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
		m_strIndexFileName = std::string("sh\\lday\\sh510300.day");
	}

	DECLARE_MESSAGE_MAP()
};

class C159915OptionView : public COptionAnalysisBaseView
{
	DECLARE_DYNCREATE(C159915OptionView)
protected:
	C159915OptionView()
	{
		_tcscpy_s(m_strViewTitle, nMaxTitle, _T("创业板ETF期权分析"));
		m_strIndexFileName = std::string("sz\\lday\\sz159915.day");
	}

	DECLARE_MESSAGE_MAP()
};

class C399905OptionView : public COptionAnalysisBaseView
{
	DECLARE_DYNCREATE(C399905OptionView)
protected:
	C399905OptionView()
	{
		_tcscpy_s(m_strViewTitle, nMaxTitle, _T("中证500期权分析"));
		m_strIndexFileName = std::string("sz\\lday\\sz399905.day");
		m_bIndex = true;
	}

	DECLARE_MESSAGE_MAP()
};
