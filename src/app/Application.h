/**
 * @file Application.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2013
 */

#ifndef APPLICATION_H
#define APPLICATION_H

#include "Exception.h"

#include <SDL.h>

#include <cstdint>
#include <memory>
#include <fstream>

namespace gl {
	class Renderer;
}

class Scene;

/**
 * Class representing SDL gui application.
 */
class SDLApplication
{
public:
	SDLApplication(int argc, char** argv);
	~SDLApplication();

	/**
	 * Run application.
	 * @return application return code
	 */
	int run();

	/**
	 * Sets window title.
	 * @param title if you pass nullptr default tile will be set.
	 */
	void setWindowTitle(const char* title);

	const char* getWindowTitle() {
		return windowTitle;
	}
private:
	static const char* DEFAULT_WND_TITLE;

	SDLApplication(const SDLApplication&);
	SDLApplication& operator=(SDLApplication);

	/// Create SDL window and init GLEW
	void createWindow(const char* windowCaption, size_t width, size_t height);
	/// Process SDL events like keyboard, mouse etc.
	void processEvents();
	/// Draw scene
	void draw();
	/// Non-draw per-frame actions like camera matrix computation
	void update();
	/// Application init.
	void init();
	/**
	 * Calculates fps.
	 * @retval fps resulting frames per second.
	 * @retval prevTime time from previous call ... sets to current time at the function end.
	 * @retval frameCount value gets incremented on each call.
	 */
	void calculateFps(double& fps, double& prevTime, uint64_t& frameCount);

	/// Gets unique time in seconds. time 0 depends on platform i.e. on unix it will be 1.1.1970 00:00
	static double getTime();

	SDL_Window* window;
	SDL_GLContext context;

	bool done;
	double fps;

	const char* windowTitle;
	size_t width;
	size_t height;

	std::unique_ptr<gl::Renderer> renderer;
	std::unique_ptr<Scene> scene;
};

/**
 * Exception class that creates error message from SDL_GetError.
 */
class SDLException : public Exception
{
public:
	SDLException() : Exception(SDL_GetError()) { }
	SDLException(const char* msg) : Exception((std::string(msg) + std::string(": ") + SDL_GetError()).c_str()) { }
};

#endif // APPLICATION_H

