#pragma once
#include <windows.h>
#include <stdint.h>
#include "keyCodes.h"
#include <windowsx.h>

#define DC_MAX_KEYS 53
#define DC_MAX_MOUSE_BUTTONS 5

namespace gameApp {
	class Input {
		friend LRESULT CALLBACK WindowCallback(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);

	public:
		struct KeyState {
			bool wasDown, isDown;
		};

		struct KeyBoardInputMap {
			KeyState keys[DC_MAX_KEYS];
		};

		struct ButtonState {
			bool wasDown, isDown;
		};

		struct Position{
			int x, y;
		};

		struct MouseInputMap {
			ButtonState buttons[DC_MAX_MOUSE_BUTTONS];
			Position position;
		};

		static KeyState GetKeyState(uint32_t keycode);

		static bool isKeyPressed(uint32_t keycode);

		static bool isKeyReleased(uint32_t keycode);

		//returns true if key has just been pressed
		static bool wasKeyHit(uint32_t keycode);
		
		static Position getMousePosition();

		static bool isMouseButtonPressed(unsigned int buttonCode);

		static bool isMouseButtonReleased(unsigned int buttonCode);

		//returns true if mouse button has just been pressed
		static bool wasMouseButtonHit(unsigned int buttonCode);
	private:
		static void processKeyboardInput(uint32_t keycode, bool wasDown, bool isDown);

		static KeyBoardInputMap keyboard;

		static MouseInputMap mouse;

		static void processMouseInput(WPARAM wParam, LPARAM lParam);

		static void updateMousePosition(LPARAM lParam);
	};
}