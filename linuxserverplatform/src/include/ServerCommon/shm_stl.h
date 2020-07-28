/**
*	@file	shm_stl.h
*
* 	@author	huanghaoyang
* 	@date	2020-04-07
*/
#ifndef _SHM_STL_H_
#define _SHM_STL_H_

#include "CXObj.h"
#include "shm_obj_pool.h"
#include <vector>
#include <map>


template <class CKey, class CData>
class ShmNode : public CXObj
{
public:
	ShmNode();
	virtual ~ShmNode();
	/**
	*	@brief �������ռ�ʱ���ã����г�ʼ��������
	*/
	virtual void Init() {}

	/**
	*	@brief  ����������ڴ�����ʱ���ã�����ָ���������ָ�����õ�
	*/
	virtual void Resume() {}

	/**
	*	@brief  ����ռ䱻����ʱ���ã��������������
	*/
	virtual void Reclaim() {}

	static CXObj* CreateObject(void* pMem)
	{
		CXObj::pCurrentObj = (byte*)pMem;
		return (CXObj*)(new ShmNode<CKey, CData>);
	}

public:
	CKey m_oKey;
	CData m_oData;
	int m_iShmIndex;
};

/// -------------- ShmNode ------------------
template <class CKey, class CData>
ShmNode<CKey, CData>::ShmNode()
{
}

template <class CKey, class CData>
ShmNode<CKey, CData>::~ShmNode()
{}


template <class CData>
class ShmVector
{
public:
	typedef ShmNode<uint64_t, CData> ShmVectorNode;
	typedef std::pair<uint64_t, ShmVectorNode*> ShmVectorKey;

public:
	/**
	*  @brief    ��ʼ������
	*  @note
	*/
	int Init(const int aiKey, int aiNum, int aiType);

	/**
	* 	@brief	������ʹ�õ�obj����
	*/
	int GetUseNum()
	{
		return m_dataPool.GetUseNum();
	}

	/**
	* 	@brief	���ؿ��е�obj����
	*/
	int GetFreeNum()
	{
		return m_dataPool.GetFreeNum();
	}

	/**
	* 	@brief	����ͷ���Ķ���ָ��
	*/
	CData* Front();

	/**
	* 	@brief	ɾ��ͷ���Ķ���
	*/
	void PopFront();

	/**
	* 	@brief	����β���Ķ���ָ��
	*/
	CData* Back();

	/**
	* 	@brief	��β������һ������
	*/
	int PushBack(CData& Node);

	/**
	* 	@brief	��ָ��λ�ò���һ������
	*/
	int Insert(int iPos, CData& Node);

	/**
	* 	@brief	ɾ��β���Ķ���
	*/
	void PopBack();

	/**
	* 	@brief	ɾ��ָ��λ���±�Ķ���
	*/
	int Delete(int iPos);
	int Delete(int iBeginPos, int EndPos);

	/**
	* 	@brief	ɾ�����е��������
	*/
	void Clear();

	/**
	* 	@brief	�����Ѿ�ʹ�õ������С
	*/
	int Size();

	/**
	* 	@brief	��ȡָ��λ���±�Ķ���
	*/
	CData* GetObj(int iPos);

	/**
	* 	@brief	��ȡ���ж����ָ������
	*/
	void GetAllObj(std::vector<CData*>& vecDatas);

	/**
	* 	@brief	����[]����������
	*/
	CData& operator[](int i)
	{
		// ����±곬����Χ��ֱ�ӹҰ�
		if (i < 0 || i >(int)m_keyVec.size())
		{
			CData* pData = (CData*)NULL;
			return *pData;
		}

		return m_keyVec[i].second->m_oData;
	}

	/**
	* 	@brief �ӹ����ڴ��ؽ�����������˳��ʱ������Ƚ�
	*/
	static bool IDCompare(const ShmVectorKey& a, const ShmVectorKey& b);

protected:
	/**
	*  @brief    �ӹ����ڴ��лָ�����
	*  @note
	*/
	virtual void Resume();

