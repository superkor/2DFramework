#include "renderDirect2D.h"
#include <windows.h>

namespace gameApp {
	void RenderDirect2D::Render() {
        getInstance().createDeviceResources();
		getInstance().m_pRenderTarget->BeginDraw();
		getInstance().m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		getInstance().m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));

		D2D1_SIZE_F rtSize = getInstance().m_pRenderTarget->GetSize();

        // Draw a grid background.
        int width = static_cast<int>(rtSize.width);
        int height = static_cast<int>(rtSize.height);

        for (int x = 0; x < width; x += 10)
        {
            getInstance().m_pRenderTarget->DrawLine(
                D2D1::Point2F(static_cast<FLOAT>(x), 0.0f),
                D2D1::Point2F(static_cast<FLOAT>(x), rtSize.height),
                getInstance().m_pLightSlateGrayBrush,
                0.5f
            );
        }

        for (int y = 0; y < height; y += 10)
        {
            getInstance().m_pRenderTarget->DrawLine(
                D2D1::Point2F(0.0f, static_cast<FLOAT>(y)),
                D2D1::Point2F(rtSize.width, static_cast<FLOAT>(y)),
                getInstance().m_pLightSlateGrayBrush,
                0.5f
            );
        }

        // Draw two rectangles.
        D2D1_RECT_F rectangle1 = D2D1::RectF(
            rtSize.width / 2 - 50.0f,
            rtSize.height / 2 - 50.0f,
            rtSize.width / 2 + 50.0f,
            rtSize.height / 2 + 50.0f
        );

        D2D1_RECT_F rectangle2 = D2D1::RectF(
            rtSize.width / 2 - 100.0f,
            rtSize.height / 2 - 100.0f,
            rtSize.width / 2 + 100.0f,
            rtSize.height / 2 + 100.0f
        );

        // Draw a filled rectangle.
        getInstance().m_pRenderTarget->FillRectangle(&rectangle1, getInstance().m_pLightSlateGrayBrush);

        // Draw the outline of a rectangle.
        getInstance().m_pRenderTarget->DrawRectangle(&rectangle2, getInstance().m_pCornflowerBlueBrush);

        getInstance().m_pRenderTarget->EndDraw();

        DiscardDeviceResources();
	}

    void RenderDirect2D::OnResize(UINT width, UINT height) {
        if (getInstance().m_pRenderTarget) {
            getInstance().m_pRenderTarget->Resize(D2D1::SizeU(width, height));
        }
    }
}