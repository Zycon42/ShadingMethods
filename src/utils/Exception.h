/**
 * @file Exception.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2012
 */

#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <cerrno>
#include <string>
#include <iostream>
#include <stdexcept>
#include <memory>

/**
 * Base class for user defined exceptions.
 */
class Exception : public std::runtime_error
{
public:
	Exception();
	Exception(const char* msg);

	virtual ~Exception() throw();

	/// Alternate name for what().
	const char* message() const { return what(); }
};

std::ostream& operator<<(std::ostream& out, Exception& e);

/**
 * Exception class that creates error message
 * from errno variable.
 */
class SystemException : public Exception
{
public:
	SystemException(int errnum = errno);
	SystemException(const char* msg, int errnum = errno);
};

#ifdef _WIN32

#define NOGDI
#include <Windows.h>

/**
 * Exception class that creates error message from
 * Win32 error codes.
 */
class Win32Exception : public Exception
{
public:
	/// Creates exception with no aditional message and errorCode from
	/// @link{GetLastError}.
	Win32Exception();
	/// Creates exception with aditional message and errorCode from @link{GetLastError}.
	Win32Exception(const char* msg);
	/// Creates exception with aditional message.
	Win32Exception(const char* msg, DWORD errorCode);
	/// Creates exception with no aditional message.
	Win32Exception(DWORD errorCode);
private:
	static std::string createMessage(const std::string& msg, DWORD errorCode);
};

/**
 * Class for Win32 errors defined by DWORD errorCode
 * returned from @link{GetLastError}.
 */
class Win32Error
{
public:
	Win32Error(DWORD code) : ecode(code) {}
	DWORD code() const { return ecode; }

	std::string message() const;
private:
	DWORD ecode;
};

#endif // _WIN32

#endif // EXCEPTION_H
