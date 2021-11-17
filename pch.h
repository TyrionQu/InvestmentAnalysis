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


#endif //PCH_H
