#pragma once
#include <windows.h>
#include <stdint.h>


namespace gameApp {
	struct RGBColor {
		uint8_t red, green, blue;
	};

	struct Rect {
		int x, y, width, height;
	};

	/**
	* Uses Windows GDI (CPU bounded)
	* TODO - Use Direct2D (uses GPU)
	*/
	class Renderer {
		friend LRESULT CALLBACK WindowCallback(HWND windowHandle, UINT message, WPARAM wPAram, LPARAM lParam);

		friend class Game;

		struct BitmapBuffer {
			int width, height;
			BITMAPINFO info;
			void* memory;
			int pitch; // how long a row is in bytes
		};
		
		struct BitmapBufferTemp {
			int width, height;
			BITMAPINFO info;
			void* memory;
			int pitch; // how long a row is in bytes
		};

	private:
		//one each for rgb, and one for padding
		static const int bytesPerPixel = 4;

		HWND windowHandle = 0;
		BitmapBuffer buffer;
		
		//buffer to be displayed on window resize
		BitmapBuffer bufferResize;
		RGBColor clearColor;
		Renderer() {
			buffer = {};
			clearColor = {
				255, 255, 255
			};
		}

		Renderer(const Renderer&) = delete;
		Renderer& operator= (const Renderer&) = delete;

		~Renderer() {}

		inline static Renderer& getInstance() {
			static Renderer renderer;
			return renderer;
		}

		inline static void setWindowHandle(HWND _windowHandle) {
			getInstance().windowHandle = _windowHandle;
		}

		static void getWindowDimensions(int* outWidth, int* outHeight);

		static void resizeFrameBuffer(int width, int height);

		static void copyBufferToWindow(HDC deviceContext, int windowWidth, int widnowHeight);

		static void clear();

		static void plotLineLow(int x0, int y0, int x1, int y1, const RGBColor& color);

		static void plotLineHigh(int x0, int y0, int x1, int y1, const RGBColor& color);

	public:
		inline static void SetClearColor(const RGBColor& color) {
			getInstance().clearColor = color;
		}

		static void SetPixel(int x, int y, const RGBColor& color);
		
		static void FillRect(const Rect& rect, const RGBColor& color);

		static void DrawRect(const Rect& rect, const RGBColor& color);

		static void DrawLine(int x0, int y0, int x1, int y1, const RGBColor& color);

		static void DrawCircle(int radius, int originX, int originY, const RGBColor& color);

		static void FillCircle(int radius, int originX, int originY, const RGBColor& color);

		static void RedrawOnResizedWindow(HDC deviceContext, int width, int height);
	};
}