/**
 * @file Exception.cpp
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2012
 */

#include "Exception.h"

#include <cstring>

Exception::Exception() : std::runtime_error("Exception") {
}

Exception::Exception(const char* msg) : std::runtime_error(msg) {
}

Exception::~Exception() throw() {
}

std::ostream& operator<<(std::ostream& out, Exception& e) {
	out << e.message();
	return out;
}

SystemException::SystemException(int errnum) : Exception(std::strerror(errnum)) {
}

SystemException::SystemException(const char* msg, int errnum) :
	Exception((std::string(msg) + ": " + std::strerror(errnum)).c_str()) { 
}
