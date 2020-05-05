#pragma once
#include "Action.h"
#include "Factory.h"

namespace Library
{
	class ActionCreateAction final : public Action
	{
		RTTI_DECLARATIONS(ActionCreateAction, Action)

	public:
		/// <summary>
		/// Default constructor: passes RTTI info to Action and sets name to "CreateAction"
		/// </summary>
		ActionCreateAction();

		/// <summary>
		/// Constructor: default + sets prototype and action names
		/// </summary>
		/// <param name="prototype">the name of the class of action to create</param>
		/// <param name="actionName">the name to give the action</param>
		ActionCreateAction(std::string prototype, std::string actionName);

		/// <summary>
		/// Creates an action of type prototype, with name actionName, and 
		/// adopts it into the parent of this action in the datum labeled "Actions"
		/// </summary>
		/// <param name="state">the current world state</param>
		/// /// <exception cref="std::runtime_error">Throws prototype if no parent, or if action/prototype empty</exception>
		void Update(WorldState& state) override;

		/// <summary>
		/// Gets the prototype name
		/// </summary>
		/// <returns>the prototype name</returns>
		const std::string& GetPrototype() const;

		/// <summary>
		/// Set prototype name
		/// </summary>
		/// <param name="prototype">the name to set</param>
		void SetPrototype(std::string prototype);

		/// <summary>
		/// Get the name of the action
		/// </summary>
		/// <returns>The name of the action</returns>
		const std::string& GetActionName() const;

		/// <summary>
		/// Sets the action name
		/// </summary>
		/// <param name="actionName">the name to give the action when created</param>
		void SetActionName(std::string actionName);

		/// <summary>
		/// Virtual copy constructor
		/// </summary>
		/// <returns>a heap allocated copy of this</returns>
		gsl::owner<ActionCreateAction*> Clone() const override;

		/// <summary>
		/// Gets signatures to populate this
		/// </summary>
		/// <returns>The signatures used to populate this on construction</returns>
		static Vector<Signature> Signatures();
	private:
		std::string mPrototype;
		std::string mActionName;
		bool bHasUpdated = false;
	};
	ConcreteFactory(ActionCreateAction, Scope)
}

