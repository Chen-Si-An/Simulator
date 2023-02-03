#include "stdafx.h"

#include "Complex.h"

#include "math.h"


CComplex::CComplex ()
{
	m_dReal = 0.;
	m_dImag = 0.;
}

CComplex::CComplex (double dReal, double dImag)
{
	m_dReal = dReal;
	m_dImag = dImag;
}

CComplex::~CComplex ()
{
}

CComplex CComplex::operator+ (const CComplex & complexNumber)
{
	double dTempReal = m_dReal + complexNumber.m_dReal;
	double dTempImag = m_dImag + complexNumber.m_dImag;

	return CComplex (dTempReal, dTempImag);
}

CComplex CComplex::operator- (const CComplex & complexNumber)
{
	double dTempReal = m_dReal - complexNumber.m_dReal;
	double dTempImag = m_dImag - complexNumber.m_dImag;

	return CComplex (dTempReal, dTempImag);
}

CComplex CComplex::operator* (const CComplex & complexNumber)
{
	double dTempReal = m_dReal * complexNumber.m_dReal - m_dImag * complexNumber.m_dImag;
	double dTempImag = m_dReal * complexNumber.m_dImag + m_dImag * complexNumber.m_dReal;

	return CComplex (dTempReal, dTempImag);
}

double CComplex::ComputeAmplitude () const
{
	return sqrt (m_dReal * m_dReal + m_dImag * m_dImag);
}