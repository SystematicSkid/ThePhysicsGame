#pragma once

struct color
{
	color() = default;
	color(uint8_t shade, uint8_t a) :
		r(shade), g(shade), b(shade), a(a) {}
	color(uint8_t r, uint8_t g, uint8_t b) :
		r(r), g(g), b(b), a(255) {}
	color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) :
		r(r), g(g), b(b), a(a) {}

	[[nodiscard]] uint32_t rgba() const
	{
		return *reinterpret_cast<const uint32_t*>(this);
	}

	operator uint32_t() const {
		return rgba();
	}

	color& operator=(uint32_t v)
	{
		*reinterpret_cast<uint32_t*>(this) = v;
		return *this;
	}

	[[nodiscard]] color lerp(const color& c2, float f) const {
		return color(
			r + static_cast<int16_t>((static_cast<float>(c2.r - r) * f)),
			g + static_cast<int16_t>((static_cast<float>(c2.g - g) * f)),
			b + static_cast<int16_t>((static_cast<float>(c2.b - b) * f)),
			a + static_cast<int16_t>((static_cast<float>(c2.a - a) * f)));
	}

	[[nodiscard]] color new_a(uint8_t na) const {
		return { r, g, b, na };
	}

	uint8_t r = 255, g = 255, b = 255, a = 255;
};

namespace render
{
	__forceinline void pixel(int x, int y, color col)
	{
		glBegin(GL_POINTS);
		{
			glColor3f((float)col.r / 255.f, (float)col.g / 255.f, (float)col.b / 255.f);
			glVertex2i(x, Renderer::Window::instance->get_height() - y);
		}
		glEnd();
	}
}