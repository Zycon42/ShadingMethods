/**
 * @file Logging.cpp
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2012
 */

#include "Logging.h"

#include <iostream>

const char* Logger::SEVERITY_STRS[] = {
	"INFO", "WARNING", "ERROR", "FATAL"
};

std::ostream* Logger::stream = &std::cerr;

static std::string fileBaseName(const std::string& path) {
	// reverse find \ or /
	size_t del = path.rfind('\\');
	if (del == std::string::npos)
		del = path.rfind('/');

	return path.substr(del == std::string::npos ? 0 : del + 1);
}

Logger::Logger(const char* file, int line, Severity severity) {
	buffer << "[" << SEVERITY_STRS[static_cast<int>(severity)] << " " << fileBaseName(file) << ":" << line << "] ";
}

Logger::~Logger() {
	*stream << buffer.str() << std::endl;
}
