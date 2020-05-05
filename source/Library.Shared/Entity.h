#pragma once
#include "Attributed.h"
#include "vector.h"
#include "Signature.h"
#include <glm/glm.hpp>
#include "Factory.h"

namespace Library
{

	class Sector; // forward declaration
	class WorldState; //Forward Declaration
	class Action; //Forward Declaration

	class Entity : public Attributed
	{
		RTTI_DECLARATIONS(Entity, Attributed)
	public:
		/// <summary>
		/// Default constructor. Populates prescribed attributes using Signatures()
		/// Name is empty and sector is nullptr
		/// </summary>
		Entity();

		/// <summary>
		/// Constructor: Sets name and sector
		/// </summary>
		/// <param name="name">The name of this entity</param>
		/// <param name="sector">The sector this entity belongs to</param>
		explicit Entity(const std::string& name, Sector* sector = nullptr);

		/// <summary>
		/// Constructor: Sets name and sector
		/// </summary>
		/// <param name="name">The name of this entity</param>
		/// <param name="sector">The sector this entity belongs to</param>
		explicit Entity(std::string&& name, Sector* sector = nullptr);

		/// <summary>
		/// Copy constructor: Default scope copy constructor
		/// </summary>
		/// <param name="rhs">the entity to copy</param>
		Entity(const Entity& rhs) = default;

		/// <summary>
		/// Move constructor: default scope move constructor
		/// </summary>
		/// <param name="rhs">the entity to move</param>
		Entity(Entity&& rhs) noexcept = default;

		/// <summary>
		/// Default scope destructor
		/// </summary>
		~Entity() = default;

		/// <summary>
		/// Copy assignment: default scope copy assignment
		/// </summary>
		/// <param name="rhs">the entity to copy</param>
		/// <returns>A copy of the entity</returns>
		Entity& operator=(const Entity& rhs) = default;

		/// <summary>
		/// Move assignment: default scope move assignment
		/// </summary>
		/// <param name="rhs">the entity to move</param>
		/// <returns>rhs entity moved into this one</returns>
		Entity& operator=(Entity&& rhs) = default;

		/// <summary>
		/// In this base class, does nothing. 
		/// Children will override this to define what they do in the update loop.
		/// </summary>
		virtual void Update(WorldState& state);

		/// <summary>
		/// Gets the name of this entity
		/// </summary>
		/// <returns>The name of this entity</returns>
		const std::string& Name() const noexcept;

		/// <summary>
		/// Sets the name of this entity
		/// </summary>
		/// <param name="name">the name of this entity</param>
		void SetName(const std::string& name) noexcept;

		/// <summary>
		/// Sets the name of this entity
		/// </summary>
		/// <param name="name">the name of this entity</param>
		void SetName(std::string&& name) noexcept;

		/// <summary>
		/// Gets the sector this entity belongs to
		/// </summary>
		/// <returns>A pointer to the sector this entity belong sto</returns>
		Sector* GetSector() const;

		/// <summary>
		/// Adopts this entity into the given sector
		/// </summary>
		/// <param name="sector">the sector this entity belongs to</param>
		void SetSector(Sector& sector);

		/// <summary>
		/// Get the actions this entity holds
		/// </summary>
		/// <returns>A reference to the datum containing actions</returns>
		Datum& Actions();

		/// <summary>
		/// Get the actions this entity holds
		/// </summary>
		/// <returns>A reference to the datum containing actions</returns>
		const Datum& Actions() const;

		/// <summary>
		/// Creates an action of type className and sets the actions name to instanceName.
		/// Adopts action into this entity
		/// </summary>
		/// <param name="className">The name of the kind of action to spawn</param>
		/// <param name="instanceName">The name to give to this action</param>
		/// <returns>A pointer to the created action</returns>
		Action* CreateAction(const std::string& className, const std::string& instanceName);

		/// <summary>
		/// Adopt an action into this entity
		/// </summary>
		/// <param name="action">The action to adopt</param>
		void AdoptAction(Action& action);

		/// <summary>
		/// Creates a new copy of this (virtual copy constructor)
		/// </summary>
		/// <returns>a pointer to a new entity</returns>
		virtual gsl::owner<Entity*> Clone() const override;

		/// <summary>
		/// Gets the prescribed attributes for an entity
		/// </summary>
		/// <returns>The prescribed attributes for an entity</returns>
		static Vector<Signature> Signatures();

		static const inline std::string ACTIONS_STRING = "Actions";
	protected:
		/// <summary>
		/// Default constructor for Derived classes. Populates prescribed attributes using Signatures()
		/// Name is empty and sector is nullptr
		/// </summary>
		/// <param name="typeID">The typeID of the derived class</param>
		explicit Entity(RTTI::IdType typeID);

		/// <summary>
		/// Constructor: Sets name and sector
		/// </summary>
		/// <param name="name">The name of this entity</param>
		/// <param name="sector">The sector this entity belongs to</param>
		/// <param name="typeID">The typeID of the derived class</param>
		Entity(RTTI::IdType typeID, const std::string& name, Sector* sector = nullptr);

		/// <summary>
		/// Constructor: Sets name and sector
		/// </summary>
		/// <param name="name">The name of this entity</param>
		/// <param name="sector">The sector this entity belongs to</param>
		/// <param name="typeID">The typeID of the derived class</param>
		Entity(RTTI::IdType typeID, std::string&& name, Sector* sector = nullptr);

		std::string mName;
		Datum* mActionsDatum = nullptr;
		static const size_t START_ACTIONS_CAPACITY = 23;
	};

	ConcreteFactory(Entity, Scope) //Creates a factory for Entity classes
}

