#pragma once

//Block�l�����O�GBlock In

#include "BlockBasis.h"

class CBlockIn : public CBlockBasis
{
private:
	//Block rect(�s�b�@�ӿ�XPin)
	CRect m_rcPinOut;

	//��J�i��flag
	int m_iInputFlag;

	//Block��
	double m_dBlkValue;

public:
	//�غc�B�Ѻc�l
	CBlockIn ();
	virtual ~CBlockIn ();

	//���o�B�]�wrect
	virtual void SetBlkRect (CPoint, int, int);
	virtual CRect GetPinOutRect () const;

	//���o�B�]�w��J�i��flag
	virtual int GetInputFlag () const;
	virtual void SetInputFlag (int);

	//���o�B�]�wBlock��
	virtual double GetBlkValue () const;
	virtual void SetBlkValue (double);
	virtual void SetBlkValue ();
	virtual int GetValueFlag () const;	//���oValue�X��

	virtual void Move (CPoint);	//����Block
	virtual	void Draw (CDC*);	//ø�sBlock

	//�P�_Block����
	virtual int BlkTypeIs () const;
};