#pragma once

namespace Engine
{
	class EntityManager
	{
	public:
		std::vector<Entity*> entity_list;
		Vector2 mouse_pos;
		bool is_mouse_down = false;
		int16_t spawn_size = 1;
		Engine::EEntityType spawn_type = EEntityType::Default;
	public:
		/* Methods */
		void AddEntity(Vector2 pos, EEntityType type)
		{
			Engine::Entity* ent = nullptr;
			switch (type)
			{
			case Engine::EEntityType::Default:
				ent = new DefaultEntity(pos);
				break;
			case Engine::EEntityType::Gas:
				ent = new Gas(pos);
				break;
			default:
				break;
			}
			this->entity_list.push_back(ent);
		}

		void RemoveEntity(Entity* ent)
		{
			for(int i = 0; i < entity_list.size(); i++)
				if (entity_list[i] == ent)
				{
					entity_list.erase(entity_list.begin() + i);
					return;
				}
		}

		bool IsPositionOccupied(Vector2 pos)
		{
			for (auto ent : entity_list)
			{
				if (pos.x == ent->position.x && pos.y == ent->position.y)
					return true;
			}
			return false;
		}

		bool IsPositionOccupied(Entity* ignore, Vector2 pos)
		{
			for (auto ent : entity_list)
			{
				if(ent == ignore)
					continue;
				if (pos.x == ent->position.x && pos.y == ent->position.y)
					return true;
			}
			return false;
		}

		bool IsOutOfBounds(Vector2 pos)
		{
			int width = Renderer::Window::instance->get_width();
			int height = Renderer::Window::instance->get_height();
			return pos.x >= 0 && pos.x <= width && pos.y >= 0 && pos.y <= height;
		}

		bool IsOutOfBounds(Entity* ent)
		{
			return this->IsOutOfBounds(ent->position);
		}

	public:
		/* Handlers */

		void Update(int dt)
		{
			/* Do continuous mouse update */
			if (this->is_mouse_down)
			{
				/* Spawn test entity */
				for (int i = 0; i < this->spawn_size; i++)
				{
					int rem = i % 3;
					Vector2 pos = this->mouse_pos;
					switch (rem)
					{
					case 0:
						pos.x += i;
						pos.y += i;
						break;
					case 1:
						pos.x += i;
						pos.y -= i;

						break;
					case 2:
						pos.x -= i;
						pos.y -= i;
						break;
					case 3:
						pos.x -= i;
						pos.y += i;
						break;
					}
					if(!IsPositionOccupied(pos))
						this->AddEntity(pos, this->spawn_type);
				}
			}

			for (auto ent : entity_list)
			{
				ent->OnSimulate(dt / 100.f);
				/*while(IsPositionOccupied(ent, ent->position))
				{
					ent->position.y += 1;
					//ent->position.x += 1;
				}*/
				if (IsOutOfBounds(ent))
				{
					int width = Renderer::Window::instance->get_width();
					int height = Renderer::Window::instance->get_height();
					if (ent->position.x < 0)
						ent->position.x += 1;
					if (ent->position.x > width)
						ent->position.x -= 1;

					if (ent->position.y < 0)
						ent->position.y += 1;
					if (ent->position.y > height)
						ent->position.y -= 1;
				}
			}
			for (auto ent : entity_list)
				ent->OnDraw();
		}

		void Input(int button, int state, Vector2 pos)
		{
			this->is_mouse_down = !state;
		}

		void KeyboardInput(UCHAR key, Vector2 pos)
		{
			switch (key)
			{
			case '0':
				this->spawn_type = EEntityType::Default;
				break;
			case '1':
				this->spawn_type = EEntityType::Gas;
				break;
			case 'm':
				this->spawn_size++;
				break;
			case 'n':
				if (this->spawn_size > 1)
					spawn_size--;
				break;
			default:
				break;
			}
		}

		void UpdateMousePosition(Vector2 pos)
		{
			pos.y = Renderer::Window::instance->get_height() - pos.y;
			this->mouse_pos = pos;
		}
	};
}