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
		
		width = gameApp::Game::getWindowWidth();
		height = gameApp::Game::getWindowHeight();

		gameApp::Renderer::FillRect({ 250, 250, 320, 180 }, { 0, 255, 0 });
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

		if (gameApp::Input::isKeyPressed(DC_W)) {
			y -= 1000.0f * delta;
			if (y <= 0) {
				y = 0;
			}
		}
		if (gameApp::Input::isKeyPressed(DC_S)) {
			y += 1000.0f * delta;
			if (y >= height) {
				y = height;
			}
		}
		if (gameApp::Input::isKeyPressed(DC_A)) {
			x -= 1000.0f * delta;
			if (x <= 0) {
				x = 0;
			}
		}
		if (gameApp::Input::isKeyPressed(DC_D)) {
			x += 1000.0f * delta;
			if (x >= width) {
				x = width;
			}
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
			wx+= (100.0f *delta);
			wy+= (100.0f *delta);
		}

		if (wx <= 0 || wy <= 0) {
			forwardW = 0;
		}
		else if (wx >= width || wy >= height) {
			forwardW = 1;
		}

		gameApp::Renderer::DrawRect({ 800, 420, 120, 100 }, { 0, 255, 255 });
		gameApp::Renderer::DrawLine(40, 100, 240, 300, { 0, 255, 0});
		gameApp::Renderer::DrawCircle(80, int(z), int(z), { 255, 0, 0});
		//gameApp::Renderer::FillCircle(50, int(x + 0.5f), int(y + 0.5f), { 255, 255, 255 });

		gameApp::Renderer::DrawRectRotated({ int(wx), int(wy), 120, 100 }, { 52, 71, 4 }, int(angle));

		gameApp::Renderer::FillRectRotated({ 100,100,120,100 }, { 0,0,0 }, int(angle));
		gameApp::Renderer::DrawRectRotated({ 100,100,120,100 }, { 255,255,255 }, int(angle));

		//gameApp::Coords coords[6] = { {500, 250}, { 100,20 }, { 400,300 }, { 20,10 }, {10,200 }, {50,30} };
		gameApp::Coords coords[6] = { {int(x + 500 + 0.5f), int(y + 250 + 0.5f)}, {int(x + 100 + 0.5f), int(y + 20 + 0.5f)}, {int(x + 400 + 0.5f), int(y + 300 + 0.5f)},
			{int(x + 20 + 0.5f), int(y + 10 + 0.5f)}, {int(x + 10 + 0.5f), int(y + 200 + 0.5f)}, {int(x + 50 + 0.5f), int(y + 30 + 0.5f)}};

		gameApp::Renderer::DrawFilledPolygon(coords, 6, { 255,255,255 });
		gameApp::Renderer::DrawPolygon(coords, 6, { 0,0,0 });

		angle += 100.0f * delta;

		gameApp::Input::Position pos = gameApp::Input::getMousePosition();

		//swprintf(charBuffer, 256, L"%d, %d\n", pos.x, pos.y);
		//OutputDebugString(charBuffer);
	});

	gameApp::Game::setWindowProperties(L"Test2", 1000, 500);

	gameApp::Renderer::SetClearColor({200, 120, 45});

	gameApp::Game::start();

	return 0;
}