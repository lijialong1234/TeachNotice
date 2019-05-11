#pragma once
#include <threads.h>
#include<Windows.h>
// Define a concrete ConnectionPool derivative.  Takes connection
// parameters as inputs to its ctor, which it uses to create the
// connections we're called upon to make.  Note that we also declare
// a global pointer to an object of this type, which we create soon
// after startup; this should be a common usage pattern, as what use
// are multiple pools?
class CSQLThreadConPool : public mysqlpp::ConnectionPool
{
public:
	// The object's only constructor
	CSQLThreadConPool(std::string dbname, std::string sServer, std::string sUser, std::string sPass) :
		conns_in_use_(0),
		db_(dbname),
		server_(sServer),
		user_(sUser),
		password_(sPass)
	{
		InitializeCriticalSection(&m_CS);
	}

	// The destructor.  We _must_ call ConnectionPool::clear() here,
	// because our superclass can't do it for us.
	~CSQLThreadConPool()
	{
		DeleteCriticalSection(&m_CS);
		clear();
	}

	// Do a simple form of in-use connection limiting: wait to return
	// a connection until there are a reasonably low number in use
	// already.  Can't do this in create() because we're interested in
	// connections actually in use, not those created.  Also note that
	// we keep our own count; ConnectionPool::size() isn't the same!
	mysqlpp::Connection* grab()
	{
		while (conns_in_use_ > 1000) {
			// cout.put('R'); cout.flush(); // indicate waiting for release
			sleep(10);
		}
		EnterCriticalSection(&m_CS);
		++conns_in_use_;
		LeaveCriticalSection(&m_CS);
		return mysqlpp::ConnectionPool::grab();
	}

	// Other half of in-use conn count limit
	void release(const mysqlpp::Connection* pc)
	{
		EnterCriticalSection(&m_CS);
		mysqlpp::ConnectionPool::release(pc);
		mysqlpp::ConnectionPool::remove(pc);
		--conns_in_use_;
		LeaveCriticalSection(&m_CS);
	}
	int GetConnections() { return conns_in_use_; }
protected:
	// Superclass overrides
	mysqlpp::Connection* create()
	{
		// Create connection using the parameters we were passed upon
		// creation.  This could be something much more complex, but for
		// the purposes of the example, this suffices.
		//cout.put('C'); cout.flush(); // indicate connection creation
		mysqlpp::Connection *pcon = new mysqlpp::Connection(false);
		if (pcon)
		{
			mysqlpp::SetCharsetNameOption *charsetOp = new mysqlpp::SetCharsetNameOption("utf8");
			pcon->set_option(charsetOp);
			pcon->set_option(new mysqlpp::ConnectTimeoutOption(5));
			pcon->set_option(new mysqlpp::ReconnectOption(false));
			pcon->set_option(new mysqlpp::MultiStatementsOption(true));
			pcon->connect(
				db_.empty() ? 0 : db_.c_str(),
				server_.empty() ? 0 : server_.c_str(),
				user_.empty() ? 0 : user_.c_str(),
				password_.empty() ? "" : password_.c_str());
		}

		return pcon;
	}

	void destroy(mysqlpp::Connection* cp)
	{
		// Our superclass can't know how we created the Connection, so
		// it delegates destruction to us, to be safe.
		//cout.put('D'); cout.flush(); // indicate connection destruction
		if (!cp)
		{
			delete cp;
		}

	}

	unsigned int max_idle_time()
	{
		// Set our idle time at an example-friendly 3 seconds.  A real
		// pool would return some fraction of the server's connection
		// idle timeout instead.
		return 3;
	}

private:
	// Number of connections currently in use
	volatile unsigned int conns_in_use_;
	CRITICAL_SECTION m_CS;
	// Our connection parameters
	std::string db_, server_, user_, password_;
};
