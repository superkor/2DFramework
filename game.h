#pragma once

#include <windows.h>
#include <string>
#include <functional>

namespace gameApp {
	class Game {
		friend LRESULT CALLBACK WindowCallback(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);
	private:
		HINSTANCE hInstance;
		HWND windowHandle;
		bool running = false;

		std::wstring windowTitle;
		int windowWidth, windowHeight;
		
		std::function<void(float delta)> update;

		void startWindow();
	
	public:
		Game();

		//Game object is a singleton (only need one class object)
		Game(const Game&) = delete;

		Game& operator= (const Game&) = delete;

		~Game() {};

		static HWND& getWindowHandle() {
			static HWND handle;
			return handle;
		}

		static Game& getInstance() {
			static Game game;
			return game;
		}

		inline static void start() {
			getInstance().startWindow();
		}

		inline static void setWindowProperties(const std::wstring& title, const int& width, const int& height) {
			getInstance().windowTitle = title;
			getInstance().windowWidth = width;
			getInstance().windowHeight = height;
		}

		inline static void setGameUpdate(const std::function<void(float delta)>& update) {
			getInstance().update = update;
		}

		inline static std::wstring getWindowTitle() { 
			return getInstance().windowTitle; 
		}

		inline static int getWindowWidth() { 
			return getInstance().windowWidth; 
		}

		inline static int getWindowHeight() { 
			return getInstance().windowHeight; 
		}
	};
}