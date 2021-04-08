#pragma once

namespace Engine
{
	class Fire : public Entity
	{
	public:
		int heat = 0;
	public:
		/* Constructor */
		Fire(Vector2 pos)
		{
			this->position = pos;
			this->velocity = { 0,0 };
			this->col = { 255,50,0 };
			this->state = EEntityState::Gas;
			this->type = EEntityType::Fire;
			this->lifetime = (xor_rand() % 40) + 1;
			this->heat = 0;
		}
	public:
		bool should_move_horizontal()
		{
			return (xor_rand() % 5) == 2;
		}
	public:
		virtual void OnSimulate(float dt)
		{
			int y_change = (int)floor(gravity * dt);
			this->position.y += y_change;
			if (should_move_horizontal())
			{
				int rand_num = (xor_rand() % 3) - 1;
				this->position.x += rand_num;
			}
			this->lifetime--;
		}

		virtual void OnDraw()
		{
			render::pixel(this->position.x, this->position.y, this->col);
		}

		virtual bool ShouldDelete()
		{
			return this->lifetime <= 0;
		}
	};
}