	static int ResumeData(ShmVectorNode* pData, void* pArgList, int iLen, int iType);

protected:
	std::vector<ShmVectorKey> m_keyVec;
	CShmObjPool<ShmVectorNode> m_dataPool;
	uint64_t m_ullGID;
	int m_aiType;
};

template <class CKey, class CData>
class ShmMap
{
public:
	typedef ShmNode<CKey, CData> ShmMapNode;
	typedef std::map<CKey, ShmMapNode*> ShmMapKeys;

public:
	/**
	*  @brief    ��ʼ������
	*  @note
	*/
	int Init(const int aiKey, int aiNum, int aiType);

	/**
	* 	@brief	������ʹ�õ�obj����
	*/
	int GetUseNum()
	{
		return m_dataPool.GetUseNum();
	}

	/**
	* 	@brief	���ؿ��е�obj����
	*/
	int GetFreeNum()
	{
		return m_dataPool.GetFreeNum();
	}

	/**
	* 	@brief	����ָ����key����Ԫ�ض���
	*/
	CData* Get(const CKey oKey);

	/**
	* 	@brief	����һ��Ԫ�ض���
	*/
	int Insert(const CKey& oKey, const CData& Node);

	/**
	* 	@brief	ɾ��ָ��key��Ԫ�ض���
	*/
	int Delete(const CKey& oKey);

	/**
	* 	@brief	ɾ�����е��������
	*/
	void Clear();

	/**
	* 	@brief	�����Ѿ�ʹ�õ������С
	*/
	int Size();

	/**
	* 	@brief	��ȡ���ж����map�б�
	*/
	void GetAllObj(std::vector<std::pair<CKey, CData*> >& allObjs);

	/**
	* 	@brief	����[]����������
	*/
	CData& operator[](const CKey& oKey)
	{
		// ģ��map��û�оͲ���һ����Ԫ��
		CData* pData = Get(oKey);
		if (pData == NULL)
		{
			Insert(oKey, CData());
			pData = Get(oKey);
		}

		return *pData;
	}

protected:
	/**
	*  @brief    �ӹ����ڴ��лָ�����
	*  @note
	*/
	virtual void Resume();

	static int ResumeData(ShmNode<CKey, CData>* pData, void* pArgList, int iLen, int iType);

protected:
	ShmMapKeys m_keyMap;
	CShmObjPool<ShmMapNode> m_dataPool;
	int m_aiType;
};


template <class CData>
class ShmData
{
public:
	/**
	*  @brief    ��ʼ������
	*  @note
	*/
	int Init(const int aiKey, int aiType)
	{
		m_aiType = aiType;
		int aiNum = 1;
		int aiUint = sizeof(ShmNode<int, CData>);
		uint64_t ullNeedSize = sizeof(OBJPOOLHEAD) + sizeof(int) * aiNum + sizeof(char) * aiNum + aiUint * aiNum + 32;
		int iRet = m_dataPool.CreateObjPool(aiKey, ullNeedSize, aiUint, aiNum, aiType, __FILE__, __LINE__);
		if (iRet < 0)
		{
			LOG_ERR("%s:Create Shared-Mem Failed,iRet[%d]\n", __PRETTY_FUNCTION__, iRet);
			return iRet;
		}
		if (ESSM_Resume == m_dataPool.GetStartFlag())
		{
			//Resume();
		}

		return m_dataPool.GetStartFlag();
	}

	/**
	* 	@brief	��ȡ�������ṹ��ָ��
	*/
	CData* GetDataPtr()
	{
		ShmNode<int, CData>* pNode = m_dataPool.ForceGetObj(0, m_aiType);
		return &pNode->m_oData;
	}

	/**
	* 	@brief	����->����������
	*/
	CData* operator->()
	{
		ShmNode<int, CData>* pNode = m_dataPool.ForceGetObj(0, m_aiType);
		return &pNode->m_oData;
	}

	/**
	* 	@brief	��������0����
	*/
	void Reset()
	{
		ShmNode<int, CData>* pNode = m_dataPool.ForceGetObj(0, m_aiType);
		memset(&pNode->m_oData, 0, sizeof(CData));
	}

protected:
	CShmObjPool<ShmNode<int, CData> > m_dataPool;
	int m_aiType;
};

