#pragma once

namespace Engine
{
	class EntityManager
	{
	public:
		std::vector<Entity*> entity_list;
		Vector2 mouse_pos;
		bool is_mouse_down = false;
		int mouse_button = 0;
		int fire_frame_count = 0;
		int16_t spawn_size = 1;
		Engine::EEntityType spawn_type = EEntityType::Sand;
	public:
		/* Methods */
		void AddEntity(Vector2 pos, EEntityType type)
		{
			/* Check for menu bounds */

			Engine::Entity* ent = nullptr;
			switch (type)
			{
			case Engine::EEntityType::Sand:
				ent = new Sand(pos);
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
			case Engine::EEntityType::Barrier:
				ent = new Barrier(pos);
				break;
			case Engine::EEntityType::Oil:
				ent = new Oil(pos);
				break;
			case Engine::EEntityType::Acid:
				ent = new Acid(pos);
				break;
			case Engine::EEntityType::Cloud:
				ent = new Cloud(pos);
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

		void Clear()
		{
			for (auto ent : entity_list)
				delete ent;
			
			entity_list.clear();
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
					return ent;
			}
			return nullptr;
		}

		bool IsOutOfBounds(Vector2 pos)
		{
			int width = Renderer::Window::instance->get_width() - 1;
			int height = Renderer::Window::instance->get_height() - 51;

			return pos.x < 0 || pos.x >= width || pos.y < 0 || pos.y >= height;
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
			case Engine::EEntityType::Sand:
				new_entity = new Sand(original->position);
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
			case Engine::EEntityType::Barrier:
				new_entity = new Barrier(original->position);
				break;
			case Engine::EEntityType::Oil:
				new_entity = new Oil(original->position);
				break;
			case Engine::EEntityType::Acid:
				new_entity = new Acid(original->position);
				break;
			case Engine::EEntityType::Cloud:
				new_entity = new Cloud(original->position);
				break;
			default:
				return;
			}
			memcpy(original, new_entity, sizeof(Entity));

		}

		void Impulse(Vector2 pos, float force)
		{
			for (auto* ent : this->entity_list)
			{
				auto length = static_cast<float>(ent->Distance(pos));
				if(length > force * 10)
					continue;
				Vector2 dir = { pos.x - ent->position.x, pos.y - ent->position.y };
				int16_t x_normalized = (int16_t)((dir.x / length));
				int16_t y_normalized = (int16_t)((dir.y / length));
				x_normalized = clamp<int16_t>(x_normalized, -max_velocity, max_velocity);
				y_normalized = clamp<int16_t>(y_normalized, -max_velocity, max_velocity);
				Vector2 normalized_dir = { x_normalized, y_normalized };
				ent->velocity.x += (-normalized_dir.x * force);
				ent->velocity.y += (-normalized_dir.y * force);
			}
		}

		void ImpulseBugged(Vector2 pos, float force)
		{
			for (auto* ent : this->entity_list)
			{
				auto length = static_cast<float>(ent->Distance(pos));
				if (length > force * 50)
					continue;
				Vector2 dir = { pos.x - ent->position.x, pos.y - ent->position.y };
				int16_t x_normalized = (int16_t)((dir.x / length));
				int16_t y_normalized = (int16_t)((dir.y / length));
				x_normalized = clamp<int16_t>(x_normalized, -max_velocity, max_velocity);
				y_normalized = clamp<int16_t>(y_normalized, -max_velocity, max_velocity);
				Vector2 normalized_dir = { x_normalized, y_normalized };
				ent->velocity.x += (normalized_dir.x * force);
				ent->velocity.y += (normalized_dir.y * force);
			}
		}

	public:
		/* Particle relation updates */

		void handle_fire()
		{
			this->fire_frame_count = 0;
			for (auto ent : this->entity_list)
			{
				if(ent->type != EEntityType::Fire)
					continue;
				auto closest_ent = this->GetClosestOfType(ent, EEntityType::Gas);
				if (!closest_ent)
					closest_ent = this->GetClosestOfType(ent, EEntityType::Oil);
				if (closest_ent)
				{
					if (ent->Distance(closest_ent) <= 8)
					{
						ConvertEntity(closest_ent, EEntityType::Fire);
						ImpulseBugged(ent->position, 1.f);
						if (++this->fire_frame_count >= max_fire_per_frame)
							return;
					}
				}
			}
		}

		void handle_clouds()
		{
			for (auto ent : this->entity_list)
			{
				if (ent->type != EEntityType::Cloud) continue;
				
				Cloud* c = (Cloud*)ent;
				if (c->tick >= c->raindelay) {
					c->reset_tick();
					AddEntity(c->position, EEntityType::Water);
				}
				
			}
		}

