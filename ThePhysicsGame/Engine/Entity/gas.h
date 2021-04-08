#pragma once

namespace Engine
{
	class Gas : public Entity
	{
	public:
		/* Constructor */
		Gas(Vector2 pos)
		{
			this->position = pos;
			this->velocity = { 0,0 };
			this->col = { 255,172,166, 100};
			this->state = EEntityState::Gas;
			this->type = EEntityType::Gas;
			this->lifetime = -1;
		}
	public:
		int generate_random_change()
		{
			int rand_num = (xor_rand() % 3) - 1; // Do not use rand()! Huge performance hit!
			return rand_num;
		}
	public:
		virtual void OnSimulate(float dt)
		{
			if(this->velocity.x == 0 && this->velocity.y == 0)
			{
				this->position.x += generate_random_change();
				this->position.y += generate_random_change();
				return;
			}

			this->position.x = this->position.x + this->velocity.x;
			this->position.y = this->position.y + this->velocity.y;
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