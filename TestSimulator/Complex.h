#pragma once

class CComplex
{
public:
	double m_dReal;	//複數實部
	double m_dImag;	//複數虛部

public:
	//建構、解構子
	CComplex ();
	CComplex (double, double);
	~CComplex ();

	//重載運算元
	CComplex operator+ (const CComplex &);
	CComplex operator- (const CComplex &);
	CComplex operator* (const CComplex &);

	//計算複數振幅大小
	double ComputeAmplitude () const;
};