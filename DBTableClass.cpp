#include "pch.h"

#include "DBTableClass.h"

CDataSource dbconn;

HRESULT OpenDatabase()
{
	return dbconn.OpenFromInitializationString(L"Provider=Microsoft.ACE.OLEDB.12.0;Data Source=TransactionRecords.accdb;");
}

void CloseDatabase()
{
	dbconn.Close();
}

HRESULT SetDBProperties(CSession* session, CDBPropSet* pPropSet, bool bUpdate)
{
	HRESULT hr;

	hr = session->Open(dbconn);
	if (FAILED(hr))
		return hr;

	if (pPropSet == nullptr)
		return hr;

	bool ret = pPropSet->AddProperty(DBPROP_CANFETCHBACKWARDS, true, DBPROPOPTIONS_OPTIONAL);
	ret = pPropSet->AddProperty(DBPROP_CANSCROLLBACKWARDS, true, DBPROPOPTIONS_OPTIONAL);

	if (!bUpdate) return hr;

	ret = pPropSet->AddProperty(DBPROP_IRowsetChange, true, DBPROPOPTIONS_OPTIONAL);
	ret = pPropSet->AddProperty(DBPROP_UPDATABILITY, DBPROPVAL_UP_CHANGE | DBPROPVAL_UP_INSERT | DBPROPVAL_UP_DELETE);
	return hr;
}