		void handle_acid()
		{
			for (auto ent : this->entity_list)
			{
				if (ent->type != EEntityType::Acid)
					continue;
				auto closest_ent = this->GetClosestOfType(ent, EEntityType::Barrier);
				if (!closest_ent)
					closest_ent = this->GetClosestOfType(ent, EEntityType::Bouncy);
				if (!closest_ent)
					closest_ent = this->GetClosestOfType(ent, EEntityType::Sand);
				if (!closest_ent)
					closest_ent = this->GetClosestOfType(ent, EEntityType::Oil);
				if (!closest_ent)
					closest_ent = this->GetClosestOfType(ent, EEntityType::Water);
				if (closest_ent)
				{
					if (ent->Distance(closest_ent) <= 1)
					{
						if (closest_ent->type == EEntityType::Water)
						{
							this->ConvertEntity(closest_ent, EEntityType::Gas);
							ent->lifetime--;
							continue;
						}
						this->RemoveEntity(closest_ent);
						ent->lifetime--;
					}
				}
			}
		}

		void handle_physics(int dt)
		{
			for (auto ent : entity_list)
			{
				//memset(entity_map, 0, sizeof(entity_map)); // Zero array
				ent->OnSimulate(dt / 100.f);

				/* Collision Handling */
				if (ent->type == EEntityType::Sand)
				{
					while (IsPositionOccupied(ent, ent->position) && (GetEntityCollided(ent, ent->position) != nullptr))
					{
						Entity* collide = GetEntityCollided(ent, ent->position);
						if (collide->state == EEntityState::Solid)
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
						else break;
					}
					ent->position.y -= 1;
					if (IsPositionOccupied(ent, ent->position) && (GetEntityCollided(ent, ent->position) != nullptr))
					{
						Entity* collide = GetEntityCollided(ent, ent->position);
						if (collide->state == EEntityState::Solid)
							ent->velocity.y = 0;
						else if (collide->state == EEntityState::Liquid)
							ent->velocity.y = -1;
					}
					else
						ent->velocity.y = -gravity;
					ent->position.y += 1;
				}

				else if (ent->type == EEntityType::Water || ent->type == EEntityType::Acid)
				{
					int rand = generate_random_change();
					ent->position.x += rand * 3;
					if (IsPositionOccupied(ent, ent->position))
						ent->position.x -= rand * 3;

					while (IsPositionOccupied(ent, ent->position) && (GetEntityCollided(ent, ent->position) != nullptr))
					{
						Entity* collide = GetEntityCollided(ent, ent->position);
						if ((collide->state == EEntityState::Liquid) && (collide->type != EEntityType::Oil))
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
						else if (collide->state == EEntityState::Solid)
						{
							while (IsPositionOccupied(ent, ent->position))
								ent->position.y += 1;
						}
						else break;
					}
					ent->position.y -= 1;
					if ((IsPositionOccupied(ent, ent->position)) && (GetEntityCollided(ent, ent->position)->type != EEntityType::Oil))
						ent->velocity.y = 0;
					else
						ent->velocity.y = -gravity;
					ent->position.y += 1;
				}

				else if (ent->type == EEntityType::Oil)
				{
					int rand = generate_random_change();
					ent->position.x += rand * 3;
					if (IsPositionOccupied(ent, ent->position))
						ent->position.x -= rand * 3;

					while (IsPositionOccupied(ent, ent->position) && (GetEntityCollided(ent, ent->position) != nullptr))
					{
						Entity* collide = GetEntityCollided(ent, ent->position);
						if (collide->type == EEntityType::Water)
						{
							ent->position.y += 1;
							break;
						}
						else if (collide->state == EEntityState::Liquid)
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
						else if (collide->state == EEntityState::Solid)
						{
							while (IsPositionOccupied(ent, ent->position))
								ent->position.y += 1;
						}
						else break;
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
					{
						ent->velocity.x *= -0.5;
						ent->position.x += 1;
					}
					if (ent->position.x > width)
					{
						ent->velocity.x *= -0.5;
						ent->position.x -= 1;
					}

					if (ent->position.y < 0)
					{
						ent->velocity.y *= -0.5;
						ent->position.y += 1;

					}
					if (ent->position.y > height)
					{
						ent->velocity.y *= -0.5;
						ent->position.y -= 1;

					}
				}

				//entity_map[ent->position.x][ent->position.y] = ent;
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
				if (mouse_pos.y <= 450 && !is_combo_open)
				{

					/* Spawn test entity */
					for (int i = 0; i < this->spawn_size; i++)
					{
						int rem = i % spawn_size + 1;
						Vector2 pos = this->mouse_pos;
						int rand1 = xor_rand() % (2 * rem) - rem;
						pos.x += rand1;
						pos.y += std::min(xor_rand() % (2 * rem) - rem - (rand1 / 2), xor_rand() % (2 * rem) - rem + (rand1 / 2));
						if (!IsPositionOccupied(pos) && this->mouse_button == GLUT_LEFT_BUTTON)
						{
							this->AddEntity(pos, this->spawn_type);
						}
					}

					if (this->mouse_button == GLUT_RIGHT_BUTTON)
					{
						this->Impulse(mouse_pos, this->spawn_size);
					}
				}
			}

			/* Physics handler */
			handle_physics(dt);
			
			/* Particle handlers */
			handle_fire();
			handle_acid();
			handle_clouds();

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
			this->mouse_button = button;
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