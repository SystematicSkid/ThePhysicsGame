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
			/* Check for menu bounds */

			Engine::Entity* ent = nullptr;
			switch (type)
			{
			case Engine::EEntityType::Default:
				ent = new DefaultEntity(pos);
				break;
			case Engine::EEntityType::Gas:
				ent = new Gas(pos);
				break;
			case Engine::EEntityType::Fire:
				ent = new Fire(pos);
				break;
			case Engine::EEntityType::Bouncy:
				ent = new Bouncy(pos);
				break;
			case Engine::EEntityType::Smoke:
				ent = new Smoke(pos);
				break;
			case Engine::EEntityType::Water:
				ent = new Water(pos);
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
					delete ent;
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

		Entity* GetEntityCollided(Entity* ignore, Vector2 pos)
		{
			for (auto ent : entity_list)
			{
				if (ent == ignore)
					continue;
				if (pos.x == ent->position.x && pos.y == ent->position.y)
					ent;
			}
			return nullptr;
		}

		bool IsOutOfBounds(Vector2 pos)
		{
			int width = Renderer::Window::instance->get_width() - 1;
			int height = Renderer::Window::instance->get_height() - 1;
			return pos.x >= 0 && pos.x <= width && pos.y >= 0 && pos.y <= height;
		}

		bool IsOutOfBounds(Entity* ent)
		{
			return this->IsOutOfBounds(ent->position);
		}

		int generate_random_change()
		{
			int rand_num = (xor_rand() % 3) - 1; // Do not use rand()! Huge performance hit!
			return rand_num;
		}

		Entity* GetClosestOfType(Entity* from, EEntityType type)
		{
			float closest_dist = 100000.f;
			Entity* closest_ent = nullptr;
			for (auto ent : this->entity_list)
			{
				if(ent == from)
					continue;
				if(ent->type != type)
					continue;
				Vector2 delta = { from->position.x - ent->position.x, from->position.y - ent->position.y };
				float dist = sqrt(delta.x * delta.x + delta.y * delta.y);
				if (dist < closest_dist)
				{
					closest_dist = dist;
					closest_ent = ent;
				}
			}
			return closest_ent;
		}

		void ConvertEntity(Entity* original, EEntityType new_type)
		{
			if (original->type == new_type)
				return; // No need!

			Entity* new_entity = nullptr;
			switch (new_type)
			{
			case Engine::EEntityType::Default:
				new_entity = new DefaultEntity(original->position);
				break;
			case Engine::EEntityType::Gas:
				new_entity = new Gas(original->position);
				break;
			case Engine::EEntityType::Fire:
				new_entity = new Fire(original->position);
				break;
			case Engine::EEntityType::Bouncy:
				new_entity = new Bouncy(original->position);
				break;
			case Engine::EEntityType::Smoke:
				new_entity = new Smoke(original->position);
				break;
			case Engine::EEntityType::Water:
				new_entity = new Water(original->position);
				break;
			default:
				return;
			}
			memcpy(original, new_entity, sizeof(Entity));

		}

		void Impulse(Vector2 pos, float force)
		{

		}

	public:
		/* Particle relation updates */

		void handle_fire()
		{
			for (auto ent : this->entity_list)
			{
				if(ent->type != EEntityType::Fire)
					continue;
				auto closest_ent = this->GetClosestOfType(ent, EEntityType::Gas);
				if (closest_ent)
				{
					if (ent->Distance(closest_ent) <= 4)
					{
						ConvertEntity(closest_ent, EEntityType::Fire);
					}
				}
			}
		}


	public:
		/* Handlers */

		void Update(int dt)
		{
			/* Do continuous mouse update */
			if (this->is_mouse_down)
			{
				/* Check if mouse pos is behind menu */
				if(mouse_pos.y <= 450 && !is_combo_open)
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
						if (!IsPositionOccupied(pos))
							this->AddEntity(pos, this->spawn_type);
					}
				}
			}

			for (auto ent : entity_list)
			{
				//memset(entity_map, 0, sizeof(entity_map)); // Zero array
				ent->OnSimulate(dt / 100.f);

				/* Collision Handling */
				if (ent->type == EEntityType::Default)
				{
					while (IsPositionOccupied(ent, ent->position))
					{
						int rand = generate_random_change();
						ent->position.x += rand;
						if (IsPositionOccupied(ent, ent->position))
							ent->position.x -= 2 * rand;
						if (IsPositionOccupied(ent, ent->position))
						{
							ent->position.x += rand;
							if (ent->velocity.y <= 0)
								ent->position.y += 1;
							if (ent->velocity.y > 0)
								ent->position.y -= 1;
						}
					}
					ent->position.y -= 1;
					if (IsPositionOccupied(ent, ent->position))
						ent->velocity.y = 0;
					else
						ent->velocity.y = -gravity;
					ent->position.y += 1;
				}

				else if (ent->type == EEntityType::Water)
				{
					int rand = generate_random_change();
					ent->position.x += rand;
					if (IsPositionOccupied(ent, ent->position))
						ent->position.x -= rand;

					while (IsPositionOccupied(ent, ent->position))
					{
						int rand = generate_random_change();
						ent->position.x += rand;
						if (IsPositionOccupied(ent, ent->position))
							ent->position.x -= 2 * rand;
						if (IsPositionOccupied(ent, ent->position))
						{
							ent->position.x += rand;
							if (ent->velocity.y <= 0)
								ent->position.y += 1;
							if (ent->velocity.y > 0)
								ent->position.y -= 1;
						}
					}
					ent->position.y -= 1;
					if (IsPositionOccupied(ent, ent->position))
						ent->velocity.y = 0;
					else
						ent->velocity.y = -gravity;
					ent->position.y += 1;
				}


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

				//entity_map[ent->position.x][ent->position.y] = ent;
			}
			
			/* Particle handlers */
			handle_fire();

			/* Remove and delete entities that have 'expired' */
			std::for_each(entity_list.rbegin(), entity_list.rend(),
				[=](const auto& ent)
				{
					if (ent->ShouldDelete())
					{
						int rand_num = xor_rand() % 10;
						if (ent->type == EEntityType::Fire && rand_num == 2) // random chance
						{
							this->ConvertEntity(ent, EEntityType::Smoke);
						}
						else
							this->RemoveEntity(ent);
					}
				});
			for (auto ent : entity_list)
				ent->OnDraw();
		}

		void Input(int button, int state, Vector2 pos)
		{
			this->is_mouse_down = !state;
			pos.y = Renderer::Window::instance->get_height() - pos.y;
			this->mouse_pos = pos;
		}

		void KeyboardInput(UCHAR key, Vector2 pos)
		{
			/*switch (key)
			{
			case '0':
				this->spawn_type = EEntityType::Default;
				break;
			case '1':
				this->spawn_type = EEntityType::Gas;
				break;
			case '2':
				this->spawn_type = EEntityType::Fire;
				break;
			case '3':
				this->spawn_type = EEntityType::Bouncy;
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
			}*/
		}

		void UpdateMousePosition(Vector2 pos)
		{
			pos.y = Renderer::Window::instance->get_height() - pos.y;
			this->mouse_pos = pos;
		}
	};
}