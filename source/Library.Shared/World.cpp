#include "pch.h"
#include "World.h"
#include "Sector.h"
#include "Factory.h"
#include "Scope.h"
#include "Action.h"

namespace Library
{
	RTTI_DEFINITIONS(World)

	World::World() : Attributed(World::TypeIdClass()), mEventQueue(mClock)
	{
		mSectorsDatum = Find(SECTORS_STRING);
		assert(mSectorsDatum != nullptr);
		mClock.UpdateGameTime(mTime);
		mState.SetGameTime(mTime);
		mState.mWorld = this;
	}

	World::World(const std::string& name) :
		Attributed(World::TypeIdClass()), mName(name), mEventQueue(mClock)
	{
		mSectorsDatum = Find(SECTORS_STRING);
		assert(mSectorsDatum != nullptr);
		mClock.UpdateGameTime(mTime);
		mState.SetGameTime(mTime);
		mState.mWorld = this;
	}

	World::World(std::string&& name) :
		Attributed(World::TypeIdClass()), mName(std::move(name)), mEventQueue(mClock)
	{
		mSectorsDatum = Find(SECTORS_STRING);
		assert(mSectorsDatum != nullptr);
		mClock.UpdateGameTime(mTime);
		mState.SetGameTime(mTime);
		mState.mWorld = this;
	}

	const GameClock& World::GetGameClock() const
	{
		return mClock;
	}

	WorldState& World::GetWorldState()
	{
		return mState;
	}

	const WorldState& World::GetWorldState() const
	{
		return mState;
	}

	const std::string& World::Name() const noexcept
	{
		return mName;
	}

	void World::SetName(const std::string& name) noexcept
	{
		assert(!name.empty());
		mName = name;
	}

	void World::SetName(std::string&& name) noexcept
	{
		assert(!name.empty());
		mName = std::move(name); //is std::move needed here?
	}

	EventQueue& World::GetEventQueue()
	{
		return mEventQueue;
	}

	const EventQueue& World::GetEventQueue() const
	{
		return mEventQueue;
	}

	void World::Update()
	{
		mClock.UpdateGameTime(mTime);
		mEventQueue.Update(mTime);
		AddActions();
		for (size_t i = 0; i < mSectorsDatum->Size(); i++)
		{
			Scope& sector = mSectorsDatum->Get<Scope>(i);
			assert(sector.Is(Sector::TypeIdClass()));
			mState.mSector = &static_cast<Sector&>(sector);
			static_cast<Sector&>(sector).Update(mState);
		}
		mState.mSector = nullptr; //reset sector ptr
		DestroyActions();
	}

	Datum& World::Sectors()
	{
		assert(mSectorsDatum != nullptr); //sanity check
		return *mSectorsDatum;
	}

	const Datum& World::Sectors() const
	{
		assert(mSectorsDatum != nullptr); //sanity check
		return *mSectorsDatum;
	}

	Sector* World::CreateSector(const std::string& instanceName)
	{
		//create sector
		auto newSector = Factory<Scope>::Create("Sector");
		assert(newSector != nullptr);
		assert(newSector->Is(Sector::TypeIdClass()));

		//set sector info
		Sector* newSecPtr = static_cast<Sector*>(newSector);
		newSecPtr->SetName(instanceName);
		
		//adopt sector into this
		AdoptSector(*newSecPtr);
		return newSecPtr;
	}

	void World::AdoptSector(Sector& sector)
	{
		sector.SetWorld(*this);
	}

	void World::AddActionToCreateQueue(std::string prototype, std::string name, Scope* target, std::string key)
	{
		mAddActionList.PushBack(AddActionInfo(std::move(prototype), std::move(name), target, std::move(key)));
	}

	void World::AddActionToDestroyQueue(Action& action)
	{
		mDestroyActionList.PushBack(&action);
	}


	Vector<Signature> World::Signatures()
	{
		return Vector<Signature>
		{
			Signature("Name", DatumType::String, 1, offsetof(World, mName)),
			Signature(SECTORS_STRING, DatumType::Table, START_SECTOR_CAPACITY, 0)
		};
	}

	void World::AddActions()
	{
		for (auto& actionInfo : mAddActionList)
		{
			//create action
			Scope* actionScope = Factory<Scope>::Create(actionInfo.PrototypeName);
			assert(actionScope != nullptr);
			assert(actionScope->Is(Action::TypeIdClass()));
			Action* actionPtr = static_cast<Action*>(actionScope);
			//set name
			actionPtr->SetName(actionInfo.ActionName);

			//add to desired Attributed Scope (Entity, ActionList, World, Sector)
			assert(actionInfo.Target != nullptr); //TODO: change this to ref so this isn't needed
			actionInfo.Target->Adopt(*actionPtr, actionInfo.Key);
		}

		//clear list
		mAddActionList.Clear();
	}

	void World::DestroyActions()
	{
		for (Action* action : mDestroyActionList)
		{
			delete(action); //destructor will call orphan
		}
		mDestroyActionList.Clear();
	}
}