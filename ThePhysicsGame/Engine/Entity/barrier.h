#pragma once

namespace Engine
{
	class Barrier : public Entity
	{
	public:
		/* Constructor */
		Barrier(Vector2 pos)
		{
			this->position = pos;
			this->col = { 77,77,77, 100};
			this->velocity = { 0,0 };
			this->state = EEntityState::Solid;
			this->type = EEntityType::Barrier;
			this->lifetime = -1;
		}
	public:
		virtual void OnSimulate(float dt)
		{
		}

		virtual void OnDraw()
		{
			render::pixel(this->position.x, this->position.y, this->col);
			render::pixel(this->position.x+1, this->position.y, this->col);
			render::pixel(this->position.x+1, this->position.y+1, this->col);
			render::pixel(this->position.x, this->position.y+1, this->col);
		}

		virtual bool ShouldDelete()
		{
			return this->lifetime == 0;
		}
	};
}