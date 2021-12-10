#pragma once

#include <atlbase.h>
#include <atldbcli.h>


class CSecuritiesTypeTable
{
public:
	LONG  m_SerialNo;
	TCHAR m_SecuritiesType[11];

	BEGIN_COLUMN_MAP(CSecuritiesTypeTable)
		COLUMN_ENTRY(1, m_SerialNo)
		COLUMN_ENTRY(2, m_SecuritiesType)
	END_COLUMN_MAP()
};

class CTradingTypeTable
{
public:
	LONG  m_SerialNo;
	TCHAR m_TradingName[11];

	BEGIN_COLUMN_MAP(CTradingTypeTable)
		COLUMN_ENTRY(1, m_SerialNo)
		COLUMN_ENTRY(2, m_TradingName)
	END_COLUMN_MAP()
};

class CSecuritiesTable
{
public:
	LONG  m_SecurityCode;
	TCHAR m_SecurityName[16];

	BEGIN_COLUMN_MAP(CSecuritiesTable)
		COLUMN_ENTRY(1, m_SecurityCode)
		COLUMN_ENTRY(2, m_SecurityName)
	END_COLUMN_MAP()
};

class CTradingRecordTable
{
public:
	LONG   m_SerialNo;
	DATE   m_TradingDate;
	LONG   m_TradingType;
	double m_AccountBalance;
	LONG   m_SecurityCode;
	BYTE   m_SecurityType;
	double m_Price;
	LONG   m_Volume;
	double m_Amount;

	BEGIN_COLUMN_MAP(CTradingRecordTable)
		COLUMN_ENTRY(1, m_SerialNo)
		COLUMN_ENTRY(2, m_TradingDate)
		COLUMN_ENTRY(3, m_TradingType)
		COLUMN_ENTRY(4, m_AccountBalance)
		COLUMN_ENTRY(5, m_SecurityCode)
		COLUMN_ENTRY(6, m_SecurityType)
		COLUMN_ENTRY(7, m_Price)
		COLUMN_ENTRY(8, m_Volume)
		COLUMN_ENTRY(9, m_Amount)
	END_COLUMN_MAP()
};

HRESULT OpenDatabase();
void CloseDatabase();
HRESULT SetDBProperties(CSession* session, CDBPropSet* pPropSet, bool bUpdate=false);

template <class TAccessor>
HRESULT UpdateDatabaseTable(CString strCommand)
{
	CSession session;
	CCommand<CAccessor<TAccessor> > UpdateCommand;
	CDBPropSet ps(DBPROPSET_ROWSET);
	HRESULT hr;

	hr = SetDBProperties(&session, &ps, true);
	if (FAILED(hr)) return hr;

	hr = UpdateCommand.Open(session, strCommand, &ps, 0, DBGUID_DEFAULT, false);

	UpdateCommand.Close();
	UpdateCommand.ReleaseCommand();
	session.Close();
	return hr;
}

template<class T>
HRESULT GetTableTopLine(CString strCommand, T* pResult, int nCol = 1)
{
	CSession    session;
	CDBPropSet  ps(DBPROPSET_ROWSET);

	HRESULT hr = SetDBProperties(&session, &ps);
	if (FAILED(hr))
		return hr;
	CCommand<CDynamicAccessor> rs;
	try {
		hr = rs.Open(session, strCommand);
		hr = rs.MoveFirst();
		if (hr != DB_S_ENDOFROWSET)
		{
			DBTYPE type;
			rs.GetColumnType(1, &type);
			switch (type)
			{
			case DBTYPE_I2:
			case DBTYPE_I4:
			case DBTYPE_R4:
			case DBTYPE_R8:
			case DBTYPE_CY:
			case DBTYPE_BOOL:
			case DBTYPE_I1:
			case DBTYPE_UI2:
			case DBTYPE_UI4:
			case DBTYPE_I8:
			case DBTYPE_UI8:
			case DBTYPE_NUMERIC:
				rs.GetValue(rs.GetColumnName(nCol), pResult);
				break;
			case DBTYPE_STR:
			case DBTYPE_WSTR:
			{
				DBLENGTH nlen;
				rs.GetLength(rs.GetColumnName(nCol), &nlen);
				DBORDINAL nColumn = nCol;
				rs.TranslateColumnNo(nColumn);
				TCHAR* p = (TCHAR*)rs._GetDataPtr(nColumn);
				_tcscpy_s((TCHAR*)pResult, nlen, p);
				break;
			}
			default:
				break;
			}
		}
	}
	catch (...)
	{
	}
	rs.Close();
	rs.ReleaseCommand();
	session.Close();
	return hr;
}

template <class TAccessor>
HRESULT RetrieveData(CString strCommand, std::function<void(int n, CCommand<CAccessor<TAccessor> >& DBTable)> const& callback, std::function<void()> final_callback)
{
	CCommand<CAccessor<TAccessor> > DBTable;
	CSession    session;
	CDBPropSet  ps(DBPROPSET_ROWSET);

	HRESULT hr = SetDBProperties(&session, &ps);
	if (FAILED(hr))
		return hr;

	hr = DBTable.Open(session, strCommand, &ps);
	if (FAILED(hr))
	{
		session.Close();
		return hr;
	}

	hr = DBTable.MoveFirst();

	int index = 0;
	while (SUCCEEDED(hr) && (hr != DB_S_ENDOFROWSET))
	{
		callback(index, DBTable);

		hr = DBTable.MoveNext();
		index++;
	}

	DBTable.Close();
	DBTable.ReleaseCommand();
	session.Close();

	final_callback();

	return S_OK;
}