#include "pch.h"
#include "CppUnitTest.h"
#include "Hashmap.h"
//#include "DefaultHash.h"
#include "Foo.h"
#include <gsl/gsl>
#include <glm/glm.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;
using namespace UnitTests;
using namespace std;
using namespace std::string_literals;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<Library::Hashmap<int, Foo>::Iterator>(const Library::Hashmap<int, Foo>::Iterator& t)
	{
		RETURN_WIDE_STRING(&t);
	}

	template<>
	inline std::wstring ToString<Library::Hashmap<int, Foo>::Iterator>(const Library::Hashmap<int, Foo>::Iterator* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Library::Hashmap<int, Foo>::Iterator>(Library::Hashmap<int, Foo>::Iterator* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Library::Hashmap<int, Foo>::ConstIterator>(const Library::Hashmap<int, Foo>::ConstIterator& t)
	{
		RETURN_WIDE_STRING(&t);
	}

	template<>
	inline std::wstring ToString<Library::Hashmap<int, Foo>::ConstIterator>(const Library::Hashmap<int, Foo>::ConstIterator* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Library::Hashmap<int, Foo>::ConstIterator>(Library::Hashmap<int, Foo>::ConstIterator* t)
	{
		RETURN_WIDE_STRING(t);
	}
}


namespace UnitTestLibraryDesktop
{
	TEST_CLASS(HashmapTests)
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
	
		TEST_METHOD(Constructor)
		{
			Hashmap<int, Foo> hashmap(10);
			Assert::AreEqual(10_z, hashmap.Capacity());
			Assert::AreEqual(0_z, hashmap.Size());

			Hashmap<int, Foo> hashmap2(0);
			Assert::AreEqual(Hashmap<int, Foo>::DEFAULT_CAPACITY, hashmap2.Capacity());
			Assert::AreEqual(0_z, hashmap2.Size());
		}

		TEST_METHOD(InitializerList)
		{
			Hashmap<std::string, int> map =
			{
				std::pair<const std::string, int>("Test1", 1),
				std::pair<const std::string, int>("Test2", 2),
				std::pair<const std::string, int>("Test3", 3),
				std::pair<const std::string, int>("Test4", 4)
			};

			Assert::AreEqual(map.Size(), 4_z);
			Assert::AreEqual(map["Test1"], 1);
			Assert::AreEqual(map["Test2"], 2);
			Assert::AreEqual(map["Test3"], 3);
			Assert::AreEqual(map["Test4"], 4);
		}

		TEST_METHOD(MoveSemantics)
		{
			std::pair<int, Foo> a(1, Foo(1)), b(2, Foo(2)), c(3, Foo(3)), d(11, Foo(11));
			Hashmap<int, Foo> hashmap(10);
			hashmap.Insert(a);
			hashmap.Insert(b);
			hashmap.Insert(c);

			Hashmap<int, Foo> hashmap2(std::move(hashmap));
			Assert::AreEqual(a.second, hashmap2[1]);
			Assert::AreEqual(b.second, hashmap2[2]);
			Assert::AreEqual(c.second, hashmap2[3]);
			Assert::AreEqual(3_z, hashmap2.Size());

			Hashmap<int, Foo> hashmap3;
			hashmap3 = (std::move(hashmap2));
			Assert::AreEqual(a.second, hashmap3[1]);
			Assert::AreEqual(b.second, hashmap3[2]);
			Assert::AreEqual(c.second, hashmap3[3]);
			Assert::AreEqual(3_z, hashmap3.Size());
		}

		TEST_METHOD(LoadFactor)
		{
			std::pair<int, Foo> a(1, Foo(1)), b(2, Foo(2)), c(3, Foo(3)), d(11, Foo(11));
			Hashmap<int, Foo> hashmap(10);

			//test 1: nothing in list
			Assert::AreEqual(hashmap.Load_Factor(), 0.0f);

			//test 2: stuff in list
			hashmap.Insert(a);
			hashmap.Insert(b);
			hashmap.Insert(c);
			hashmap.Insert(d);

			Assert::AreEqual(hashmap.Load_Factor(), 0.4f);

			//test 3: capacity = 0
			Hashmap<int, Foo> hashmap0;
			Assert::AreEqual(hashmap0.Load_Factor(), 0.0f);
		}

		TEST_METHOD(ContainsKey)
		{
			std::pair<int, Foo> a(1, Foo(1)), b(2, Foo(2)), d(11, Foo(11));

			//test one: empty list
			Hashmap<int, Foo> hashmap(10);
			Assert::IsFalse(hashmap.ContainsKey(1));
			
			//test 2: does not exist (nothing at that hashIndex either)
			hashmap.Insert(a);
			hashmap.Insert(b);
			hashmap.Insert(d);
			Assert::IsFalse(hashmap.ContainsKey(3));

			//test 3: does exist (only key at that hashindex)
			Assert::IsTrue(hashmap.ContainsKey(2));

			//test 4: does exist (is the first key at that hash index)
			Assert::IsTrue(hashmap.ContainsKey(1));

			//test 5: does exist (is not the first key at that hash index)
			Assert::IsTrue(hashmap.ContainsKey(11));

		}

		TEST_METHOD(Insert)
		{
			//test one: Insert into a list that only has a capacity of 1 (will always be first item)
			Hashmap<int, Foo> hashmap(1);
			hashmap.Insert(std::pair<int, Foo>(1, Foo(1)));
			Assert::AreEqual((*(hashmap.begin())).second, Foo(1));
			Assert::AreEqual(hashmap.Size(), 1_z);

			std::pair<int, Foo> a(1, Foo(1)), b(2, Foo(2)), c(3, Foo(3)), d(11, Foo(11)), e(1, Foo(2));
			//test 2: various normal inserts
			Hashmap<int, Foo> hashmap2(10);
			hashmap2.Insert(a);
			Assert::AreEqual((*(hashmap2.begin())).second, a.second);
			Assert::AreEqual(hashmap2.Size(), 1_z);
			hashmap2.Insert(b);
			Assert::AreEqual((*(hashmap2.begin())).second, b.second);
			Assert::AreEqual(hashmap2.Size(), 2_z);
			hashmap2.Insert(c);
			Assert::AreEqual((*(hashmap2.begin())).second, c.second);
			Assert::AreEqual(hashmap2.Size(), 3_z);

			//test 3: Chain test (both a and d will get the same hashindex)
			Hashmap<int, Foo> hashmap3(10);
			hashmap3.Insert(a);
			hashmap3.Insert(d);
			Hashmap<int, Foo>::Iterator it = hashmap3.begin();
			Assert::AreEqual((*it).second, a.second);
			it++;
			Assert::AreEqual((*it).second, d.second);

			//test 4: same key test (a and e have the same key)
			auto result = hashmap3.Insert(e);
			Hashmap<int, Foo>::Iterator it2 = result.first;
			Assert::AreEqual((*it2).second, a.second);
			Assert::AreEqual(hashmap3.begin(), it2);
			Assert::AreEqual(hashmap3.Size(), 2_z);

			//test 5: capacity = 0 (NO LONGER POSSIBLE)
			/*Hashmap<int, Foo> hashmap0;
			Assert::AreEqual(hashmap0.Insert(a), hashmap0.end());*/
		}

		TEST_METHOD(Find)
		{
			std::pair<int, Foo> a(1, Foo(1)), b(2, Foo(2)), c(3, Foo(3)), d(11, Foo(11));

			//test 1: search empty hashmap (non-const)
			Hashmap<int, Foo> hashmap(10);
			Assert::AreEqual(hashmap.Find(1), hashmap.end());

			//test 1: search empty hashmap (const)
			const Hashmap<int, Foo>& constHashmap = hashmap;
			Assert::AreEqual(constHashmap.Find(1), constHashmap.end());

			//test 2: search for valid element (first in chain) (non-const)
			hashmap.Insert(a);
			Assert::AreEqual(hashmap.Find(1), hashmap.begin());

			//test 2: search for valid element (first in chain) (const)
			const Hashmap<int, Foo>& constHashmap2 = hashmap;
			Assert::AreEqual(constHashmap2.Find(1), constHashmap.begin());

			//test 3: Search for valid element (not first in chain) (non-const)
			auto result = hashmap.Insert(d);
			Hashmap<int, Foo>::Iterator it = result.first;
			Assert::AreEqual(hashmap.Find(11), it);

			//test 3: Search for valid element (not first in chain) (const)
			const Hashmap<int, Foo>& constHashmap3 = hashmap;
			Hashmap<int, Foo>::ConstIterator constIt = it;
			Assert::AreEqual(constHashmap3.Find(11), constIt);

			//test 4: does not exist (non-const)
			Assert::AreEqual(hashmap.Find(21), hashmap.end());

			//test 4: does not exist (const)
			Assert::AreEqual(constHashmap3.Find(21), constHashmap3.end());

			//test 5: default constructed hashmap (nothing happens)
			Hashmap<int, Foo> hashmapDefault;
			hashmapDefault.Find(0);
			const Hashmap<int, Foo> hashmapDefaultConst;
			hashmapDefaultConst.Find(0);
		}

		TEST_METHOD(At)
		{
			std::pair<int, Foo> a(1, Foo(1)), b(2, Foo(2)), c(3, Foo(3)), d(11, Foo(11));
			//test 1: key does not exist (throw exception) (non-const)
			Hashmap<int, Foo> hashmap(10);
			Assert::ExpectException<std::runtime_error>([&hashmap] { hashmap.At(1); });

			//test 1: key does not exist (throw exception) (const)
			const Hashmap<int, Foo>& constHashmap = hashmap;
			Assert::ExpectException<std::runtime_error>([&constHashmap] { constHashmap.At(1); });

			//test 2: key does exist (non-const)
			hashmap.Insert(a);
			hashmap.Insert(b);
			hashmap.Insert(c);
			hashmap.Insert(d);
			Assert::AreEqual(hashmap.At(a.first), a.second);
			Assert::AreEqual(hashmap.At(b.first), b.second);
			Assert::AreEqual(hashmap.At(c.first), c.second);
			Assert::AreEqual(hashmap.At(d.first), d.second);


			//test 2: key does exist (const)
			const Hashmap<int, Foo>& constHashmap2 = hashmap;
			Assert::AreEqual(constHashmap2.At(a.first), a.second);
			Assert::AreEqual(constHashmap2.At(b.first), b.second);
			Assert::AreEqual(constHashmap2.At(c.first), c.second);
			Assert::AreEqual(constHashmap2.At(d.first), d.second);

		}

		TEST_METHOD(Clear)
		{
			std::pair<int, Foo> a(1, Foo(1)), b(2, Foo(2)), c(3, Foo(3)), d(11, Foo(11));
			Hashmap<int, Foo> hashmap(10);
			hashmap.Insert(a);
			hashmap.Insert(b);
			hashmap.Insert(c);
			hashmap.Insert(d);

			hashmap.Clear();
			Assert::AreEqual(hashmap.Size(), 0_z);
			Assert::AreEqual(hashmap.Capacity(), 10_z);
		}

		TEST_METHOD(Remove) 
		{
			std::pair<int, Foo> a(1, Foo(1)), b(2, Foo(2)), c(3, Foo(3)), d(11, Foo(11));

			//test 1: empty list (nothing happens)
			Hashmap<int, Foo> hashmap(10);
			hashmap.Remove(1);

			//test 2: key does not exist (nothing happens)
			hashmap.Insert(a);
			hashmap.Insert(b);
			hashmap.Insert(c);
			hashmap.Insert(d);

			hashmap.Remove(5);
			Assert::AreEqual(hashmap.Size(), 4_z);

			//test 2: valid remove
			hashmap.Remove(a.first);
			Assert::AreEqual(hashmap.Size(), 3_z);
			Assert::IsFalse(hashmap.ContainsKey(a.first));

			hashmap.Remove(b.first);
			Assert::AreEqual(hashmap.Size(), 2_z);
			Assert::IsFalse(hashmap.ContainsKey(b.first));

			hashmap.Remove(c.first);
			Assert::AreEqual(hashmap.Size(), 1_z);
			Assert::IsFalse(hashmap.ContainsKey(c.first));

			hashmap.Remove(d.first);
			Assert::AreEqual(hashmap.Size(), 0_z);
			Assert::IsFalse(hashmap.ContainsKey(d.first));
			
		}

		TEST_METHOD(IndexOperator)
		{
			std::pair<int, Foo> a(1, Foo(1)), b(2, Foo(2)), c(3, Foo(3)), d(11, Foo(11));
			//test 1: key does not exist (default construct new element) (non-const)
			Hashmap<int, Foo> hashmap(10);
			Assert::AreEqual(hashmap[5], Foo());
			Assert::AreEqual(hashmap.Size(), 1_z);
			Assert::IsTrue(hashmap.ContainsKey(5));
			//Assert::ExpectException<std::runtime_error>([&hashmap] { hashmap[1]; });

			//test 1: key does not exist (throw exception) (const)
			const Hashmap<int, Foo>& constHashmap = hashmap;
			Assert::ExpectException<std::runtime_error>([&constHashmap] { constHashmap[1]; });

			//test 2: key does exist (non-const)
			hashmap.Insert(a);
			hashmap.Insert(b);
			hashmap.Insert(c);
			hashmap.Insert(d);
			Assert::AreEqual(hashmap[a.first], a.second);
			Assert::AreEqual(hashmap[b.first], b.second);
			Assert::AreEqual(hashmap[c.first], c.second);
			Assert::AreEqual(hashmap[d.first], d.second);


			//test 2: key does exist (const)
			const Hashmap<int, Foo>& constHashmap2 = hashmap;
			Assert::AreEqual(constHashmap2[a.first], a.second);
			Assert::AreEqual(constHashmap2[b.first], b.second);
			Assert::AreEqual(constHashmap2[c.first], c.second);
			Assert::AreEqual(constHashmap2[d.first], d.second);
		}

		/************************************************************************/
		/*************************Iterator Functions*****************************/
		/************************************************************************/
		TEST_METHOD(begin)
		{
			std::pair<int, Foo> a(1, Foo(1)), b(2, Foo(2));

			//test 1: empty list (non-const)
			Hashmap<int, Foo> hashmap(10);
			Assert::AreEqual(hashmap.begin(), hashmap.end());
			
			//test 1: empty list (const)
			const Hashmap<int, Foo>& constHashmap = hashmap;
			Assert::AreEqual(hashmap.cbegin(), constHashmap.begin());

			//test 2: list with stuff in it (non-const)
			hashmap.Insert(a);
			hashmap.Insert(b);

			//Note: Additive hash puts a at index 9 and b at index 8, thus the start of our contents should be b
			Assert::AreEqual(hashmap.begin()->second, b.second);

			//test 2: list with stuff in it (const)
			const Hashmap<int, Foo>& constHashmap2 = hashmap;
			Assert::AreEqual(hashmap.cbegin()->second, b.second);
			Assert::AreEqual(constHashmap2.begin()->second, b.second);
		}

		TEST_METHOD(end)
		{
			//non-const
			Hashmap<int, Foo> hashmap(10);
			Assert::ExpectException<std::runtime_error>([&hashmap] { *hashmap.end(); });

			//const
			const Hashmap<int, Foo>& constHashmap = hashmap;
			Assert::ExpectException<std::runtime_error>([&constHashmap] { *constHashmap.end(); });
			Assert::ExpectException<std::runtime_error>([&hashmap] { *hashmap.cend(); });
		}

		TEST_METHOD(Comparison)
		{
			std::pair<int, Foo> a(1, Foo(1)), b(2, Foo(2)), d(11, Foo(11));

			//test 1: are equal (non-const)
			Hashmap<int, Foo> hashmap(10);
			hashmap.Insert(a);
			hashmap.Insert(b);
			hashmap.Insert(d);
			Assert::AreEqual(hashmap.begin(), hashmap.begin());

			//test 1: are equal (const)
			const Hashmap<int, Foo>& constHashmap = hashmap;
			Assert::AreEqual(constHashmap.begin(), hashmap.cbegin());

			//test 2: Different owners (not equal) (non-const)
			Hashmap<int, Foo> hashmap2(10);
			Assert::IsTrue(hashmap2.end() != hashmap.end()); 

			//test 2: Different owners (not equal) (const)
			const Hashmap<int, Foo>& constHashmap2 = hashmap2;
			Assert::IsTrue(constHashmap2.end() != constHashmap.end()); 

			//test 3: different index (not equal) (non-const)
			Hashmap<int, Foo>::Iterator it = hashmap.begin();
			Assert::IsTrue(hashmap.begin() != ++it);

			//test 3: different index (not equal) (const)
			Hashmap<int, Foo>::ConstIterator constIt = hashmap.cbegin();
			Assert::IsTrue(hashmap.cbegin() != ++constIt);

			//test 4: different chain iterator (not equal) (non-const)
			Hashmap<int, Foo>::Iterator it2 = it;
			Assert::IsTrue(++it != it2);

			//test 4: different chain iterator (not equal) (const)
			Hashmap<int, Foo>::ConstIterator constIt2 = constIt;
			Assert::IsTrue(++constIt != constIt2);
		}

		TEST_METHOD(Dereference)
		{
			std::pair<int, Foo> a(1, Foo(1));

			//test 1: try to dereference end (non-const)
			Hashmap<int, Foo> hashmap(10);
			Assert::ExpectException<std::runtime_error>([&hashmap] { *hashmap.end(); });

			//test 1: try to dereference end (const)
			Assert::ExpectException<std::runtime_error>([&hashmap] { *hashmap.cend(); });

			//test 2: invalid owner (non-const)
			Hashmap<int, Foo>::Iterator invalidIT;
			Assert::ExpectException<std::runtime_error>([&invalidIT] { *invalidIT; });

			//test 2: invalid owner (const)
			Hashmap<int, Foo>::ConstIterator invalidConstIT;
			Assert::ExpectException<std::runtime_error>([&invalidConstIT] { *invalidConstIT; });

			//test 3: valid iterator (non-const)
			hashmap.Insert(a);
			Assert::AreEqual((*(hashmap.begin())).second, a.second);

			//test 3: valid iterator (const)
			Assert::AreEqual((*(hashmap.cbegin())).second, a.second);
		}

		TEST_METHOD(Increment) 
		{
			std::pair<int, Foo> a(1, Foo(1)), b(2, Foo(2)), c(3, Foo(3)), d(11, Foo(11));
			Hashmap<int, Foo> hashmap(10);

			//test 1: invalid (non-const)
			Hashmap<int, Foo>::Iterator invalidIT;
			Assert::ExpectException<std::runtime_error>([&invalidIT] { invalidIT++; });
			Assert::ExpectException<std::runtime_error>([&invalidIT] { ++invalidIT; });

			//test 1: invalid (const)
			Hashmap<int, Foo>::ConstIterator invalidConstIT;
			Assert::ExpectException<std::runtime_error>([&invalidConstIT] { invalidConstIT++; });
			Assert::ExpectException<std::runtime_error>([&invalidConstIT] { ++invalidConstIT; });

			//test 2: valid iterations (non-const)
			hashmap.Insert(a);
			hashmap.Insert(b);
			hashmap.Insert(c);
			hashmap.Insert(d);
			Hashmap<int, Foo>::Iterator it = hashmap.begin();
			Assert::AreEqual(it->second, c.second);
			it++;
			Assert::AreEqual(it->second, b.second);
			Assert::AreEqual((++it)->second, a.second);
			Assert::AreEqual((++it)->second, d.second);

			//test 2: valid iterations (const)
			const Hashmap<int, Foo>& constHashmap = hashmap;
			Hashmap<int, Foo>::ConstIterator constIt = constHashmap.begin();
			Assert::AreEqual(constIt->second, c.second);
			constIt++;
			Assert::AreEqual(constIt->second, b.second);
			Assert::AreEqual((++constIt)->second, a.second);
			Assert::AreEqual((++constIt)->second, d.second);

			//test 3: iterate to end (non-const)
			Assert::AreEqual(++it, hashmap.end());

			//test 3: iterate to end (const)
			Assert::AreEqual(++constIt, constHashmap.end());

			//test 4: iterate past end (error) (non-const)
			Assert::ExpectException<std::runtime_error>([&it] { ++it; });

			//test 4: iterate past end (error) (const)
			Assert::ExpectException<std::runtime_error>([&constIt] { ++constIt; });
		}

		TEST_METHOD(ConstItConstructor)
		{
			std::pair<int, Foo> a(1, Foo(1)), b(2, Foo(2));
			Hashmap<int, Foo> hashmap(10);
			hashmap.Insert(a);
			hashmap.Insert(b);
			Hashmap<int, Foo>::ConstIterator it = hashmap.begin();
			Assert::AreEqual(it, hashmap.cbegin());
		}

	private:
		static _CrtMemState sStartMemState;	//for memory leak detection
	};
	_CrtMemState HashmapTests::sStartMemState;
}