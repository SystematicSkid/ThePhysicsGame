#pragma once

namespace Engine
{
	class Cloud : public Entity
	{
	public:
		int16_t tick;
		int16_t raindelay;
	public:
		/* Constructor */
		Cloud(Vector2 pos)
		{
			this->position = pos;
			this->velocity = { 1,0 };
			this->col = { 173,196,240, 100 };
			this->state = EEntityState::Gas;
			this->type = EEntityType::Cloud;
			this->tick = 0;
			this->raindelay = (xor_rand() % 75) + 60;
			this->lifetime = 9.9e22;
		}
	public:
		void reset_tick() {
			this->tick = 0;
		}
	public:
		virtual void OnSimulate(float dt)
		{
			this->tick++;
			this->lifetime--;
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