#include "pch.h"
#include "CppUnitTest.h"
#include "TypeRegistry.h"
#include "World.h"
#include "Sector.h"
#include "Entity.h"
#include "TestAction.h"
#include "ActionIncrement.h"
#include "ActionCreateAction.h"
#include "ActionDestroyAction.h"
#include "ActionListSwitch.h"
#include "JsonTableParseHelper.h"
#include "JsonParseMaster.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;
using namespace UnitTests;
using namespace std;
using namespace std::string_literals;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
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
	inline std::wstring ToString<TestAction>(const TestAction& t)
	{
		RETURN_WIDE_STRING(&t);
	}

	template<>
	inline std::wstring ToString<ActionList>(const ActionList& t)
	{
		RETURN_WIDE_STRING(&t);
	}

	template<>
	inline std::wstring ToString<ActionList>(ActionList* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<TestAction>(TestAction* t)
	{
		RETURN_WIDE_STRING(t);
	}
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(ActionParsingTests)
	{
	public:
		TEST_METHOD_INITIALIZE(Initialize)
		{

#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
#endif
			TypeRegistry::Create();
			TypeRegistry::RegisterType(Entity::TypeIdClass(), Entity::Signatures());
			TypeRegistry::RegisterType(ActionList::TypeIdClass(), ActionList::Signatures());
			TypeRegistry::RegisterType(TestAction::TypeIdClass(), TestAction::Signatures());
			TypeRegistry::RegisterType(ActionIncrement::TypeIdClass(), ActionIncrement::Signatures());
			TypeRegistry::RegisterType(ActionCreateAction::TypeIdClass(), ActionCreateAction::Signatures());
			TypeRegistry::RegisterType(ActionDestroyAction::TypeIdClass(), ActionDestroyAction::Signatures());
			TypeRegistry::RegisterType(Sector::TypeIdClass(), Sector::Signatures());
			TypeRegistry::RegisterType(World::TypeIdClass(), World::Signatures());
			TypeRegistry::RegisterType(ActionListSwitch::TypeIdClass(), ActionListSwitch::Signatures());
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

		TEST_METHOD(ParseTestAction)
		{
			SectorFactory sectFact;
			EntityFactory entFact;
			TestActionFactory testActionFact;
			World world;
			TableSharedData sharedData(world);
			JsonParseMaster master(sharedData);
			JsonTableParseHelper helper;
			master.AddHelper(helper);
			master.Initialize();

			//check world
			Assert::IsTrue(master.ParseFromFile("Content\\TestAction.json"));
			Assert::AreEqual(3_z, world.Size()); //3 because of "this"

			//check Sector
			Assert::AreEqual(1_z, world.Sectors().Size());
			Sector& sector = *world.Sectors().Get<Scope>().As<Sector>(); //this can go so wrong
			Assert::IsTrue(sector["Name"] == "Sector1");

			//check Entity
			Assert::AreEqual(1_z, sector.Entities().Size());
			Entity& entity = *sector.Entities().Get<Scope>().As<Entity>();
			Assert::IsTrue(entity["Name"] == "Entity1");

			//check actions
			Assert::AreEqual(2_z, entity.Actions().Size());
			TestAction* action1 = entity.Actions().Get<Scope>().As<TestAction>();
			Assert::IsNotNull(action1);
			Assert::IsTrue(action1->Append("Name") == "testAction1");
			Assert::AreEqual(0, action1->mUpdateCount);
			
			TestAction* action2 = entity.Actions().Get<Scope>(1).As<TestAction>();
			Assert::IsNotNull(action2);
			Assert::IsTrue(action2->Append("Name") == "testAction2");
			Assert::AreEqual(0, action2->mUpdateCount);


			//Check that update updates actions (sanity)
			world.Update();
			world.Update();
			Assert::AreEqual(2, action1->mUpdateCount);
			Assert::AreEqual(2, action2->mUpdateCount);

			//double check parent stuff (sanity)
			Assert::AreEqual(&entity, action1->GetEntity());
			Assert::AreEqual(&entity, action2->GetEntity());
			Assert::IsNull(action1->GetActionList());
			Assert::IsNull(action2->GetActionList());
		}

		TEST_METHOD(ParseActionList)
		{
			SectorFactory sectFact;
			EntityFactory entFact;
			TestActionFactory testActionFact;
			ActionListFactory actionListFact;
			World world;
			TableSharedData sharedData(world);
			JsonParseMaster master(sharedData);
			JsonTableParseHelper helper;
			master.AddHelper(helper);
			master.Initialize();

			//check world
			Assert::IsTrue(master.ParseFromFile("Content\\ActionList.json"));
			Assert::AreEqual(3_z, world.Size()); //3 because of "this"

			//check Sector
			Assert::AreEqual(1_z, world.Sectors().Size());
			Sector& sector = *world.Sectors().Get<Scope>().As<Sector>(); //this can go so wrong
			Assert::IsTrue(sector["Name"] == "Sector1");

			//check Entity
			Assert::AreEqual(1_z, sector.Entities().Size());
			Entity& entity = *sector.Entities().Get<Scope>().As<Entity>();
			Assert::IsTrue(entity["Name"] == "Entity1");

			//check action list
			Assert::AreEqual(1_z, entity.Actions().Size());
			ActionList* actionList = entity.Actions().Get<Scope>().As<ActionList>();
			Assert::IsNotNull(actionList);
			Assert::IsTrue(actionList->Append("Name") == "ActionList");

			//check actions
			Assert::AreEqual(2_z, actionList->Actions().Size());
			TestAction* action1 = actionList->Actions().Get<Scope>().As<TestAction>();
			Assert::IsNotNull(action1);
			Assert::IsTrue(action1->Append("Name") == "testAction1");
			Assert::AreEqual(0, action1->mUpdateCount);

			TestAction* action2 = actionList->Actions().Get<Scope>(1).As<TestAction>();
			Assert::IsNotNull(action2);
			Assert::IsTrue(action2->Append("Name") == "testAction2");
			Assert::AreEqual(0, action2->mUpdateCount);


			//Check that update updates actions (sanity)
			world.Update();
			world.Update();
			Assert::AreEqual(2, action1->mUpdateCount);
			Assert::AreEqual(2, action2->mUpdateCount);

			//double check parent stuff (sanity)
			Assert::AreEqual(&entity, action1->GetEntity());
			Assert::AreEqual(&entity, action2->GetEntity());
			Assert::AreEqual(actionList, action1->GetActionList());
			Assert::AreEqual(actionList, action2->GetActionList());
		}

		TEST_METHOD(ParseActionListSwitch)
		{
			SectorFactory sectFact;
			EntityFactory entFact;
			TestActionFactory testActionFact;
			ActionListFactory actionListFact;
			ActionListSwitchFactory aciontListSwitchFact;
			World world;
			TableSharedData sharedData(world);
			JsonParseMaster master(sharedData);
			JsonTableParseHelper helper;
			master.AddHelper(helper);
			master.Initialize();

			//check world
			Assert::IsTrue(master.ParseFromFile("Content\\ActionListSwitch.json"));
			Assert::AreEqual(3_z, world.Size()); //3 because of "this"

			//check Sector
			Assert::AreEqual(1_z, world.Sectors().Size());
			Sector& sector = *world.Sectors().Get<Scope>().As<Sector>(); //this can go so wrong
			Assert::IsTrue(sector["Name"] == "Sector1");

			//check Entity
			Assert::AreEqual(1_z, sector.Entities().Size());
			Entity& entity = *sector.Entities().Get<Scope>().As<Entity>();
			Assert::IsTrue(entity["Name"] == "Entity1");

			//check action list
			Assert::AreEqual(1_z, entity.Actions().Size());
			ActionListSwitch* actionList = entity.Actions().Get<Scope>().As<ActionListSwitch>();
			Assert::IsNotNull(actionList);
			Assert::AreEqual("ActionListSwitch"s, actionList->Name());
			Assert::AreEqual("testAction2"s, actionList->GetValue());

			//check actions
			Assert::AreEqual(3_z, actionList->Actions().Size());
			TestAction* action1 = actionList->Actions().Get<Scope>().As<TestAction>();
			Assert::IsNotNull(action1);
			Assert::IsTrue(action1->Append("Name") == "testAction1");
			Assert::AreEqual(0, action1->mUpdateCount);

			TestAction* action2 = actionList->Actions().Get<Scope>(1).As<TestAction>();
			Assert::IsNotNull(action2);
			Assert::IsTrue(action2->Append("Name") == "testAction2");
			Assert::AreEqual(0, action2->mUpdateCount);

			TestAction* action3 = actionList->Actions().Get<Scope>(2).As<TestAction>();
			Assert::IsNotNull(action3);
			Assert::IsTrue(action3->Append("Name") == "testAction3");
			Assert::AreEqual(0, action3->mUpdateCount);

			//check that switch works
			world.Update();
			world.Update();
			Assert::AreEqual(0, action1->mUpdateCount);
			Assert::AreEqual(2, action2->mUpdateCount);
			Assert::AreEqual(0, action3->mUpdateCount);
		}

	private:
		static _CrtMemState sStartMemState;	//for memory leak detection
	};
	_CrtMemState ActionParsingTests::sStartMemState;
}