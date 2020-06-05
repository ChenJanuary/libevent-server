#pragma once

/***********************************************************************内核定义********************************************************************************/

const int MIN_USER_ACCOUNT_LEN = 4;			// 玩家账号最小长度
const int MAX_USER_ACCOUNT_LEN = 48;		// 玩家账号最大长度

const int MAX_USER_NAME_LEN = 64;			// 玩家昵称最大长度
const int MAX_USER_HEADURL_LEN = 256;		// 玩家头像最大长度

const int USER_MD5_PASSWD_LEN = 32;					// 密码长度(都是MD5)
const int MAX_USER_MAD_PASSWD_SIZE = 48;			// 最大密码缓冲区大小

const int MAX_PRIVATE_DESK_PASSWD_LEN = 20;			// 私有房最大密码长度
const int PRIVATE_DESK_PASSWD_LEN = 6;				// 私有房密码长度

const int MAX_TEMP_SENDBUF_SIZE = 8 * 1024;			// 临时发送缓冲区大小，也是单条数据包的最大长度，可以改成16K

const int MAX_REDIS_COMMAND_SIZE = 2048;			// redis命令缓冲区大小
const int MAX_SQL_STATEMENT_SIZE = 2048;			// sql语句缓冲区大小

const int MAX_IP_ADDR_SIZE = 128;		// 最大IP地址长度
const int MAX_NUM_IP_ADDR_SIZE = 48;	// 最大数字IP地址长度
const int MAX_DB_USER_SIZE = 48;		// 最大db_user缓冲区大小
const int MAX_DB_PASSWD_SIZE = 48;		// 最大db_passwd缓冲区大小
const int MAX_DB_NAME_SIZE = 48;		// 最大db_name缓冲区大小

const int MAX_LOG_BUF_SIZE = 1024;		// log缓冲区大小
const int MAX_FILE_NAME_SIZE = 256;		// 最大文件名长度
const int MAX_FUNC_NAME_SIZE = 128;		// 最大函数名长度
const int MAX_LOG_FILE_SIZE = 1024 * 1024 * 32;  //日子文件的最大的长度，超过长度会生成新的文件

const int MAX_BANK_PASSWD_MIN_LEN = 6;	// 银行密码最小长度

const int MAX_TOKEN_BUF_SIZE = 48;		// token的长度


/////////////////////////////////////////////////////////
//web 相关
#define  WEB_ADDRESS	"headimgqn.szysx8.cn"
const int MIN_MAN_HEADURL_ID = 1001;
const int MAX_MAN_HEADURL_ID = 3287;
const int MIN_WOMAN_HEADURL_ID = 1000001;
const int MAX_WOMAN_HEADURL_ID = 1003803;

/////////////////////////////////////////////////////////
// 心跳、socket相关
#define CHECK_HEAETBEAT_SECS					15		// 心跳定时器时间(s)
#define KEEP_ACTIVE_HEARTBEAT_COUNT				3		// 前端和服务器的心跳
#define BINDID_SOCKET_USERID_TIME				30		// 每个连接socket，绑定玩家时间，单位秒
#define MAX_LOGON_SERVER_COUNT					128		// 最多登陆服数量
#define INVALID_SOCKET							(-1)	// 无效的socket值
#define SOCKET_ERROR							(-1)	// 网络接口出错返回值

/////////////////////////////////////////////////////////
// 性能测试
const int MAX_COST_BUF_SIZE = 256;						// 缓冲区长度
#define ALL_STATISTICS_FUNC_COST_TIME			1000000 // 调用接口的总耗时，超过这个时间就输出日志(us) 1000000
#define MIN_STATISTICS_FUNC_COST_TIME			200000	// 调用接口的总耗时，超过这个时间就输出日志(us)，针对只调用一次的函数

/////////////////////////////////枚举模块/////////////////////////////////////////
// 玩家性别
enum UserSex
{
	USER_DEX_FEMALE = 0,//女性
	USER_SEX_MALE = 1,  //男性
};

// 服务器类型
enum ServiceType
{
	SERVICE_TYPE_BEGIN = 0,
	SERVICE_TYPE_LOGON,		// 大厅
	SERVICE_TYPE_LOADER,	// 游戏
	SERVICE_TYPE_CENTER,	// 中心服
	SERVICE_TYPE_PHP,		// PHP服务器
	SERVICE_TYPE_HTTP,		// http服务器
	SERVICE_TYPE_WORK,		// work服务器
	SERVICE_TYPE_END,
};

// 线程类型
enum ThreadType
{
	THREAD_TYPE_MAIN = 1,	// 主线程
	THREAD_TYPE_LOGIC = 2,	// 业务逻辑线程
	THREAD_TYPE_ASYNC = 3,	// 异步线程
	THREAD_TYPE_ACCEPT = 4,	// 连接线程
	THREAD_TYPE_RECV = 5,	// 接收线程
	THREAD_TYPE_SEND = 6,	// 发送线程
};

// 日志等级
enum LogLevel
{
	LOG_LEVEL_INFO = 0,						//info
	LOG_LEVEL_WARNNING,						//warnning
	LOG_LEVEL_ERROR,						//error
	LOG_LEVEL_INFO_CONSOLE,					//info Console
	LOG_LEVEL_ERROR_CONSOLE,				//error Console
	LOG_LEVEL_ERROR_SYS,					//error system
	LOG_LEVEL_END,
};

// 数据库类型
enum DBType
{
	DB_TYPE_COMMON = 0,					//普通数据库，主要存放用户数据和配置数据
	DB_TYPE_LOG,						//记录数据库，主要存放游戏记录，包括金币变化，钻石变化等等
	DB_TYPE_PHP,						//PHP数据库，主要存放php相关的表
	DB_TYPE_MAX,						//总数据库最大索引
};

// socket类型
enum SocketType
{
	SOCKET_TYPE_TCP = 0,				//tcp
	SOCKET_TYPE_WEBSOCKET = 1,			//websocket
};

//////////////////////////////////////////////////////////////////////////