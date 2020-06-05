#include "CommonHead.h"
#include "configManage.h"
#include "log.h"
#include "DataBase.h"


CDataBaseManage::CDataBaseManage()
{
	m_bInit = false;
	m_bRun = false;
	m_hThread = 0;
	m_pInitInfo = NULL;
	m_pKernelInfo = NULL;
	m_pHandleService = NULL;
	m_pMysqlHelper = NULL;

}

CDataBaseManage::~CDataBaseManage()
{
	m_bInit = false;
	m_pInitInfo = NULL;
	m_hThread = 0;
	m_pKernelInfo = NULL;
	m_pHandleService = NULL;
}

//开始服务
bool CDataBaseManage::Start()
{
	INFO_LOG("DataBaseManage start begin ...");

	if (m_bRun == true || m_bInit == false)
	{
		ERROR_LOG("DataBaseManage already running or not inited m_bRun=%d m_bInit=%d", m_bRun, m_bInit);
		return false;
	}

	m_bRun = true;

	SQLConnectReset();

	//建立数据处理线程
	pthread_t threadID = 0;
	int err = pthread_create(&threadID, NULL, DataServiceThread, (void*)this);
	if (err != 0)
	{
		SYS_ERROR_LOG("DataServiceThread failed");
		return false;
	}

	m_hThread = threadID;

	// 关联日志文件
	int roomID = 0;
	if (m_pInitInfo)
	{
		roomID = m_pInitInfo->uRoomID;
	}
	GameLogManage()->AddLogFile(threadID, THREAD_TYPE_ASYNC, roomID);

	INFO_LOG("DataBaseManage start end.");

	return true;
}

bool CDataBaseManage::Stop()
{
	INFO_LOG("DataBaseManage stop begin...");

	if (!m_bRun)
	{
		ERROR_LOG("DataBaseManage is not running...");
		return false;
	}

	m_bRun = false;

	// 清理dataline
	m_DataLine.CleanLineData();

	// 关闭数据库处理线程句柄
	if (m_hThread)
	{
		pthread_cancel(m_hThread);
		m_hThread = 0;
	}

	//关闭数据库连接，析构函数会自动释放连接
	SafeDeleteArray(m_pMysqlHelper);

	INFO_LOG("DataBaseManage stop end.");

	return true;
}

//取消初始化
bool CDataBaseManage::UnInit()
{
	return true;
}

//加入处理队列
bool CDataBaseManage::PushLine(DataBaseLineHead* pData, UINT uSize, UINT uHandleKind, UINT uIndex, UINT uMsgID)
{
	//处理数据
	pData->uMsgID = uMsgID;
	pData->uIndex = uIndex;
	pData->uHandleKind = uHandleKind;
	return m_DataLine.AddData(&pData->dataLineHead, uSize, 0) != 0;
}

//数据库处理线程
void* CDataBaseManage::DataServiceThread(void* pThreadData)
{
	//数据定义
	CDataBaseManage* pDataManage = (CDataBaseManage*)pThreadData;//数据库管理指针
	CDataLine* pDataLine = &pDataManage->m_DataLine;			//数据队列指针
	IDataBaseHandleService* pHandleService = pDataManage->m_pHandleService;	//数据处理接口

	//数据缓存
	DataLineHead* pDataLineHead = NULL;

	sleep(1);

	//INFO_LOG("DataServiceThread starting...");

	while (pDataManage->m_bRun == true)
	{
		try
		{
			//获取数据
			unsigned int bytes = pDataLine->GetData(&pDataLineHead);
			if (bytes == 0 || pDataLineHead == NULL)
			{
				continue;
			}

			//处理数据
			pHandleService->HandleDataBase((DataBaseLineHead*)pDataLineHead);

			// 释放内存
			if (pDataLineHead)
			{
				free(pDataLineHead);
			}
		}
		catch (...)
		{
			ERROR_LOG("CATCH:%s with %s\n", __FILE__, __FUNCTION__);
		}
	}

	INFO_LOG("DataServiceThread exit.");

	pthread_exit(NULL);
}

//重联数据库
bool CDataBaseManage::SQLConnectReset()
{
	if (m_pMysqlHelper)
	{
		SafeDeleteArray(m_pMysqlHelper);
	}

	m_pMysqlHelper = new CMysqlHelper[DB_TYPE_PHP];

	for (int i = 0; i < DB_TYPE_PHP; i++)
	{
		const DBConfig& dbConfig = ConfigManage()->GetDBConfig(i);

		//初始化mysql对象并建立连接
		m_pMysqlHelper[i].init(dbConfig.ip, dbConfig.user, dbConfig.passwd, dbConfig.dbName, "", dbConfig.port);
		try
		{
			m_pMysqlHelper[i].connect();
		}
		catch (MysqlHelper_Exception & excep)
		{
			ERROR_LOG("连接数据库失败:%s", excep.errorInfo.c_str());
			return false;
		}
	}

	return true;
}

bool CDataBaseManage::CheckSQLConnect()
{
	if (!m_pMysqlHelper)
	{
		return false;
	}

	char buf[128] = "";
	sprintf(buf, "select * from %s LIMIT 1", TBL_BASE_GAME);
	bool bConect = false;

	for (int i = 0; i < DB_TYPE_PHP; i++)
	{
		try
		{
			m_pMysqlHelper[i].sqlExec(buf);
		}
		catch (...)
		{
			bConect = true;
		}

		if (bConect)
		{
			try
			{
				m_pMysqlHelper[i].connect();
			}
			catch (MysqlHelper_Exception & excep)
			{
				ERROR_LOG("连接数据库失败:%s", excep.errorInfo.c_str());
				return false;
			}
		}
	}

	return true;
}



//***********************************************************************************************//
CDataBaseHandle::CDataBaseHandle()
{
	m_pInitInfo = NULL;
	m_pKernelInfo = NULL;
	m_pRusultService = NULL;
	m_pDataBaseManage = NULL;
}

CDataBaseHandle::~CDataBaseHandle()
{
}

bool CDataBaseHandle::SetParameter(IAsynThreadResultService* pRusultService, CDataBaseManage* pDataBaseManage, ManageInfoStruct* pInitData, KernelInfoStruct* pKernelData)
{
	m_pInitInfo = pInitData;
	m_pKernelInfo = pKernelData;
	m_pRusultService = pRusultService;
	m_pDataBaseManage = pDataBaseManage;

	return true;
}

//初始化函数
bool CDataBaseManage::Init(ManageInfoStruct* pInitInfo, KernelInfoStruct* pKernelInfo, IDataBaseHandleService* pHandleService, IAsynThreadResultService* pResultService)
{
	if (!pInitInfo || !pKernelInfo || !pHandleService || !pResultService)
	{
		throw new CException("CDataBaseManage::Init 参数错误!", (UINT)0x407, true);
	}

	//效验参数
	if (m_bInit == true || m_bRun == true)
	{
		throw new CException("CDataBaseManage::Init 状态效验失败", (UINT)0x408, true);
	}

	//设置数据
	m_pInitInfo = pInitInfo;
	m_pKernelInfo = pKernelInfo;
	m_pHandleService = pHandleService;
	m_DataLine.CleanLineData();

	//设置数据
	m_bInit = true;

	return true;
}