/////////////////////////////////ShmVector/////////////////////////////////////////
/**
*  @brief    ��ʼ������
*  @note
*/
template <class CData>
int ShmVector<CData>::Init(const int aiKey, int aiNum, int aiType)
{
	m_ullGID = 0;
	m_aiType = aiType;
	// ���һ���ڵ㵱β���
	aiNum += 1;
	int aiUint = sizeof(ShmVectorNode);
	uint64_t ullNeedSize = sizeof(OBJPOOLHEAD) + sizeof(int) * aiNum + sizeof(char) * aiNum + aiUint * aiNum + 32;
	int iRet = m_dataPool.CreateObjPool(aiKey, ullNeedSize, aiUint, aiNum, aiType, __FILE__, __LINE__);
	if (iRet < 0)
	{
		LOG_ERR("%s:Create Shared-Mem Failed,iRet[%d]\n", __PRETTY_FUNCTION__, iRet);
		return iRet;
	}
	if (ESSM_Resume == m_dataPool.GetStartFlag())
	{
		Resume();
	}

	return m_dataPool.GetStartFlag();
}

template <class CData>
int ShmVector<CData>::ResumeData(ShmVectorNode* pData, void* pArgList, int iLen, int iType)
{
	if (NULL == pData || NULL == pArgList)
		return -1;

	ShmVector<CData>* pShmVector = (ShmVector<CData>*)pArgList;
	pShmVector->m_keyVec.push_back(std::make_pair(pData->m_oKey, pData));
	if (pData->m_oKey > pShmVector->m_ullGID)
		pShmVector->m_ullGID = pData->m_oKey;

	return 0;
}

template <class CData>
bool ShmVector<CData>::IDCompare(const ShmVectorKey& a, const ShmVectorKey& b)
{
	return a.first < b.first;
}

/**
*  @brief    �ӹ����ڴ��лָ�����
*  @note
*/
template <class CData>
void ShmVector<CData>::Resume()
{
	m_keyVec.clear();
	m_dataPool.ProcessAllObj(ResumeData, this, 0, 0);
	std::sort(m_keyVec.begin(), m_keyVec.end(), IDCompare);
}

/**
* 	@brief	����ͷ���Ķ���ָ��
*/
template <class CData>
CData* ShmVector<CData>::Front()
{
	if (m_keyVec.size() == 0)
		return NULL;

	ShmVectorKey& key = m_keyVec.front();
	return &key.second->m_oData;
}

/**
* 	@brief	ɾ��ͷ���Ķ���
*/
template <class CData>
void ShmVector<CData>::PopFront()
{
	if (m_keyVec.size() == 0)
		return;

	m_dataPool.FreeObj(m_aiType, m_keyVec.front().second->m_iShmIndex);
	m_keyVec.erase(m_keyVec.begin());
}

/**
* 	@brief	����β���Ķ���ָ��
*/
template <class CData>
CData* ShmVector<CData>::Back()
{
	if (m_keyVec.size() == 0)
		return NULL;

	ShmVectorKey& key = m_keyVec.back();
	return &key.second->m_oData;
}

/**
* 	@brief	��β������һ������
*/
template <class CData>
int ShmVector<CData>::PushBack(CData& Node)
{
	return Insert(m_keyVec.size(), Node);
}

/**
* 	@brief	��ָ��λ�ò���һ������
*/
template <class CData>
int ShmVector<CData>::Insert(int iPos, CData& Node)
{
	if (iPos < 0 || iPos >(int)m_keyVec.size())
		return -1;

	int aiIndex = 0;
	ShmVectorNode* pNode = m_dataPool.GetFreeObj(m_aiType, &aiIndex);
	if (pNode == NULL)
		return -1;

	pNode->m_oData = Node;
	pNode->m_iShmIndex = aiIndex;
	m_ullGID++;
	pNode->m_oKey = m_ullGID;
	m_keyVec.insert(m_keyVec.begin() + iPos, std::make_pair(pNode->m_oKey, pNode));

	return 0;
}

