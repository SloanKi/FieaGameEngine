#include "pch.h"
#include "CppUnitTest.h"
#include "Scope.h"
#include "Foo.h"
#include "Datum.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;
using namespace UnitTests;
using namespace std;
using namespace std::string_literals;


namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<Library::Datum>(const Library::Datum& t)
	{
		RETURN_WIDE_STRING(&t);
	}

	template<>
	inline std::wstring ToString<Library::Datum>(Library::Datum* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Library::Datum>(const Library::Datum* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Library::Scope>(const Library::Scope& t)
	{
		RETURN_WIDE_STRING(&t);
	}

	template<>
	inline std::wstring ToString<Library::Scope>(const Library::Scope* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Library::Scope>(Library::Scope* t)
	{
		RETURN_WIDE_STRING(t);
	}
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(ScopeTests)
	{
	public:
		//check for memory leaks
		TEST_METHOD_INITIALIZE(Initialize)
		{
#if defined(DEBUG) || defined(_DEBUG)
			_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF);
			_CrtMemCheckpoint(&sStartMemState);
#endif
		}

		//check for memory leaks
		TEST_METHOD_CLEANUP(Cleanup)
		{
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
			Scope scope, scope2;
			Foo foo1, foo2;

			//equals
			Assert::IsTrue(scope.Equals(&scope2));
			Assert::IsFalse(scope.Equals(nullptr));
			Assert::IsFalse(scope.Equals(&foo1));
			Assert::IsFalse(foo2.Equals(&scope));

			//Is
			Assert::IsTrue(scope.Is(scope.TypeIdClass()));
			Assert::IsTrue(scope.Is("Scope"));
			Assert::IsFalse(scope.Is(Foo::TypeIdClass()));
			Assert::IsFalse(scope.Is("Foo"));
		}

		TEST_METHOD(Constructor)
		{
			//test 1: no defined capacity (will get default)
			Scope scope;
			Assert::AreEqual(scope.Size(), 0_z);
			Assert::AreEqual(scope.Capacity(), Scope::DEFAULT_CAPACITY);
			Assert::IsTrue(scope.GetParent() == nullptr);
			
			//test 2: defined capacity
			Scope scope2(20);
			Assert::AreEqual(scope2.Size(), 0_z);
			Assert::AreEqual(scope2.Capacity(), 20_z);
			Assert::IsTrue(scope.GetParent() == nullptr);

			//test 3: 0 capacity (throw exception)
			//Assert::ExpectException<std::runtime_error>([] {Scope scope3(0); });
		}

		TEST_METHOD(DestructorEdge)
		{
			//test to see if calling delete on a child will safely remove it from list
			Scope scope;
			Scope* child = new Scope();
			scope.Adopt(*child, "Powers");
			delete(child);
			Assert::IsNull(scope.FindContainedScope(*child).first);
			Assert::IsTrue(scope["Powers"].IsEmpty());
		}

		TEST_METHOD(isEmpty)
		{
			//test 1: empty
			Scope scope;
			Assert::IsTrue(scope.IsEmpty());

			//test 2: not empty
			bool entryCreated;
			scope.Append("Health", entryCreated);
			Assert::IsFalse(scope.IsEmpty());
		}

		TEST_METHOD(CopySemantics)
		{
			//test 1: copy default/empty scope
			Scope scope;
			Scope scopeCopy = scope;
			Assert::AreEqual(scope, scopeCopy);
			Assert::AreEqual(scope.Size(), scopeCopy.Size());
			Assert::AreEqual(scope.Capacity(), scopeCopy.Capacity());
			Assert::IsNull(scopeCopy.GetParent());

			//test 2: copy list with stuff in it
			Datum& healthDat = scope.Append("Health");
			Datum& nameDat = scope.Append("Name");
			healthDat.PushBack(100);
			nameDat.PushBack("Bob");
			scopeCopy = scope;
			Assert::AreEqual(scope, scopeCopy);
			Assert::AreEqual(scope.Size(), scopeCopy.Size());
			Assert::AreEqual(scope.Capacity(), scopeCopy.Capacity());
			Assert::IsNull(scopeCopy.GetParent());
			Assert::AreNotSame(scope[0], scopeCopy[0]);

			////test 3: copy scope with a child scope
			Scope* child = new Scope();
			child->Append("Health") = 10;
			scope.Adopt(*child, "Minions");
			Scope scopeCopy2 = scope;
			scopeCopy = scope;
			Assert::AreEqual(scopeCopy2, scope);
			Assert::AreEqual(scopeCopy, scope);
			Assert::IsNull(scopeCopy2.FindContainedScope(*child).first);
			Assert::IsNull(scopeCopy.FindContainedScope(*child).first);
			Assert::AreNotSame(scope[0], scopeCopy[0]);
			Assert::AreNotSame(scope[0], scopeCopy2[0]);
			Assert::AreNotSame(scope["Minions"], scopeCopy["Minions"]);
			Assert::AreNotSame(scope["Minions"], scopeCopy2["Minions"]);

			//test 4: assign child scope to a new scope
			*child = scopeCopy;
			Assert::AreEqual(*child, scopeCopy);
			Assert::AreEqual(scope.FindContainedScope(*child).first, &scope["Minions"]);

			//test 5: assign child scope to it's parent
			Assert::ExpectException<std::runtime_error>([&child, &scope] {*child = scope; });

			//*child = scope;
			/*Assert::AreEqual(*child, scope);
			Assert::AreEqual(scope.FindContainedScope(*child).first, &scope["Minions"]);*/
		}

		TEST_METHOD(MoveSemantics)
		{
			Scope scope;
			scope["Name"] = "Bob"s;
			scope["Health"] = 100;
			Scope* child = new Scope();
			scope.Adopt(*child, "Powers");
			child->Append("Name") = "Laser Power"s;


			//move constructor
			Scope scopeCopy = scope;

			Assert::AreNotSame(scope.AppendScope("child"), scopeCopy.AppendScope("child"));

			Scope moveScope(std::move(scope));
			Assert::AreEqual(moveScope, scopeCopy);
			
			//move assignment
			Scope moveScope2;
			moveScope2 = std::move(moveScope);
			Assert::AreEqual(moveScope2, scopeCopy);
		}

		TEST_METHOD(Search)
		{
			bool throwaway;
			Scope* scopePtr;

			//test 1: Found in self (non-const)
			Scope scope;
			scope["X"] = 10;
			Assert::AreEqual(&scope["X"], scope.Search("X", &scopePtr));

			//test 1: Found in self (const)
			const Scope& scopeConst = scope;
			Assert::AreEqual(scopeConst["X"], scopeConst.Search("X", &scopePtr));

			//test 2: Found in parent (non-const)
			Scope& child = scope.AppendScope("Child", throwaway);
			child["Y"] = 2.0f;
			Assert::AreEqual(&scope["X"], child.Search("X", &scopePtr));

			//test 2: Found in parent (const)
			const Scope& childConst = child;
			Assert::AreEqual(scopeConst["X"], childConst.Search("X", &scopePtr));
			

			//test 3: not found (non-const)
			Assert::IsNull(scope.Search("Y"));
			Assert::IsNull(child.Search("M"));

			//test 3: not found (const)
			Assert::IsNull(scopeConst.Search("Y"));
			Assert::IsNull(childConst.Search("M"));
		}

		TEST_METHOD(Equality)
		{
			{
				//test 1: two default constructed scopes
				Scope scope;
				Scope scope2;
				Assert::AreEqual(scope, scope2);

				//test 2: two scopes with datums in same order
				Datum& healthDat = scope.Append("Health");
				Datum& nameDat = scope.Append("Name");
				Datum& healthDat2 = scope2.Append("Health");
				Datum& nameDat2 = scope2.Append("Name");

				Assert::AreEqual(scope, scope2);

				healthDat.PushBack(100);
				nameDat.PushBack("Bob");
				healthDat2.PushBack(100);
				nameDat2.PushBack("Bob");

				Assert::AreEqual(scope, scope2);
				
				//test 3: two scopes with a same key but datums are different
				healthDat2.PushBack(500);
				Assert::IsTrue(scope != scope2);
			}

			//test 4: two scopes with datums in different order
			{
				Scope scope;
				Scope scope2;

				Datum& healthDat = scope.Append("Health");
				Datum& nameDat = scope.Append("Name");
				Datum& nameDat2 = scope2.Append("Name");
				Datum& healthDat2 = scope2.Append("Health");

				Assert::AreEqual(scope, scope2);

				healthDat.PushBack(100);
				nameDat.PushBack("Bob");
				healthDat2.PushBack(100);
				nameDat2.PushBack("Bob");

				Assert::AreEqual(scope, scope2);

				//test 5: scopes with different number of elements
				scope.Append("Powers");
				Assert::IsTrue(scope != scope2);

				//test 6: scope with keys the other scope doesn't have
				scope2.Append("NotPowers");
				Assert::IsTrue(scope != scope2);
			}


			//test 7: scopes with a nested child
			{
				Scope scope, scope2;
				Scope* child = new Scope();
				Scope* child2 = new Scope();
				scope.Adopt(*child, "Powers");
				scope2.Adopt(*child2, "Powers");

				Assert::AreEqual(scope, scope2);
				Assert::AreEqual(*child, *child2);
				Assert::IsTrue(scope != *child);
				Assert::IsTrue(scope2 != *child2);
			}


			//test 8: same scope
			{
				Scope scope;
				Scope scope2 = scope;
				Assert::AreEqual(scope, scope2);
			}
		}

		TEST_METHOD(Append)
		{
			//test 1: new item
			Scope scope;
			bool entryCreated;
			Datum& healthDat = scope.Append("Health", entryCreated);
			Assert::IsTrue(entryCreated);
			Assert::AreEqual(scope.Size(), 1_z);

			Datum& nameDat = scope.Append("Name", entryCreated);
			Assert::IsTrue(entryCreated);
			Assert::AreEqual(scope.Size(), 2_z);

			//test 2: append existing item
			nameDat.PushBack("Bob");
			healthDat.PushBack(100);

			Datum& nameDat2 = scope.Append("Name", entryCreated);
			Assert::IsFalse(entryCreated);
			Datum& healthDat2 = scope.Append("Health", entryCreated);
			Assert::IsFalse(entryCreated);

			Assert::AreEqual(nameDat, nameDat2);
			Assert::AreEqual(healthDat, healthDat2);
			Assert::AreSame(nameDat, nameDat2);
			Assert::AreSame(healthDat, healthDat2);

			//test 3: empty string
			Assert::ExpectException<std::runtime_error>([&scope] { scope.Append(""); });
		}

		TEST_METHOD(AppendScope)
		{
			Scope scope;
			Scope* scopePtr;
			bool entryCreated;

			//test 1: create new entry and scope
			scopePtr = &scope.AppendScope("Powers", entryCreated);
			Assert::IsTrue(entryCreated);
			Assert::AreEqual(scopePtr->GetParent(), &scope);

			//test 2: append existing scope 
			//Assert::AreEqual(scopePtr, &scope.AppendScope("Powers"));
			//Assert::AreSame(*scopePtr, scope.AppendScope("Powers"));	//sanity check

			//test 3: append to create new entry, append scope to add scope (should work)
			scope.Append("Arms", entryCreated);
			Assert::IsTrue(entryCreated);
			scopePtr = &scope.AppendScope("Arms", entryCreated);
			Assert::IsFalse(entryCreated);
			Assert::AreEqual(scopePtr->GetParent(), &scope);
			//Assert::AreEqual(scopePtr, &scope.AppendScope("Arms"));
			//Assert::AreSame(*scopePtr, scope.AppendScope("Arms"));	//sanity check

			//test 4: append to create new entry, modify dat to make not scope, then append scope (should throw exception)
			Datum& dat = scope.Append("health");
			dat.SetType(DatumType::Integer);
			Assert::ExpectException<runtime_error>([&scope] { scope.AppendScope("health"); });

			//test 5: append scope on a non-empty non-table entry (should throw exception)
			dat.PushBack(100);
			Assert::ExpectException<runtime_error>([&scope] { scope.AppendScope("health"); });
		
			//test 6: empty string
			Assert::ExpectException<std::runtime_error>([&scope] { scope.AppendScope(""); });
		}

		TEST_METHOD(Adopt)
		{
			//test 1: adopt a scope with name that doesn't exist
			Scope scope;
			Scope* child = new Scope();
			scope.Adopt(*child, "Powers");
			Assert::AreEqual(child->GetParent(), &scope);
			Assert::AreSame(scope["Powers"][0], *child);

			//test 2: adopt a scope with name that does exist
			Scope* child2 = new Scope();
			scope.Adopt(*child2, "Powers");
			Assert::AreEqual(child2->GetParent(), &scope);
			Assert::AreSame(scope["Powers"][1], *child2);


			//test 4: steal the child of some other scope
			Scope newScope;
			newScope.Adopt(*child, "Powers");
			Assert::AreEqual(child->GetParent(), &newScope);
			Assert::AreSame(newScope["Powers"][0], *child);
			Assert::IsNull(scope.FindContainedScope(*child).first);
			Assert::AreSame(scope["Powers"][0], *child2);

			//test 4: adopt a scope with name that matches a datum of another type (throw exception)
			Scope* child3 = new Scope();
			scope["Health"] = 100;
			Assert::ExpectException<runtime_error>([&scope, &child3] {scope.Adopt(*child3, "Health"); });
			delete(child3);

			//test 5: adopt a scope that is already a child (nothing happens)
			scope.Adopt(*child2, "Powers");

			//test 6: empty name
			Scope* child4 = new Scope();
			Assert::ExpectException<std::runtime_error>([&scope, &child4] { scope.Adopt(*child4, ""); });
			delete (child4);
		}

		TEST_METHOD(Orphan)
		{
			//test 1: orphan on scope with no parent (nothing happens)
			Scope scope;
			scope.Orphan();

			//test 2: orphan a scope with a parent
			Scope* child = new Scope();
			scope.Adopt(*child, "Powers");
			Assert::AreEqual(scope.FindContainedScope(*child).first, &scope["Powers"]);
			Assert::AreEqual(scope.FindContainedScope(*child).second, 0_z);
			child->Orphan();
			Assert::IsNull(scope.FindContainedScope(*child).first);
			Assert::IsNull(child->GetParent());
			Assert::AreEqual(scope.Size(), 1_z); //did not delete the entry, just the scope inside the datum
			//delete(child);

			//test 3: orphan a nested scope that holds another nested scope (sanity checks for memory leaks)
			Scope* child2 = new Scope();
			Scope* child3 = new Scope();
			scope.Adopt(*child, "Powers");
			child->Adopt(*child2, "Laser Powers");
			child2->Adopt(*child3, "Zap");
			child->Orphan();
			Assert::IsNull(scope.FindContainedScope(*child).first);
			Assert::IsNull(child->GetParent());
			Assert::AreEqual(child2->GetParent(), child);
			Assert::AreEqual(child3->GetParent(), child2);

			child3->Orphan();
			Assert::IsNull(child2->FindContainedScope(*child3).first);
			Assert::IsNull(child3->GetParent());
			Assert::AreEqual(child2->GetParent(), child);

			delete(child); //should also delete child2 
			delete(child3);

		}

		TEST_METHOD(Find)
		{
			bool entryCreated;

			//test 1: do not exist (non-const)
			Scope scope;
			Assert::IsNull(scope.Find("Health"));

			//test 1: do not exist (const)
			{
				const Scope& scopeConst = scope;
				Assert::IsNull(scopeConst.Find("Health"));
			}

			//test2: does exist (non-const)
			Datum& healthDat = scope.Append("Health", entryCreated);
			Datum& nameDat = scope.Append("Name", entryCreated);
			Assert::AreSame(*(scope.Find("Health"s)), healthDat);
			Assert::AreSame(*(scope.Find("Name"s)), nameDat);

			//test 2:does exist (const)
			{
				const Scope& scopeConst = scope;
				Assert::AreSame(*(scopeConst.Find("Health"s)), healthDat);
				Assert::AreSame(*(scopeConst.Find("Name"s)), nameDat);
			}
		}

		TEST_METHOD(FindContainedScope)
		{
			//test 1: exists (first element)
			Scope scope;
			Scope* child = new Scope();
			scope.Adopt(*child, "Powers");
			Assert::AreEqual(scope.FindContainedScope(*child).first, &scope["Powers"]);
			Assert::AreEqual(scope.FindContainedScope(*child).second, 0_z);

			//test 2: exists (not first element)
			Scope* child2 = new Scope();
			Scope* child3 = new Scope();
			scope.Adopt(*child2, "Powers");
			scope.Adopt(*child3, "Powers");
			Assert::AreEqual(scope.FindContainedScope(*child2).first, &scope["Powers"]);
			Assert::AreEqual(scope.FindContainedScope(*child2).second, 1_z);
			Assert::AreEqual(scope.FindContainedScope(*child3).first, &scope["Powers"]);
			Assert::AreEqual(scope.FindContainedScope(*child3).second, 2_z);

			//test 3: does not exist
			Scope* child4 = new Scope();
			Assert::IsNull(scope.FindContainedScope(*child4).first);
			Assert::AreEqual(scope.FindContainedScope(*child4).second, 0_z);
			delete(child4);
		}

		TEST_METHOD(IndexOpString)
		{
			//test 1: new item
			Scope scope;
			Datum& healthDat = scope["Health"];
			Assert::AreEqual(scope.Size(), 1_z);

			Datum& nameDat = scope["Name"];
			Assert::AreEqual(scope.Size(), 2_z);

			//test 2: append existing item
			nameDat.PushBack("Bob");
			healthDat.PushBack(100);

			Datum& nameDat2 = scope["Name"];
			Datum& healthDat2 = scope["Health"];

			Assert::AreSame(nameDat, nameDat2);
			Assert::AreSame(healthDat, healthDat2);


			//test 3: (const) item exists
			const Scope& scopeConst = scope;
			Assert::AreSame(*(scopeConst.Find("Health"s)), healthDat);
			Assert::AreSame(*(scopeConst.Find("Name"s)), nameDat);

			//test 4: (const) item does not exist
			Assert::IsNull(scopeConst.Find("Powers"s));
		}

		TEST_METHOD(IndexOpSizet)
		{
			//test 1: valid (non-const)
			Scope scope;
			Datum& healthDat = scope["Health"];
			Datum& nameDat = scope["Name"];
			nameDat.PushBack("Bob");
			healthDat.PushBack(100);

			Assert::AreSame(scope[0], healthDat);
			Assert::AreSame(scope[1], nameDat);

			//test 2: valid (const)
			const Scope& scopeConst = scope;
			Assert::AreSame(scopeConst[0], healthDat);
			Assert::AreSame(scopeConst[1], nameDat);

			//test 3: out of bounds (non-const) (throw error)
			Assert::ExpectException<std::runtime_error>([&scope] { scope[3]; });	

			//test 3: out of bounds (const) (throw error)
			Assert::ExpectException<std::runtime_error>([&scopeConst] { scopeConst[3]; });
		}

	private:
		static _CrtMemState sStartMemState;	//for memory leak detection
	};
	_CrtMemState ScopeTests::sStartMemState;
}