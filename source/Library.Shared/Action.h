#pragma once
#include "Attributed.h"
#include "vector.h"
#include "Signature.h"
#include <glm/glm.hpp>

namespace Library
{
	class Entity;	//Forward declaration
	class WorldState;	//Forward Declaration
	class ActionList; //Forward Declaration

	class Action : public Attributed
	{
		RTTI_DECLARATIONS(Action, Attributed)
	public:
		/// <summary>
		/// Copy constructor: Default scope copy constructor
		/// </summary>
		/// <param name="rhs">the action to copy</param>
		Action(const Action& rhs) = default;

		/// <summary>
		/// Move constructor: default scope move constructor
		/// </summary>
		/// <param name="rhs">the action to move</param>
		Action(Action&& rhs) noexcept = default;

		/// <summary>
		/// Default scope destructor
		/// </summary>
		~Action() = default;

		/// <summary>
		/// Copy assignment: default scope copy assignment
		/// </summary>
		/// <param name="rhs">the action to copy</param>
		/// <returns>A copy of the action</returns>
		Action& operator=(const Action& rhs) = default;

		/// <summary>
		/// Move assignment: default scope move assignment
		/// </summary>
		/// <param name="rhs">the action to move</param>
		/// <returns>rhs action moved into this one</returns>
		Action& operator=(Action&& rhs) = default;

		/// <summary>
		/// In this base class, does nothing. (PURE VIRTUAL)
		/// Children will override this to define what they do in the update loop.
		/// </summary>
		/// <param name="state">the world state</param>
		virtual void Update(WorldState& state) = 0;

		/// <summary>
		/// Gets the name of this action
		/// </summary>
		/// <returns>The name of this action</returns>
		const std::string& Name() const noexcept;

		/// <summary>
		/// Sets the name of this action
		/// </summary>
		/// <param name="name">the name of this action</param>
		void SetName(const std::string& name) noexcept;

		/// <summary>
		/// Sets the name of this action
		/// </summary>
		/// <param name="name">the name of this action</param>
		void SetName(std::string&& name) noexcept;

		/// <summary>
		/// Gets the entity this action belongs to
		/// </summary>
		/// <returns>A pointer to the entity this action belongs to or nullptr if it doesn't belong to an entity</returns>
		/// <remarks>If the parent is an ActionList, will search up for a parent Entity. if none found, returns nullptr.</remarks>
		Entity* GetEntity() const;

		/// <summary>
		/// Adopts this action into the given entity
		/// </summary>
		/// <param name="entity">the entity this action belongs to</param>
		void SetEntity(Entity& entity);

		/// <summary>
		/// Gets the ActionList this action belongs to
		/// </summary>
		/// <returns>A pointer to the ActionList this action is in, or nullptr if it is not in an ActionList</returns>
		ActionList* GetActionList() const;
		
		/// <summary>
		/// Adopts this into an ActionList
		/// </summary>
		/// <param name="actionList">the ActionList to be added to</param>
		void SetActionList(ActionList& actionList);

		/// <summary>
		/// Creates a new copy of this (PURE VIRTUAL)
		/// </summary>
		/// <returns>a pointer to a new action</returns>
		virtual gsl::owner<Action*> Clone() const override = 0;

		/// <summary>
		/// Gets the prescribed attributes for an action
		/// </summary>
		/// <returns>The prescribed attributes for an action</returns>
		static Vector<Signature> Signatures();

	protected:
		/// <summary>
		/// "Virtual Constructor" passes correct typeID to Attributed so it can get signatures
		/// </summary>
		/// <param name="typeID">The typeID of the action</param>
		explicit Action(RTTI::IdType typeID);

		/// <summary>
		/// Constructor: Sets name and entity
		/// </summary>
		/// <param name="name">The name of this action</param>
		/// <param name="entity">The entity this action belongs to</param>
		/// <param name="typeID">The typeID of the action</param>
		Action(RTTI::IdType typeID, const std::string& name, Entity* entity = nullptr);

		/// <summary>
		/// Constructor: Sets name and entity
		/// </summary>
		/// <param name="name">The name of this action</param>
		/// <param name="entity">The entity this action belongs to</param>
		/// <param name="typeID">The typeID of the action</param>
		Action(RTTI::IdType typeID, std::string&& name, Entity* entity = nullptr);

		void QueueDeleteIfRunOnce(WorldState& state);

		std::string mName;
		bool bRunOnce = false; //todo: make this a prescribed attribute
	};
}

