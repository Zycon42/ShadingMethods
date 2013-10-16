/**
 * @file main.cpp
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2013
 */

#include "Exception.h"
#include "Logging.h"

#include "Application.h"

#include <memory>

int main(int argc, char** argv) {
	std::ofstream loggingFile("log.txt");
	if (loggingFile)
		Logger::setOutputStream(loggingFile);

	try {
		SDLApplication app(argc, argv);
		return app.run();
	} catch (SDLException& e) {
		LOG(FATAL) << "Unhandled exception: " << e.what();
	} catch (std::exception& e) {
		LOG(FATAL) << "Unhandled exception: " << e.what();
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Unhandled exception", e.what(), nullptr);
	}

	return 1;
}