#include "pch.h"
#include "CppUnitTest.h"
#include "FooFactory.h"
#include "Foo.h"
#include "AttributedFoo.h"
#include "Scope.h"
#include "TypeRegistry.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;
using namespace UnitTests;
using namespace std;
using namespace std::string_literals;



ConcreteFactory(AttributedFoo, Scope)



namespace UnitTestLibraryDesktop
{

	TEST_CLASS(FactoryTests)
	{
	public:
		//check for memory leaks
		TEST_METHOD_INITIALIZE(Initialize)
		{

#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
#endif
			//FooFactory* factory = new FooFactory(); (THIS WORKS BUT THERE ARE LEAKS BECAUSE IT IS NOT LATER FREED)
			//FooFactory factory; (THIS DOES NOT WORK BECAUSE IT GOES OUT OF SCOPE AND REMOVES ITSELF)
			//factory; //get rid of warnings
			TypeRegistry::Create();
			TypeRegistry::RegisterType(AttributedFoo::TypeIdClass(), AttributedFoo::Signatures());
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

		TEST_METHOD(FooFactoryTest)
		{
			FooFactory factory;
			Assert::AreEqual(Factory<RTTI>::Size(), 1_z);

			//find test
			auto fooFactory = Factory<RTTI>::Find("Foo");
			Assert::IsNotNull(fooFactory);

			//create test
			auto newFoo = Factory<RTTI>::Create("Foo");
			Assert::IsNotNull(newFoo);
			Assert::IsTrue(newFoo->Is(Foo::TypeIdClass()));
			delete(newFoo);

			//name test
			Assert::AreEqual(factory.ClassName(), "Foo"s);

			//clear test
			Factory<RTTI>::Clear();
			Assert::AreEqual(Factory<RTTI>::Size(), 0_z);
		}

		TEST_METHOD(MacroFactories)
		{
			FooFactory fooFactory;
			AttributedFooFactory attrFooFactory;

			Assert::AreEqual(Factory<RTTI>::Size(), 1_z);
			Assert::AreEqual(Factory<Scope>::Size(), 1_z);

			Assert::AreEqual(attrFooFactory.ClassName(), "AttributedFoo"s);

			//find test
			auto fooFacPtr = Factory<RTTI>::Find("Foo");
			Assert::IsNotNull(fooFacPtr);
			auto attrFooFacPtr = Factory<Scope>::Find(attrFooFactory.ClassName());
			Assert::IsNotNull(attrFooFacPtr);

			//create test
			auto newFoo = Factory<RTTI>::Create("Foo");
			Assert::IsNotNull(newFoo);
			Assert::IsTrue(newFoo->Is(Foo::TypeIdClass()));
			Assert::IsFalse(newFoo->Is(AttributedFoo::TypeIdClass()));
			delete(newFoo);

			auto newAttrFoo = Factory<Scope>::Create(attrFooFactory.ClassName());
			Assert::IsNotNull(newAttrFoo);
			Assert::IsTrue(newAttrFoo->Is(AttributedFoo::TypeIdClass()));
			Assert::IsTrue(newAttrFoo->Is(Attributed::TypeIdClass()));
			Assert::IsTrue(newAttrFoo->Is(Scope::TypeIdClass()));
			Assert::IsFalse(newAttrFoo->Is(Foo::TypeIdClass()));
			delete(newAttrFoo);

			//clear test
			Factory<RTTI>::Clear();
			Assert::AreEqual(Factory<RTTI>::Size(), 0_z);
			Factory<Scope>::Clear();
			Assert::AreEqual(Factory<Scope>::Size(), 0_z);
		}

	private:
		static _CrtMemState sStartMemState;	//for memory leak detection
	};
	_CrtMemState FactoryTests::sStartMemState;
}