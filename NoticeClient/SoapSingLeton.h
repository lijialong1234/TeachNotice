#pragma once
//ά��soap����
class CSoapSingLeton
{
private:
	CSoapSingLeton();
	~CSoapSingLeton();
	//�Ѹ��ƹ��캯����=������Ҳ��Ϊ˽��,��ֹ������
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

