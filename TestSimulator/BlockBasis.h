#pragma once

//Block��¦���O

class CBlockBasis
{
protected:
	CRect m_rcBlk;	//Block rect

	int m_iBlkNum;	//Block��vector�����s��

public:
	//�غc�B�Ѻc�l
	CBlockBasis ();		
	virtual ~CBlockBasis ();	

	//���o�B�]�wrect
	CRect GetBlkRect () const;	
	virtual void SetBlkRect (CPoint, int, int);	
	virtual CRect GetPinIn1Rect () const;		
	virtual CRect GetPinIn2Rect () const;		
	virtual CRect GetPinOutRect () const;		

	//���o�B�]�wBlock�s��
	int GetBlkNum () const;	
	void SetBlkNum (int);	

	//���o�B�]�w�s����Block������
	virtual CBlockBasis* GetBlkHead1Ptr () const;	
	virtual CBlockBasis* GetBlkHead1Ptr (int) const;
	virtual CBlockBasis* GetBlkHead2Ptr () const;
	virtual void SetBlkHead1Ptr (CBlockBasis*);
	virtual void SetBlkHead2Ptr (CBlockBasis*);
	virtual void DeleteBlkHead1Ptr (int);	//��Block Out�s������vector���M�����w������

	//���o�B�]�w�s����Block���s��
	virtual int GetBlkHead1Num () const;
	virtual int GetBlkHead1Num (int) const;
	virtual int GetBlkHead2Num () const;
	virtual void SetBlkHead1Num (int) ;
	virtual void SetBlkHead2Num (int) ;
	virtual void ClearBlkHead1Num ();		//�M��Block Out�s���s��vector

	//���o�B�]�w�s��Block Out���ƥ�
	virtual int GetBlkHeadSize () const;
	virtual void SetBlkHeadSize (int);

	//���o�B�]�wBlock In, Fun��flag
	virtual int GetInputFlag () const;
	virtual void SetInputFlag (int);
	virtual int GetOperFlag () const;
	virtual void SetOperFlag (int);

	//���o�B�]�wBlock��
	virtual double GetBlkValue () const;
	virtual double GetBlkValue (int) const;	//���oBlock Out���wchannel����
	virtual void SetBlkValue (double);
	virtual void SetBlkValue ();
	virtual int GetValueFlag () const;
	virtual int GetValueFlag (int) const;

	virtual void Move (CPoint);		//����Block
	virtual void Draw (CDC*);		//ø�sBlock
	virtual void DrawLine (CDC*);	//ø�sBlock�s���u

	//�P�_Block����
	virtual int BlkTypeIs () const;
};