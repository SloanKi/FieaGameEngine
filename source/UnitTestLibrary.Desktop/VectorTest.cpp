#include "pch.h"
#include "CppUnitTest.h"
#include "vector.h"
#include "Foo.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;
using namespace UnitTests;
using namespace std;
using namespace std::string_literals;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<Library::Vector<Foo>::Iterator>(const Library::Vector<Foo>::Iterator& t)
	{
		RETURN_WIDE_STRING(&t);
	}

	template<>
	inline std::wstring ToString<Library::Vector<Foo>::Iterator>(const Library::Vector<Foo>::Iterator* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Library::Vector<Foo>::Iterator>(Library::Vector<Foo>::Iterator* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Library::Vector<Foo>::ConstIterator>(const Library::Vector<Foo>::ConstIterator& t)
	{
		RETURN_WIDE_STRING(&t);
	}

	template<>
	inline std::wstring ToString<Library::Vector<Foo>::ConstIterator>(const Library::Vector<Foo>::ConstIterator* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Library::Vector<Foo>::ConstIterator>(Library::Vector<Foo>::ConstIterator* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Library::Vector<Foo>>(const Library::Vector<Foo>& t)
	{
		RETURN_WIDE_STRING(&t);
	}
}


namespace UnitTestLibraryDesktop
{
	TEST_CLASS(VectorTests)
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

		TEST_METHOD(VectorEquality)
		{
			//test 1: equal
			Vector<Foo> vec{ Foo(1), Foo(2), Foo(3) };
			Vector<Foo> copy = vec;
			Assert::AreEqual(copy, vec);

			//test 2: different sizes
			copy.PopBack();
			Assert::IsTrue(copy != vec);
			
			//test 3: same sizes but not equal
			copy.PushBack(Foo(1));
			Assert::IsTrue(copy != vec);

			//test 4: equal to self
			Assert::AreEqual(copy, copy);
		}

		TEST_METHOD(InitializerList)
		{
			Vector<Foo> list = { Foo(1), Foo(2), Foo(3) };
			Assert::AreEqual(list[0], Foo(1));
			Assert::AreEqual(list[1], Foo(2));
			Assert::AreEqual(list[2], Foo(3));
			Assert::AreEqual(list.Size(), 3_z);
			Assert::AreEqual(list.Capacity(), 3_z);
		}

		TEST_METHOD(Constructors)
		{
			//test 1: default
			Vector<Foo> list;
			Assert::AreEqual(list.Size(), 0_z);
			Assert::AreEqual(list.Capacity(), 0_z);

			//test 2: specify capacity
			Vector<Foo> list2(20);
			Assert::AreEqual(list2.Size(), 0_z);
			Assert::AreEqual(list2.Capacity(), 20_z);
		}

		TEST_METHOD(Copy)
		{
			//test 1: empty list
			Vector<Foo> list;
			Vector<Foo> listCopy = list;
			Assert::AreEqual(listCopy.Size(), 0_z);
			Assert::AreEqual(listCopy.Capacity(), 0_z);

			//test 2: list with items
			Foo a(5), b(10), c(11);
			list.PushBack(a);
			list.PushBack(b);
			list.PushBack(c);
			Vector<Foo> listCopy2 = list;
			Assert::AreEqual(listCopy2.Size(), 3_z);
			Assert::AreEqual(listCopy2.Capacity(), Vector<Foo>::DEFAULT_CAPACITY);
			Assert::AreEqual(listCopy2[0], a);
			Assert::AreEqual(listCopy2[1], b);
			Assert::AreEqual(listCopy2[2], c);
			Assert::AreNotSame(listCopy2[0], list[0]);

		}

		TEST_METHOD(MoveSemantics)
		{
			Vector<Foo> list;
			uint16_t data = 5;
			list.PushBack(Foo(data));
			list.PushBack(Foo(data * 2));
			list.PushBack(Foo(data * 3));

			Vector<Foo> list2(std::move(list));
			Assert::AreEqual(Foo(data), list2[0]);
			Assert::AreEqual(Foo(data * 3), list2[2]);
			Assert::AreEqual(3_z, list2.Size());

			Vector<Foo> list3;
			list3 = (std::move(list2));
			Assert::AreEqual(Foo(data), list3[0]);
			Assert::AreEqual(Foo(data * 3), list3[2]);
			Assert::AreEqual(3_z, list3.Size());
		}

		TEST_METHOD(RandomAccessOperators)
		{
			//test 1: out of bounds on empty list (throw exception)
			Vector<Foo> list;
			const Vector<Foo>& constList = list;
			Assert::ExpectException<std::runtime_error>([&list] { list[0]; });
			Assert::ExpectException<std::runtime_error>([&constList] { constList[0]; });

			//test 2: multiple elements
			Foo a(5);
			Foo b(10);
			Foo c(11);
			list.PushBack(a);
			list.PushBack(b);
			list.PushBack(c);
			Assert::AreEqual(a, list[0]);
			Assert::AreEqual(b, list[1]);
			Assert::AreEqual(c, list[2]);

			const Vector<Foo>& constList2 = list;
			Assert::AreEqual(a, constList2[0]);
			Assert::AreEqual(b, constList2[1]);
			Assert::AreEqual(c, constList2[2]);

			//test 3: out of bounds on list with elements (throw exception) (overkill but who cares)
			Assert::ExpectException<std::runtime_error>([&list] { list[3]; });
			Assert::ExpectException<std::runtime_error>([&constList2] { constList2[3]; });

		}

		TEST_METHOD(SizeCapacityEmpty)
		{
			//test 1: empty
			Vector<Foo> list;
			Assert::AreEqual(list.Size(), 0_z);
			Assert::IsTrue(list.IsEmpty());
			Assert::AreEqual(list.Capacity(), 0_z);			

			//test 2: not empty
			list.PushBack(Foo(5));
			Assert::AreEqual(list.Size(), 1_z);
			Assert::IsFalse(list.IsEmpty());
			Assert::AreEqual(list.Capacity(), Vector<Foo>::DEFAULT_CAPACITY);
		}

		TEST_METHOD(Reserve)
		{
			Foo a(5);
			Foo b(10);
			Foo c(11);

			//Test 1: Reserve for a list without memory already allocated
			Vector<Foo> list;
			list.Reserve(20);
			Assert::AreEqual(list.Capacity(), 20_z);

			//Test 2: Reserve more memory for list with items already in it
			list.PushBack(a);
			list.PushBack(b);
			list.PushBack(c);
			list.Reserve(30);
			Assert::AreEqual(list.Capacity(), 30_z);
			Assert::AreEqual(list.Size(), 3_z);
			Assert::AreEqual(a, list[0]);
			Assert::AreEqual(b, list[1]);
			Assert::AreEqual(c, list[2]);

			//Test 3: Try to shrink array (nothing happens)
			list.Reserve(15);
			Assert::AreEqual(list.Capacity(), 30_z);
			Assert::AreEqual(list.Size(), 3_z);
			Assert::AreEqual(a, list[0]);
			Assert::AreEqual(b, list[1]);
			Assert::AreEqual(c, list[2]);

		}

		TEST_METHOD(ReCapacity)
		{
			Foo a(5), b(10), c(11);

			//test 1: resizing a list with capacity of 0
			Vector<Foo> list;
			list.ReCapacity(20_z);
			Assert::AreEqual(list.Capacity(), 20_z);

			//test 2: expand list
			list.PushBack(a);
			list.PushBack(b);
			list.PushBack(c);
			list.ReCapacity(30);
			Assert::AreEqual(list.Capacity(), 30_z);
			Assert::AreEqual(list.Size(), 3_z);
			Assert::AreEqual(a, list[0]);
			Assert::AreEqual(b, list[1]);
			Assert::AreEqual(c, list[2]);

			//test 3: shrink capacity smaller than size
			list.ReCapacity(2);
			Assert::AreEqual(list.Capacity(), 2_z);
			Assert::AreEqual(list.Size(), 2_z);
			Assert::AreEqual(a, list[0]);
			Assert::AreEqual(b, list[1]);

			//test 4: ReCapacity to 0
			list.ReCapacity(0);
			Assert::AreEqual(list.Capacity(), 0_z);
			Assert::AreEqual(list.Size(), 0_z);
		}

		TEST_METHOD(Resize)
		{
			Foo a(5), b(10), c(11), d;

			//test 1: resizing a list with capacity of 0
			Vector<Foo> list;
			list.Resize(5_z);
			Assert::AreEqual(list.Capacity(), 5_z);
			Assert::AreEqual(list.Size(), list.Capacity());

			//make sure properly default constructed
			for (size_t i = 0; i < list.Size(); i++)
			{
				Assert::AreEqual(list[i], d);
			}

			//test 2: expand existing list
			list.Clear(); //remove previous default constructed items
			list.PushBack(a);
			list.PushBack(b);
			list.PushBack(c);
			list.Resize(10);
			Assert::AreEqual(list.Capacity(), 10_z);
			Assert::AreEqual(list.Size(), list.Capacity());
			Assert::AreEqual(a, list[0]);
			Assert::AreEqual(b, list[1]);
			Assert::AreEqual(c, list[2]);

			//make sure default constructed
			for (size_t i = 3; i < list.Size(); i++)
			{
				Assert::AreEqual(list[i], d);
			}

			//test 3: shrink capacity smaller than size
			list.Resize(2);
			Assert::AreEqual(list.Capacity(), 2_z);
			Assert::AreEqual(list.Size(), list.Capacity());
			Assert::AreEqual(a, list[0]);
			Assert::AreEqual(b, list[1]);

			//test 4: Resize to 0
			list.Resize(0);
			Assert::AreEqual(list.Capacity(), 0_z);
			Assert::AreEqual(list.Size(), 0_z);
		}

		TEST_METHOD(PushBack)
		{
			Foo a(5), b(10), c(11), d(12), e(13), f(14);

			//Test one: push to empty list
			Vector<Foo> list;
			list.PushBack(a);
			Assert::AreEqual(list[0], a);
			Assert::AreEqual(list.Size(), 1_z);
			Assert::AreEqual(list.Capacity(), Vector<Foo>::DEFAULT_CAPACITY);

			//Test 2: push to list with extra capacity
			list.PushBack(b);
			list.PushBack(c);
			list.PushBack(d);
			list.PushBack(e);
			Assert::AreEqual(list[0], a);
			Assert::AreEqual(list[4], e);
			Assert::AreEqual(list.Size(), 5_z);
			Assert::AreEqual(list.Capacity(), Vector<Foo>::DEFAULT_CAPACITY);

			//test 3: push to full list
			list.PushBack(f);
			Assert::AreEqual(list[5], f);
			Assert::AreEqual(list.Size(), 6_z);
			Assert::AreEqual(list.Capacity(), 8_z);
		}

		TEST_METHOD(PopBack)
		{
			//test one: empty list (nothing happens)
			Vector<Foo> list;
			list.PopBack();
			Assert::AreEqual(list.Size(), 0_z);

			//test 2: pop actual element
			Foo a(5), b(10), c(11);
			list.PushBack(a);
			list.PushBack(b);
			list.PushBack(c);
			list.PopBack();
			Assert::AreEqual(list.Size(), 2_z);
			Assert::AreEqual(list.Capacity(), list.DEFAULT_CAPACITY);
			Assert::AreEqual(list[0], a);
			Assert::AreEqual(list[1], b);
		}

		TEST_METHOD(Clear)
		{
			Foo a(5), b(10), c(11);
			Vector<Foo> list;
			list.PushBack(a);
			list.PushBack(b);
			list.PushBack(c);
			list.Clear();
			Assert::AreEqual(list.Size(), 0_z);
			Assert::AreEqual(list.Capacity(), list.DEFAULT_CAPACITY);
		}

		TEST_METHOD(Assignment)
		{
			//Test 1: both empty
			Vector<Foo> list;
			Vector<Foo> listCopy;
			listCopy = list;
			Assert::AreEqual(listCopy.Size(), 0_z);
			Assert::AreEqual(listCopy.Capacity(), 0_z);
			
			//Test 2: list has elements, listCopy is empty
			Foo a(5), b(10), c(11);
			list.PushBack(a);
			list.PushBack(b);
			list.PushBack(c);
			listCopy = list;
			Assert::AreEqual(listCopy.Size(), 3_z);
			Assert::AreEqual(listCopy.Capacity(), list.DEFAULT_CAPACITY);
			for (int i = 0; i < 3; i++)
			{
				Assert::AreEqual(list[i], listCopy[i]);
				Assert::AreNotSame(list[i], listCopy[i]);
			}
			
			//Test 3: Copy an empty list over a list with items in it
			Vector<Foo> list2;
			listCopy = list2;
			Assert::AreEqual(listCopy.Size(), 0_z);
			Assert::AreEqual(listCopy.Capacity(), 0_z);

		}

		TEST_METHOD(Find)
		{
			uint16_t data = 5;

			//Test 1: empty list (non-const)
			Vector<Foo> list;
			Vector<Foo>::Iterator emptyIt = list.Find(Foo(data));
			Assert::AreEqual(emptyIt, list.end());

			//Test 1: empty list (const)
			const Vector<Foo>& listConst = list;
			Vector<Foo>::ConstIterator emptyItConst = listConst.Find(Foo(data));
			Assert::AreEqual(emptyItConst, listConst.end());

			//Test 2: exists in list (non-const)
			list.PushBack(Foo(data));
			list.PushBack(Foo(data * 2));
			list.PushBack(Foo(data * 3));
			Vector<Foo>::Iterator validIt = list.Find(Foo(data * 2));
			Assert::AreEqual(*validIt, Foo(data * 2));
			validIt++;
			Assert::AreEqual(*validIt, Foo(data * 3));

			//Test 2: exists in list (const)
			const Vector<Foo>& listConst2 = list;
			Vector<Foo>::ConstIterator validItConst = listConst2.Find(Foo(data * 2));
			Assert::AreEqual(*validItConst, Foo(data * 2));
			validItConst++;
			Assert::AreEqual(*validItConst, Foo(data * 3));

			//Test 3: does not exist in list (non-const)
			Vector<Foo>::Iterator invalidIt = list.Find(Foo(data * 4));
			Assert::AreEqual(invalidIt, list.end());

			//Test 3: does not exist in list (non-const)
			Vector<Foo>::ConstIterator invalidItConst = listConst2.Find(Foo(data * 4));
			Assert::AreEqual(invalidItConst, list.cend());
		}

		TEST_METHOD(Remove) 
		{
			Foo a(5), b(10), c(11), d(12), e(13), f(14);

			//test 1: remove from empty list
			Vector<Foo> list;
			list.Remove(list.begin());

			//test 2: remove the only element in the list
			list.PushBack(a);
			Assert::AreEqual(list.Size(), 1_z);
			list.Remove(list.Find(a));
			Assert::IsTrue(list.IsEmpty());

			//test 3: Remove last element
			list.PushBack(a);
			list.PushBack(b);
			list.PushBack(c);
			list.Remove(list.Find(c));
			Assert::AreEqual(list.Size(), 2_z);
			Assert::AreEqual(list[0], a);
			Assert::AreEqual(list[1], b);

			//test 4: remove element that's not front or back
			// this test is the one that causes memory leaks
			list.PushBack(c);
			list.PushBack(d);
			list.Remove(list.Find(b));
			Assert::AreEqual(list.Size(), 3_z);
			Assert::AreEqual(list[0], a);
			Assert::AreEqual(list[1], c);
			Assert::AreEqual(list[2], d);

			////Test 5: iterator does not belong to this list
			Vector<Foo> list2;
			Assert::ExpectException<std::runtime_error>([&list, &list2] { list.Remove(list2.begin()); });

		}

		/************************************************************************/
		/*************************Iterator Functions*****************************/
		/************************************************************************/
		TEST_METHOD(begin) 
		{
			Foo a(5), b(10), c(11);

			//test one: empty list (non-const)
			Vector<Foo> list;
			Vector<Foo>::Iterator start(list.begin());
			Assert::AreEqual(start, Vector<Foo>::Iterator(list, 0));

			//test one: empty list (non-const)
			const Vector<Foo>& constList = list;
			Vector<Foo>::ConstIterator constStart(constList.begin());
			Assert::AreEqual(constStart, Vector<Foo>::ConstIterator(constList, 0));

			//test two: list with items (non-const)
			list.PushBack(a);
			list.PushBack(b);
			Assert::AreEqual(list.begin(), Vector<Foo>::Iterator(list, 0));

			//test two: list with items (const)
			const Vector<Foo>& constList2 = list;
			Vector<Foo>::ConstIterator constStart2(constList2.begin());
			Assert::AreEqual(constStart2, Vector<Foo>::ConstIterator(constList2, 0));
		}

		TEST_METHOD(end)
		{
			Foo a(5), b(10), c(11);

			//test one: empty list (non-const)
			Vector<Foo> list;
			Vector<Foo>::Iterator back(list.end());
			Assert::AreEqual(back, Vector<Foo>::Iterator(list, 0));

			//test one: empty list (non-const)
			const Vector<Foo>& constList = list;
			Assert::AreEqual(constList.end(), Vector<Foo>::ConstIterator(constList, 0));

			//test two: list with items (non-const)
			list.PushBack(a);
			list.PushBack(b);
			Assert::AreEqual(list.end(), Vector<Foo>::Iterator(list, 2));

			//test two: list with items (const)
			const Vector<Foo>& constList2 = list;
			Assert::AreEqual(constList2.end(), Vector<Foo>::ConstIterator(constList2, 2));
		}

		TEST_METHOD(Dereference)
		{
			//test 1: invalid (non-const)
			Vector<Foo> list;
			Vector<Foo>::Iterator start = list.begin();
			Assert::ExpectException<std::runtime_error>([&start] { *start; });

			//test 1: invalid (const)
			Assert::ExpectException<std::runtime_error>([&list] { *(list.cbegin()); });
			

			//test 2: valid (non-const)
			list.PushBack(Foo(5));
			start = list.begin();
			Assert::AreEqual(*start, Foo(5));

			//test 2: valid (non-const)
			Assert::AreEqual(*(list.cbegin()), Foo(5));
			
		}

		TEST_METHOD(Equality)
		{
			//test 1: are equal (non-const)
			Vector<Foo> list;
			uint16_t data = 5;
			list.PushBack(Foo(data));
			list.PushBack(Foo(data * 2));
			list.PushBack(Foo(data * 3));
			Vector<Foo>::Iterator start(list, 1);
			Vector<Foo>::Iterator start2(list, 1);
			Assert::AreEqual(start, start2);

			//test 1: are equal (const)
			Vector<Foo>::ConstIterator constStart(list.begin());
			Vector<Foo>::ConstIterator constStart2(list.cbegin());
			Assert::AreEqual(constStart, constStart2);

			//Test 2: Same owner, different index (not equal) (non-const)
			Vector<Foo>::Iterator back(list.end());
			Assert::IsTrue(back != start);	//Assert::AreNotEqual does not invoke operator!=

			//Test 2: Same owner, different index (not equal) (const)
			Vector<Foo>::ConstIterator constBack = list.cend();
			Assert::IsTrue(constBack != constStart);

			//Test 3: Different owners, same index (not equal) (non-const)
			Vector<Foo> list2;
			Vector<Foo>::Iterator diffBack(list2.end());
			Assert::IsTrue(back != diffBack);	//Assert::AreNotEqual does not invoke operator!=

			//Test 3: Different owners, same index (not equal) (const)
			Vector<Foo>::Iterator constDiffBack(list2.end());
			Assert::IsTrue(constBack != constDiffBack);	//Assert::AreNotEqual does not invoke operator!=
		}

		TEST_METHOD(CopyAndAssignment)
		{
			Foo a(5), b(10), c(11);

			//copy
			Vector<Foo> list;
			list.PushBack(a);
			list.PushBack(b);
			Vector<Foo>::Iterator start = list.begin();
			Vector<Foo>::Iterator copy = start;
			Assert::AreEqual(start, copy);

			//assignment test one: assign a default constructed iterator to a dif iterator
			Vector<Foo>::Iterator assgn;
			assgn = start;
			Assert::AreEqual(assgn, start);

			//assignment test 2: assign a non-default iterator to a dif iterator
			Vector<Foo>::Iterator last(list, 1);
			assgn = Vector<Foo>::Iterator(list, 1);
			Assert::AreEqual(assgn, last);
		}

		TEST_METHOD(CopyAndAssignmentConst)
		{
			Foo a(5), b(10), c(11);

			//copy
			Vector<Foo> list;
			list.PushBack(a);
			list.PushBack(b);
			Vector<Foo>::ConstIterator start = list.cbegin();
			Vector<Foo>::ConstIterator copy = start;
			Assert::AreEqual(start, copy);

			//assignment test one: assign a default constructed iterator to a dif iterator
			Vector<Foo>::ConstIterator assgn;
			assgn = start;
			Assert::AreEqual(assgn, start);

			//assignment test 2: assign a non-default iterator to a dif iterator
			Vector<Foo>::ConstIterator last(list, 1);
			assgn = Vector<Foo>::Iterator(list, 1);
			Assert::AreEqual(assgn, last);
		}

		TEST_METHOD(ConstructorExceptions)
		{
			Vector<Foo> list;
			Assert::ExpectException<std::runtime_error>([&list] { Vector<Foo>::Iterator it(list, 1); });
			Assert::ExpectException<std::runtime_error>([&list] { Vector<Foo>::ConstIterator it(list, 1); });
		}

		TEST_METHOD(ConstIteratorConstructor)
		{
			Vector<Foo> list;
			uint16_t data = 5;
			list.PushBack(Foo(data));
			list.PushBack(Foo(data * 2));
			list.PushBack(Foo(data * 3));

			Vector<Foo>::Iterator start = list.begin();
			Vector<Foo>::ConstIterator startConst(start);
			Vector<Foo>::ConstIterator startConst2 = list.cbegin();

			Assert::AreEqual(*start, *startConst);
			Assert::AreEqual(startConst, startConst2);
		}

		TEST_METHOD(Increment)
		{
			//test 1: increment when mIndex = mSize (non-const)
			Vector<Foo> list;
			Vector<Foo>::Iterator test = list.begin();
			Assert::ExpectException<std::runtime_error>([&test] { test++; });
			Assert::ExpectException<std::runtime_error>([&test] { ++test; });

			//test 1: increment when mIndex = mSize (const)
			Vector<Foo>::ConstIterator testConst(list.cbegin());
			Assert::ExpectException<std::runtime_error>([&testConst] { testConst++; });
			Assert::ExpectException<std::runtime_error>([&testConst] { ++testConst; });

			//Test 2: increment to end  (non-const)
			uint16_t data = 5;
			list.PushBack(Foo(data));
			Vector<Foo>::Iterator test2 = list.begin();
			test2++;
			Assert::AreEqual(test2, list.end());
			Assert::ExpectException<std::runtime_error>([&test2] { test2++; });
			Assert::ExpectException<std::runtime_error>([&test2] { ++test2; });

			//Test 2: increment to end (const)
			Vector<Foo>::ConstIterator testConst2(list.cbegin());
			testConst2++;
			Assert::AreEqual(testConst2, list.cend());
			Assert::ExpectException<std::runtime_error>([&testConst2] { testConst2++; });
			Assert::ExpectException<std::runtime_error>([&testConst2] { ++testConst2; });

			//test 3: increment normally (non-const)
			list.PushBack(Foo(data * 2));
			list.PushBack(Foo(data * 3));
			Vector<Foo>::Iterator test3 = list.begin();
			test3++;
			Assert::AreEqual(*test3, Foo(data * 2));
			++test3;
			Assert::AreEqual(*test3, Foo(data * 3));

			//test 3: increment normally (const)
			Vector<Foo>::ConstIterator testConst3(list.cbegin());
			testConst3++;
			Assert::AreEqual(*testConst3, Foo(data * 2));
			++testConst3;
			Assert::AreEqual(*testConst3, Foo(data * 3));
		}

	private:
		static _CrtMemState sStartMemState;	//for memory leak detection
	};
	_CrtMemState VectorTests::sStartMemState;
}