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

	/*Vector2 operator+(Vector2& v)
	{
		this->x += v.x;
		this->y += v.y;
		return *this;
	}
	Vector2 operator-(Vector2& v)
	{
		this->x -= v.x;
		this->y -= v.y;
		return *this;
	}*/
};

const int gravity = 10;
const int max_fire_per_frame = 150;
const int16_t max_velocity = 3;
static bool is_combo_open = false;


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

/* Renderer -> ImGui */
#include "./Renderer/imgui/imgui.h"
#include "./Renderer/imgui/imgui_internal.h"
#include "./Renderer/imgui/imgui_impl_glut.h"
#include "./Renderer/imgui/imgui_impl_opengl2.h"

/* Engine -> Entity */
#include "./Engine/Entity/entity.h"
#include "./Engine/Entity/sand.h"
#include "./Engine/Entity/gas.h"
#include "./Engine/Entity/fire.h"
#include "./Engine/Entity/smoke.h"
#include "./Engine/Entity/bouncy.h"
#include "./Engine/Entity/water.h"
#include "./Engine/Entity/barrier.h"
#include "./Engine/Entity/oil.h"
#include "./Engine/Entity/acid.h"
#include "./Engine/Entity/cloud.h"

/* Engine */
#include "./Engine/entitymanager.h"


/* All our opengl callbacks */
namespace Core
{
	int time_scale = 1;
	int last_deltatime = 0;
	Engine::EntityManager* entity_manager = nullptr;
	static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	Engine::EEntityType selected_entity_type = Engine::EEntityType::Sand;
	int spawn_count = 1;
	bool show_debug = false;
	static float frames_per_second = 0.f;
	static float logged_fps = 0.f;
	static float last_time = 0.f;

	VOID render_menu()
	{
		ImGui::SetNextWindowSize({ 500, 50 });
		ImGui::SetNextWindowPos({ 0,0 });
		ImGui::Begin("Menu", NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
		const char* particles[] = { "Sand", "Gas", "Fire", "Bouncy", "Smoke", "Water", "Barrier", "Oil", "Acid", "Cloud" };
		static const char* current_item = NULL;
		ImGui::SetNextItemWidth(150.f);
		if (ImGui::BeginCombo("Type", current_item))
		{
			is_combo_open = true;
			for (int i = 0; i < IM_ARRAYSIZE(particles); i++)
			{
				bool is_selected = (current_item == particles[i]);
				if (ImGui::Selectable(particles[i], is_selected))
				{
					current_item = particles[i];
					selected_entity_type = (Engine::EEntityType)i;
					entity_manager->spawn_type = selected_entity_type;
				}
				if (is_selected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		else
		{
			is_combo_open = false;
		}
		ImGui::SameLine();
		ImGui::SetNextItemWidth(125.f);
		ImGui::SliderInt("Amount", &spawn_count, 1, 50);
		entity_manager->spawn_size = spawn_count;
		ImGui::SameLine(0.f, 10.f);
		ImGui::SetNextItemWidth(125.f);
		if (ImGui::Button("Clear", { 100, 25 }))
		{
			entity_manager->Clear();
		}
		ImGui::End();

		if(show_debug)
		{
			ImGui::SetNextWindowPos({ 0, 50 });
			ImGui::SetNextWindowSize({ 125,70 });
			ImGui::Begin("Debug");
			ImGui::Text("Particles: %i", entity_manager->entity_list.size());
			ImGui::Text("FPS: %.0f", logged_fps);
			ImGui::End();
		}
	
	}

	VOID render_scene()
	{
		float current_time = GetTickCount() * 0.001f;
		frames_per_second++;
		if (current_time - last_time > 1.f)
		{
			last_time = current_time;
			logged_fps = frames_per_second;
			frames_per_second = 0;
		}
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

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL2_NewFrame();
		ImGui_ImplGLUT_NewFrame();

		render_menu();

		// Rendering
		ImGui::Render();
		ImGuiIO& io = ImGui::GetIO();
		ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

		glutSwapBuffers();
	}

	VOID handle_mouse(int button, int state, int x, int y)
	{
		ImGui_ImplGLUT_MouseFunc(button, state, x, y);
		if (!entity_manager)
			return;
		Vector2 pos{ x, y };
		entity_manager->Input(button, state,pos);
	}

	VOID handle_mouse_move(int x, int y)
	{
		ImGui_ImplGLUT_MotionFunc(x, y);
		if (!entity_manager)
			return;
		Vector2 pos{ x, y };
		entity_manager->UpdateMousePosition(pos);
	}

	VOID handle_keyboard(UCHAR key, int x, int y)
	{
		ImGui_ImplGLUT_KeyboardFunc(key, x, y);
		if (key == 'd')
			show_debug = !show_debug;
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
		

		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		ImGui::StyleColorsLight();

		ImGui_ImplGLUT_Init();
		ImGui_ImplGLUT_InstallFuncs();
		ImGui_ImplOpenGL2_Init();

		glutMouseFunc(handle_mouse);
		glutIdleFunc(idle);
		glutMotionFunc(handle_mouse_move);
		glutKeyboardFunc(handle_keyboard);

		glutMainLoop();

		// Cleanup
		ImGui_ImplOpenGL2_Shutdown();
		ImGui_ImplGLUT_Shutdown();
		ImGui::DestroyContext();

		return TRUE;
	}
}