/**
 * @file Exception-win32.cpp
 *
 * @brief Win32 only file for Exception.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2012
 */

#include "Exception.h"

#ifndef _WIN32
#error "This file is for Win32 only!"
#endif

#include <sstream>

Win32Exception::Win32Exception(DWORD errorCode)
	: Exception(createMessage(std::string(), errorCode).c_str()) { }

Win32Exception::Win32Exception()
	: Exception(createMessage(std::string(), GetLastError()).c_str()) { }

Win32Exception::Win32Exception(const char* msg)
	: Exception(createMessage(msg, GetLastError()).c_str()) { }

Win32Exception::Win32Exception(const char* msg, DWORD errorCode)
	: Exception(createMessage(msg, errorCode).c_str()) { }

std::string Win32Exception::createMessage(const std::string& msg, DWORD errorCode) {
	std::ostringstream s;
	s << msg << " [" << errorCode << "] " << Win32Error(errorCode).message();
	return s.str();
}

struct LocalFreeHelper
{
	void operator()(void* ptr) {
		LocalFree(ptr);
	}
};

std::string Win32Error::message() const {
	// use unique_ptr to gain exception safety
	std::unique_ptr<void, LocalFreeHelper> buff;
	LPSTR buffPtr;
	DWORD bufferLength = FormatMessageA(
		FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		ecode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&buffPtr),
		0,
		NULL);
	buff.reset(buffPtr);
	return std::string(buffPtr, bufferLength);
}
