/*
 * ConstValue.h
 *
 *  Created on: 2016-4-14
 *      Author: liang
 */
#include <iostream>
#ifndef CONSTVALUE_H_
#define CONSTVALUE_H_

class ConstValue {
public:
	ConstValue();
	virtual ~ConstValue();
	static std::string USER_PERFIX;
	static std::string WORD_PERFIX;
	static std::string TOKEN_PERFIX;

	static std::string USER_NUM;
	static std::string WORD_NUM;
	//	static std::string TOKEN_NUM_PERFIX;

	static long UNDEFINE_USER_ID;
	static long UNDEFINE_WORD_ID;

	static char CHINA_START;
	static char CHINA_END;
};

#endif /* CONSTVALUE_H_ */
