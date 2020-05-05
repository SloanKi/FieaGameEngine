#include "pch.h"
#include "CppUnitTest.h"
#include "TypeRegistry.h"
#include "Attributed.h"
#include "Entity.h"
#include "Sector.h"
#include "World.h"
#include "WorldState.h"
#include "TestEntity.h"
#include "JsonTableParseHelper.h"
#include "JsonParseMaster.h"
#include "TestAction.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;
using namespace UnitTests;
using namespace std;
using namespace std::string_literals;



namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<Library::DatumType>(const Library::DatumType& t)
	{
		RETURN_WIDE_STRING(&t);
	}

	template<>
	inline std::wstring ToString<Sector>(Sector* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<World>(World* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<class std::chrono::high_resolution_clock::time_point>(const class std::chrono::high_resolution_clock::time_point& t)
	{
		RETURN_WIDE_STRING(&t);
	}

	template<>
	inline std::wstring ToString<GameTime>(GameTime* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Library::Datum>(const Library::Datum& t)
	{
		RETURN_WIDE_STRING(&t);
	}

	template<>
	inline std::wstring ToString<Sector>(const Sector& t)
	{
		RETURN_WIDE_STRING(&t);
	}

	template<>
	inline std::wstring ToString<Entity>(const Entity& t)
	{
		RETURN_WIDE_STRING(&t);
	}

	template<>
	inline std::wstring ToString<Entity>(Entity* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<TestEntity>(const TestEntity& t)
	{
		RETURN_WIDE_STRING(&t);
	}

	template<>
	inline std::wstring ToString<TestAction>(TestAction* t)
	{
		RETURN_WIDE_STRING(&t);
	}
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(EntitySectorWorldTests)
	{
	public:
		//check for memory leaks
		TEST_METHOD_INITIALIZE(Initialize)
		{

#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
#endif
			TypeRegistry::Create();
			TypeRegistry::RegisterType(Entity::TypeIdClass(), Entity::Signatures());
			TypeRegistry::RegisterType(TestEntity::TypeIdClass(), TestEntity::Signatures());
			TypeRegistry::RegisterType(Sector::TypeIdClass(), Sector::Signatures());
			TypeRegistry::RegisterType(World::TypeIdClass(), World::Signatures());
			TypeRegistry::RegisterType(TestAction::TypeIdClass(), TestAction::Signatures());
		}

		//check for memory leaks
		TEST_METHOD_CLEANUP(Cleanup)
		{
			TypeRegistry::Shutdown();
#if defined(DEBUG) || defined(_DEBUG)
			_CrtMemState endMemState, diffMemState;
			_CrtMemCheckpoint(&endMemState);
			if (_CrtMemDifference(&diffMemState, &sStartMemState, &endMemState))
			{
				_CrtMemDumpStatistics(&diffMemState);
				Assert::Fail(L"Memory Leaks!");
			}
#endif
		}

		TEST_METHOD(WorldStateGameTime)
		{
			//quick sanity check on how this works
			GameClock clock;
			GameTime time;
			Assert::AreNotEqual(clock.CurrentTime(), time.CurrentTime());
			clock.UpdateGameTime(time);
			Assert::AreEqual(clock.CurrentTime(), time.CurrentTime());

			//State gametime test
			WorldState state;
			GameTime time2;
			Assert::IsNull(state.GetGameTime());
			state.SetGameTime(time2);
			Assert::AreEqual(state.GetGameTime(), &time2);
			clock.UpdateGameTime(time2);
			Assert::AreEqual(clock.CurrentTime(), state.GetGameTime()->CurrentTime());
		}

		/************************************************************************/
		/**************************Entity Functions******************************/
		/************************************************************************/

		TEST_METHOD(EntityCtor)
		{
			//Default ctor
			{
				Entity entity;
				Assert::IsTrue(entity["this"] == &entity);
				Assert::IsTrue(entity["Name"] == "");
				Assert::AreEqual(entity["Actions"].Type(), DatumType::Table);
				Assert::AreEqual(entity["Actions"].Capacity(), 23_z);
				Assert::IsNull(entity.GetSector());
			}

			//ctor with name (r value)
			{
				Entity entity("Hello");
				Assert::IsTrue(entity["this"] == &entity);
				Assert::IsTrue(entity["Name"] == "Hello");
				Assert::AreEqual(entity["Actions"].Type(), DatumType::Table);
				Assert::AreEqual(entity["Actions"].Capacity(), 23_z);
				Assert::IsNull(entity.GetSector());
			}

			//ctor with name (l value)
			{
				std::string name = "Hello";
				Entity entity(name);
				Assert::IsTrue(entity["this"] == &entity);
				Assert::IsTrue(entity["Name"] == name);
				Assert::AreEqual(entity["Actions"].Type(), DatumType::Table);
				Assert::AreEqual(entity["Actions"].Capacity(), 23_z);
				Assert::IsNull(entity.GetSector());
			}

			//ctor with name and sector
			{
				Sector sector;
				Entity entity("Hello", &sector);
				Assert::IsTrue(entity["this"] == &entity);
				Assert::IsTrue(entity["Name"] == "Hello");
				Assert::AreEqual(entity["Actions"].Type(), DatumType::Table);
				Assert::AreEqual(entity["Actions"].Capacity(), 23_z);
				Assert::AreEqual(entity.GetSector(), &sector);
			}
		}

		TEST_METHOD(EntityRTTICtors)
		{
			//Default ctor
			{
				TestEntity entity;
				Assert::IsTrue(entity["this"] == &entity);
				Assert::IsTrue(entity["Name"] == "");
				Assert::AreEqual(entity["Actions"].Type(), DatumType::Table);
				Assert::AreEqual(entity["Actions"].Capacity(), 23_z);
				Assert::IsNull(entity.GetSector());
				Assert::IsTrue(entity.Is(TestEntity::TypeIdClass()));
			}

			//ctor with name (r value)
			{
				TestEntity entity("Hello");
				Assert::IsTrue(entity["this"] == &entity);
				Assert::IsTrue(entity["Name"] == "Hello");
				Assert::AreEqual(entity["Actions"].Type(), DatumType::Table);
				Assert::AreEqual(entity["Actions"].Capacity(), 23_z);
				Assert::IsNull(entity.GetSector());
			}

			//ctor with name (l value)
			{
				std::string name = "Hello";
				TestEntity entity(name);
				Assert::IsTrue(entity["this"] == &entity);
				Assert::IsTrue(entity["Name"] == name);
				Assert::AreEqual(entity["Actions"].Type(), DatumType::Table);
				Assert::AreEqual(entity["Actions"].Capacity(), 23_z);
				Assert::IsNull(entity.GetSector());
			}

			//ctor with name and sector
			{
				Sector sector;
				TestEntity entity("Hello", &sector);
				Assert::IsTrue(entity["this"] == &entity);
				Assert::IsTrue(entity["Name"] == "Hello");
				Assert::AreEqual(entity["Actions"].Type(), DatumType::Table);
				Assert::AreEqual(entity["Actions"].Capacity(), 23_z);
				Assert::AreEqual(entity.GetSector(), &sector);
			}
		}

		TEST_METHOD(EntityName)
		{
			Entity entity;
			Assert::AreEqual(entity.Name(), ""s);
			Assert::IsTrue(entity["Name"] == "");

			//r value
			entity.SetName("Bob");
			Assert::AreEqual(entity.Name(), "Bob"s);
			Assert::IsTrue(entity["Name"] == "Bob");

			//l value
			std::string name = "not bob";
			entity.SetName(name);
			Assert::AreEqual(entity.Name(), name);
			Assert::IsTrue(entity["Name"] == name);
		}

		TEST_METHOD(EntitySector)
		{
			Sector sector;
			Entity* entity = new Entity();

			Assert::IsNull(entity->GetSector());
			Assert::AreEqual(sector.Entities().Size(), 0_z);
			entity->SetSector(sector);
			Assert::AreEqual(entity->GetSector(), &sector);
			Assert::AreEqual(sector.Entities().Size(), 1_z);

			//check that switching to another sector will remove from old one
			Sector sector2;
			Assert::AreEqual(sector2.Entities().Size(), 0_z);
			entity->SetSector(sector2);
			Assert::AreEqual(entity->GetSector(), &sector2);
			Assert::AreEqual(sector.Entities().Size(), 0_z);
			Assert::AreEqual(sector2.Entities().Size(), 1_z);
		}

		TEST_METHOD(EntityClone) 
		{
			Entity entity("test");
			Entity* clone = entity.Clone();
			Assert::AreNotSame(*clone, entity);
			Assert::AreEqual(*clone, entity);
			delete(clone);

			TestEntity testEntity;
			TestEntity* testClone = testEntity.Clone();
			Assert::AreNotSame(testEntity, *testClone);
			Assert::AreEqual(testEntity, *testClone);
			delete(testClone);
		}

		TEST_METHOD(EntityActions)
		{
			TestActionFactory actionFact;
			Entity entity;
			Datum& actionDat = entity.Actions();
			Assert::AreEqual(DatumType::Table, actionDat.Type());
			Assert::AreEqual(23_z, actionDat.Capacity());
			Assert::AreEqual(0_z, actionDat.Size());

			auto action = entity.CreateAction("TestAction", "Instance");
			Assert::AreEqual(1_z, actionDat.Size());
			Assert::IsNotNull(action);
			Assert::AreEqual("Instance"s, action->Name());
			Assert::AreEqual(&entity, action->GetEntity());
			Assert::IsTrue(action->Is(TestAction::TypeIdClass()));

			//adopt action test
			TestAction action2;
			entity.AdoptAction(action2);
			Assert::AreEqual(2_z, actionDat.Size());
			Assert::AreEqual(&entity, action2.GetEntity());

			//const actions()
			const Entity& constEntity = entity;
			Assert::AreEqual(2_z, constEntity.Actions().Size());
		}

		TEST_METHOD(EntityUpdate)
		{
			WorldState state;
			Entity entity;
			TestAction act1, act2, act3;
			entity.AdoptAction(act1);
			entity.AdoptAction(act2);
			entity.AdoptAction(act3);

			//make sure they are in there
			Assert::AreEqual(entity.Actions().Size(), 3_z);

			//make sure update is being called on all actions
			entity.Update(state);
			Assert::AreEqual(act1.mUpdateCount, 1);
			Assert::AreEqual(act2.mUpdateCount, 1);
			Assert::AreEqual(act3.mUpdateCount, 1);

			//2x more for sanity
			entity.Update(state);
			entity.Update(state);
			Assert::AreEqual(act1.mUpdateCount, 3);
			Assert::AreEqual(act2.mUpdateCount, 3);
			Assert::AreEqual(act3.mUpdateCount, 3);
		}
		
		/************************************************************************/
		/**************************Sector Functions******************************/
		/************************************************************************/
		TEST_METHOD(SectorCtor)
		{
			//default ctor
			{
				Sector sector;
				Assert::IsTrue(sector["this"] == &sector);
				Assert::IsTrue(sector["Name"] == "");
				Assert::AreEqual(sector["Entities"].Type(), DatumType::Table);
				Assert::AreEqual(sector["Entities"].Capacity(), 23_z);
				Assert::IsNull(sector.GetWorld());
			}

			//ctor with name (l value)
			{
				std::string name = "Hello";
				Sector sector(name);
				Assert::IsTrue(sector["this"] == &sector);
				Assert::IsTrue(sector["Name"] == name);
				Assert::AreEqual(sector["Entities"].Type(), DatumType::Table);
				Assert::AreEqual(sector["Entities"].Capacity(), 23_z);
				Assert::IsNull(sector.GetWorld());
			}

			//ctor with name (r value)
			{
				Sector sector("Hello");
				Assert::IsTrue(sector["this"] == &sector);
				Assert::IsTrue(sector["Name"] == "Hello");
				Assert::AreEqual(sector["Entities"].Type(), DatumType::Table);
				Assert::AreEqual(sector["Entities"].Capacity(), 23_z);
				Assert::IsNull(sector.GetWorld());
			}

			//ctor with name and world
			{
				World world;
				Sector sector("Hello", &world);
				Assert::IsTrue(sector["this"] == &sector);
				Assert::IsTrue(sector["Name"] == "Hello");
				Assert::AreEqual(sector["Entities"].Type(), DatumType::Table);
				Assert::AreEqual(sector["Entities"].Capacity(), 23_z);
				Assert::AreEqual(sector.GetWorld(), &world);
			}
		}

		TEST_METHOD(SectorName)
		{
			Sector sector;
			Assert::AreEqual(sector.Name(), ""s);
			Assert::IsTrue(sector["Name"] == "");

			//r value
			sector.SetName("Bob");
			Assert::AreEqual(sector.Name(), "Bob"s);
			Assert::IsTrue(sector["Name"] == "Bob");

			//l value
			std::string name = "not bob";
			sector.SetName(name);
			Assert::AreEqual(sector.Name(), name);
			Assert::IsTrue(sector["Name"] == name);
		}

		TEST_METHOD(SectorWorld)
		{
			World world;
			Sector* sector = new Sector();

			Assert::IsNull(sector->GetWorld());
			Assert::AreEqual(world.Sectors().Size(), 0_z);
			sector->SetWorld(world);
			Assert::AreEqual(sector->GetWorld(), &world);
			Assert::AreEqual(world.Sectors().Size(), 1_z);

			//check that switching to another sector will remove from old one
			World world2;
			Assert::AreEqual(world2.Sectors().Size(), 0_z);
			sector->SetWorld(world2);
			Assert::AreEqual(sector->GetWorld(), &world2);
			Assert::AreEqual(world.Sectors().Size(), 0_z);
			Assert::AreEqual(world2.Sectors().Size(), 1_z);
		}

		TEST_METHOD(SectorEntities)
		{
			EntityFactory entFact;
			TestEntityFactory testEntFact;
			Sector sector;
			Datum& entDat = sector.Entities();
			Assert::AreEqual(entDat.Type(), DatumType::Table);
			Assert::AreEqual(entDat.Capacity(), 23_z);
			Assert::AreEqual(entDat.Size(), 0_z);

			auto entity = sector.CreateEntity("Entity", "Instance");
			Assert::AreEqual(entDat.Size(), 1_z);
			Assert::IsNotNull(entity);
			Assert::AreEqual(entity->Name(), "Instance"s);
			Assert::AreEqual(entity->GetSector(), &sector);

			auto testEntity = sector.CreateEntity("TestEntity", "TestInstance");
			Assert::AreEqual(entDat.Size(), 2_z);
			Assert::IsNotNull(entity);
			Assert::AreEqual(testEntity->Name(), "TestInstance"s);
			Assert::AreEqual(testEntity->GetSector(), &sector);

			//adopt ent test
			Entity entity2;
			sector.AdoptEntity(entity2);
			Assert::AreEqual(entDat.Size(), 3_z);
			Assert::AreEqual(entity2.GetSector(), &sector);

			//const Entities()
			const Sector& constSector = sector;
			Assert::AreEqual(constSector.Entities().Size(), 3_z);
		}

		TEST_METHOD(SectorUpdate)
		{
			WorldState state;
			Sector sector;
			TestEntity ent1, ent2, ent3;
			sector.AdoptEntity(ent1);
			sector.AdoptEntity(ent2);
			sector.AdoptEntity(ent3);

			//make sure they are in there
			Assert::AreEqual(sector.Entities().Size(), 3_z);

			//make sure update is being called on all entities
			sector.Update(state);
			Assert::AreEqual(ent1.mUpdateCount, 1);
			Assert::AreEqual(ent2.mUpdateCount, 1);
			Assert::AreEqual(ent3.mUpdateCount, 1);

			//2x more for sanity
			sector.Update(state);
			sector.Update(state);
			Assert::AreEqual(ent1.mUpdateCount, 3);
			Assert::AreEqual(ent2.mUpdateCount, 3);
			Assert::AreEqual(ent3.mUpdateCount, 3);
		}

		TEST_METHOD(SectorClone)
		{
			Sector sector("test");
			Sector* clone = sector.Clone();
			Assert::AreNotSame(*clone, sector);
			Assert::AreEqual(*clone, sector);
			delete(clone);
		}

		/************************************************************************/
		/***************************World Functions******************************/
		/************************************************************************/
		TEST_METHOD(WorldCtor)
		{
			//default ctor
			{
				World world;
				Assert::IsTrue(world["this"] == &world);
				Assert::IsTrue(world["Name"] == "");
				Assert::AreEqual(world["Sectors"].Type(), DatumType::Table);
				Assert::AreEqual(world["Sectors"].Capacity(), 23_z);
			}

			//ctor with name (l value)
			{
				std::string name = "Hello";
				World world(name);
				Assert::IsTrue(world["this"] == &world);
				Assert::IsTrue(world["Name"] == name);
				Assert::AreEqual(world["Sectors"].Type(), DatumType::Table);
				Assert::AreEqual(world["Sectors"].Capacity(), 23_z);
			}

			//ctor with name (r value)
			{
				World world("Hello");
				Assert::IsTrue(world["this"] == &world);
				Assert::IsTrue(world["Name"] == "Hello");
				Assert::AreEqual(world["Sectors"].Type(), DatumType::Table);
				Assert::AreEqual(world["Sectors"].Capacity(), 23_z);
			}
		}

		TEST_METHOD(WorldName)
		{
			World world;
			Assert::AreEqual(world.Name(), ""s);
			Assert::IsTrue(world["Name"] == "");

			//r value
			world.SetName("Bob");
			Assert::AreEqual(world.Name(), "Bob"s);
			Assert::IsTrue(world["Name"] == "Bob");

			//l value
			std::string name = "not bob";
			world.SetName(name);
			Assert::AreEqual(world.Name(), name);
			Assert::IsTrue(world["Name"] == name);
		}

		TEST_METHOD(WorldSectors)
		{
			SectorFactory sectFactory;
			World world;
			Datum& sectDat = world.Sectors();
			Assert::AreEqual(sectDat.Type(), DatumType::Table);
			Assert::AreEqual(sectDat.Capacity(), 23_z);
			Assert::AreEqual(sectDat.Size(), 0_z);

			auto sector = world.CreateSector("Sector1");
			Assert::AreEqual(sectDat.Size(), 1_z);
			Assert::IsNotNull(sector);
			Assert::AreEqual(sector->Name(), "Sector1"s);
			Assert::AreEqual(sector->GetWorld(), &world);

			//adopt sect test
			Sector sect2;
			world.AdoptSector(sect2);
			Assert::AreEqual(sectDat.Size(), 2_z);
			Assert::AreEqual(sect2.GetWorld(), &world);
		}

		TEST_METHOD(WorldUpdate)
		{
			World world;
			Sector sector1, sector2, sector3;
			TestEntity ent1, ent2, ent3;
			sector1.AdoptEntity(ent1);
			sector2.AdoptEntity(ent2);
			sector3.AdoptEntity(ent3);
			world.AdoptSector(sector1);
			world.AdoptSector(sector2);
			world.AdoptSector(sector3);

			//make sure they are in there
			Assert::AreEqual(world.Sectors().Size(), 3_z);

			//make sure update is being called on all entities
			world.Update();
			Assert::AreEqual(ent1.mUpdateCount, 1);
			Assert::AreEqual(ent2.mUpdateCount, 1);
			Assert::AreEqual(ent3.mUpdateCount, 1);

			//2x more for sanity
			world.Update();
			world.Update();
			Assert::AreEqual(ent1.mUpdateCount, 3);
			Assert::AreEqual(ent2.mUpdateCount, 3);
			Assert::AreEqual(ent3.mUpdateCount, 3);
		}

		TEST_METHOD(WorldClockState) 
		{
			World world;
			auto& clock = world.GetGameClock();
			auto& state = world.GetWorldState();
			Assert::AreEqual(clock.CurrentTime(), state.GetGameTime()->CurrentTime());
		}

		TEST_METHOD(WorldActionCreateList)
		{
			TestActionFactory actionFactory;
			World world;
			Sector* sector = new Sector("Sector", &world);
			Entity* entity = new Entity("Entity", sector);

			//add 1
			Assert::AreEqual(0_z, entity->Actions().Size());
			world.AddActionToCreateQueue("TestAction", "Action1", entity, Entity::ACTIONS_STRING);
			Assert::AreEqual(0_z, entity->Actions().Size());
			world.Update();
			Assert::AreEqual(1_z, entity->Actions().Size());
			Scope& actionScope = entity->Actions().Get<Scope>();
			TestAction* action = actionScope.As<TestAction>();
			Assert::IsNotNull(action);
			Assert::AreEqual(1, action->mUpdateCount);

			//update again to make sure it doesn't add a second time
			world.Update();
			Assert::AreEqual(1_z, entity->Actions().Size());
			Assert::AreEqual(2, action->mUpdateCount);

			//Add two more for funsies
			world.AddActionToCreateQueue("TestAction", "Action2", entity, Entity::ACTIONS_STRING);
			world.AddActionToCreateQueue("TestAction", "Action3", entity, Entity::ACTIONS_STRING);
			Assert::AreEqual(1_z, entity->Actions().Size());
			world.Update();
			Assert::AreEqual(3_z, entity->Actions().Size());
			TestAction* action2 = entity->Actions().Get<Scope>(1).As<TestAction>(); //chain of death right here
			TestAction* action3 = entity->Actions().Get<Scope>(2).As<TestAction>(); //chain of death right here
			Assert::IsNotNull(action2);
			Assert::IsNotNull(action3);
			Assert::AreEqual(3, action->mUpdateCount);
			Assert::AreEqual(1, action2->mUpdateCount);
			Assert::AreEqual(1, action3->mUpdateCount);
			//check again that it wont add a second time
			world.Update();
			Assert::AreEqual(3_z, entity->Actions().Size());
			Assert::AreEqual(4, action->mUpdateCount);
			Assert::AreEqual(2, action2->mUpdateCount);
			Assert::AreEqual(2, action3->mUpdateCount);
		}

		TEST_METHOD(WorldActionDeleteList)
		{
			World world;
			Sector* sector = new Sector("Sector", &world);
			Entity* entity = new Entity("Entity", sector);

			TestAction* action = new TestAction("action", entity);
			TestAction* action2 = new TestAction("action2", entity);
			Assert::AreEqual(2_z, entity->Actions().Size());

			world.AddActionToDestroyQueue(*action);
			//won't remove until update
			Assert::AreEqual(2_z, entity->Actions().Size());
			world.Update();

			//ensure removed properly
			Assert::AreEqual(1_z, entity->Actions().Size());
			auto& actionScope = entity->Actions().Get<Scope>();
			Assert::AreEqual(action2, actionScope.As<TestAction>());

			world.AddActionToDestroyQueue(*action2);
			//won't remove until update
			Assert::AreEqual(1_z, entity->Actions().Size());
			world.Update();
			Assert::AreEqual(0_z, entity->Actions().Size());
		}

		/************************************************************************/
		/**************************Tests for all 3*******************************/
		/************************************************************************/
		TEST_METHOD(RTTI)
		{
			Entity entity;
			Sector sector;
			World world;

			//is (entity)
			Assert::IsTrue(entity.Is(Entity::TypeIdClass()));
			Assert::IsTrue(entity.Is(Attributed::TypeIdClass()));
			Assert::IsTrue(entity.Is(Scope::TypeIdClass()));
			Assert::IsFalse(entity.Is(Sector::TypeIdClass()));
			Assert::IsFalse(entity.Is(World::TypeIdClass()));

			//is (sector)
			Assert::IsTrue(sector.Is(Sector::TypeIdClass()));
			Assert::IsTrue(sector.Is(Attributed::TypeIdClass()));
			Assert::IsTrue(sector.Is(Scope::TypeIdClass()));
			Assert::IsFalse(sector.Is(Entity::TypeIdClass()));
			Assert::IsFalse(sector.Is(World::TypeIdClass()));

			//is (world)
			Assert::IsTrue(world.Is(World::TypeIdClass()));
			Assert::IsTrue(world.Is(Attributed::TypeIdClass()));
			Assert::IsTrue(world.Is(Scope::TypeIdClass()));
			Assert::IsFalse(world.Is(Entity::TypeIdClass()));
			Assert::IsFalse(world.Is(Sector::TypeIdClass()));

			//as (entity)
			Assert::IsNotNull(entity.As<Entity>());
			Assert::IsNotNull(entity.As<Attributed>());
			Assert::IsNotNull(entity.As<Scope>());
			Assert::IsNull(entity.As<Sector>());
			Assert::IsNull(entity.As<World>());

			//as (sector)
			Assert::IsNotNull(sector.As<Sector>());
			Assert::IsNotNull(sector.As<Attributed>());
			Assert::IsNotNull(sector.As<Scope>());
			Assert::IsNull(sector.As<Entity>());
			Assert::IsNull(sector.As<World>());

			//as (world)
			Assert::IsNotNull(world.As<World>());
			Assert::IsNotNull(world.As<Attributed>());
			Assert::IsNotNull(world.As<Scope>());
			Assert::IsNull(world.As<Sector>());
			Assert::IsNull(world.As<Entity>());
		}

		/************************************************************************/
		/**************************Parsing Tests*********************************/
		/************************************************************************/
		TEST_METHOD(ParseEntity)
		{
			EntityFactory entFact;
			Scope scope;
			TableSharedData sharedData(scope);
			JsonParseMaster master(sharedData);
			JsonTableParseHelper helper;
			master.AddHelper(helper);
			master.Initialize();

			const std::string entityJson =
				R"({
					 "Entity": {
					    "type": "table",
					    "class": "Entity",
                        "value": {
					   	   "Name": {
                             "type": "string",
                             "value": "Entity1"
                           }
                        }
                     },
					 "Entity2": {
					    "type": "table",
					    "class": "Entity",
                        "value": {
					   	   "Name": {
                             "type": "string",
                             "value": "Entity2"
                           }
                        }
                     }
				   })";

			Assert::IsTrue(master.Parse(entityJson));
			Assert::AreEqual(scope.Size(), 2_z);

			auto entityDat = scope["Entity"];
			Assert::AreEqual(entityDat.Size(), 1_z);
			Scope& entity = entityDat.Get<Scope>();
			Assert::IsTrue(entity.Is(Entity::TypeIdClass()));
			Assert::IsTrue(entity["Name"] == "Entity1");
			Assert::AreEqual(entity.As<Entity>()->Name(), "Entity1"s); //sanity

			auto entityDat2 = scope["Entity2"];
			Assert::AreEqual(entityDat2.Size(), 1_z);
			Scope& entity2 = entityDat2.Get<Scope>();
			Assert::IsTrue(entity2.Is(Entity::TypeIdClass()));
			Assert::IsTrue(entity2["Name"] == "Entity2");
		}

		TEST_METHOD(ParseSector)
		{
			SectorFactory sectFact;
			EntityFactory entFact;
			Scope scope;
			TableSharedData sharedData(scope);
			JsonParseMaster master(sharedData);
			JsonTableParseHelper helper;
			master.AddHelper(helper);
			master.Initialize();

			Assert::IsTrue(master.ParseFromFile("Content\\Sector.json"));
			Assert::AreEqual(scope.Size(), 1_z);

			//check sector itself
			auto sectorDat = scope["Sector"];
			Assert::AreEqual(sectorDat.Size(), 1_z);
			Scope& sector = sectorDat.Get<Scope>();
			Assert::IsTrue(sector.Is(Sector::TypeIdClass()));
			Assert::IsTrue(sector["Name"] == "Sector1");
			Assert::AreEqual(sector.As<Sector>()->Name(), "Sector1"s); //sanity
			auto& entDat = sector.As<Sector>()->Entities();
			Assert::AreSame(entDat, sector["Entities"]);

			//double check entities
			Assert::AreEqual(entDat.Size(), 2_z);
			Scope& entity1 = entDat.Get<Scope>();
			Assert::IsTrue(entity1.Is(Entity::TypeIdClass()));
			Assert::IsTrue(entity1["Name"] == "Entity1");

			Scope& entity2 = entDat.Get<Scope>(1);
			Assert::IsTrue(entity2.Is(Entity::TypeIdClass()));
			Assert::IsTrue(entity2["Name"] == "Entity2");
		}

		TEST_METHOD(ParseWorld)
		{
			SectorFactory sectFact;
			EntityFactory entFact;
			TestEntityFactory testEntFact;
			World world;
			TableSharedData sharedData(world);
			JsonParseMaster master(sharedData);
			JsonTableParseHelper helper;
			master.AddHelper(helper);
			master.Initialize();

			Assert::IsTrue(master.ParseFromFile("Content\\World.json"));
			Assert::AreEqual(world.Size(), 3_z); //3 because of "this"

			//get sectors 
			auto sectorDat = world["Sectors"];
			Assert::AreEqual(sectorDat.Size(), 2_z);
			Sector& sector1 = *sectorDat.Get<Scope>().As<Sector>();
			Sector& sector2 = *sectorDat.Get<Scope>(1).As<Sector>();
			Assert::AreNotSame(sector1, sector2);
			
			//sector 1
			Assert::IsTrue(sector1["Name"] == "Sector1");
			auto& entDat1 = sector1.Entities();
			Assert::AreSame(entDat1, sector1["Entities"]);
			Assert::AreEqual(entDat1.Size(), 2_z);
			//entities in sector1
			Scope& entity1 = entDat1.Get<Scope>();
			Assert::IsTrue(entity1.Is(Entity::TypeIdClass()));
			Assert::IsTrue(entity1["Name"] == "Entity1");
			Scope& entity2 = entDat1.Get<Scope>(1);
			Assert::IsTrue(entity2.Is(Entity::TypeIdClass()));
			Assert::IsTrue(entity2["Name"] == "Entity2");

			//sector 2
			Assert::IsTrue(sector2["Name"] == "Sector2");
			auto& entDat2 = sector2.Entities();
			Assert::AreSame(entDat2, sector2["Entities"]);
			Assert::AreEqual(entDat2.Size(), 2_z);
			//entities in sector2
			Scope& testEntity = entDat2.Get<Scope>();
			Assert::IsTrue(testEntity.Is(TestEntity::TypeIdClass()));
			Assert::IsTrue(testEntity["Name"] == "TestEntity");
			Scope& entity3 = entDat2.Get<Scope>(1);
			Assert::IsTrue(entity3.Is(Entity::TypeIdClass()));
			Assert::IsTrue(entity3["Name"] == "Entity3");

			//final check: do update and check test entity updated
			world.Update();
			Assert::AreEqual(testEntity.As<TestEntity>()->mUpdateCount, 1);
		}


	private:
		static _CrtMemState sStartMemState;	//for memory leak detection
	};
	_CrtMemState EntitySectorWorldTests::sStartMemState;
}