#pragma once
#include "Action.h"
#include "Factory.h"


namespace Library
{
	class ActionList : public Action
	{
		RTTI_DECLARATIONS(ActionList, Action)
	public:
		/// <summary>
		/// Default constructor: Passes RTTI typeID to Action's "virtual" default constructor
		/// </summary>
		ActionList();

		/// <summary>
		/// Constructor: sets name and entity
		/// </summary>
		/// <param name="name">The name of this actionlist</param>
		/// <param name="entity">The entity this actionlist belongs to</param>
		explicit ActionList(const std::string& name, Entity* entity = nullptr);

		/// <summary>
		/// Constructor: sets name and entity
		/// </summary>
		/// <param name="name">The name of this actionlist</param>
		/// <param name="entity">The entity this actionlist belongs to</param>
		explicit ActionList(std::string&& name, Entity* entity = nullptr);

		/// <summary>
		/// Copy constructor: Default scope copy constructor
		/// </summary>
		/// <param name="rhs">the actionlist to copy</param>
		ActionList(const ActionList& rhs) = default;

		/// <summary>
		/// Move constructor: default scope move constructor
		/// </summary>
		/// <param name="rhs">the actionlist to move</param>
		ActionList(ActionList&& rhs) noexcept = default;

		/// <summary>
		/// Default scope destructor
		/// </summary>
		~ActionList() = default;

		/// <summary>
		/// Copy assignment: default scope copy assignment
		/// </summary>
		/// <param name="rhs">the actionlist to copy</param>
		/// <returns>A copy of the actionlist</returns>
		ActionList& operator=(const ActionList& rhs) = default;

		/// <summary>
		/// Move assignment: default scope move assignment
		/// </summary>
		/// <param name="rhs">the actionlist to move</param>
		/// <returns>rhs actionlist moved into this one</returns>
		ActionList& operator=(ActionList&& rhs) = default;


		/// <summary>
		/// Updates every action contained in this action list
		/// </summary>
		/// <param name="state">the world state</param>
		virtual void Update(WorldState& state);

		/// <summary>
		/// Get the actions this ActionList holds
		/// </summary>
		/// <returns>A reference to the datum containing actions</returns>
		Datum& Actions();

		/// <summary>
		/// Get the actions this ActionList holds
		/// </summary>
		/// <returns>A reference to the datum containing actions</returns>
		const Datum& Actions() const;

		/// <summary>
		/// Creates an action of type className and sets the actions name to instanceName.
		/// Adopts action into this ActionList
		/// </summary>
		/// <param name="className">The name of the kind of action to spawn</param>
		/// <param name="instanceName">The name to give to this action</param>
		/// <returns>A pointer to the created action</returns>
		Action* CreateAction(const std::string& className, const std::string& instanceName);

		/// <summary>
		/// Adopts an action into this ActionList
		/// </summary>
		/// <param name="action">the action to adopt</param>
		void AdoptAction(Action& action);

		/// <summary>
		/// Creates a new copy of this (virtual copy constructor)
		/// </summary>
		/// <returns>a pointer to a new ActionList (that is a copy of this)</returns>
		virtual gsl::owner<ActionList*> Clone() const override;

		/// <summary>
		/// Gets the prescribed attributes for an actionlist
		/// </summary>
		/// <returns>The prescribed attributes for an actionlist</returns>
		static Vector<Signature> Signatures();

		static const inline std::string ACTIONS_STRING = "Actions";

	protected:
		/// <summary>
		/// "Virtual Constructor" passes correct typeID to Action so it can get signatures
		/// </summary>
		/// <param name="typeID">The typeID of the actionlist</param>
		explicit ActionList(RTTI::IdType typeID);

		/// <summary>
		/// Constructor: sets name and entity
		/// </summary>
		/// <param name="typeID">The typeID of the actionlist</param>
		/// <param name="name">The name of this actionlist</param>
		/// <param name="entity">The entity this actionlist belongs to</param>
		ActionList(RTTI::IdType typeID, const std::string& name, Entity* entity = nullptr);

		/// <summary>
		/// Constructor: sets name and entity
		/// </summary>
		/// <param name="typeID">The typeID of the actionlist</param>
		/// <param name="name">The name of this actionlist</param>
		/// <param name="entity">The entity this actionlist belongs to</param>
		ActionList(RTTI::IdType typeID, std::string&& name, Entity* entity = nullptr);

		Datum* mActionsDatum = nullptr;
		static const size_t START_ACTIONS_CAPACITY = 23;
	};
	ConcreteFactory(ActionList, Scope)
}