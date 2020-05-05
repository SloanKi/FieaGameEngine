#pragma once
#include "Attributed.h"
#include "vector.h"
#include "Signature.h"
#include "WorldState.h"
#include "EventQueue.h"

namespace Library
{
	class Sector; //forward declaration

	class World final : public Attributed
	{
		RTTI_DECLARATIONS(World, Attributed)
	public:
		/// <summary>
		/// Default constructor. Populates prescribed attributes using Signatures()
		/// Name is empty
		/// </summary>
		World();

		/// <summary>
		/// Constructor: Sets name
		/// </summary>
		/// <param name="name">The name of this World</param>
		explicit World(const std::string& name);

		/// <summary>
		/// Constructor: Sets name
		/// </summary>
		/// <param name="name">The name of this World</param>
		explicit World(std::string&& name);

		~World() = default;

		//No move/copy semantics: Assumption = 1 world
		World(const World& rhs) = delete;
		World(World && rhs) noexcept = delete;
		World& operator=(const World & rhs) = delete;
		World& operator=(World && rhs) = delete;

		/// <summary>
		/// Gets the clock
		/// </summary>
		/// <returns>a reference to the game clock</returns>
		const GameClock& GetGameClock() const;

		/// <summary>
		/// Gets the world state
		/// </summary>
		/// <returns>a reference to the world state</returns>
		WorldState& GetWorldState();

		/// <summary>
		/// Gets the world state
		/// </summary>
		/// <returns>a reference to the world state</returns>
		const WorldState& GetWorldState() const;

		/// <summary>
		/// Gets the name of this World
		/// </summary>
		/// <returns>The name of this World</returns>
		const std::string& Name() const noexcept;

		/// <summary>
		/// Sets the name of this World
		/// </summary>
		/// <param name="name">the name of this World</param>
		void SetName(const std::string & name) noexcept;

		/// <summary>
		/// Sets the name of this World
		/// </summary>
		/// <param name="name">the name of this World</param>
		void SetName(std::string && name) noexcept;

		EventQueue& GetEventQueue();
		const EventQueue& GetEventQueue() const;

		/// <summary>
		/// Updates the gametime of the world state, then calls the update of all sectors with this new gametime
		/// </summary>
		void Update();

		/// <summary>
		/// Gets the datum holding the Sectors that belong to this world
		/// </summary>
		/// <returns>A reference to the datum holding the sectors that belong to this world</returns>
		Datum& Sectors();

		/// <summary>
		/// Gets the datum holding the Sectors that belong to this world
		/// </summary>
		/// <returns>A reference to the datum holding the sectors that belong to this world</returns>
		const Datum& Sectors() const;

		/// <summary>
		/// Uses sector factory to create a new sector and adopts it into this world
		/// </summary>
		/// <param name="instanceName">The name of the new sector</param>
		/// <returns>A pointer to the new sector</returns>
		Sector* CreateSector(const std::string& instanceName);

		/// <summary>
		/// Adopts a sector into this world
		/// </summary>
		/// <param name="sector">the sector to adopt</param>
		void AdoptSector(Sector& sector);

		/// <summary>
		/// Adds the action to the create queue to be created at the start of the next update.
		/// </summary>
		/// <param name="prototype">The class of action</param>
		/// <param name="name">The name to give the action</param>
		/// <param name="target">The scope we are going to put this in</param>
		/// <param name="key">The key to put this under in the target scope</param>
		void AddActionToCreateQueue(std::string prototype, std::string name, Scope* target, std::string key);

		/// <summary>
		/// Adds an action to the destroy queue. Will be destroyed at end of update.
		/// </summary>
		/// <param name="action"></param>
		void AddActionToDestroyQueue(Action& action);

		/// <summary>
		/// Gets the prescribed attributes for a World
		/// </summary>
		/// <returns>The prescribed attributes for a World</returns>
		static Vector<Signature> Signatures();

		static const inline std::string SECTORS_STRING = "Sectors";
	private:
		void AddActions();
		void DestroyActions();
		GameClock mClock;
		GameTime mTime;
		WorldState mState;
		std::string mName;
		Datum* mSectorsDatum = nullptr;
		static const size_t START_SECTOR_CAPACITY = 23;
		struct AddActionInfo 
		{
			std::string PrototypeName; //the name of the class of action
			std::string ActionName; //the name to give this action
			Scope* Target; //The scope to add this action to
			std::string Key; //The key to put this action under in the target
			AddActionInfo(std::string prototype, std::string name, Scope* target, std::string key) :
				PrototypeName(std::move(prototype)), ActionName(std::move(name)), Target(target), Key(std::move(key)) {};
		};
		Vector<AddActionInfo> mAddActionList;
		Vector<Action*> mDestroyActionList;
		EventQueue mEventQueue;
	};
}

