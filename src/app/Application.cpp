/**
 * @file Application.cpp
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2013
 */

#include "Application.h"

#include "Logging.h"
#include "Common.h"
#include "Renderer.h"
#include "Scene.h"
#include "BaseSceneObject.h"
#include "ShaderManager.h"
#include "FpsCamera.h"
#include "Light.h"

#include "CitySceneGenerator.h"

#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/swizzle.hpp>

#include <sstream>

const char* SDLApplication::DEFAULT_WND_TITLE = "Test app";

SDLApplication::SDLApplication(int argc, char** argv) 
	: window(nullptr), context(nullptr), done(false), fps(60.0), windowTitle(DEFAULT_WND_TITLE),
	  renderer(new gl::Renderer())
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		throw SDLException("SDL_Init failed");

	createWindow(windowTitle, 800, 600);
}

SDLApplication::~SDLApplication() {
	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

int SDLApplication::run() {
	init();

	uint64_t frameCount = 0;
	double prevTime = getTime();
	while (!done) {
		processEvents();
		update();
		draw();
		calculateFps(fps, prevTime, frameCount);
	}
	
	return 0;
}

void SDLApplication::setWindowTitle(const char* title) {
	if (title == nullptr)
		windowTitle = DEFAULT_WND_TITLE;
	else
		windowTitle = title;

	SDL_SetWindowTitle(window, windowTitle);
}

void SDLApplication::createWindow(const char* windowCaption, size_t width, size_t height) {
	this->width = width;
	this->height = height;

	// Set double buffering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// Create window
	window = SDL_CreateWindow(windowCaption, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		width, height, SDL_WINDOW_OPENGL);
	if (window == nullptr)
		throw SDLException("SDL_CreateWindow failed");

	context = SDL_GL_CreateContext(window);

	LOG(INFO) << "OpenGL version: " << glGetString(GL_VERSION);

	// Init GLEW
	GLenum err;
	glewExperimental = GL_TRUE;
	if ((err = glewInit()) != GLEW_OK)
		throw Exception((const char*)glewGetErrorString(err));

	LOG(INFO) << "Using GLEW " << glewGetString(GLEW_VERSION);

	// GL_EXT_direct_state_access detection broken in glew on my amd radeon 6630m
	// TODO: check if desired functions are not null manually
	/*if (!GLEW_EXT_direct_state_access)
		throw Exception("Missing EXT_direct_state_access extension!");*/

	grabMouse(true);
}

void SDLApplication::init() {
	renderer->initialize();
	renderer->setViewport(Viewport(static_cast<float>(width), static_cast<float>(height)));

	scene = std::unique_ptr<Scene>(new Scene(renderer.get()));
	renderer->setScene(scene.get());

	CitySceneGenerator generator;
	generator.generate(scene.get());

	camera = std::unique_ptr<FpsCamera>(new FpsCamera(renderer.get()));
	camera->setProjectionMatrix(glm::perspective(60.0f, (float)width / height, 0.1f, 1000.0f));
	camera->setPosition(0.0f, 0.0f, 250.0f);
	camera->setMovementSpeed(60.0f);

	renderer->setCamera(camera.get());

	light = std::unique_ptr<Light>(new Light(renderer.get()));
	light->setPosition(glm::vec4(0.5f, 0.0f, -1.0f, 0.0f));
	light->setAmbient(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	light->setDiffuse(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	light->setSpecular(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

	/*light->toggleShadowSource(true);
	// axis aligned box that containing scene TODO: make it something move clever than hard coded values :D
	glm::mat4 lightProjection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, -5.0f, 5.0f);
	// rotate so that light direction in camera space is -z
	glm::mat4 lightView = glm::lookAt(glm::swizzle<glm::X, glm::Y, glm::Z>(-light->position()), 
		glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	light->setViewProjection(lightProjection * lightView);*/

	light->flushChanges();

	renderer->setLight(light.get());
}

void SDLApplication::update() {
	std::ostringstream ss;
	ss << windowTitle << " - " << fps;
	SDL_SetWindowTitle(window, ss.str().c_str());

	handleKeyboard();

	camera->update();
}

void SDLApplication::draw() {
	
	renderer->drawFrame();

	SDL_GL_SwapWindow(window);
}

void SDLApplication::processEvents() {
	SDL_Event event;  
	while(SDL_PollEvent(&event)) {
		switch (event.type)  
		{  
		case SDL_QUIT:
			done = true; 
			break;

		case SDL_KEYUP:
		case SDL_KEYDOWN:
			keyboardHandler.handleEvent(event.key);
			break;

		case SDL_MOUSEMOTION:
			handleMouseMove(event.motion.xrel, event.motion.yrel);
			break;
		}  
	}
}

void SDLApplication::handleKeyboard() {
	if (keyboardHandler.isPressed(SDLK_ESCAPE)) {
		SDL_Event quitEvent = { SDL_QUIT };
		SDL_PushEvent(&quitEvent);
		return;
	}

	if (keyboardHandler.isPressedOnce(SDLK_LALT)) {
		grabMouse(!mouseGrabbed);
	}

	if (keyboardHandler.isPressed(SDLK_w))
		camera->goForward(fps);
	if (keyboardHandler.isPressed(SDLK_s))
		camera->goBackward(fps);
	if (keyboardHandler.isPressed(SDLK_a))
		camera->goLeft(fps);
	if (keyboardHandler.isPressed(SDLK_d))
		camera->goRight(fps);

	if (keyboardHandler.isPressedOnce(SDLK_b))
		renderer->toggleBboxVisibility();

	static const float rollSpeed = 45.0f;
	if (keyboardHandler.isPressed(SDLK_q))
		camera->roll(-rollSpeed / fps);
	if (keyboardHandler.isPressed(SDLK_e))
		camera->roll(rollSpeed / fps);
}

void SDLApplication::handleMouseMove(int xrel, int yrel) {
	camera->yaw(static_cast<float>(xrel));
	camera->pitch(static_cast<float>(yrel));
}

void SDLApplication::grabMouse(bool flag) {
	if (SDL_SetRelativeMouseMode(flag ? SDL_TRUE : SDL_FALSE) == -1) {
		LOG(ERROR) << "SDL_SetRelativeMouseMode failed: " << SDL_GetError();
		mouseGrabbed = false;
		return;
	}
	mouseGrabbed = flag;
}

void SDLApplication::calculateFps(float& fps, double& prevTime, uint64_t& frameCount) {
	static bool first = true;

	frameCount++;

	double currentTime = getTime();
	double timeInterval = currentTime - prevTime;
	if (timeInterval > 1.0 || first) {
		fps = static_cast<float>(frameCount / timeInterval);
		prevTime = currentTime;
		frameCount = 0;
		first = false;
	}
}

double SDLApplication::getTime() {
	static uint64_t freq;
	static bool first = true;
	if (first) {
		first = false;
		freq = SDL_GetPerformanceFrequency();
	}

	uint64_t counter = SDL_GetPerformanceCounter();
	return static_cast<double>(counter) / static_cast<double>(freq);
}
