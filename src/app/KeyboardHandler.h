/**
 * @file KeyboardHandler.h
 *
 * @author Jan Dušek <xdusek17@stud.fit.vutbr.cz>
 * @date 2013
 */

#ifndef KEYBOARD_HANDLER_H
#define KEYBOARD_HANDLER_H

#include <SDL.h>

#include <unordered_map>

/**
 * Tracks state of keyboard.
 * Using this class we can allow multiple key presses.
 */
class KeyboardHandler
{
public:
	void handleEvent(SDL_KeyboardEvent& event) {
		m_keyStates[event.keysym.sym] = event.state == SDL_PRESSED ? true : false;
	}

	bool isPressed(SDL_Keycode code){
		return m_keyStates[code] == true;
	}

	bool isReleased(SDL_Keycode code) {
		return m_keyStates[code] == false;
	}
private:
	std::unordered_map<SDL_Keycode, bool> m_keyStates;
};

#endif // !KEYBOARD_HANDLER_H
