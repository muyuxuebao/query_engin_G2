/*
 * QueryEngineServer.cpp
 *
 *  Created on: May 10, 2016
 *      Author: liang
 */

#include "QueryEngineServer.h"

boost::shared_ptr<QueryEngineServer> QueryEngineServer::getInstance() {
	{
		boost::mutex::scoped_lock lock(get_instance_mutex);
		if (INSTANCE == NULL) {
			INSTANCE = boost::shared_ptr<QueryEngineServer>(
					new QueryEngineServer);
		}
	}
	return INSTANCE;
}

boost::shared_ptr<QueryEngineServer> QueryEngineServer::INSTANCE;
boost::mutex QueryEngineServer::get_instance_mutex;

int getPort() {
	boost::shared_ptr<Configure> configure = Configure::getInstance();
	return configure->server_port;
}

bool QueryEngineServer::init() {
	int port = getPort();
	shared_ptr<RedisProxyServiceHandler> handler(
			new RedisProxyServiceHandler());
	shared_ptr<TProcessor> processor(new RedisProxyServiceProcessor(handler));
	shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
	shared_ptr<TTransportFactory> transportFactory(
			new TBufferedTransportFactory());
	shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

	server = boost::shared_ptr<TSimpleServer>(
			new TSimpleServer(processor, serverTransport, transportFactory,
					protocolFactory));
	return true;

}

void QueryEngineServer::run() {
	server->serve();
}

bool QueryEngineServer::stop() {
	server->stop();
	return true;
}

QueryEngineServer::QueryEngineServer() {
}

QueryEngineServer::~QueryEngineServer() {
}

