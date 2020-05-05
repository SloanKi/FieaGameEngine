#pragma once
#include "ActionList.h"
#include "Factory.h"

namespace Library
{
	class ActionListSwitch final : public ActionList
	{
		RTTI_DECLARATIONS(ActionListSwitch, ActionList)

	public:
		using CacheType = Hashmap<const std::string, Action*>;

		/// <summary>
		/// Default constructor: passes RTTI info to ActionList, sets name to "Switch",
		/// and sets default CaseCache capacity to DEFAULT_CACHE_CAPACITY
		/// </summary>
		ActionListSwitch();

		/// <summary>
		/// Update: Calls update on the action that is specified by Value.
		/// If value was changed between updates, searches for and caches the new action.
		/// </summary>
		/// <param name="state">The current world state</param>
		/// <exception cref="std::runtime_error">Throws exception if case with name value does not exist</exception>
		void Update(WorldState& state) override;

		/// <summary>
		/// Sets the value (The name of the case to execute)
		/// </summary>
		/// <param name="value">The case name to execute each update</param>
		void SetValue(std::string value);

		/// <summary>
		/// Gets the current value
		/// </summary>
		/// <returns>The current value (name of the case executing each update)</returns>
		const std::string& GetValue() const;

		/// <summary>
		/// Adds a case to this switch.
		/// </summary>
		/// <param name="action">The action to add</param>
		/// <param name="caseName">The name of this case</param>
		void AddCase(Action& action, std::string caseName);

		/// <summary>
		/// Removes a case from this switch
		/// NOTE: THIS ALSO DELETES THE ACTION INSTANCE ASSOCIATED WITH THE CASE NAME
		/// If case does not exist, nothing happens.
		/// </summary>
		/// <param name="caseName">The name of the case to remove</param>
		void RemoveCase(const std::string& caseName);

		/// <summary>
		/// Gets the hashmap containing casename, action* pairs
		/// </summary>
		/// <returns>hashmap containing casename, action* pairs</returns>
		CacheType& GetCases();

		/// <summary>
		/// Gets the hashmap containing casename, action* pairs
		/// </summary>
		/// <returns>hashmap containing casename, action* pairs</returns>
		const CacheType& GetCases() const;

		/// <summary>
		/// Virtual copy constructor
		/// </summary>
		/// <returns>A heap allocated copy of this</returns>
		gsl::owner<ActionListSwitch*> Clone() const;

		/// <summary>
		/// Gets the signatures used to populate this object
		/// </summary>
		/// <returns>the signatures used to populate this object</returns>
		static Vector<Signature> Signatures();
	private:
		static const size_t DEFAULT_CACHE_CAPACITY = 11;
		std::string mValue;
		CacheType mCaseCache;
		Action* mCurrentAction = nullptr;
		bool bValueChanged = true; //used to indicate when value is changed
	};
	ConcreteFactory(ActionListSwitch, Scope)
}