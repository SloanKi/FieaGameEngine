#include "pch.h"
#include "CppUnitTest.h"
#include "Foo.h"
#include "AttributedFoo.h"
#include "TypeRegistry.h"
#include "Attributed.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;
using namespace UnitTests;
using namespace std;
using namespace std::string_literals;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<glm::vec4>(const glm::vec4& t)
	{
		RETURN_WIDE_STRING(&t);
	}

	template<>
	inline std::wstring ToString<glm::mat4>(const glm::mat4& t)
	{
		RETURN_WIDE_STRING(&t);
	}

	template<>
	inline std::wstring ToString<AttributedFoo>(const AttributedFoo& t)
	{
		RETURN_WIDE_STRING(&t);
	}

	template<>
	inline std::wstring ToString<Vector<std::pair<const std::string, Datum>*>>(const Vector<std::pair<const std::string, Datum>*>& t)
	{
		RETURN_WIDE_STRING(&t);
	}


}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(AttributedTests)
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

		TEST_METHOD(RTTI)
		{
			Scope scope;
			Foo foo;
			AttributedFoo attrFoo, attrFoo2;

			//equals
			Assert::IsTrue(attrFoo.Equals(&attrFoo2));
			Assert::IsFalse(attrFoo.Equals(nullptr));
			Assert::IsFalse(attrFoo.Equals(&foo));
			Assert::IsFalse(attrFoo.Equals(&scope));

			//Is
			Assert::IsTrue(attrFoo.Is(attrFoo.TypeIdClass()));
			Assert::IsTrue(attrFoo.Is("AttributedFoo"));
			Assert::IsTrue(attrFoo.Is(scope.TypeIdClass()));
			Assert::IsTrue(attrFoo.Is("Scope"));
			Assert::IsFalse(attrFoo.Is(foo.TypeIdClass()));
			Assert::IsFalse(attrFoo.Is("Foo"));
			
		}

		TEST_METHOD(CtorAndPopulate)
		{
			
			AttributedFoo foo;
			Assert::IsTrue(foo["this"] == &foo);
			Assert::IsTrue(foo["ExternalInteger"] == foo.ExternalInteger);

			//sanity check that datum is external and pointing to member variable
			foo.ExternalInteger = 22;
			Assert::IsTrue(foo["ExternalInteger"] == foo.ExternalInteger);
			
			foo.ExternalFloat = 7.231f;
			Assert::IsTrue(foo["ExternalFloat"] == foo.ExternalFloat);

			foo.ExternalVector = glm::vec4(2.9102f);
			Assert::IsTrue(foo["ExternalVector"] == foo.ExternalVector);

			foo.ExternalMatrix = glm::mat4(7.680f);
			Assert::IsTrue(foo["ExternalMatrix"] == foo.ExternalMatrix);

			foo.ExternalString = "Woof";
			Assert::IsTrue(foo["ExternalString"] == foo.ExternalString);

			Foo newRTTI(78);
			foo.ExternalRTTI = &newRTTI;
			Assert::IsTrue(foo["ExternalRTTI"] == foo.ExternalRTTI);

			//internal scope
			bool entryCreated;
			Assert::IsTrue(foo["InternalScope"].IsEmpty());
			Assert::AreEqual(foo["InternalScope"].Capacity(), 1_z);
			foo.AppendScope("InternalScope", entryCreated);
			Assert::IsFalse(entryCreated);
			

			//check arrays match
			Datum dat = foo["ExternalIntArr"];
			Assert::AreEqual(dat.Size(), 5_z);
			Assert::AreEqual(dat.Get<int>(), foo.ExternalIntArr[0]);
			Assert::AreEqual(dat.Get<int>(4), foo.ExternalIntArr[4]);

			dat = foo["ExternalFloatArr"];
			Assert::AreEqual(dat.Size(), 5_z);
			Assert::AreEqual(dat.Get<float>(), foo.ExternalFloatArr[0]);
			Assert::AreEqual(dat.Get<float>(4), foo.ExternalFloatArr[4]);

			dat = foo["ExternalVecArr"];
			Assert::AreEqual(dat.Size(), 5_z);
			Assert::AreEqual(dat.Get<glm::vec4>(), foo.ExternalVecArr[0]);
			Assert::AreEqual(dat.Get<glm::vec4>(4), foo.ExternalVecArr[4]);

			dat = foo["ExternalMatArr"];
			Assert::AreEqual(dat.Size(), 5_z);
			Assert::AreEqual(dat.Get<glm::mat4>(), foo.ExternalMatArr[0]);
			Assert::AreEqual(dat.Get<glm::mat4>(4), foo.ExternalMatArr[4]);

			dat = foo["ExternalStringArr"];
			Assert::AreEqual(dat.Size(), 5_z);
			Assert::AreEqual(dat.Get<std::string>(), foo.ExternalStringArr[0]);
			Assert::AreEqual(dat.Get<std::string>(4), foo.ExternalStringArr[4]);


			//internal scope arr
			Assert::IsTrue(foo["InternalScopeArr"].IsEmpty());
			Assert::AreEqual(foo["InternalScopeArr"].Capacity(), 5_z);
			foo.AppendScope("InternalScopeArr", entryCreated);
			Assert::IsFalse(entryCreated);
		}

		TEST_METHOD(CopySemantics)
		{
			//copy constructor
			AttributedFoo foo;
			foo.ExternalInteger = 55;

			AttributedFoo copyFoo = foo;
			Assert::AreEqual(copyFoo, foo);

			//make sure the external storage was properly set
			foo.ExternalFloat = 3.2f;
			foo.ExternalString = "Howdy"s;
			Assert::AreNotEqual(copyFoo, foo);

			//copy assign
			copyFoo = foo;
			Assert::AreEqual(copyFoo, foo);
		}

		TEST_METHOD(MoveSemantics)
		{
			//move constructor
			AttributedFoo foo;
			foo.ExternalInteger = 55;
			AttributedFoo copyFoo = foo;
			AttributedFoo moveFoo = std::move(copyFoo);
			Assert::AreEqual(moveFoo, foo);
			moveFoo.ExternalFloat = 2.0f;
			Assert::IsTrue(moveFoo["ExternalFloat"] == moveFoo.ExternalFloat);

			//move assign
			AttributedFoo copyFoo2 = foo;
			moveFoo = std::move(copyFoo2);
			Assert::AreEqual(moveFoo, foo);
			moveFoo.ExternalFloat = 2.0f;
			Assert::IsTrue(moveFoo["ExternalFloat"] == moveFoo.ExternalFloat);
		}

		TEST_METHOD(IsAttribute)
		{
			AttributedFoo foo;
			Assert::IsTrue(foo.IsAttribute("ExternalInteger"));
			Assert::IsTrue(foo.IsAttribute("ExternalFloat"));
			Assert::IsTrue(foo.IsAttribute("ExternalVector"));
			Assert::IsTrue(foo.IsAttribute("ExternalMatrix"));
			Assert::IsTrue(foo.IsAttribute("ExternalString"));
			Assert::IsTrue(foo.IsAttribute("ExternalRTTI"));
			Assert::IsTrue(foo.IsAttribute("InternalScope"));
			Assert::IsTrue(foo.IsAttribute("ExternalIntArr"));
			Assert::IsTrue(foo.IsAttribute("ExternalFloatArr"));
			Assert::IsTrue(foo.IsAttribute("ExternalVecArr"));
			Assert::IsTrue(foo.IsAttribute("ExternalMatArr"));
			Assert::IsTrue(foo.IsAttribute("ExternalStringArr"));
			Assert::IsTrue(foo.IsAttribute("InternalScopeArr"));

			Assert::IsFalse(foo.IsAttribute("Meow"));
		}

		TEST_METHOD(IsPrescribedAuxillary)
		{
			AttributedFoo foo;
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalInteger"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalFloat"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalVector"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalMatrix"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalString"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalRTTI"));
			Assert::IsTrue(foo.IsPrescribedAttribute("InternalScope"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalIntArr"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalFloatArr"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalVecArr"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalMatArr"));
			Assert::IsTrue(foo.IsPrescribedAttribute("ExternalStringArr"));
			Assert::IsTrue(foo.IsPrescribedAttribute("InternalScopeArr"));
			Assert::IsFalse(foo.IsPrescribedAttribute("Meow"));

			foo.AppendAuxiliaryAttribute("Health");
			Assert::IsTrue(foo.IsAuxiliaryAttribute("Health"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("ExternalInteger"));
			Assert::IsFalse(foo.IsAuxiliaryAttribute("Meow"));
		}

		TEST_METHOD(AppendAuxiliary)
		{
			//test 1: valid append (create new item)
			bool entryCreated;
			AttributedFoo foo;
			foo.AppendAuxiliaryAttribute("Health", entryCreated) = 100;
			Assert::IsTrue(entryCreated);
			Assert::IsTrue(foo["Health"] == 100);

			//test 2: valid append (edit existing)
			foo.AppendAuxiliaryAttribute("Health", entryCreated) = 50;
			Assert::IsFalse(entryCreated);
			Assert::IsTrue(foo["Health"] == 50);
			foo.AppendAuxiliaryAttribute("Health") = 25;
			Assert::IsTrue(foo["Health"] == 25);

			//test 3: append a prescribed attribute (throw exception)
			Assert::ExpectException<std::runtime_error>([&foo] { foo.AppendAuxiliaryAttribute("ExternalInteger"); });
			Assert::ExpectException<std::runtime_error>([&foo, &entryCreated] { foo.AppendAuxiliaryAttribute("ExternalInteger", entryCreated); });
		}

		TEST_METHOD(AttributesPrescribedAuxillary)
		{
			//Attributes
			AttributedFoo foo;
			auto attributes = foo.Attributes();
			Assert::AreEqual(attributes.Size(), foo.Size());
			Assert::AreEqual(attributes[0]->first, "this"s);
			Assert::IsTrue(attributes[0]->second == &foo);
			Assert::AreEqual(attributes[1]->first, "ExternalInteger"s);
			Assert::IsTrue(attributes[1]->second == foo[1]);

			foo.AppendAuxiliaryAttribute("Health");
			foo["Health"] = 50;
			foo.AppendAuxiliaryAttribute("Name");
			foo["Name"] = "Bob";

			//Prescribed Attributes
			auto prescribedAttr = foo.PrescribedAttributes();
			Assert::AreEqual(prescribedAttr.Size(), foo.Size() - 2);
			Assert::AreEqual(prescribedAttr, attributes);

			//sanity check on normal attributes
			attributes = foo.Attributes();
			Assert::AreEqual(attributes.Size(), foo.Size());
			Assert::IsTrue(attributes[attributes.Size() - 1]->second == "Bob"s);


			//Auxiliary Attributes
			auto auxiliaryAttr = foo.AuxiliaryAttributes();
			Assert::AreEqual(auxiliaryAttr.Size(), 2_z);
			Assert::AreEqual(auxiliaryAttr[0]->first, "Health"s);
			Assert::IsTrue(auxiliaryAttr[0]->second == 50);
			Assert::AreEqual(auxiliaryAttr[1]->first, "Name"s);
			Assert::IsTrue(auxiliaryAttr[1]->second == "Bob"s);

			//aux but empty
			AttributedFoo foo2;
			Assert::IsTrue(foo2.AuxiliaryAttributes().IsEmpty());
		}

	private:
		static _CrtMemState sStartMemState;	//for memory leak detection
	};
	_CrtMemState AttributedTests::sStartMemState;
}