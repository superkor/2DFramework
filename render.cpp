#include "render.h"
#include <math.h>

namespace gameApp {
	void Renderer::SetPixel(int x, int y, const RGBColor& color) {
		BitmapBuffer& buffer = getInstance().buffer;

		//clipping
		if (x < 0 || x >= buffer.width || y < 0 || y >= buffer.height) {
			return;
		}

		//convert (u8, u8, u8) rgb color to u32 representation
		uint32_t raw_color = (color.red << 16) | (color.green << 8) | (color.blue);

		uint8_t* row = (uint8_t*)buffer.memory + x * bytesPerPixel + y * buffer.pitch;

		uint32_t* pixel = (uint32_t*)row;
		*pixel = raw_color;
	}

	void Renderer::FillRect(const Rect& rect, const RGBColor& color)
	{
		BitmapBuffer& buffer = getInstance().buffer;

		//get top left corner of the rect
		int minX = rect.x;
		int minY = rect.y;

		//get bottom right corner of the rect
		int maxX = rect.x + rect.width;
		int maxY = rect.y + rect.height;

		// clipping
		if (minX < 0) {
			minX = 0;
		}
		if (minY < 0) {
			minY = 0;
		}
		if (maxX > buffer.width) {
			maxX = buffer.width;
		}
		if (maxY > buffer.height) {
			maxY = buffer.height;
		}

		// convert (u8, u8, u8) rgb color to u32 representation
		uint32_t raw_color = (color.red << 16) | (color.green << 8) | (color.blue << 0);

		uint8_t* row = (uint8_t*)buffer.memory + minX * bytesPerPixel + minY * buffer.pitch;
		for (int y = minY; y < maxY; y++)
		{
			uint32_t* pixel = (uint32_t*)row;
			for (int x = minX; x < maxX; x++)
			{
				*pixel++ = raw_color;
			}
			row += buffer.pitch;
		}
	}

	void Renderer::getWindowDimensions(int* outWidth, int* outHeight) {
		RECT clientRect;
		GetClientRect(getInstance().windowHandle, &clientRect);

		*outWidth = clientRect.right - clientRect.left;
		*outHeight = clientRect.bottom - clientRect.top;
	}

