#include "pch.h"
#include "CppUnitTest.h"
#include "TypeRegistry.h"
#include "EventMessageAttributed.h"
#include "World.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;
//using namespace UnitTests;
using namespace std;
using namespace std::string_literals;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<World>(World* t)
	{
		RETURN_WIDE_STRING(t);
	}
	template<>
	inline std::wstring ToString<World>(const World* t)
	{
		RETURN_WIDE_STRING(t);
	}
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(EventMessageAttributedTests)
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
			TypeRegistry::RegisterType(EventMessageAttributed::TypeIdClass(), EventMessageAttributed::Signatures());
			TypeRegistry::RegisterType(World::TypeIdClass(), World::Signatures());
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

		TEST_METHOD(RTTI_Tests)
		{
			EventMessageAttributed eventMsg;

			//is
			Assert::IsTrue(eventMsg.Is(eventMsg.TypeIdInstance()));
			Assert::IsTrue(eventMsg.Is(EventMessageAttributed::TypeIdClass()));
			Assert::IsTrue(eventMsg.Is(Attributed::TypeIdClass()));
			Assert::IsTrue(eventMsg.Is(Scope::TypeIdClass()));
			Assert::IsFalse(eventMsg.Is(World::TypeIdClass()));

			//as
			Assert::IsNotNull(eventMsg.As<EventMessageAttributed>());
			Assert::IsNotNull(eventMsg.As<Attributed>());
			Assert::IsNotNull(eventMsg.As<Scope>());
			Assert::IsNull(eventMsg.As<World>());
		}

		TEST_METHOD(Ctors)
		{
			//default ctor
			{
				EventMessageAttributed eventMsg;
				Assert::IsTrue(eventMsg["Subtype"] == ""s);
				Assert::AreEqual(1_z, eventMsg["Subtype"].Size());
				Assert::AreEqual(1_z, eventMsg["Subtype"].Capacity());
				Assert::IsNull(eventMsg.GetWorld());
			}
			
			//ctor with name, world ptr
			{
				World world;
				EventMessageAttributed eventMsg("A subtype", &world);
				Assert::IsTrue(eventMsg["Subtype"] == "A subtype"s);
				Assert::AreEqual(1_z, eventMsg["Subtype"].Size());
				Assert::AreEqual(1_z, eventMsg["Subtype"].Capacity());
				Assert::AreEqual(&world, eventMsg.GetWorld());
			}
		}

		TEST_METHOD(GetSetSubtype)
		{
			EventMessageAttributed eventMsg;
			Assert::AreEqual(""s, eventMsg.GetSubtype());
			eventMsg.SetSubtype("A subtype");
			Assert::AreEqual("A subtype"s, eventMsg.GetSubtype());
		}

		TEST_METHOD(GetSetWorld)
		{
			World world;
			EventMessageAttributed eventMsg;
			Assert::IsNull(eventMsg.GetWorld());
			eventMsg.SetWorld(world);
			Assert::AreEqual(&world, eventMsg.GetWorld());

			//const check
			const EventMessageAttributed& constEventMsg = eventMsg;
			Assert::AreEqual(&const_cast<const World&>(world), constEventMsg.GetWorld());
		}

		TEST_METHOD(Clone)
		{
			World world;
			EventMessageAttributed eventMsg("Subtype", &world);
			Assert::AreEqual("Subtype"s, eventMsg.GetSubtype());
			Assert::AreEqual(&world, eventMsg.GetWorld());

			Scope* eventMsgCloneScope = eventMsg.Clone();
			Assert::IsNotNull(eventMsgCloneScope);
			Assert::IsTrue(eventMsgCloneScope->Is(EventMessageAttributed::TypeIdClass()));
			Assert::AreEqual(eventMsg.GetSubtype(), eventMsgCloneScope->As<EventMessageAttributed>()->GetSubtype());
			Assert::AreEqual(eventMsg.GetWorld(), eventMsgCloneScope->As<EventMessageAttributed>()->GetWorld());
			delete(eventMsgCloneScope);
		}

	private:
		static _CrtMemState sStartMemState;	//for memory leak detection
	};
	_CrtMemState EventMessageAttributedTests::sStartMemState;
}