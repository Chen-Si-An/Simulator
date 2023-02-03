#pragma once

class CComplex
{
public:
	double m_dReal;	//�Ƽƹ곡
	double m_dImag;	//�ƼƵ곡

public:
	//�غc�B�Ѻc�l
	CComplex ();
	CComplex (double, double);
	~CComplex ();

	//�����B�⤸
	CComplex operator+ (const CComplex &);
	CComplex operator- (const CComplex &);
	CComplex operator* (const CComplex &);

	//�p��ƼƮ��T�j�p
	double ComputeAmplitude () const;
};