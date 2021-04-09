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
		int generate_random_change_x()
		{
			int rand_num = (xor_rand() % 3) - 1; // Do not use rand()! Huge performance hit!
			return rand_num;
		}

		int generate_random_change_y()
		{
			int rand_num = (xor_rand() % 2);
			if (xor_rand() % 2 == 0)
				rand_num -= 1;
			return rand_num;
		}
	public:
		virtual void OnSimulate(float dt)
		{

			
			if(this->velocity.x == 0 && this->velocity.y == 0)
			{
				this->position.x += generate_random_change_x();
				this->position.y += generate_random_change_y();
				if (this->position.y > Renderer::Window::instance->get_height())
				{
					this->position.y = Renderer::Window::instance->get_height() - 51;
				}
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