/**
* 	@brief	ɾ��β���Ķ���
*/
template <class CData>
void ShmVector<CData>::PopBack()
{
	if (m_keyVec.size() == 0)
		return;

	m_dataPool.FreeObj(m_aiType, m_keyVec.back().second->m_iShmIndex);
	m_keyVec.pop_back();
}

/**
* 	@brief	ɾ��ָ��λ���±�Ķ���
*/
template <class CData>
int ShmVector<CData>::Delete(int iPos)
{
	if (iPos < 0 || m_keyVec.size() <= iPos)
		return -1;

	m_dataPool.FreeObj(m_aiType, m_keyVec[iPos].second->m_iShmIndex);
	m_keyVec.erase(m_keyVec.begin() + iPos);

	return 0;
}

template <class CData>
int ShmVector<CData>::Delete(int iBeginPos, int iEndPos)
{
	if (iBeginPos < 0 || iEndPos < 0 || iEndPos < iBeginPos || m_keyVec.size() < iEndPos)
		return -1;

	for (int i = iBeginPos; i < iEndPos; i++)
	{
		m_dataPool.FreeObj(m_aiType, m_keyVec[i].second->m_iShmIndex);
	}

	m_keyVec.erase(m_keyVec.begin() + iBeginPos, m_keyVec.begin() + iEndPos);

	return 0;
}


/**
* 	@brief	ɾ�����е��������
*/
template <class CData>
void ShmVector<CData>::Clear()
{
	for (size_t i = 0; i < m_keyVec.size(); i++)
	{
		m_dataPool.FreeObj(m_aiType, m_keyVec[i].second->m_iShmIndex);
	}

	m_keyVec.clear();
}

/**
* 	@brief	�����Ѿ�ʹ�õ������С
*/
template <class CData>
int ShmVector<CData>::Size()
{
	return m_keyVec.size();
}

/**
* 	@brief	��ȡָ��λ���±�Ķ���
*/
template <class CData>
CData* ShmVector<CData>::GetObj(int iPos)
{
	// ����±곬����Χ��ֱ�ӹҰ�
	if (iPos < 0 || iPos >(int)m_keyVec.size())
	{
		CData* pData = (CData*)NULL;
		return pData;
	}

	return &m_keyVec[iPos].second->m_oData;
}

/**
* 	@brief	��ȡ���ж����ָ������
*/
template <class CData>
void ShmVector<CData>::GetAllObj(std::vector<CData*>& vecDatas)
{
	vecDatas.clear();
	for (size_t i = 0; i < m_keyVec.size(); i++)
	{
		vecDatas.push_back(m_dataPool.GetObj(m_keyVec[i].second));
	}
}


/////////////////////////////////ShmMap/////////////////////////////////////////
/**
*  @brief    ��ʼ������
*  @note
*/
template <class CKey, class CData>
int ShmMap<CKey, CData>::Init(const int aiKey, int aiNum, int aiType)
{
	m_aiType = aiType;
	// ���һ���ڵ㵱β���
	aiNum += 1;
	int aiUint = sizeof(ShmMapNode);
	uint64_t ullNeedSize = sizeof(OBJPOOLHEAD) + sizeof(int) * aiNum + sizeof(char) * aiNum + aiUint * aiNum + 32;
	int iRet = m_dataPool.CreateObjPool(aiKey, ullNeedSize, aiUint, aiNum, aiType, __FILE__, __LINE__);
	if (iRet < 0)
	{
		LOG_ERR("%s:Create Shared-Mem Failed,iRet[%d]\n", __PRETTY_FUNCTION__, iRet);
		return iRet;
	}
	if (ESSM_Resume == m_dataPool.GetStartFlag())
	{
		Resume();
	}

	return m_dataPool.GetStartFlag();
}

/**
* 	@brief	����ָ����key����Ԫ�ض���
*/
template <class CKey, class CData>
CData* ShmMap<CKey, CData>::Get(CKey oKey)
{
	if (m_keyMap.find(oKey) == m_keyMap.end())
		return NULL;

	return &m_keyMap[oKey]->m_oData;
}

