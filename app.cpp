#include "app.h"
#include <windows.h>
#include <string>

appEntryPoint{

	float x = 250.0f, y = 250.0f;

	int width, height;

	std::wstring title = L"Test";

	float z = 0.0f;

	bool forward = 0;

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


		gameApp::Renderer::DrawRect({ 800, 420, 120, 100 }, { 0, 255, 255 });
		gameApp::Renderer::DrawLine(40, 100, 240, 300, { 0, 255, 0});
		gameApp::Renderer::DrawCircle(80, int(100.0f + z), int(100.0f + z), { 255, 0, 0});
		gameApp::Renderer::FillCircle(50, int(x + 0.5f), int(y + 0.5f), { 255, 255, 255 });

		gameApp::Input::Position pos = gameApp::Input::getMousePosition();

		//swprintf(charBuffer, 256, L"%d, %d\n", pos.x, pos.y);
		//OutputDebugString(charBuffer);
	});

	gameApp::Game::setWindowProperties(L"Test2", 1000, 500);

	gameApp::Renderer::SetClearColor({200, 120, 45});

	gameApp::Game::start();

	return 0;
}