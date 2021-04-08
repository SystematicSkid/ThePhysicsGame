#pragma once

namespace Engine
{
	enum class EEntityType : int8_t
	{
		Default = 0,
		Gas,
		Fire,
	};

	enum class EEntityState : int8_t
	{
		Solid = 0,
		Liquid,
		Gas,
	};

	#pragma pack(1)
	class Entity
	{
	public:
		/* Fields */
		Vector2 position;
		Vector2 velocity;
		color col;
		EEntityType type;
		EEntityState state;
		int8_t lifetime;
	public:
		/* Constructors */
		Entity()
		{
			position = { 0, 0 };
			col = { 255,255,255 };
			type = EEntityType::Default;
		}

		Entity(Vector2 position) : position(position)
		{
			col = { 255,255,255 };
			type = EEntityType::Default;
			state = EEntityState::Solid;
		}

		Entity(Vector2 position, EEntityType type, EEntityState state) : position(position), type(type), state(state)
		{
			col = { 255,255,255 };
		}
	public:
		/* Implemented methods */
		//...

		/* Virtual methods */
		virtual void OnSimulate(float dt) = 0;
		virtual void OnDraw() = 0;
		virtual bool ShouldDelete() = 0;
	}; // Sizeof Entity: 0xE
}