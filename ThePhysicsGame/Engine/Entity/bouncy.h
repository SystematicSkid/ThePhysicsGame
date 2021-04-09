#pragma once

namespace Engine
{
	class Bouncy : public Entity
	{
	public:
		int16_t numBounces;
	public:
		/* Constructor */
		Bouncy(Vector2 pos)
		{
			this->position = pos;
			this->velocity = { 0,0 };
			int seed = (xor_rand() % 2);
			if (seed) this->velocity.x = 1;
			else this->velocity.x = -1;
			this->numBounces = 1;
			this->col = { 27,181,53, 100 };
			this->state = EEntityState::Solid;
			this->type = EEntityType::Bouncy;
			this->lifetime = -1;
		}
	public:
		virtual void OnSimulate(float dt)
		{
			int fall = (int)floor(gravity * dt);
			this->velocity.y -= fall;

			this->position.x += this->velocity.x;
			this->position.y += this->velocity.y;
			
			if (this->position.x < 0)
			{
				this->position.x = 0;
				this->velocity.x *= -1;
				this->velocity.x /= 2;
			}
			else if (this->position.x > Renderer::Window::instance->get_width())
			{
				this->position.x = Renderer::Window::instance->get_width();
				this->velocity.x *= -1;
				this->velocity.x /= 2;
			}
			
			if (this->position.y < 0)
			{
				this->position.y = 0;
				this->velocity.y *= -0.8;

			}
			else if (this->position.y > Renderer::Window::instance->get_height())
			{
				this->position.y = Renderer::Window::instance->get_height();
				this->velocity.y *= -0.8;

			}
		}

		virtual void OnDraw()
		{
			render::pixel(this->position.x, this->position.y, this->col);
		}

		virtual bool ShouldDelete()
		{
			return this->lifetime == 0;
		}
	};
}