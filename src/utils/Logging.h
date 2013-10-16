/**
 * @file Logging.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2012
 */

#ifndef LOGGING_H
#define LOGGING_H

#include <sstream>

/**
 * Logs messages.
 */
class Logger
{
public:
	enum class Severity { Info, Warning, Error, Fatal };

	Logger(const char* file, int line, Severity severity);
	~Logger();

	template <typename T>
	Logger& operator<<(T value) {
		buffer << value;
		return *this;
	}

	static void setOutputStream(std::ostream& stream) {
		Logger::stream = &stream;
	}
private:
	static std::ostream* stream;
	static const char* SEVERITY_STRS[];

	std::ostringstream buffer;
};

#define INTERNAL_LOG_INFO Logger(__FILE__, __LINE__, Logger::Severity::Info)
#define INTERNAL_LOG_WARNING Logger(__FILE__, __LINE__, Logger::Severity::Warning)
#define INTERNAL_LOG_ERROR Logger(__FILE__, __LINE__, Logger::Severity::Error)
#define INTERNAL_LOG_FATAL Logger(__FILE__, __LINE__, Logger::Severity::Fatal)

#define LOG(severity) INTERNAL_LOG_##severity

#endif // LOGGING_H
