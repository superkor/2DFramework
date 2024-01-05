#include <windows.h>
#include "renderDirect2D.h"

namespace gameApp {
    void RenderDirect2D::BeginDraw() {
        getInstance().createDeviceResources();
        getInstance().m_pRenderTarget->BeginDraw();
    }

    void RenderDirect2D::EndDraw() {
        getInstance().m_pRenderTarget->EndDraw();
        DiscardDeviceResources();
    }

    void RenderDirect2D::ClearWindow(unsigned int color = 0xFFFFFFFF) {
        getInstance().m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
        getInstance().m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF(color)));
    }

    WindowSize RenderDirect2D::GetWindowSize() {
         D2D1_SIZE_F window = getInstance().m_pRenderTarget->GetSize();

         return { window.width, window.height };
    }

    void RenderDirect2D::DrawLine(float x0, float y0, float x1, float y1, unsigned int hex, float alpha, float width) {
        ID2D1SolidColorBrush* brush = NULL;

        getInstance().m_pRenderTarget->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF(hex, alpha)),
            &brush);

        getInstance().m_pRenderTarget->DrawLine(
            D2D1::Point2F(x0, y0),
            D2D1::Point2F(x1, y1),
            brush,
            width
        );

        brush->Release();
    }

	void RenderDirect2D::Render() {
        BeginDraw();
        ClearWindow();

        WindowSize rtSize = GetWindowSize();

        // Draw a grid background.
        int width = static_cast<int>(rtSize.width);
        int height = static_cast<int>(rtSize.height);

        for (int x = 0; x < width; x += 10)
        {
            DrawLine(x, 0.f, x, rtSize.height, 0x778899, 1.f, 0.5f);
            /*getInstance().m_pRenderTarget->DrawLine(
                D2D1::Point2F(static_cast<FLOAT>(x), 0.0f),
                D2D1::Point2F(static_cast<FLOAT>(x), rtSize.height),
                getInstance().m_pLightSlateGrayBrush,
                0.5f
            );*/
        }

        for (int y = 0; y < height; y += 10)
        {
            DrawLine(0.f, y, rtSize.width, y, 0x778899, 1.f, 0.5f);

            /*getInstance().m_pRenderTarget->DrawLine(
                D2D1::Point2F(0.0f, static_cast<FLOAT>(y)),
                D2D1::Point2F(rtSize.width, static_cast<FLOAT>(y)),
                getInstance().m_pLightSlateGrayBrush,
                0.5f
            );*/
        }

        struct RectFloat rect = { rtSize.width / 2 - 50.0f, rtSize.height / 2 - 50.0f, 100.0f, 100.0f };


        // Draw two rectangles.
        D2D1_RECT_F rectangle1 = D2D1::RectF(
            rtSize.width / 2 - 50.0f,
            rtSize.height / 2 - 50.0f,
            rtSize.width / 2 + 50.0f,
            rtSize.height / 2 + 50.0f
        );

        //D2D1_RECT_F rectangle2 = D2D1::RectF(
        //    rtSize.width / 2 - 100.0f,
        //    rtSize.height / 2 - 100.0f,
        //    rtSize.width / 2 + 100.0f,
        //    rtSize.height / 2 + 100.0f
        //);

        // Draw a filled rectangle.
        //getInstance().m_pRenderTarget->FillRectangle(&rectangle1, getInstance().m_pLightSlateGrayBrush);

        FillRect(rect, 0x778899, 1.0f);

        rect = { rtSize.width / 2 - 100.0f, rtSize.height / 2 - 100.0f, 200.0f, 200.0f };

        DrawRect(rect, 0x4E9997, 1.0f);

        // Draw the outline of a rectangle.
        //getInstance().m_pRenderTarget->DrawRectangle(&rectangle2, getInstance().m_pCornflowerBlueBrush);

        EndDraw();
	}

    void RenderDirect2D::OnResize(UINT width, UINT height) {
        if (getInstance().m_pRenderTarget) {
            getInstance().m_pRenderTarget->Resize(D2D1::SizeU(width, height));
        }
    }

    void RenderDirect2D::DrawRect(const struct RectFloat& rect, unsigned int hex, float alpha){
        D2D1_RECT_F rectangle = D2D1::RectF(
            rect.x,
            rect.y,
            rect.x + rect.width,
            rect.y + rect.height
        );

        ID2D1SolidColorBrush* brush = NULL;

        getInstance().m_pRenderTarget->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF(hex, alpha)),
            &brush);

        getInstance().m_pRenderTarget->DrawRectangle(&rectangle, brush);

        brush->Release();

    }
    
    void RenderDirect2D::FillRect(const struct RectFloat& rect, unsigned int hex, float alpha) {
        D2D1_RECT_F rectangle = D2D1::RectF(
            rect.x,
            rect.y,
            rect.x + rect.width,
            rect.y + rect.height
        );

        ID2D1SolidColorBrush* brush = NULL;


        getInstance().m_pRenderTarget->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF( hex, alpha )),
            &brush);

        getInstance().m_pRenderTarget->FillRectangle(&rectangle, brush);

        brush->Release();

    }
}