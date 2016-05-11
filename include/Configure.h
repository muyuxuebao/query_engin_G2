/*
 * Configure.h
 *
 *  Created on: May 10, 2016
 *      Author: liang
 */
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <libconfig.h++>
#include <string>
#include <sstream>

#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>

using namespace std;
using namespace libconfig;

#ifndef CONFIGURE_H_
#define CONFIGURE_H_

const std::string d_cfg_path = "./conf/query_engine.cfg";

class Configure {
public:
	static boost::shared_ptr<Configure> getInstance();
	virtual ~Configure();

	int server_port;

private:
	static boost::shared_ptr<Configure> INSTANCE;
	Configure(std::string cfg_path);
	std::string cfg_path;
	static boost::mutex get_instance_mutex;

};

#endif /* CONFIGURE_H_ */
