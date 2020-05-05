#pragma once
#include "Action.h"
#include "Factory.h"

namespace Library
{
	class ActionDestroyAction final : public Action
	{
		RTTI_DECLARATIONS(ActionDestroyAction, Action)

	public:
		/// <summary>
		/// Default constructor: passes RTTI info to Action and sets name to "DestroyAction"
		/// </summary>
		ActionDestroyAction();

		/// <summary>
		/// Constructor: sets name of action to destroy
		/// </summary>
		/// <param name="actionName">the name to give the action</param>
		ActionDestroyAction(std::string actionName);

		/// <summary>
		/// Destroys an action with name actionName
		/// </summary>
		/// <param name="state">the current world state</param>
		/// <exception cref="std::runtime_error">Throws exception if no parent, or if actionname empty</exception>
		void Update(WorldState& state) override;

		/// <summary>
		/// Get the name of the action
		/// </summary>
		/// <returns>The name of the action</returns>
		const std::string& GetActionName() const;

		/// <summary>
		/// Sets the action name
		/// </summary>
		/// <param name="actionName">the name of the action to destroy</param>
		void SetActionName(std::string actionName);

		/// <summary>
		/// Virtual copy constructor
		/// </summary>
		/// <returns>a heap allocated copy of this</returns>
		gsl::owner<ActionDestroyAction*> Clone() const override;

		/// <summary>
		/// Gets signatures to populate this
		/// </summary>
		/// <returns>The signatures used to populate this on construction</returns>
		static Vector<Signature> Signatures();
	private:
		std::string mActionName;
		bool bHasUpdated = false;
	};
	ConcreteFactory(ActionDestroyAction, Scope)
}

