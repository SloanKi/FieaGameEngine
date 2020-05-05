#include "pch.h"
#include "Reaction.h"

namespace Library 
{
	RTTI_DEFINITIONS(Reaction)

	//protected
	Reaction::Reaction(RTTI::IdType typeID)
		: ActionList(typeID)
	{}

	//protected
	Reaction::Reaction(RTTI::IdType typeID, std::string name, Entity* entity) :
		ActionList(typeID, std::move(name), entity)
	{}
}