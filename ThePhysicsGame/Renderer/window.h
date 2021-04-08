#pragma once

namespace Renderer
{
	class Window
	{
	public:
		static Window* instance;
	private:
		int window_handle;
		int width, height;
	public:
		/* Constructor */

		Window(const char* title, int width, int height)
		{
			int c = 1; const char* v[] = { "params" }; // Kinda hacky lol
			glutInit(&c, (char**)v);
			glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
			glutInitWindowSize(width, height);
			glutInitWindowPosition(100, 100);

			this->window_handle = glutCreateWindow(title);
			this->width = width;
			this->height = height;

			Window::instance = this;
		}
	public:
		/* Methods */
		int32_t get_width()
		{
			return this->width;
		}

		int32_t get_height()
		{
			return this->height;
		}
	};
	Renderer::Window* Window::instance = new Window("PhySim", 500,500);
}