	void Renderer::resizeFrameBuffer(int width, int height) {
		BitmapBuffer& buffer = getInstance().buffer;

		if (buffer.memory) {
			VirtualFree(buffer.memory, 0, MEM_RELEASE);
		}

		buffer.width = width;
		buffer.height = height;

		buffer.info.bmiHeader.biSize = sizeof(buffer.info.bmiHeader);
		buffer.info.bmiHeader.biWidth = buffer.width;
		buffer.info.bmiHeader.biHeight = -(buffer.height);
		buffer.info.bmiHeader.biPlanes = 1;
		buffer.info.bmiHeader.biBitCount = 32;
		buffer.info.bmiHeader.biCompression = BI_RGB;

		int bufferMemorySize = buffer.width * buffer.height * bytesPerPixel;
		buffer.memory = VirtualAlloc(0, bufferMemorySize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
		buffer.pitch = buffer.width * bytesPerPixel;
	}

	void Renderer::copyBufferToWindow(HDC deviceContext, int windowWidth, int windowHeight)
	{
		BitmapBuffer& buffer = getInstance().buffer;

		StretchDIBits(
			deviceContext,
			0, 0, windowWidth, windowHeight,
			0, 0, buffer.width, buffer.height,
			buffer.memory,
			&(buffer.info),
			DIB_RGB_COLORS,
			SRCCOPY
		);
	}

	void Renderer::RedrawOnResizedWindow(HDC deviceContext, int width, int height) {
		BitmapBuffer& buffer = getInstance().buffer;

		if (!buffer.memory) {
			return;
		}

		if (width == 0 || height == 0) {
			return;
		}

		BitmapBuffer temp;

		int bufferMemorySize = width * height * bytesPerPixel;
		temp.memory = VirtualAlloc(0, bufferMemorySize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

		temp.info.bmiHeader.biSize = sizeof(buffer.info.bmiHeader);
		temp.info.bmiHeader.biWidth = buffer.width;
		temp.info.bmiHeader.biHeight = -(buffer.height);
		temp.info.bmiHeader.biPlanes = 1;
		temp.info.bmiHeader.biBitCount = 32;
		temp.info.bmiHeader.biCompression = BI_RGB;

		Rect rect = { 0, 0, buffer.width, buffer.height };
		RGBColor color = getInstance().clearColor;

		int tempPitch = width * bytesPerPixel;

		//get top left corner of the rect
		int minX = rect.x;
		int minY = rect.y;

		//get bottom right corner of the rect
		int maxX = rect.x + rect.width;
		int maxY = rect.y + rect.height;

		// clipping
		if (minX < 0) {
			minX = 0;
		}
		if (minY < 0) {
			minY = 0;
		}
		if (maxX > width) {
			maxX = width;
		}
		if (maxY > height) {
			maxY = height;
		}

		// convert (u8, u8, u8) rgb color to u32 representation
		uint32_t raw_color = (color.red << 16) | (color.green << 8) | (color.blue << 0);

		uint8_t* row = (uint8_t*)temp.memory + minX * bytesPerPixel + minY * tempPitch;
		for (int y = minY; y < maxY; y++)
		{
			uint32_t* pixel = (uint32_t*)row;
			for (int x = minX; x < maxX; x++)
			{
				*pixel++ = raw_color;
			}
			row += tempPitch;
		}

		StretchDIBits(
			deviceContext,
			0, 0, width, height,
			0, 0, width, height,
			temp.memory,
			&(buffer.info),
			DIB_RGB_COLORS,
			SRCCOPY
		);

		resizeFrameBuffer(width, height);

		clear();

		VirtualFree(temp.memory, 0, MEM_RELEASE);
	}

	void Renderer::clear()
	{
		BitmapBuffer& buffer = getInstance().buffer;

		FillRect({ 0, 0, buffer.width, buffer.height }, getInstance().clearColor);
	}

	void Renderer::DrawRect(const Rect& rect, const RGBColor& color) {
		BitmapBuffer& buffer = getInstance().buffer;

		int minX = rect.x;
		int minY = rect.y;
		int maxX = rect.x + rect.width;
		int maxY = rect.y + rect.height;

		// clipping
		if (minX < 0) {
			minX = 0;
		}
		if (minY < 0) {
			minY = 0;
		}
		if (maxX > buffer.width) {
			maxX = buffer.width;
		}
		if (maxY > buffer.height) {
			maxY = buffer.height;
		}

		for (int x = minX; x <= maxX; x++)
		{
			SetPixel(x, minY, color);
			SetPixel(x, maxY, color);
		}

		for (int y = minY; y <= maxY; y++)
		{
			SetPixel(minX, y, color);
			SetPixel(maxX, y, color);
		}
	}

	void Renderer::PlotLineLow(int x0, int y0, int x1, int y1, const RGBColor& color)
	{
		int dx = x1 - x0;
		int dy = y1 - y0;
		int yi = 1;
		if (dy < 0)
		{
			yi = -1;
			dy = -dy;
		}
		int D = (2 * dy) - dx;
		int y = y0;

		for (int x = x0; x <= x1; x++)
		{
			SetPixel(x, y, color);
			if (D > 0)
			{
				y = y + yi;
				D = D + (2 * (dy - dx));
			}
			else
			{
				D = D + 2 * dy;
			}
		}
	}

	void Renderer::PlotLineHigh(int x0, int y0, int x1, int y1, const RGBColor& color)
	{
		int dx = x1 - x0;
		int dy = y1 - y0;
		int xi = 1;
		if (dx < 0)
		{
			xi = -1;
			dx = -dx;
		}
		int D = (2 * dx) - dy;
		int x = x0;

		for (int y = y0; y <= y1; y++)
		{
			SetPixel(x, y, color);
			if (D > 0)
			{
				x = x + xi;
				D = D + (2 * (dx - dy));
			}
			else
			{
				D = D + 2 * dx;
			}
		}
	}

	void Renderer::DrawLine(int x0, int y0, int x1, int y1, const RGBColor& color)
	{
		if (abs(y1 - y0) < abs(x1 - x0))
		{
			if (x0 > x1)
				PlotLineLow(x1, y1, x0, y0, color);
			else
				PlotLineLow(x0, y0, x1, y1, color);
		}
		else
		{
			if (y0 > y1)
				PlotLineHigh(x1, y1, x0, y0, color);
			else
				PlotLineHigh(x0, y0, x1, y1, color);
		}
	}

	void Renderer::DrawCircle(int radius, int originX, int originY, const RGBColor& color)
	{
		for (int y = -radius; y <= radius; y++)
			for (int x = -radius; x <= radius; x++)
				if (x * x + y * y > radius * radius - radius && x * x + y * y < radius * radius + radius)
					SetPixel(originX + x, originY + y, color);
	}

	void Renderer::FillCircle(int radius, int originX, int originY, const RGBColor& color)
	{
		for (int y = -radius; y <= radius; y++)
			for (int x = -radius; x <= radius; x++)
				if (x * x + y * y < radius * radius + radius)
					SetPixel(originX + x, originY + y, color);
	}

	int Renderer::RotateXCord(int x, int y, float angle) {
		return (x * cos(angle * PI / 180) - y * sin(angle * PI / 180)) + 0.5;
	}

	int Renderer::RotateYCord(int x, int y, float angle) {
		return (x * sin(angle * PI / 180) + y * cos(angle * PI / 180)) + 0.5;
	}

	void Renderer::DrawRectRotated(const Rect& rect, const RGBColor& color, float angle) {
		int xCenter = (2 * rect.x + rect.width) / 2;
		int yCenter = (2 * rect.y + rect.height) / 2;

		//center rectangle at origin
		int centeredXMin = rect.x - xCenter;
		int centeredXMax = rect.x + rect.width - xCenter;
		int centeredYMin = rect.y - yCenter;
		int centeredYMax = rect.y + rect.height - yCenter;

		//rotate the corners
		int centeredRotatedBottomLeftX = RotateXCord(centeredXMin, centeredYMin, angle);
		int centeredRotatedBottomLeftY = RotateYCord(centeredXMin, centeredYMin, angle);
		int centeredRotatedTopLeftX = RotateXCord(centeredXMin, centeredYMax, angle);
		int centeredRotatedTopLeftY = RotateYCord(centeredXMin, centeredYMax, angle);
		int centeredRotatedBottomRightX = RotateXCord(centeredXMax, centeredYMin, angle);
		int centeredRotatedBottomRightY = RotateYCord(centeredXMax, centeredYMin, angle);
		int centeredRotatedTopRightX = RotateXCord(centeredXMax, centeredYMax, angle);
		int centeredRotatedTopRightY = RotateYCord(centeredXMax, centeredYMax, angle);

		//translate back to original center
		centeredRotatedBottomLeftX += xCenter;
		centeredRotatedBottomLeftY += yCenter;
		centeredRotatedTopLeftX += xCenter;
		centeredRotatedTopLeftY += yCenter;
		centeredRotatedBottomRightX += xCenter;
		centeredRotatedBottomRightY	+= yCenter;
		centeredRotatedTopRightX += xCenter;
		centeredRotatedTopRightY += yCenter;

		DrawLine(centeredRotatedBottomLeftX, centeredRotatedBottomLeftY, centeredRotatedTopLeftX, centeredRotatedTopLeftY, color);
		DrawLine(centeredRotatedTopLeftX, centeredRotatedTopLeftY, centeredRotatedTopRightX, centeredRotatedTopRightY, color);
		DrawLine(centeredRotatedTopRightX, centeredRotatedTopRightY, centeredRotatedBottomRightX, centeredRotatedBottomRightY, color);
		DrawLine(centeredRotatedBottomRightX, centeredRotatedBottomRightY, centeredRotatedBottomLeftX, centeredRotatedBottomLeftY, color);
	}

	void Renderer::FillRectRotated(const Rect& rect, const RGBColor& color, float angle) {
		int xCenter = (2 * rect.x + rect.width) / 2;
		int yCenter = (2 * rect.y + rect.height) / 2;

		int width = 0, height = 0;

		getWindowDimensions(&width, &height);

		//center rectangle at origin
		int centeredXMin = rect.x - xCenter;
		int centeredXMax = rect.x + rect.width - xCenter;
		int centeredYMin = rect.y - yCenter;
		int centeredYMax = rect.y + rect.height - yCenter;

		//rotate the corners
		int centeredRotatedBottomLeftX = RotateXCord(centeredXMin, centeredYMin, angle);
		int centeredRotatedBottomLeftY = RotateYCord(centeredXMin, centeredYMin, angle);
		int centeredRotatedTopLeftX = RotateXCord(centeredXMin, centeredYMax, angle);
		int centeredRotatedTopLeftY = RotateYCord(centeredXMin, centeredYMax, angle);
		int centeredRotatedBottomRightX = RotateXCord(centeredXMax, centeredYMin, angle);
		int centeredRotatedBottomRightY = RotateYCord(centeredXMax, centeredYMin, angle);
		int centeredRotatedTopRightX = RotateXCord(centeredXMax, centeredYMax, angle);
		int centeredRotatedTopRightY = RotateYCord(centeredXMax, centeredYMax, angle);

		//translate back to original center
		centeredRotatedBottomLeftX += xCenter;
		centeredRotatedBottomLeftY += yCenter;
		centeredRotatedTopLeftX += xCenter;
		centeredRotatedTopLeftY += yCenter;
		centeredRotatedBottomRightX += xCenter;
		centeredRotatedBottomRightY += yCenter;
		centeredRotatedTopRightX += xCenter;
		centeredRotatedTopRightY += yCenter;

		int jMin = max(0, min(centeredRotatedBottomLeftX, min(centeredRotatedTopLeftX, min(centeredRotatedBottomRightX, centeredRotatedTopRightX))));
		int iMin = max(0, min(centeredRotatedBottomLeftY, min(centeredRotatedTopLeftY, min(centeredRotatedBottomRightY, centeredRotatedTopRightY))));		
		int jMax = min(width, max(centeredRotatedBottomLeftX, max(centeredRotatedTopLeftX, max(centeredRotatedBottomRightX, centeredRotatedTopRightX))));
		int iMax = min(height, max(centeredRotatedBottomLeftY, max(centeredRotatedTopLeftY, max(centeredRotatedBottomRightY, centeredRotatedTopRightY))));

		//Point-In-Polygon Algorithm - most efficient code
		float xCorners[4] = { centeredRotatedBottomRightX, centeredRotatedBottomLeftX, centeredRotatedTopLeftX, centeredRotatedTopRightX };
		float yCorners[4] = { centeredRotatedBottomRightY, centeredRotatedBottomLeftY, centeredRotatedTopLeftY, centeredRotatedTopRightY };
		float constant[4] = {};
		float multiple[4] = {};

		int r = 3;

		for (int t = 0; t < 4; t++) {
			if (yCorners[r] == yCorners[t]) {
				constant[t] = xCorners[t];
				multiple[t] = 0;
			}
			else {
				constant[t] = xCorners[t] - (yCorners[t] * xCorners[r]) / (yCorners[r] - yCorners[t]) + (yCorners[t] * xCorners[t]) / (yCorners[r] - yCorners[t]);
				multiple[t] = (xCorners[r] - xCorners[t]) / (yCorners[r] - yCorners[t]);
			}
			r = t;
		}

		bool in = 0;
		bool current = 0, previous = 0;

		for (int i = iMin; i < iMax; i++){
			for (int j = jMin; j < jMax; j++) {
				//logic for filling in the rect
				current = yCorners[3] > i;
				in = 0;
				for (int t = 0; t < 4; t++) {
					previous = current;
					current = yCorners[t] > i;
					if (current != previous) {
						in ^= i * multiple[t] + constant[t] < j;
					}
				}

				if (in) {
					SetPixel(j, i, color);
				}
			}
		}
	}

}