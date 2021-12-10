// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface 
// (the "Fluent UI") and is provided only as referential material to supplement the 
// Microsoft Foundation Classes Reference and related electronic documentation 
// included with the MFC C++ library software.  
// License terms to copy, use or distribute the Fluent UI are available separately.  
// To learn more about our Fluent UI licensing program, please visit 
// https://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "pch.h"
#include "framework.h"
#include "InvestmentAnalysis.h"

#include "ChildFrm.h"
#include "InvestmentAnalysisDoc.h"
#include "TransactionRecordView.h"
#include "TransactionSummariesView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWndEx)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWndEx)
END_MESSAGE_MAP()

// CChildFrame construction/destruction

CChildFrame::CChildFrame() noexcept
{
}

CChildFrame::~CChildFrame()
{
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs
	if( !CMDIChildWndEx::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWndEx::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWndEx::Dump(dc);
}
#endif //_DEBUG

// CChildFrame message handlers


BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	std::function<void(int n, CCommand<CAccessor<CTradingRecordTable> >& DBTable)> callback = [&](int n, CCommand<CAccessor<CTradingRecordTable> >& DBTable)
	{
		auto it = m_tradingRecords.find(DBTable.m_SecurityCode);
		if (it == m_tradingRecords.end())
		{
			std::shared_ptr<CTradingRecord> trading_recode = std::make_shared<CTradingRecord>();
			trading_recode->m_nBuyVolume = (DBTable.m_TradingType == 8 || DBTable.m_TradingType == 1) ? DBTable.m_Volume : 0;
			trading_recode->m_nBuyAmount = DBTable.m_TradingType == 8 ? DBTable.m_Amount : 0;
			trading_recode->m_nBuyCount = DBTable.m_TradingType == 8 ? 1 : 0;
			trading_recode->m_nSellCount = DBTable.m_TradingType == 9 ? 1 : 0;
			trading_recode->m_nSoldVolume = DBTable.m_TradingType == 9 ? DBTable.m_Volume : 0;
			trading_recode->m_nSoldAmount = DBTable.m_TradingType == 9 ? DBTable.m_Amount : 0;
			CString strCmd, strTableName;
			switch (DBTable.m_SecurityType)
			{
			case 1:
				strTableName = "CNStockTable";
				break;
			case 2:
				strTableName = "CNBondTable";
				break;
			case 3:
				strTableName = "CNFundTable";
				break;
			case 99:
				strTableName = "HKStockTable";
				break;
			case 255:
				strTableName = "TerminatedSecuritiesTable";
				break;
			default:
				break;
			}
			strCmd.Format(L"SELECT SecurityName FROM %s WHERE (SecurityCode = %d)", strTableName.GetString(), DBTable.m_SecurityCode);
			HRESULT hr = GetTableTopLine<TCHAR>(strCmd, trading_recode->m_strStockName);
			if (FAILED(hr) || (hr == DB_S_ENDOFROWSET))
			{
				MessageBox(L"当前股票代码不存在");
			}
			m_tradingRecords[DBTable.m_SecurityCode] = trading_recode;
		}
		else
		{
			if (DBTable.m_TradingType == 8)
			{
				it->second->m_nBuyVolume += DBTable.m_Volume;
				it->second->m_nBuyAmount += DBTable.m_Amount;
				it->second->m_nBuyCount++;
			}
			else if (DBTable.m_TradingType == 9)
			{
				it->second->m_nSellCount++;
				it->second->m_nSoldVolume += DBTable.m_Volume;
				it->second->m_nSoldAmount += DBTable.m_Amount;
			}
			else
			{
				it->second->m_nSoldAmount += DBTable.m_Amount;
				it->second->m_nBuyVolume += DBTable.m_Volume;
			}
		}
	};

	std::function<void()> final_callback = [&]()
	{
		//m_comboMarket.SetCurSel(0);
	};

	if (theApp.m_pDocTemplateTrading != pContext->m_pNewDocTemplate)
		return CMDIChildWndEx::OnCreateClient(lpcs, pContext);

	// create splitter window
	if (!m_wndSplitter.CreateStatic(this, 1, 2))
		return FALSE;

	if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CTransactionSummariesView), CSize(400, 100), pContext) ||
		!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CTransactionRecordView), CSize(400, 100), pContext))
	{
		m_wndSplitter.DestroyWindow();
		return FALSE;
	}

	concurrency::task_group taskgroup;

	taskgroup.run_and_wait([&] {
		CString strCmd;
		strCmd.Format(L"SELECT SerialNo, TradingDate, TradingType, AccountBalance, SecurityCode, SecurityType, Price, Volume, Amount FROM TradingRecordTable WHERE TradingType = 1 OR TradingType = 8 OR TradingType = 9 ORDER BY SerialNo");
		RetrieveData<CTradingRecordTable>(strCmd, callback, final_callback);
		});

	return TRUE;
}
