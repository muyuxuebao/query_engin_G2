/*
 * QueryEngineServer.h
 *
 *  Created on: May 10, 2016
 *      Author: liang
 */

#include "RedisProxyServiceHandler.h"
#include "Configure.h"

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

#ifndef QUERYENGINESERVER_H_
#define QUERYENGINESERVER_H_

class QueryEngineServer {
public:
	virtual ~QueryEngineServer();
	static boost::shared_ptr<QueryEngineServer> getInstance();

	bool init();
	void run();
	bool stop();
private:
	QueryEngineServer();
	static boost::shared_ptr<QueryEngineServer> INSTANCE;
	boost::shared_ptr<TSimpleServer> server;
	static boost::mutex get_instance_mutex;
};

#endif /* QUERYENGINESERVER_H_ */
