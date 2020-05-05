#include "pch.h"
#include "WorldState.h"

namespace Library
{
	GameTime* Library::WorldState::GetGameTime() const
	{
		return mGameTime;
	}

	void WorldState::SetGameTime(GameTime& time)
	{
		mGameTime = &time;
	}
}
