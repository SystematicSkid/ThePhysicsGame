#pragma once

namespace Engine
{
	class Water : public Entity
	{
	public:
		/* Constructor */
		Water(Vector2 pos)
		{
			this->position = pos;
			this->col = { 21,135,255 };
			this->velocity = { 0,0 };
			this->state = EEntityState::Liquid;
			this->type = EEntityType::Water;
			this->lifetime = -1;
		}
	public:
		virtual void OnSimulate(float dt)
		{
			//int y_change = (int)floor(gravity * dt);
			if (this->position.y + this->velocity.y <= 0)
			{
				this->position.y = 0;
				this->velocity.y = 0;
				/*int rand_dir = rand() % 1;
				if (rand_dir)
					this->velocity.x += 1;
				else
					this->velocity.x -= 1;*/
			}
			/*else
				this->velocity.y -= y_change;*/
			this->velocity.x = clamp<int16_t>(this->velocity.x, -max_velocity, max_velocity);
			this->velocity.y = clamp<int16_t>(this->velocity.y, -max_velocity, max_velocity);
			this->position.x += this->velocity.x;
			this->position.y += this->velocity.y;
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