/**
* 	@brief	����һ��Ԫ�ض���
*/
template <class CKey, class CData>
int ShmMap<CKey, CData>::Insert(const CKey& oKey, const CData& Node)
{
	// �Ѿ����ڵ����¸�ֵ
	CData* pData = Get(oKey);
	if (pData != NULL)
	{
		*pData = Node;
	}
	else
	{
		int aiIndex = 0;
		ShmMapNode* pNode = m_dataPool.GetFreeObj(m_aiType, &aiIndex);
		if (pNode == NULL)
			return -1;

		pNode->m_oData = Node;
		pNode->m_iShmIndex = aiIndex;
		pNode->m_oKey = oKey;
		m_keyMap[oKey] = pNode;
	}

	return 0;
}

/**
* 	@brief	ɾ��ָ��key��Ԫ�ض���
*/
template <class CKey, class CData>
int ShmMap<CKey, CData>::Delete(const CKey& oKey)
{
	if (m_keyMap.find(oKey) == m_keyMap.end())
		return -1;

	m_dataPool.FreeObj(m_aiType, m_keyMap[oKey]->m_iShmIndex);
	m_keyMap.erase(oKey);

	return 0;
}

/**
* 	@brief	ɾ�����е��������
*/
template <class CKey, class CData>
void ShmMap<CKey, CData>::Clear()
{
	// forѭ������m_keyMap���벻����ֻ������ķ�ʽ��ʵ��
	std::vector<int> vIdxList;
	vIdxList.reserve(m_keyMap.size());    //Ԥ����ռ�
	int iCount = 0;
	int iSize = m_dataPool.GetFreeNum() + m_dataPool.GetUseNum();
	for (int i = 1; i < iSize; ++i)
	{
		if (iCount >= m_dataPool.GetUseNum())
			break;

		ShmMapNode* pDataNode = m_dataPool.GetObj(i, m_aiType);
		if (pDataNode == NULL)
			continue;

		vIdxList.push_back(pDataNode->m_iShmIndex);
		++iCount;
	}

	for (size_t i = 0; i < vIdxList.size(); i++)
	{
		m_dataPool.FreeObj(m_aiType, vIdxList[i]);
	}

	m_keyMap.clear();
}

/**
* 	@brief	�����Ѿ�ʹ�õ������С
*/
template <class CKey, class CData>
int ShmMap<CKey, CData>::Size()
{
	return m_keyMap.size();
}

/**
* 	@brief	��ȡ���ж����map�б�
*/
template <class CKey, class CData>
void ShmMap<CKey, CData>::GetAllObj(std::vector<std::pair<CKey, CData*> >& allObjs)
{
	allObjs.clear();

	// forѭ������m_keyMap���벻����ֻ������ķ�ʽ��ʵ��
	allObjs.reserve(m_keyMap.size());    //Ԥ����ռ�
	int iCount = 0;
	int iSize = m_dataPool.GetFreeNum() + m_dataPool.GetUseNum();
	for (int i = 1; i < iSize; ++i)
	{
		if (iCount >= m_dataPool.GetUseNum())
			break;

		ShmMapNode* pDataNode = m_dataPool.GetObj(i, m_aiType);
		if (pDataNode == NULL)
			continue;

		allObjs.push_back(std::make_pair(pDataNode->m_oKey, &pDataNode->m_oData));
		++iCount;
	}
}

/**
*  @brief    �ӹ����ڴ��лָ�����
*  @note
*/
template <class CKey, class CData>
void ShmMap<CKey, CData>::Resume()
{
	m_keyMap.clear();
	m_dataPool.ProcessAllObj(ResumeData, this, 0, 0);
}

template <class CKey, class CData>
int ShmMap<CKey, CData>::ResumeData(ShmNode<CKey, CData>* pData, void* pArgList, int iLen, int iType)
{
	if (NULL == pData || NULL == pArgList)
		return -1;

	ShmMap<CKey, CData>* pShmMap = (ShmMap<CKey, CData>*)pArgList;
	if (pShmMap->Get(pData->m_oKey) != NULL)
	{
		LOG_ERR("%s:Create Shared-Mem Failed,iIndex[%d]\n", __PRETTY_FUNCTION__, pData->m_iShmIndex);
		return -1;
	}

	pShmMap->m_keyMap[pData->m_oKey] = pData;

	return 0;
}

#endif

