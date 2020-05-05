#pragma once
#include "Action.h"
#include "Factory.h"

namespace Library
{
	class ActionIncrement final : public Action
	{
		RTTI_DECLARATIONS(ActionIncrement, Action)

	public:
		/// <summary>
		/// Default constructor: Sets name to Increment
		/// </summary>
		ActionIncrement();

		/// <summary>
		/// Increment the given operand by step amount. 
		/// If operand datum not yet set, searches using operand string. 
		/// Does not yet support any kind of addressing yet. Just takes the first datum search gives.
		/// </summary>
		/// <param name="state">The current world state</param>
		/// <exception cref="std::runtime_error">Throws exception if operand cannot be found</exception>
		void Update(WorldState& state) override;

		/// <summary>
		/// Set the operand string
		/// </summary>
		/// <param name="operand">The name of the operand</param>
		void SetOperandString(const std::string& operand);

		/// <summary>
		/// Get the current operand string
		/// </summary>
		/// <returns>The name of the operand this action is incrementing</returns>
		const std::string& GetOperandString() const;

		/// <summary>
		/// Set the amount to increment by each frame
		/// </summary>
		/// <param name="step">the amount to increment</param>
		void SetStep(int step);

		/// <summary>
		/// Get the current step amount (the amount incremented each frame)
		/// </summary>
		/// <returns>The current step value</returns>
		int GetStep() const;

		/// <summary>
		/// Virtual copy constructor
		/// </summary>
		/// <returns>A heap allocated copy of this</returns>
		gsl::owner<ActionIncrement*> Clone() const override;

		/// <summary>
		/// Signatures for prescribed attributes of this class
		/// </summary>
		/// <returns>Signatures for prescribed attributes of this class</returns>
		static Vector<Signature> Signatures();
	private:
		std::string mOperandString; //the name of the operand
		int mStep = 1;
		Datum* mOperandDatum = nullptr;
	};
	ConcreteFactory(ActionIncrement, Scope)
}

