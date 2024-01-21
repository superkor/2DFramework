#include "game.h"
#include "render.h"
#include <windows.h>
#include "input.h"
#include "renderDirect2D.h"

namespace gameApp {
	Game::Game() {
		windowTitle = L"Test";
		windowWidth = 400;
		windowHeight = 400;
	}
	WINDOWPLACEMENT g_wpPrev = { sizeof(g_wpPrev) };


	// handles window events
	LRESULT CALLBACK WindowCallback(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam) {
		LRESULT result = 0;

		switch (message) {
			case WM_CLOSE:
				{
					Game::getInstance().running = false;
					OutputDebugString(L"window closed\n");

				}
				break;

			case WM_DESTROY:
				{
					Game::getInstance().running = false;
					OutputDebugString(L"window destroy\n");
				}
				
				break;

			case WM_SYSKEYDOWN:
			case WM_SYSKEYUP:
			case WM_KEYUP:
			case WM_KEYDOWN:
			{
				//get virtual key code
				uint32_t VKCode = wParam;
				bool wasDown = (lParam & (1 << 30)) != 0;
				bool isDown = (lParam & (1 << 31)) == 0;

				Input::processKeyboardInput(VKCode, wasDown, isDown);

			} break;

			case WM_LBUTTONDOWN:
			case WM_LBUTTONUP:
			case WM_RBUTTONDOWN:
			case WM_RBUTTONUP:
			case WM_MBUTTONDOWN:
			case WM_MBUTTONUP:
			case WM_XBUTTONDOWN:
			case WM_XBUTTONUP:
			{
				Input::processMouseInput(wParam, lParam);
			} break;

			case WM_MOUSEMOVE:
			{
				Input::updateMousePosition(lParam);
			} break;

			case WM_SIZE:
			{
				unsigned int width = LOWORD(lParam);
				unsigned int height = HIWORD(lParam);
				if (!Game::getInstance().renderEngineOption) {
					wchar_t buf[256];
					swprintf(buf, 256, L"width %d height %d\n", width, height);
					OutputDebugString(buf);

					Game::setWindowProperties(Game::getWindowTitle(), width, height);

					PAINTSTRUCT paint;
					HDC deviceContext = BeginPaint(windowHandle, &paint);
					Renderer::RedrawOnResizedWindow(deviceContext, width, height);

					EndPaint(windowHandle, &paint);

					RedrawWindow(windowHandle, 0, 0, RDW_INVALIDATE);
				}
				else {
					RenderDirect2D::OnResize(width, height);
				}


			} break;
			
			case WM_PAINT: 
			{
				if (!Game::getInstance().renderEngineOption) {
					int width, height;
					Renderer::getWindowDimensions(&width, &height);



					PAINTSTRUCT paint;
					HDC device_context = BeginPaint(windowHandle, &paint);

					Renderer::copyBufferToWindow(device_context, width, height);

					EndPaint(windowHandle, &paint);
				}
				else {
					//Render Direct2D
					//OutputDebugString(L"Draw Direct2D frame\n");
					Game::getInstance().update(1.0f);
					/*RenderDirect2D::BeginDraw();
					RenderDirect2D::ClearWindow(0xFFFFFFFF);
					RenderDirect2D::EndDraw();*/
					ValidateRect(windowHandle, NULL);
				}

		
			}
				
			break;

			default:
				result = DefWindowProc(windowHandle, message, wParam, lParam);
		}


		return result;
	}

	void Game::startWindow(bool renderOption) {
		Renderer::resizeFrameBuffer(windowWidth, windowHeight);
		
		const wchar_t* className = L"Game Window";

		WNDCLASSEX windowClass = {};

		windowClass.cbSize = sizeof(WNDCLASSEX);
		windowClass.style = CS_HREDRAW | CS_VREDRAW;
		windowClass.lpfnWndProc = WindowCallback;
		windowClass.hInstance = hInstance;
		windowClass.lpszClassName = className;

		windowClass.hCursor = LoadCursorW(NULL, IDC_ARROW);

		if (!RegisterClassEx(&windowClass)) {
			OutputDebugString(L"Failed to register window class\n");
			return;
		}

		DWORD windowStyle = WS_MAXIMIZEBOX | WS_SYSMENU | WS_CAPTION;

		RECT windowRect = {};
		windowRect.left = 0;
		windowRect.top = 0;
		windowRect.right = windowWidth;
		windowRect.bottom = windowHeight;

		AdjustWindowRectEx(&windowRect, windowStyle, false, 0);
		
		windowHandle = CreateWindowEx(
			0,
			className,
			windowTitle.c_str(),
			/*WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX |*/ WS_OVERLAPPEDWINDOW  | WS_VISIBLE,
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			windowRect.right - windowRect.left,
			windowRect.bottom - windowRect.top,
			0,
			0,
			hInstance,
			0
		);

		if (!windowHandle) {
			OutputDebugString(L"Failed to create a window\n");
			return;
		}

		running = true;

		if (!renderOption) {
			Renderer::setWindowHandle(windowHandle);
		}
		else {
			RenderDirect2D::setWindowHandle(windowHandle);
		}

		//init clock
		LARGE_INTEGER cpuFreq;
		QueryPerformanceFrequency(&cpuFreq);
		
		LARGE_INTEGER lastCounter;
		QueryPerformanceCounter(&lastCounter);

		while (running) {

			LARGE_INTEGER currCounter;
			QueryPerformanceCounter(&currCounter);

			int64_t counterElpsed = currCounter.QuadPart - lastCounter.QuadPart;

			float delta = (float)counterElpsed / (float)cpuFreq.QuadPart; // in seconds

			lastCounter = currCounter;

			MSG message;
			while (PeekMessage(&message, 0, 0, 0, PM_REMOVE)) {
				if (message.message == WM_QUIT) {
					running = false;
				}

				TranslateMessage(&message);
				DispatchMessage(&message);
			}


			if (!renderOption) {
				Renderer::clear();
				getInstance().update(delta);

				HDC deviceContext = GetDC(windowHandle);
				int width, height;
				Renderer::getWindowDimensions(&width, &height);

				Renderer::copyBufferToWindow(deviceContext, width, height);

				ReleaseDC(windowHandle, deviceContext);

			}

			if (renderOption) {
				getInstance().update(delta);
			}

		}

		PostQuitMessage(0);

	}
}
