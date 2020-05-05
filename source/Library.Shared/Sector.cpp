#include "pch.h"
#include "Sector.h"
#include "Entity.h"
#include "World.h"

namespace Library 
{
	RTTI_DEFINITIONS(Sector)

	Sector::Sector() : Attributed(Sector::TypeIdClass())
	{
		mEntitiesDatum = Find(ENTITIES_STRING);
		assert(mEntitiesDatum != nullptr);
	}

	Sector::Sector(const std::string& name, World* world) :
		Attributed(Sector::TypeIdClass()), mName(name)
	{
		if (world != nullptr) { SetWorld(*world); }
		mEntitiesDatum = Find(ENTITIES_STRING);
		assert(mEntitiesDatum != nullptr);
	}

	Sector::Sector(std::string&& name, World* world) :
		Attributed(Sector::TypeIdClass()), mName(name)
	{
		if (world != nullptr) { SetWorld(*world); }
		mEntitiesDatum = Find(ENTITIES_STRING);
		assert(mEntitiesDatum != nullptr);
	}

	const std::string& Sector::Name() const noexcept
	{
		return mName;
	}

	void Sector::SetName(const std::string& name) noexcept
	{
		assert(!name.empty());
		mName = name;
	}

	void Sector::SetName(std::string&& name) noexcept
	{
		assert(!name.empty());
		mName = std::move(name); //is std::move needed here?
	}

	World* Sector::GetWorld() const
	{
		Scope* parent = GetParent();
		//sanity check: if parent isn't nullptr, make sure it is a sector
		assert(parent != nullptr ? parent->Is(World::TypeIdClass()) : true);
		return static_cast<World*>(parent);
	}

	void Sector::SetWorld(World& world)
	{
		world.Adopt(*this, World::SECTORS_STRING);
	}

	void Sector::Update(WorldState& state)
	{
		for (size_t i = 0; i < mEntitiesDatum->Size(); i++)
		{
			Scope& entity = mEntitiesDatum->Get<Scope>(i);
			assert(entity.Is(Entity::TypeIdClass()));
			state.mEntity = &static_cast<Entity&>(entity);
			static_cast<Entity&>(entity).Update(state);
		}
		state.mEntity = nullptr; //reset entity ptr
	}

	Datum& Sector::Entities()
	{
		assert(mEntitiesDatum != nullptr); //sanity check
		return *mEntitiesDatum;
	}

	const Datum& Sector::Entities() const
	{
		assert(mEntitiesDatum != nullptr); //sanity check
		return *mEntitiesDatum;
	}

	Entity* Sector::CreateEntity(const std::string& className, const std::string& instanceName)
	{
		//use entity factory to make new object
		auto newEntity = Factory<Scope>::Create(className)->As<Entity>();
		assert(newEntity != nullptr);

		//set entity info
		newEntity->SetName(instanceName);

		//adopt into this 
		AdoptEntity(*newEntity);
		return newEntity;
	}

	void Sector::AdoptEntity(Entity& entity)
	{
		entity.SetSector(*this);
	}

	gsl::owner<Sector*> Sector::Clone() const
	{
		return new Sector(*this);
	}

	Vector<Signature> Sector::Signatures()
	{
		return Vector<Signature>
		{
			Signature("Name", DatumType::String, 1, offsetof(Sector, mName)),
			Signature(ENTITIES_STRING, DatumType::Table, START_ENTITY_CAPACITY, 0)
		};
	}

}
