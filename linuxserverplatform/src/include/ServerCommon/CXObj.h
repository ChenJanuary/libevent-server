#ifndef _CXOBJ_H_
#define _CXOBJ_H_

#include <stdlib.h>

#define byte unsigned char 
#define SHM_CHECK_BYTE  'X'

class CXObj;

#define DECLARE_DYN  public:    static CXObj* CreateObject(void* pMem);
//#define IMPLEMENT_DYN(class_name)  CXObj* class_name::CreateObject( void *pMem ) {    CXObj::pCurrentObj = (byte *)pMem;    return (CXObj *)(new class_name); }
#define IMPLEMENT_DYN(class_name) \
CXObj* class_name::CreateObject( void *pMem ) \
{\
CXObj::pCurrentObj = (byte *)pMem;    \
return (CXObj *)(new class_name); \
}


class CXObj
{
public:
	/**
	*	@brief �������ռ�ʱ���ã����г�ʼ��������
	*/
	virtual void Init() = 0;

	/**
	*	@brief  ����������ڴ�����ʱ���ã�����ָ���������ָ�����õ�
	*/
	virtual void Resume() = 0;

	/**
	*	@brief  ����ռ䱻����ʱ���ã��������������
	*/
	virtual void Reclaim() = 0;

	/**
	*	@brief  ����ڴ��Ƿ��ƻ�
	*/
	inline bool CheckMem() { return (SHM_CHECK_BYTE == m_cCheckByte); }

	CXObj() { m_cCheckByte = SHM_CHECK_BYTE; };
	virtual ~CXObj() {};

protected:
	static void* operator new(size_t nSize)
	{
		if (NULL == pCurrentObj)
		{
			return (void*)NULL;
		}

		return (void*)pCurrentObj;
	}

	static void  operator delete(void* pMem)
	{
		return;
	}

	static byte* pCurrentObj;
	char  m_cCheckByte;

};

#endif

