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
	int m_nOpen{ 0 };
	int m_nHigh{ 0 };
	int m_nLow{ MAXINT };
	int m_nClose;
	float m_nAmount{ 0.0 }; // total money
	int m_nVolume{ 0 }; // volumn
	float x {0.0};

	void update(const CStockPrice& rhs)
	{
		if (m_nOpen == 0)
			m_nOpen = rhs.m_nOpen;
		if (m_nHigh < rhs.m_nHigh)
			m_nHigh = rhs.m_nHigh;
		if (m_nLow > rhs.m_nLow)
			m_nLow = rhs.m_nLow;
		m_nClose = rhs.m_nClose;
		if (x == 0.0)
			x = rhs.x;
	}

	void reset()
	{
		m_nOpen = 0;
		m_nHigh = 0;
		m_nLow = MAXINT;
		m_nAmount = 0.0;
		m_nVolume = 0;
		x = 0.0;
	}
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
