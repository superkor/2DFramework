#pragma once
#pragma comment(lib, "d2d1.lib")

#include <windows.h>
#include <wchar.h>
#include <math.h>
#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>
#include "game.h"


namespace gameApp {
	/**
	* Renderer Engine within the Framework using Direct2D API (GPU Bound)
	*/
	class RenderDirect2D {
		friend LRESULT CALLBACK WindowCallback(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);

		friend class Game;
	private:
		ID2D1Factory* m_pDirect2dFactory;
		ID2D1HwndRenderTarget* m_pRenderTarget;
		ID2D1SolidColorBrush* m_pLightSlateGrayBrush;
		ID2D1SolidColorBrush* m_pCornflowerBlueBrush;
		HWND windowHandle = 0;

		RenderDirect2D() {
			m_pDirect2dFactory = NULL;
			m_pRenderTarget = NULL;
			m_pLightSlateGrayBrush = NULL;
			m_pCornflowerBlueBrush = NULL;

			D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pDirect2dFactory);
		}

		~RenderDirect2D() {
			DiscardDeviceResources();
		}

		RenderDirect2D(const RenderDirect2D&) = delete;
		RenderDirect2D& operator= (const RenderDirect2D&) = delete;

		inline static void setWindowHandle(HWND _windowHandle) {
			getInstance().windowHandle = _windowHandle;
		}


	public:
		inline static RenderDirect2D& getInstance() {
			static RenderDirect2D renderer;
			return renderer;
		}

		//initially implementing the direct2d quick start as test https://learn.microsoft.com/en-us/windows/win32/direct2d/direct2d-quickstart
		inline static void createDeviceResources() {
			if (getInstance().m_pRenderTarget) {
				return;
			}

			RECT rc;
			GetClientRect(getInstance().windowHandle, &rc);

			D2D1_SIZE_U size = D2D1::SizeU(
				rc.right - rc.left,
				rc.bottom - rc.top
			);

			getInstance().m_pDirect2dFactory->CreateHwndRenderTarget(
				D2D1::RenderTargetProperties(),
				D2D1::HwndRenderTargetProperties(getInstance().windowHandle, size),
				&getInstance().m_pRenderTarget);

			getInstance().m_pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::LightSlateGray),
				&getInstance().m_pLightSlateGrayBrush);
			
			getInstance().m_pRenderTarget->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::CornflowerBlue),
				&getInstance().m_pCornflowerBlueBrush);
		}

		static void Render();

		static void OnResize(UINT width, UINT height);

		static inline void DiscardDeviceResources() {
			if (!getInstance().m_pDirect2dFactory) {
				getInstance().m_pDirect2dFactory->Release();
			}
			if (!getInstance().m_pRenderTarget) {
				getInstance().m_pRenderTarget->Release();
			}
			if (!getInstance().m_pLightSlateGrayBrush) {
				getInstance().m_pLightSlateGrayBrush->Release();
			}
			if (!getInstance().m_pCornflowerBlueBrush) {
				getInstance().m_pCornflowerBlueBrush->Release();
			}
		}
	};
}