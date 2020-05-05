#pragma once
#include "Attributed.h"
#include "vector.h"
#include "Signature.h"
#include "Factory.h"


namespace Library
{

	class Entity; //forward declaration
	class World; //forward declaration
	class WorldState; //forward declaration

	class Sector final : public Attributed
	{
		RTTI_DECLARATIONS(Sector, Attributed)
	public:
		/// <summary>
		/// Default constructor. Populates prescribed attributes using Signatures()
		/// Name is empty and world is nullptr
		/// </summary>
		Sector();

		/// <summary>
		/// Constructor: Sets name and world
		/// </summary>
		/// <param name="name">The name of this Sector</param>
		/// <param name="sector">The world this Sector belongs to</param>
		explicit Sector(const std::string& name, World* world = nullptr);

		/// <summary>
		/// Constructor: Sets name and world
		/// </summary>
		/// <param name="name">The name of this Sector</param>
		/// <param name="sector">The world this Sector belongs to</param>
		explicit Sector(std::string&& name, World* world = nullptr);

		/// <summary>
		/// Default Scope Copy Constructor
		/// </summary>
		/// <param name="rhs">the sector to copy</param>
		Sector(const Sector& rhs) = default;

		/// <summary>
		/// Default scope move constructor
		/// </summary>
		/// <param name="rhs">the sector to move</param>
		Sector(Sector && rhs) noexcept = default;

		/// <summary>
		/// Default destructor
		/// </summary>
		~Sector() = default;

		/// <summary>
		/// Default scope copy assignment
		/// </summary>
		/// <param name="rhs">the sector to copy</param>
		/// <returns>a copy of rhs</returns>
		Sector& operator=(const Sector & rhs) = default;

		/// <summary>
		/// Default scope move assignment
		/// </summary>
		/// <param name="rhs">the sector to move</param>
		/// <returns>rhs moved into this</returns>
		Sector& operator=(Sector && rhs) = default;

		/// <summary>
		/// Gets the name of this Sector
		/// </summary>
		/// <returns>The name of this Sector</returns>
		const std::string& Name() const noexcept;

		/// <summary>
		/// Sets the name of this Sector
		/// </summary>
		/// <param name="name">the name of this Sector</param>
		void SetName(const std::string & name) noexcept;

		/// <summary>
		/// Sets the name of this Sector
		/// </summary>
		/// <param name="name">the name of this Sector</param>
		void SetName(std::string && name) noexcept;

		/// <summary>
		/// gets the world this sector belongs to
		/// </summary>
		/// <returns>A pointer to the world this sector belongs to</returns>
		World* GetWorld() const;

		/// <summary>
		/// Adopts this sector into the world
		/// </summary>
		/// <param name="world">the world this sector belongs to</param>
		void SetWorld(World& world);

		/// <summary>
		/// Calls update on all entities
		/// </summary>
		/// <param name="state">The current world state</param>
		void Update(WorldState& state);

		/// <summary>
		/// Gets a reference to the datum that holds the entities of this sector
		/// </summary>
		/// <returns>A reference to the datum that holds the entities of this sector</returns>
		Datum& Entities();

		/// <summary>
		/// Gets a reference to the datum that holds the entities of this sector
		/// </summary>
		/// <returns>A reference to the datum that holds the entities of this sector</returns>
		const Datum& Entities() const;

		/// <summary>
		/// Creates an entity with the given Class, adopts it into this sector, and sets its name to instanceName.
		/// </summary>
		/// <param name="className">The name of the type of entity to create</param>
		/// <param name="instanceName">The name this instance of the entity should have</param>
		/// <returns>A ptr to the new entity created</returns>
		Entity* CreateEntity(const std::string& className, const std::string& instanceName);

		/// <summary>
		/// Thin wrapper for Adopt (from scope)
		/// </summary>
		/// <param name="entity">the entity to adopt</param>
		void AdoptEntity(Entity& entity);

		/// <summary>
		/// Creates a new copy of this
		/// </summary>
		/// <returns>a pointer to a new sector</returns>
		virtual gsl::owner<Sector*> Clone() const override;

		/// <summary>
		/// Gets the prescribed attributes for a Sector
		/// </summary>
		/// <returns>The prescribed attributes for a Sector</returns>
		static Vector<Signature> Signatures();


		static const inline std::string ENTITIES_STRING = "Entities";
	private:
		std::string mName;
		Datum* mEntitiesDatum = nullptr;
		static const size_t START_ENTITY_CAPACITY = 23;
	};

	ConcreteFactory(Sector, Scope);
}

