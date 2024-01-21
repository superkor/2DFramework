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
	struct RectFloat {
		float x, y, width, height;
	};

	struct WindowSize {
		float width, height;
	};

	/// <summary></summary>
	/// <param name="option"> 0: Rotation, 1: Scale, 2: Skew, 3: Translation </param>
	/// <param name="x"> x coord </param>
	/// <param name="y"> y coord </param>
	/// <param name="param1"> parameter 1 for the appropriate transformation </param>
	/// <param name="param2"> parameter 2 for the appropriate transformation (if applicable) </param>
	struct Transformation {
		unsigned char option;
		float x, y, param1, param2;
	};

	/// <summary>
	/// List of all transformations to be done to a geometry
	/// </summary>
	struct TransformationList {
		struct Transformation *transformations;
		unsigned int length;
	};

	class RenderDirect2D {
		friend LRESULT CALLBACK WindowCallback(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam);

		friend class Game;

	private:
		ID2D1Factory* m_pDirect2dFactory;
		ID2D1HwndRenderTarget* m_pRenderTarget;
		HWND windowHandle = 0;

		RenderDirect2D() {
			m_pDirect2dFactory = NULL;
			m_pRenderTarget = NULL;
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

		/// <summary>
		/// 
		/// </summary>
		/// <param name="transformation"><see cref="struct Transformation"> </see></param>
		/// <returns></returns>
		void setTransformation(const struct TransformationList& transformations);

		D2D_MATRIX_3X2_F prevTransform = {};

		/// <summary>
		/// Resets to default transformation matrix
		/// Used before drawing a new geometry
		/// </summary>
		inline static void ResetTransformation() {
			getInstance().m_pRenderTarget->SetTransform(getInstance().prevTransform);
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
				&getInstance().m_pRenderTarget
			);
		}

		static void OnResize(UINT width, UINT height);

		static inline void DiscardDeviceResources() {
			if (!getInstance().m_pDirect2dFactory) {
				getInstance().m_pDirect2dFactory->Release();
			}
			if (!getInstance().m_pRenderTarget) {
				getInstance().m_pRenderTarget->Release();
			}
		}

		static void DrawRect(const struct RectFloat& rect, unsigned int hex, float alpha, const struct TransformationList &transformations);
		
		static void FillRect(const struct RectFloat& rect, unsigned int hex, float alpha, const struct TransformationList& transformations);

		static void BeginDraw();

		static void EndDraw();

		static void ClearWindow(unsigned int color);

		static WindowSize GetWindowSize();

		static void DrawLine(float x0, float y0, float x1, float y1, unsigned int hex, float alpha, float width, const struct TransformationList& transformations);

	};
}