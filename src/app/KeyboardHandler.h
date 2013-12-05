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
		auto key = event.keysym.sym;
		if (event.state == SDL_PRESSED) {
			m_keyStates[key].pressed = true;
		} else if (event.state == SDL_RELEASED) {
			m_keyStates[key].once = false;
			m_keyStates[key].pressed = false;
		}
	}

	bool isPressed(SDL_Keycode code) {
		return m_keyStates[code].pressed == true;
	}

	bool isPressedOnce(SDL_Keycode code) {
		bool pressed = m_keyStates[code].pressed;
		bool result = pressed == true && m_keyStates[code].once == false;
		if (pressed)
			m_keyStates[code].once = true;
		return result;
	}

	bool isReleased(SDL_Keycode code) {
		return m_keyStates[code].pressed == false;
	}
private:
	struct State 
	{
		bool pressed;
		bool once;
	};

	std::unordered_map<SDL_Keycode, State> m_keyStates;
};

#endif // !KEYBOARD_HANDLER_H
