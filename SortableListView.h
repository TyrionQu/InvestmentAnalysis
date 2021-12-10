#pragma once


// CSortableListView view

class CSortableListView : public CListView
{
	DECLARE_DYNCREATE(CSortableListView)

protected:
	CSortableListView();           // protected constructor used by dynamic creation
	virtual ~CSortableListView();
	LONG    m_nCurrentSortedColumn{ -1 };
	LONG    m_nCurrtentSortedType{ 0 };

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLvnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdraw(NMHDR* pNMHDR, LRESULT* pResult);
};


