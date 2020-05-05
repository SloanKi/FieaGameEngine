#pragma once
#include "GameTime.h"

namespace Library
{
	class WorldState final
	{
	public:
		WorldState() = default;
		WorldState(const WorldState& rhs) = default;
		WorldState(WorldState&& rhs) noexcept = default;
		~WorldState() = default;
		WorldState& operator=(const WorldState& rhs) = default;
		WorldState& operator=(WorldState&& rhs) noexcept = default;

		/// <summary>
		/// Gets the current game time
		/// </summary>
		/// <returns>a pointer to the game time object</returns>
		GameTime* GetGameTime() const;

		/// <summary>
		/// Sets the current game time
		/// </summary>
		/// <param name="time">the gametime to set this to</param>
		void SetGameTime(GameTime& time);

		class World* mWorld = nullptr; //address of world currently being processed
		class Sector* mSector = nullptr; //address of sector currently being processed
		class Entity* mEntity = nullptr; //address of entity currently being processed
		class Action* mAction; //address of action currently being processed
	private:
		GameTime* mGameTime = nullptr;
	};
}

