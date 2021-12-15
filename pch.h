// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include "framework.h"

#include <vector>
#include <string>
#include <thread>
#include <functional>

#include <ppl.h>

#include "DBTableClass.h"

struct CStockPrice
{
	int m_nDate;
	int m_nOpen;
	int m_nHigh;
	int m_nLow;
	int m_nClose;
	float m_nAmount; // total money
	int m_nVolume; // volumn
	float x;
};

struct CTradingRecord
{
	TCHAR   m_strStockName[32] = { 0 };
	double  m_nBuyAmount;
	LONG	m_nBuyVolume;
	LONG	m_nSoldVolume;
	double	m_nSoldAmount;
	LONG	m_nSellCount;
	LONG	m_nBuyCount;
};

struct security_id
{
	LONG m_nSecurityCode;
	BYTE m_nSecurityType;

	security_id(LONG nSecurityCode, BYTE nSecurityType) : m_nSecurityCode(nSecurityCode), m_nSecurityType(nSecurityType) {}

	bool operator<(const security_id& rhs) const
	{
		if (m_nSecurityCode < rhs.m_nSecurityCode) return false;
		if (m_nSecurityCode > rhs.m_nSecurityCode) return true;
		return m_nSecurityType < rhs.m_nSecurityType;
	}
};


#endif //PCH_H
