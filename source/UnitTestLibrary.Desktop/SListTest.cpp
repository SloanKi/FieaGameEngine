#include "pch.h"
#include "CppUnitTest.h"
#include "SList.h"
#include "Foo.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Library;
using namespace UnitTests;
using namespace std;
using namespace std::string_literals;

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	inline std::wstring ToString<Library::SList<Foo>::Iterator>(const Library::SList<Foo>::Iterator& t)
	{
		RETURN_WIDE_STRING(&t);
	}

	template<>
	inline std::wstring ToString<Library::SList<Foo>::Iterator>(const Library::SList<Foo>::Iterator* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Library::SList<Foo>::Iterator>(Library::SList<Foo>::Iterator* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Library::SList<Foo>::ConstIterator>(const Library::SList<Foo>::ConstIterator& t)
	{
		RETURN_WIDE_STRING(&t);
	}

	template<>
	inline std::wstring ToString<Library::SList<Foo>::ConstIterator>(const Library::SList<Foo>::ConstIterator* t)
	{
		RETURN_WIDE_STRING(t);
	}

	template<>
	inline std::wstring ToString<Library::SList<Foo>::ConstIterator>(Library::SList<Foo>::ConstIterator* t)
	{
		RETURN_WIDE_STRING(t);
	}
}

namespace UnitTestLibraryDesktop
{
	TEST_CLASS(SListTests)
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

		TEST_METHOD(Copy)
		{
			//test 1: empty list
			SList<Foo> list; 
			SList<Foo> listCopy(list);
			Assert::IsTrue(listCopy.IsEmpty());

			//Test 2: list with one element
			uint16_t data = 17;
			list.PushFront(Foo(data));
			SList<Foo> listCopy2(list);
			Assert::IsFalse(listCopy2.IsEmpty());
			Assert::AreEqual(listCopy2.Front(), list.Front());
			Assert::AreEqual(listCopy2.Back(), list.Back());
			Assert::AreNotSame(list.Front(), listCopy2.Front());
			Assert::AreNotSame(list.Back(), listCopy2.Back());

			//Test 3: list with several elements
			list.PushFront(Foo(data));
			list.PushFront(Foo(data*2));
			list.PushFront(Foo(data*3));
			SList<Foo> listCopy3(list);
			Assert::IsFalse(listCopy3.IsEmpty());
			Assert::AreEqual(listCopy3.Front(), Foo(data * 3));
			Assert::AreEqual(listCopy3.Back(), Foo(data));
			Assert::AreNotSame(list.Front(), listCopy3.Front());
			Assert::AreNotSame(list.Back(), listCopy3.Back());
		}

		TEST_METHOD(Assignment)
		{
			//test 1: empty list
			SList<Foo> list;
			SList<Foo> listCopy;
			listCopy = list;
			Assert::IsTrue(listCopy.IsEmpty());

			//Test 2: list with one element
			uint16_t data = 17;
			list.PushFront(Foo(data));
			listCopy = list;
			Assert::IsFalse(listCopy.IsEmpty());
			Assert::AreEqual(listCopy.Front(), list.Front());
			Assert::AreEqual(listCopy.Back(), list.Back());
			Assert::AreNotSame(list.Front(), listCopy.Front());
			Assert::AreNotSame(list.Back(), listCopy.Back());

			//Test 3: list with several elements
			list.PushFront(Foo(data));
			list.PushFront(Foo(data * 2));
			list.PushFront(Foo(data * 3));
			listCopy = list;
			Assert::IsFalse(listCopy.IsEmpty());
			Assert::AreEqual(listCopy.Front(), Foo(data * 3));
			Assert::AreEqual(listCopy.Back(), Foo(data));
			Assert::AreNotSame(list.Front(), listCopy.Front());
			Assert::AreNotSame(list.Back(), listCopy.Back());
		}

		TEST_METHOD(Size)
		{
			//test 1: empty
			SList<Foo> list;
			Assert::AreEqual(list.Size(), 0_z);

			//test 2: adding elements
			list.PushFront(Foo(12));
			Assert::AreEqual(list.Size(), 1_z);
			list.PushFront(Foo(15));
			Assert::AreEqual(list.Size(), 2_z);

			//test 3: removing elements
			list.PopFront();
			Assert::AreEqual(list.Size(), 1_z);
			list.PopFront();
			Assert::AreEqual(list.Size(), 0_z);
		}

		TEST_METHOD(IsEmpty)
		{
			//test 1: list is empty
			SList<Foo> list;
			Assert::IsTrue(list.IsEmpty());

			//test 2: list is not empty
			int data = 10;
			list.PushFront(Foo(data));
			Assert::IsFalse(list.IsEmpty());
		}

		TEST_METHOD(FrontAndBack)
		{
			SList<Foo> list;
			const SList<Foo>& constList = list;

			//test 1: list is empty, should throw exception
			Assert::ExpectException<std::runtime_error>([&list] { list.Front(); });
			Assert::ExpectException<std::runtime_error>([&list] { list.Back(); });
			Assert::ExpectException<std::runtime_error>([&constList] { constList.Front(); });
			Assert::ExpectException<std::runtime_error>([&constList] { constList.Back(); });

			//test 2: one element, back and front should be the same
			uint16_t data = 12;
			list.PushFront(Foo(data));
			const SList<Foo>& constList2 = list;
			Assert::AreEqual(list.Front(), Foo(data));
			Assert::AreEqual(list.Back(), Foo(data));
			Assert::AreEqual(constList2.Front(), Foo(data));
			Assert::AreEqual(constList2.Back(), Foo(data));

			//Test 3: two elements, back and front should be different
			list.PushFront(Foo(data * 2));
			const SList<Foo>& constList3 = list;
			Assert::AreEqual(list.Front(), Foo(data * 2));
			Assert::AreEqual(list.Back(), Foo(data));
			Assert::AreEqual(constList3.Front(), Foo(data * 2));
			Assert::AreEqual(constList3.Back(), Foo(data));
		}

		TEST_METHOD(PushFront)
		{
			//Test one: pushing an element into an empty list
			SList<Foo> list;
			uint16_t data = 5;
			list.PushFront(Foo(data));
			Assert::IsFalse(list.IsEmpty());
			Assert::AreEqual(list.Front(), Foo(data));
			Assert::AreEqual(list.Back(), Foo(data));

			//Test 2: pushing more elements
			list.PushFront(Foo(data * 2));
			list.PushFront(Foo(data * 3));
			Assert::IsTrue(list.Size() == 3);
			Assert::AreEqual(list.Front(), Foo(data * 3));
			Assert::AreEqual(list.Back(), Foo(data));
		}

		TEST_METHOD(PushBack)
		{
			//Test one: pushing an element into an empty list
			SList<Foo> list;
			uint16_t data = 5;
			SList<Foo>::Iterator it = list.PushBack(Foo(data));
			Assert::IsFalse(list.IsEmpty());
			Assert::AreEqual(list.Front(), Foo(data));
			Assert::AreEqual(list.Back(), Foo(data));
			Assert::AreEqual(*it, Foo(data));

			//Test 2: pushing more elements
			list.PushBack(Foo(data * 2));
			list.PushBack(Foo(data * 3));
			Assert::IsTrue(list.Size() == 3);
			Assert::AreEqual(list.Front(), Foo(data));
			Assert::AreEqual(list.Back(), Foo(data * 3));
		}

		TEST_METHOD(PushBackRValue)
		{
			//Test one: pushing an element into an empty list
			SList<Foo> list;
			uint16_t data = 5;
			Foo a(data);
			SList<Foo>::Iterator it = list.PushBack(std::move(a));
			Assert::IsFalse(list.IsEmpty());
			Assert::AreEqual(list.Front(), Foo(data));
			Assert::AreEqual(list.Back(), Foo(data));
			Assert::AreEqual(*it, Foo(data));

			//Test 2: pushing more elements
			list.PushBack(Foo(data * 2));
			list.PushBack(Foo(data * 3));
			Assert::IsTrue(list.Size() == 3);
			Assert::AreEqual(list.Front(), Foo(data));
			Assert::AreEqual(list.Back(), Foo(data * 3));
		}

		TEST_METHOD(PopFront)
		{
			SList<Foo> list;
			uint16_t data = 5;
			list.PushFront(Foo(data));
			list.PushFront(Foo(data*2));
			list.PushFront(Foo(data*3));

			//Test one: removing from a list of 3 or more
			list.PopFront();
			Assert::IsTrue(list.Size() == 2);
			Assert::AreEqual(list.Front(), Foo(data * 2));
			Assert::AreEqual(list.Back(), Foo(data));
			
			//test two: removing from a list of 2
			list.PopFront();
			Assert::IsTrue(list.Size() == 1);
			Assert::AreEqual(list.Front(), Foo(data));
			Assert::AreEqual(list.Back(), list.Front());

			//test three: removing only element in a list
			list.PopFront();
			Assert::IsTrue(list.IsEmpty());
		}

		TEST_METHOD(PopBack)
		{
			SList<Foo> list;
			uint16_t data = 5;
			list.PushBack(Foo(data));
			list.PushBack(Foo(data * 2));
			list.PushBack(Foo(data * 3));

			//Test one: removing from a list of 3 or more
			list.PopBack();
			Assert::IsTrue(list.Size() == 2);
			Assert::AreEqual(list.Back(), Foo(data * 2));
			Assert::AreEqual(list.Front(), Foo(data));

			//test two: removing from a list of 2
			list.PopBack();
			Assert::IsTrue(list.Size() == 1);
			Assert::AreEqual(list.Back(), Foo(data));
			Assert::AreEqual(list.Back(), list.Front());

			//test three: removing only element in a list
			list.PopBack();
			Assert::IsTrue(list.IsEmpty());
		}

		TEST_METHOD(Clear)
		{
			//Test 1: list that is already empty
			SList<Foo> list;
			list.Clear();
			Assert::IsTrue(list.IsEmpty());


			//Test 2: list with one element
			uint16_t data = 5;
			list.PushBack(Foo(data));
			list.Clear();
			Assert::IsTrue(list.IsEmpty());

			//Test 3: list with two elements
			list.PushBack(Foo(data * 2));
			list.PushBack(Foo(data * 3));
			list.Clear();
			Assert::IsTrue(list.IsEmpty());


			//Test 4: list with 3 elements
			list.PushBack(Foo(data));
			list.PushBack(Foo(data * 2));
			list.PushBack(Foo(data * 3));
			list.Clear();
			Assert::IsTrue(list.IsEmpty());
		}
	
		TEST_METHOD(MoveSemantics)
		{
			SList<Foo> list;
			uint16_t data = 5;
			list.PushBack(Foo(data));
			list.PushBack(Foo(data * 2));
			list.PushBack(Foo(data * 3));

			SList<Foo> list2(std::move(list));
			Assert::AreEqual(Foo(data), list2.Front());
			Assert::AreEqual(Foo(data * 3), list2.Back());
			Assert::AreEqual(3_z, list2.Size());

			SList<Foo> list3;
			list3 = (std::move(list2));
			Assert::AreEqual(Foo(data), list3.Front());
			Assert::AreEqual(Foo(data * 3), list3.Back());
			Assert::AreEqual(3_z, list3.Size());

			
			Assert::ExpectException<std::runtime_error>([&list3] { list3 = std::move(list3); });
		}

		TEST_METHOD(InsertAfter)
		{
			uint16_t data = 5;

			//Test 1: insert in an empty list
			SList<Foo> list;
			SList<Foo>::Iterator emptyIt = list.begin();
			SList<Foo>::Iterator frontIt = list.InsertAfter(emptyIt, Foo(data));
			Assert::AreEqual(*frontIt, Foo(data));
			Assert::AreEqual(list.Front(), Foo(data));
			Assert::AreEqual(list.Back(), Foo(data));

			//Test 2: insert at end
			SList<Foo>::Iterator backIt = list.InsertAfter(list.end(), Foo(data * 2));
			Assert::AreEqual(*backIt, Foo(data * 2));
			Assert::AreEqual(list.Back(), Foo(data * 2));

			//Test 3: insert between two elements
			SList<Foo>::Iterator midIt = list.InsertAfter(list.begin(), Foo(data * 3));
			Assert::AreEqual(*midIt, Foo(data * 3));
			midIt++;
			Assert::AreEqual(*midIt, Foo(data * 2));

		}

		TEST_METHOD(Find)
		{
			uint16_t data = 5;

			//Test 1: empty list (non-const)
			SList<Foo> list;
			SList<Foo>::Iterator emptyIt = list.Find(Foo(data));
			Assert::AreEqual(emptyIt, list.end());

			//Test 1: empty list (const)
			const SList<Foo>& listConst = list;
			SList<Foo>::ConstIterator emptyItConst = listConst.Find(Foo(data));
			Assert::AreEqual(emptyItConst, listConst.end());

			//Test 2: exists in list (non-const)
			list.PushBack(Foo(data));
			list.PushBack(Foo(data * 2));
			list.PushBack(Foo(data * 3));
			SList<Foo>::Iterator validIt = list.Find(Foo(data * 2));
			Assert::AreEqual(*validIt, Foo(data * 2));
			validIt++;
			Assert::AreEqual(*validIt, list.Back());

			//Test 2: exists in list (const)
			const SList<Foo>& listConst2 = list;
			SList<Foo>::ConstIterator validItConst = listConst2.Find(Foo(data * 2));
			Assert::AreEqual(*validItConst, Foo(data * 2));
			validItConst++;
			Assert::AreEqual(*validItConst, listConst2.Back());

			//Test 3: does not exist in list (non-const)
			SList<Foo>::Iterator invalidIt = list.Find(Foo(data * 4));
			Assert::AreEqual(invalidIt, list.end());

			//Test 3: does not exist in list (non-const)
			SList<Foo>::ConstIterator invalidItConst = listConst2.Find(Foo(data * 4));
			Assert::AreEqual(invalidItConst, list.cend());
		}

		TEST_METHOD(Remove)
		{
			uint16_t data = 5;

			//Test 1: remove  from empty list
			SList<Foo> list;
			list.Remove(list.begin());
			
			//Test 2: remove the only element in the list
			list.PushFront(Foo(data));
			Assert::AreEqual(list.Size(), 1_z);
			list.Remove(list.begin());
			Assert::IsTrue(list.IsEmpty());
			Assert::ExpectException<std::runtime_error>([&list] { list.Front(); });
			Assert::ExpectException<std::runtime_error>([&list] { list.Back(); });

			//Test 3: Remove last element
			list.PushFront(Foo(data));
			list.PushFront(Foo(data * 2));
			list.PushFront(Foo(data * 3));
			list.Remove(list.Find(Foo(data)));
			Assert::AreEqual(list.Size(), 2_z);
			Assert::AreEqual(list.Front(), Foo(data * 3));
			Assert::AreEqual(list.Back(), Foo(data * 2));

			//Test 4: remove element that's not front or back
			list.PushFront(Foo(data * 4));
			list.PushFront(Foo(data * 5));
			list.Remove(list.Find(Foo(data * 3)));
			Assert::AreEqual(list.Size(), 3_z);
			Assert::AreEqual(list.Front(), Foo(data * 5));
			Assert::AreEqual(list.Back(), Foo(data * 2));
			Assert::AreEqual(list.Find(Foo(data * 3)), list.end());

			//Test 5: iterator does not belong to this list
			SList<Foo> list2;
			Assert::ExpectException<std::runtime_error>([&list, &list2] { list.Remove(list2.begin()); });
		}

		/************************************************************************/
		/*************************Iterator Functions*****************************/
		/************************************************************************/
		TEST_METHOD(begin) {
			//test 1: empty list (non-const)
			SList<Foo> list;
			SList<Foo>::Iterator start(list.begin());
			Assert::ExpectException<std::runtime_error>([&start] { *start; }); //throws error because mNode = nullptr (can't dereference nullptr)

			//test 1: empty list (const)
			const SList<Foo>& listConst = list;
			SList<Foo>::ConstIterator startConst(listConst.begin());
			Assert::ExpectException<std::runtime_error>([&startConst] { *startConst; });

			//Test 2: list with items (non-const)
			uint16_t data = 5;
			list.PushBack(Foo(data));
			list.PushBack(Foo(data * 2));
			list.PushBack(Foo(data * 3));
			SList<Foo>::Iterator start2(list.begin());
			Assert::AreEqual(*start2, Foo(data));

			//Test 2: list with items (const)
			const SList<Foo>& listConst2 = list;
			SList<Foo>::ConstIterator startConst2(listConst2.begin());
			Assert::AreEqual(*startConst2, Foo(data));
		}

		TEST_METHOD(end) {
			//test 1: empty list (non-const)
			SList<Foo> list;
			SList<Foo>::Iterator start(list.end());
			Assert::ExpectException<std::runtime_error>([&start] { *start; }); //throws error because mNode = nullptr (can't dereference nullptr)

			//test 1: empty list (const)
			const SList<Foo>& listConst = list;
			SList<Foo>::ConstIterator startConst(listConst.end());
			Assert::ExpectException<std::runtime_error>([&startConst] { *startConst; });

			//Test 2: list with items (non-const)
			uint16_t data = 5;
			list.PushBack(Foo(data));
			list.PushBack(Foo(data * 2));
			list.PushBack(Foo(data * 3));
			SList<Foo>::Iterator start2(list.end());
			Assert::ExpectException<std::runtime_error>([&start2] { *start2; });

			//Test 2: list with items (const)
			const SList<Foo>& listConst2 = list;
			SList<Foo>::ConstIterator startConst2(listConst2.end());
			Assert::ExpectException<std::runtime_error>([&startConst2] { *startConst2; });
		}

		TEST_METHOD(ExtraConstructorTests)
		{
			//testing owner = nullptr runtime errors 
			Assert::ExpectException<std::runtime_error>([] { SList<Foo>::ConstIterator constIt(nullptr); });
			Assert::ExpectException<std::runtime_error>([] { SList<Foo>::ConstIterator constIt(nullptr, nullptr); });
			Assert::ExpectException<std::runtime_error>([] { SList<Foo>::Iterator it(nullptr); });
			Assert::ExpectException<std::runtime_error>([] { SList<Foo>::Iterator it(nullptr, nullptr); });
		}

		TEST_METHOD(ConstIteratorConstructor)
		{
			SList<Foo> list;
			uint16_t data = 5;
			list.PushBack(Foo(data));
			list.PushBack(Foo(data * 2));
			list.PushBack(Foo(data * 3));

			SList<Foo>::Iterator start = list.begin();
			SList<Foo>::ConstIterator startConst(start);
			SList<Foo>::ConstIterator startConst2 = list.cbegin();

			Assert::AreEqual(*start, *startConst);
			Assert::AreEqual(startConst, startConst2);
		}

		TEST_METHOD(Dereference)
		{
			//test 1: null pointer (non-const)
			SList<Foo> list;
			SList<Foo>::Iterator start(&list, nullptr);
			Assert::ExpectException<std::runtime_error>([&start] { *start; });

			//test 1: null pointer (const)
			SList<Foo>::ConstIterator startConst(&list, nullptr);
			Assert::ExpectException<std::runtime_error>([&startConst] { *startConst; });

			//Test 2: valid iterator (non-const)
			uint16_t data = 5;
			list.PushBack(Foo(data));
			SList<Foo>::Iterator start2(&list);
			Assert::AreEqual(*start2, Foo(data));

			//Test 2: valid iterator (const)
			SList<Foo>::ConstIterator start2Const(&list);
			Assert::AreEqual(*start2Const, Foo(data));
		}

		TEST_METHOD(Equality)
		{
			//test 1: are equal (non-const)
			SList<Foo> list;
			uint16_t data = 5;
			list.PushBack(Foo(data));
			list.PushBack(Foo(data * 2));
			list.PushBack(Foo(data * 3));
			SList<Foo>::Iterator start(&list);
			SList<Foo>::Iterator start2(&list);
			Assert::AreEqual(start, start2);

			//test 1: are equal (const)
			SList<Foo>::ConstIterator constStart(&list);
			SList<Foo>::ConstIterator constStart2(&list);
			Assert::AreEqual(constStart, constStart2);

			//Test 2: Same owner, different node (not equal) (non-const)
			SList<Foo>::Iterator back(list.end());
			Assert::IsTrue(back != start);	//Assert::AreNotEqual does not invoke operator!=

			//Test 2: Same owner, different node (not equal) (const)
			SList<Foo>::ConstIterator constBack = list.cend();
			Assert::IsTrue(constBack != constStart);

			//Test 3: Different owners, same node (not equal) (non-const)
			SList<Foo> list2;
			SList<Foo>::Iterator diffBack(list2.end());
			Assert::IsTrue(back != diffBack);	//Assert::AreNotEqual does not invoke operator!=

			//Test 3: Different owners, same node (not equal) (const)
			SList<Foo>::Iterator constDiffBack(list2.end());
			Assert::IsTrue(constBack != constDiffBack);	//Assert::AreNotEqual does not invoke operator!=
		}

		TEST_METHOD(Increment)
		{
			//test 1: increment when mNode = nullptr (non-const)
			SList<Foo> list; 
			SList<Foo>::Iterator test(&list);
			Assert::ExpectException<std::runtime_error>([&test] { test++; });
			Assert::ExpectException<std::runtime_error>([&test] { ++test; });

			//test 1: increment when mNode = nullptr (const)
			SList<Foo>::ConstIterator testConst(&list);
			Assert::ExpectException<std::runtime_error>([&testConst] { testConst++; });
			Assert::ExpectException<std::runtime_error>([&testConst] { ++testConst; });

			//Test 2: increment when mNode->Next = nullptr (attempt to increment out of list) (non-const)
			uint16_t data = 5;
			list.PushBack(Foo(data));
			SList<Foo>::Iterator test2(&list);
			Assert::AreEqual(++test2, list.end());
			Assert::ExpectException<std::runtime_error>([&test2] { test2++; });
			Assert::ExpectException<std::runtime_error>([&test2] { ++test2; });

			//Test 2: increment when mNode->Next = nullptr (attempt to increment out of list) (const)
			SList<Foo>::ConstIterator testConst2(&list);
			Assert::AreEqual(++testConst2, list.cend());
			Assert::ExpectException<std::runtime_error>([&testConst2] { testConst2++; });
			Assert::ExpectException<std::runtime_error>([&testConst2] { ++testConst2; });

			//test 3: increment normally (non-const)
			list.PushBack(Foo(data * 2));
			list.PushBack(Foo(data * 3));
			SList<Foo>::Iterator test3(&list);
			test3++;
			Assert::AreEqual(*test3, Foo(data * 2));
			++test3;
			Assert::AreEqual(*test3, Foo(data * 3));

			//test 3: increment normally (const)
			SList<Foo>::ConstIterator testConst3(&list);
			testConst3++;
			Assert::AreEqual(*testConst3, Foo(data * 2));
			++testConst3;
			Assert::AreEqual(*testConst3, Foo(data * 3));
		}

	private:
		static _CrtMemState sStartMemState;	//for memory leak detection
	};

	_CrtMemState SListTests::sStartMemState;
}





