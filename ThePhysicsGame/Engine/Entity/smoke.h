#pragma once

namespace Engine
{
	class Smoke : public Entity 
	{
	public:
		/* Constructor */
		Smoke(Vector2 pos)
		{
			this->position = pos;
			this->velocity = { 0,0 };
			this->col = { 59,59,59,255 };
			this->state = EEntityState::Gas;
			this->type = EEntityType::Smoke;
			this->lifetime = -1;
		}
	public:
		bool should_move_horizontal()
		{
			return (xor_rand() % 4) == 2;
		}
	public:
		virtual void OnSimulate(float dt)
		{
			int y_change = (int)floor(gravity * dt);
			this->velocity.x = 0;
			if (this->position.y + y_change > Renderer::Window::instance->get_height())
			{
				this->position.y = Renderer::Window::instance->get_height() - 51;
			}
			else
				this->velocity.y += y_change;
			if (should_move_horizontal())
			{
				int rand_num = (xor_rand() % 3) - 1;
				this->velocity.x += rand_num;
			}
			this->velocity.x = clamp<int16_t>(this->velocity.x, -max_velocity, max_velocity);
			this->velocity.y = clamp<int16_t>(this->velocity.y, -max_velocity, max_velocity);
			this->position.x += this->velocity.x;
			this->position.y += this->velocity.y;
			this->col.a--; // decrease alpha val
		}

		virtual void OnDraw()
		{
			render::pixel(this->position.x, this->position.y, this->col);
		}

		virtual bool ShouldDelete()
		{
			return this->col.a <= 0;
		}
	};
}