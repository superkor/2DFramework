#include <windows.h>
#include "renderDirect2D.h"

namespace gameApp {
    void RenderDirect2D::BeginDraw() {
        getInstance().createDeviceResources();
        getInstance().m_pRenderTarget->GetTransform(&getInstance().prevTransform);
        getInstance().m_pRenderTarget->BeginDraw();
    }

    void RenderDirect2D::EndDraw() {
        HRESULT hres = getInstance().m_pRenderTarget->EndDraw();
        if (hres == D2DERR_RECREATE_TARGET) {
            DiscardDeviceResources();
        }
        getInstance().ResetTransformation();
        
    }

    void RenderDirect2D::ClearWindow(unsigned int color = 0x7FFFFFFF) {
        getInstance().m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
        getInstance().m_pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF(color)));
    }

    WindowSize RenderDirect2D::GetWindowSize() {
         D2D1_SIZE_F window = getInstance().m_pRenderTarget->GetSize();

         return { window.width, window.height };
    }

    void RenderDirect2D::DrawLine(float x0, float y0, float x1, float y1, unsigned int hex, float alpha, float width, const struct TransformationList& transformations) {
        ID2D1SolidColorBrush* brush = NULL;

        getInstance().m_pRenderTarget->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF(hex, alpha)),
            &brush);

        getInstance().setTransformation(transformations);
        
        getInstance().m_pRenderTarget->DrawLine(
            D2D1::Point2F(x0, y0),
            D2D1::Point2F(x1, y1),
            brush,
            width
        );

        brush->Release();
        ResetTransformation();
    }

    void RenderDirect2D::setTransformation(const struct TransformationList& transformations) {
        D2D1_MATRIX_3X2_F transform = getInstance().prevTransform;
        for (unsigned int i = 0; i < transformations.length; i++) {
            if (transformations.transformations[i].option > 3) {
                continue;
            }
            switch (transformations.transformations[i].option) {
            case 0:
                transform = transform * D2D1::Matrix3x2F::Rotation(
                    transformations.transformations[i].param1,
                    D2D1::Point2F(transformations.transformations[i].x, transformations.transformations[i].y)
                );
                continue;
            case 1:
                transform = transform * D2D1::Matrix3x2F::Scale(
                    transformations.transformations[i].param1,
                    transformations.transformations[i].param2,
                    D2D1::Point2F(transformations.transformations[i].x, transformations.transformations[i].y)
                );
                continue;

            case 2:
                transform = transform * D2D1::Matrix3x2F::Skew(
                    transformations.transformations[i].param1,
                    transformations.transformations[i].param2,
                    D2D1::Point2F(transformations.transformations[i].x, transformations.transformations[i].y)
                );
                continue;

            case 3:
                transform = transform * D2D1::Matrix3x2F::Translation(
                    transformations.transformations[i].param1,
                    transformations.transformations[i].param2
                );
                continue;

            default:
                continue;
            }
        }

        getInstance().m_pRenderTarget->SetTransform(transform);
    }

    void RenderDirect2D::OnResize(UINT width, UINT height) {
        if (getInstance().m_pRenderTarget) {
            getInstance().m_pRenderTarget->Resize(D2D1::SizeU(width, height));
        }
    }

    void RenderDirect2D::DrawRect(const struct RectFloat& rect, unsigned int hex, float alpha, const struct TransformationList& transformations) {
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

        getInstance().setTransformation(transformations);

        getInstance().m_pRenderTarget->DrawRectangle(&rectangle, brush);

        brush->Release();
        ResetTransformation();

    }

    void RenderDirect2D::FillRect(const struct RectFloat& rect, unsigned int hex, float alpha, const struct TransformationList& transformations) {
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

        getInstance().setTransformation(transformations);

        getInstance().m_pRenderTarget->FillRectangle(&rectangle, brush);

        brush->Release();
        ResetTransformation();
    }
}

