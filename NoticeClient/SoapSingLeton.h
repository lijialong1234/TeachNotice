#pragma once
//维护soap单例
class CSoapSingLeton
{
private:
	CSoapSingLeton();
	~CSoapSingLeton();
	//把复制构造函数和=操作符也设为私有,防止被复制
	CSoapSingLeton(const CSoapSingLeton&) = delete;
	CSoapSingLeton& operator=(const CSoapSingLeton&) = delete;
	static CSoapSingLeton* instance;
	struct soap *m_soap;
	char * server_addr;
public:
	static CSoapSingLeton* GetInstance();
	static void DestreyInstance();
	int GlobalUpdateNotice();
};

