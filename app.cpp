#include "app.h"
#include <windows.h>
#include <string>

appEntryPoint{

	float x = 250.0f, y = 250.0f;

	int width, height;

	std::wstring title = L"Test";

	float z = 100.0f, wx = 800.0f, wy = 420.0f;

	bool forward = 0, forwardW = 0;

	float angle = 10.0f;

	bool wasPressed = 0;

	float xShift = 0.0f, yShift = 0.0f;

	WINDOWPLACEMENT g_wpPrev = { sizeof(g_wpPrev) };

	gameApp::Game::setGameUpdate([&](float delta) {

		wchar_t charBuffer[256];
		//output seconds per frame
		/*swprintf(charBuffer, 256, L"seconds per frame: %f\n", delta);
		OutputDebugString(charBuffer);*/
		//output fps


		static int frames = 0;
		static float timePassed = 0.0f;

		//counts number of frames after a second passes
		frames++;
		timePassed += delta;

		if (timePassed >= 1.0f) {
			swprintf(charBuffer, 256, L"fps: %d\n", frames);
			OutputDebugString(charBuffer);

			timePassed -= 1.0f;
			frames = 0;
		}

		if (gameApp::Game::getInstance().getOption()) {
			return;
		}
		
		width = gameApp::Game::getWindowWidth();
		height = gameApp::Game::getWindowHeight();

		gameApp::Renderer::FillRect({ int(250 + xShift), int(250 + yShift), 320, 180 }, { 0, 255, 0 });
		gameApp::Renderer::FillRect({ int(1000 + xShift), int(250 + yShift), 320, 180 }, { 0, 255, 0 });
		//gameApp::Renderer::FillRect({ int(x + 0.5f), int(y + 0.5f), 320, 180 }, { 0, 255, 0 });
		//gameApp::Renderer::SetPixel(10, 10, { 0, 0, 255 });

		HWND windowHandle = gameApp::Game::getWindowHandle();

		if (gameApp::Input::isKeyPressed(DC_F11)) {
			wasPressed = 1;
		}

		if (wasPressed && gameApp::Input::isKeyReleased(DC_F11)) {
			wasPressed = 0;
			DWORD dwStyle = GetWindowLong(windowHandle, GWL_STYLE);
			if (dwStyle & WS_OVERLAPPEDWINDOW) {
				MONITORINFO mi = { sizeof(mi) };
				if (GetWindowPlacement(windowHandle, &g_wpPrev) && GetMonitorInfo(MonitorFromWindow(windowHandle, MONITOR_DEFAULTTOPRIMARY), &mi)) {
					SetWindowLong(windowHandle, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
					SetWindowPos(windowHandle, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top,
						mi.rcMonitor.right - mi.rcMonitor.left,
						mi.rcMonitor.bottom - mi.rcMonitor.top,
						SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
				}
			}
			else {
				SetWindowLong(windowHandle, GWL_STYLE,
					dwStyle | WS_OVERLAPPEDWINDOW);
				SetWindowPlacement(windowHandle, &g_wpPrev);
				SetWindowPos(windowHandle, NULL, 0, 0, 0, 0,
					SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
					SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
			}
		}

		RECT rect = {};


		if (gameApp::Input::isKeyPressed(DC_W)) {
			y -= 1000.0f * delta;
			/*if (y <= 0) {
				y = 0;

				yShift += 1000.0f * delta;
			}*/

			yShift += 1000.0f * delta;
		}
		GetWindowRect(windowHandle, &rect);

		if (gameApp::Input::isKeyPressed(DC_S)) {
			y += 1000.0f * delta;
			//if (y >= height) {
			//	y = height;
			//	//shift window down

			//	//SetWindowPos(windowHandle, 0, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top + 1000.0f*delta, SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
			//	//gameApp::Game::setWindowProperties(gameApp::Game::getWindowTitle(), rect.right - rect.left, rect.bottom - rect.top + 1000.0f * delta);

			//	yShift -= 1000.0f * delta;
			//}

			yShift -= 1000.0f * delta;
		}
		if (gameApp::Input::isKeyPressed(DC_A)) {
			x -= 1000.0f * delta;
			/*if (x <= 0) {
				x = 0;
				
			}*/

			xShift += 1000.0f * delta;

		}

		GetWindowRect(windowHandle, &rect);

		if (gameApp::Input::isKeyPressed(DC_D)) {
			x += 1000.0f * delta;
			//if (x >= width) {
			//	x = width;

			//	//SetWindowPos(windowHandle, 0, rect.left, rect.top, rect.right - rect.left + 1000.0f * delta, rect.bottom - rect.top, SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
			//	//gameApp::Game::setWindowProperties(gameApp::Game::getWindowTitle(), rect.right - rect.left + 1000.0f * delta, rect.bottom - rect.top);

			//	xShift -= 1000.0f * delta;
			//}

			xShift -= 1000.0f * delta;

		}

		if (gameApp::Input::isMouseButtonPressed(DC_MOUSE_MIDDLE)) {
			//OutputDebugString(L"mouse middle button pressed\n");
		}

		if (forward) {
			z-= (100.0f * delta);
		}
		else {
			z+= (100.0f *delta);
		}

		if (z <= 0) {
			z = 0;
			forward = 0;
		}
		else if (z >= min(width, height)) {
			z = min(width, height);
			forward = 1;
		}
		
		if (forwardW) {
			wx-= (100.0f * delta);
			wy-= (100.0f * delta);
		}
		else {
			wx+= (100.0f * delta);
			wy+= (100.0f * delta);
		}

		if (wx <= 0 || wy <= 0) {
			forwardW = 0;
		}
		else if (wx >= width || wy >= height) {
			forwardW = 1;
		}

		gameApp::Renderer::DrawRect({ int(800 + xShift), int(420 + yShift), 120, 100 }, { 0, 255, 255 });
		gameApp::Renderer::DrawLine(int(40 + xShift), int(100 + yShift), int(240 + xShift), int(300 + yShift), { 0, 255, 0});
		gameApp::Renderer::DrawCircle(80, int(z + xShift), int(z + yShift), { 255, 0, 0});
		//gameApp::Renderer::FillCircle(50, int(x + 0.5f), int(y + 0.5f), { 255, 255, 255 });

		gameApp::Renderer::DrawRectRotated({ int(wx + xShift), int(wy + yShift), 120, 100 }, { 52, 71, 4 }, int(angle));

		gameApp::Renderer::FillRectRotated({ int(100 + xShift), int(100 + yShift), 120, 100 }, { 0,0,0 }, int(angle));
		gameApp::Renderer::DrawRectRotated({ int(100 + xShift), int(100 + yShift), 120, 100 }, { 255,255,255 }, int(angle));

		gameApp::Coords coords2[6] = { {int(500 + xShift), int(250 + yShift)}, { int(100 + xShift), int(20 + yShift)}, 
			{ int(400 + xShift), int(300 + yShift)}, { int(20 + xShift), int(10 + yShift)}, 
			{ int(10 + xShift), int(200 + yShift)}, {int(50 + xShift), int(30 + yShift)} };
		gameApp::Coords coords[6] = { {int(x + 500 + 0.5f + xShift), int(y + 250 + 0.5f + yShift)}, {int(x + 100 + 0.5f + xShift), int(y + 20 + 0.5f + yShift)},
			{int(x + 400 + 0.5f + xShift), int(y + 300 + 0.5f + yShift)},
			{int(x + 20 + 0.5f + xShift), int(y + 10 + 0.5f + yShift)}, 
			{int(x + 10 + 0.5f + xShift), int(y + 200 + 0.5f + yShift)}, 
			{int(x + 50 + 0.5f + xShift), int(y + 30 + 0.5f + yShift)}};

		gameApp::Renderer::DrawFilledPolygon(coords, 6, { 255,255,255 });
		gameApp::Renderer::DrawPolygon(coords, 6, { 0,0,0 });

		gameApp::Renderer::DrawPolygonRotated(coords2, 6, { int(300 + xShift), int(300 + yShift) }, angle, { 249, 29, 165 });
		gameApp::Renderer::FillPolygonRotated(coords2, 6, { int(300 + xShift), int(300 + yShift) }, angle, { 249, 29, 165 });

		angle += 100.0f * delta;

		gameApp::Input::Position pos = gameApp::Input::getMousePosition();

		//swprintf(charBuffer, 256, L"%d, %d\n", pos.x, pos.y);
		//OutputDebugString(charBuffer);
	});

	gameApp::Game::setWindowProperties(L"Test2", 1000, 500);

	gameApp::Renderer::SetClearColor({200, 120, 45});

	//gameApp::Game::start(0);
	gameApp::Game::start(1);

	return 0;
}