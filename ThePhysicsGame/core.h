#pragma once
/* STL & Windows */
#include <stdio.h>
#include <Windows.h>
#include <cstdint>
#include <vector>
#include <thread>
#include <random>
#include <algorithm>

/* openGL */
#include <gl/glut.h>

struct Vector2
{
	int16_t x, y;
};

const int gravity = 10;
const int16_t max_velocity = 3;

template<class T>
T clamp(T val, T min, T max) {
	return std::max(min, std::min(max, val));
}

static unsigned long x = 123456789, y = 362436069, z = 521288629;

unsigned long xor_rand(void) 
{      
	unsigned long t;
	x ^= x << 16;
	x ^= x >> 5;
	x ^= x << 1;
	t = x;
	x = y;
	y = z;
	z = t ^ x ^ y;

	return z;
}

/* Renderer */
#include "./Renderer/window.h"
#include "./Renderer/renderer.h"

/* Engine -> Entity */
#include "./Engine/Entity/entity.h"
#include "./Engine/Entity/default.h"
#include "./Engine/Entity/gas.h"
#include "./Engine/Entity/fire.h"

/* Engine */
#include "./Engine/entitymanager.h"


/* All our opengl callbacks */
namespace Core
{
	int time_scale = 1;
	int last_deltatime = 0;
	Engine::EntityManager* entity_manager = nullptr;

	VOID render_scene()
	{
		/* Add a step */
		auto sleep_time = std::chrono::milliseconds(10 / time_scale);
		std::this_thread::sleep_for(sleep_time);

		glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(0.0, 500.0, 500.0, 0.0);

		int time_since_start = glutGet(GLUT_ELAPSED_TIME);
		int dt = time_since_start - last_deltatime;
		last_deltatime = time_since_start;
		//printf("Dt: %i\n", dt);
		if (entity_manager)
		{
			entity_manager->Update(dt);
		}

		glutSwapBuffers();
	}

	VOID handle_mouse(int button, int state, int x, int y)
	{
		if (!entity_manager)
			return;
		Vector2 pos{ x, y };
		entity_manager->Input(button, state,pos);
	}

	VOID handle_mouse_move(int x, int y)
	{
		if (!entity_manager)
			return;
		Vector2 pos{ x, y };
		entity_manager->UpdateMousePosition(pos);
	}

	VOID handle_keyboard(UCHAR key, int x, int y)
	{
		if (!entity_manager)
			return;
		Vector2 pos{ x, y };
		entity_manager->KeyboardInput(key, pos);
	}

	VOID idle()
	{
		/* Force update */
		glutPostRedisplay();
	}

	BOOL initialize()
	{
		/* Initialize our window if not set by TLS */
		if (!Renderer::Window::instance)
			Renderer::Window("PhySim", 500, 500);

		/* Initialize our entity manager */
		entity_manager = new Engine::EntityManager();

		/* Set random seed */
		z = time(0);

		/* Initialize debug */
		FILE* pNewStdout = NULL;
		FILE* pNewStderr = NULL;
		FILE* pNewStdin = NULL;
		AllocConsole();
		//// Assign `stdin`, `stout`, `stderr`
		::freopen_s(&pNewStdin, ("CONIN$"), "r", stdin);
		::freopen_s(&pNewStdout, ("CONOUT$"), "w", stdout);
		::freopen_s(&pNewStderr, ("CONOUT$"), "w", stderr);

		/* Register our render callback */
		glutDisplayFunc(render_scene);
		glutMouseFunc(handle_mouse);
		glutIdleFunc(idle);
		glutMotionFunc(handle_mouse_move);
		glutKeyboardFunc(handle_keyboard);
		glutMainLoop();

		return TRUE;
	}
}