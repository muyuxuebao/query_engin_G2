#include "QueryEngineServer.h"

int main(int argc, char **argv) {
	boost::shared_ptr<QueryEngineServer> queryEngineServer =
			QueryEngineServer::getInstance();

	queryEngineServer->init();

	queryEngineServer->run();
	return 0;
}
