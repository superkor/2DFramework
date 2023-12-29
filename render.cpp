#include "render.h"

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

	void Renderer::plotLineLow(int x0, int y0, int x1, int y1, const RGBColor& color)
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

	void Renderer::plotLineHigh(int x0, int y0, int x1, int y1, const RGBColor& color)
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
				plotLineLow(x1, y1, x0, y0, color);
			else
				plotLineLow(x0, y0, x1, y1, color);
		}
		else
		{
			if (y0 > y1)
				plotLineHigh(x1, y1, x0, y0, color);
			else
				plotLineHigh(x0, y0, x1, y1, color);
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

}