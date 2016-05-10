#ifndef REDIS_H_
#define REDIS_H_

#include <iostream>
#include <string.h>
#include <string>
#include <stdio.h>
#include <list>

#include <hiredis/hiredis.h>

class Redis {
public:

	Redis() {
	}

	~Redis() {
		redisFree(this->_connect);
	}

	bool connect(std::string host, int port) {
		this->_connect = redisConnect(host.c_str(), port);
		if (this->_connect != NULL && this->_connect->err) {
			printf("connect error: %s\n", this->_connect->errstr);
			return 0;
		}
		return 1;
	}

	bool get(std::string key, std::string &value) {
		bool tag = true;
		redisReply* _reply = (redisReply*) redisCommand(this->_connect,
				"GET %s", key.c_str());
		if (_reply->type == REDIS_REPLY_STRING) {
			value = _reply->str;
		} else {
			tag = false;
		}
		freeReplyObject(_reply);
		return tag;
	}

	bool set(std::string key, std::string value) {
		bool tag = true;

		redisReply* _reply = (redisReply*) redisCommand(this->_connect,
				"SET %s %s", key.c_str(), value.c_str());
		if (_reply->type != REDIS_REPLY_STATUS) {
			tag = false;
		}
		freeReplyObject(_reply);
		return tag;
	}


	bool getPrefixKey(std::string prefix, std::list<std::string> &list) {
		bool tag = true;
		redisReply* _reply = (redisReply*) redisCommand(this->_connect,
				"KEYS %s", (prefix + "*").c_str());
		if (_reply->type == REDIS_REPLY_ARRAY) {
			for (unsigned int i = 0; i < _reply->elements; ++i) {
				redisReply* childReply = _reply->element[i];
				if (childReply->type == REDIS_REPLY_STRING) {
					list.push_back(childReply->str);
				} else {
					tag = false;
					break;
				}
			}
		} else {
			tag = false;
		}
		freeReplyObject(_reply);
		return tag;
	}

	void cleanAllKey() {
		redisReply* _reply = (redisReply*) redisCommand(this->_connect,
				"flushall");
		freeReplyObject(_reply);
	}
private:
	redisContext* _connect;
};

#endif /* REDIS_H_ */
