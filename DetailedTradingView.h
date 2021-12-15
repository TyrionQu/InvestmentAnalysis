#pragma once

#include "SortableListView.h"

// CDetailedTradingView view

class CDetailedTradingView : public CSortableListView
{
	DECLARE_DYNCREATE(CDetailedTradingView)

protected:
	CDetailedTradingView();           // protected constructor used by dynamic creation
	virtual ~CDetailedTradingView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

private:
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	void Refresh(const security_id &sid);
};


