/*
 * Configure.cpp
 *
 *  Created on: May 10, 2016
 *      Author: liang
 */

#include "Configure.h"

boost::shared_ptr<Configure> Configure::INSTANCE;
boost::mutex Configure::get_instance_mutex;

boost::shared_ptr<Configure> Configure::getInstance() {
	{
		boost::mutex::scoped_lock lock(get_instance_mutex);
		if (INSTANCE == NULL) {
			INSTANCE = boost::shared_ptr<Configure>(new Configure(d_cfg_path));
		}
	}

	return INSTANCE;
}

Configure::Configure(std::string cfg_path) {
	Config cfg;

// Read the file. If there is an error, report it and exit.
	try {
		cfg.readFile(cfg_path.c_str());
	} catch (const FileIOException &fioex) {
		std::cerr << "I/O error while reading file." << std::endl;
		exit(EXIT_FAILURE);
	} catch (const ParseException &pex) {
		std::cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
				<< " - " << pex.getError() << std::endl;
		exit(EXIT_FAILURE);
	}

// Get the store server_port.
	try {
		int server_port_t = cfg.lookup("server_port");

		this->server_port = server_port_t;

	} catch (const SettingNotFoundException &nfex) {
		cerr << "No 'name' setting in configuration file." << endl;
		exit(EXIT_FAILURE);
	}
}

Configure::~Configure() {
}

