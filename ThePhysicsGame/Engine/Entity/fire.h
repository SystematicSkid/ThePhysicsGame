#pragma once

namespace Engine
{
	class Fire : public Entity
	{
	public:
		/* Constructor */
		Fire(Vector2 pos)
		{
			this->position = pos;
			this->velocity = { 0,0 };
			this->col = { 255,50,0 };
			this->state = EEntityState::Gas;
			this->type = EEntityType::Fire;
		}

	public:
		virtual void OnSimulate(float dt)
		{
		}

		virtual void OnDraw()
		{
			render::pixel(this->position.x, this->position.y, this->col);
		}
	};
}