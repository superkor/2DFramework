#pragma once
#include <windows.h>
#include <stdint.h>

#define PI 3.1415926525897

namespace gameApp {
	struct RGBColor {
		uint8_t red, green, blue;
	};

	struct Rect {
		int x, y, width, height;
	};

	struct Coords {
		int x, y;
	};

	/**
	* Uses Windows GDI (CPU bounded) for the rendering engine
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

		static void PlotLineLow(int x0, int y0, int x1, int y1, const RGBColor& color);

		static void PlotLineHigh(int x0, int y0, int x1, int y1, const RGBColor& color);

		static int RotateXCoord(int x, int y, float angle);

		static int RotateYCoord(int x, int y, float angle);

		/**
		* @param int minMax[] - should be length 4: minY, maxY, minX, maxX
		* @param int length - length of the arrays xCorners and yCorners
		*/
		static void FillPolygon(float xCorners[], float yCorners[], int length, int minMax[], const RGBColor& color);

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

		//rotate a rect from its given coordinates by an angle about its center
		static void DrawRectRotated(const Rect& rect, const RGBColor& color, float angle);

		static void FillRectRotated(const Rect& rect, const RGBColor& color, float angle);

		static void DrawPolygon(const Coords coords[], int length, const RGBColor& color);

		static void DrawFilledPolygon(const Coords coords[], int length, const RGBColor& color);

		static void DrawPolygonRotated(const Coords coords[], int length, const Coords rotateCoord, float angle, const RGBColor& color);

		static void FillPolygonRotated(const Coords coords[], int length, const Coords rotateCoord, float angle, const RGBColor& color);
	};
}