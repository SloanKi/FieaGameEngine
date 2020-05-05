#include "pch.h"
#include "SList.h"
#include "DefaultEquality.h"

namespace Library
{
#pragma region Node
	template<typename T>
	inline SList<T>::Node::Node(const T& data, Node* next) : Data(data), Next(next) {}
	template<typename T>
	inline SList<T>::Node::Node(T&& data, Node* next) : Data(std::move(data)), Next(next) {}
#pragma endregion Node

	template<typename T>
	SList<T>::~SList()
	{
		//delete all nodes
		Clear();
	}

	template<typename T>
	SList<T>::SList(const SList<T>& origList) 
	{
		Node* copyNode = origList.mFront;
		for (size_t i = 0; i < origList.Size(); i++)
		{
			PushBack(copyNode->Data);
			copyNode = copyNode->Next;
		}
	}

	template<typename T>
	inline SList<T>::SList(SList&& rhs) :
		mSize(rhs.mSize), mFront(rhs.mFront), mBack(rhs.mBack)
	{
		rhs.mSize = 0;
		rhs.mFront = nullptr;
		rhs.mBack = nullptr;
	}

	template<typename T>
	SList<T>& SList<T>::operator=(const SList<T>& origList)
	{
		if (this != &origList) 
		{
			Clear();

			Node* copyNode = origList.mFront;
			for (size_t i = 0; i < origList.Size(); i++)
			{
				PushBack(copyNode->Data);
				copyNode = copyNode->Next;
			}
		}
		

		return *this;
	}

	template<typename T>
	inline SList<T>& SList<T>::operator=(SList&& rhs)
	{
		if (this != &rhs) {
			Clear();				//clear to prevent mem leaks

			//shallow copy
			mSize = rhs.mSize;
			mFront = rhs.mFront;
			mBack = rhs.mBack;

			//invalidate rhs
			rhs.mSize = 0;
			rhs.mFront = nullptr;
			rhs.mBack = nullptr;

			return *this;
		}
		else {
			throw std::runtime_error("Trying to move memory into current location");
		}
	}

	template<typename T>
	inline size_t SList<T>::Size() const
	{
		return mSize;
	}

	template<typename T>
	inline bool SList<T>::IsEmpty() const
	{
		return mSize == 0;
	}

	template<typename T>
	inline T& SList<T>::Front()
	{
		if (IsEmpty()) 
		{
			throw std::runtime_error("List is empty.");
		}
		return mFront->Data;
	}

	template<typename T>
	inline const T& SList<T>::Front() const
	{
		if (IsEmpty())
		{
			throw std::runtime_error("List is empty.");
		}
		return mFront->Data;
	}

	template<typename T>
	inline T& SList<T>::Back()
	{
		if (IsEmpty())
		{
			throw std::runtime_error("List is empty.");
		}
		return mBack->Data;
	}

	template<typename T>
	inline const T& SList<T>::Back() const
	{
		if (IsEmpty())
		{
			throw std::runtime_error("List is empty.");
		}
		return mBack->Data;
	}

	template<typename T>
	void SList<T>::PushFront(const T& data)
	{
		mFront = new Node(data, mFront);		//creates new node with Next* set to the current mFront, then sets mFront to this new node.
		if (IsEmpty())
		{
			mBack = mFront;
		}
		mSize++;
	}

	template<typename T>
	typename SList<T>::Iterator SList<T>::PushBack(const T& data) 
	{
		if (IsEmpty()) 
		{
			mFront = new Node(data, nullptr);
			mBack = mFront;
		}
		else {
			mBack->Next = new Node(data, nullptr);
			mBack = mBack->Next;
		}
		mSize++;
		return Iterator(this, mBack);
	}

	template<typename T>
	typename SList<T>::Iterator SList<T>::Pushback(T&& data)
	{
		if (IsEmpty())
		{
			mFront = new Node(std::move(data), nullptr);
			mBack = mFront;
		}
		else {
			mBack->Next = new Node(std::move(data), nullptr);
			mBack = mBack->Next;
		}
		mSize++;
		return Iterator(this, mBack);
	}

	template<typename T>
	void SList<T>::PopFront()
	{
		if (!IsEmpty())
		{
			Node* node = mFront->Next;
			delete mFront;
			mFront = node;

			mSize--;
			if (mSize <= 1) 
			{
				mBack = mFront;
			}
		}
	}
	
	template<typename T>
	void SList<T>::PopBack()
	{
		if (!IsEmpty())
		{
			//delete back node
			delete mBack;
			mSize--;

			//set mBack to new back node
			if (!IsEmpty()) 
			{
				mBack = mFront;
				for (size_t i = 0; i < (mSize - 1); i++)
				{
					mBack = mBack->Next;
				}
				mBack->Next = nullptr;
			}
			else 
			{
				mBack = nullptr;
				mFront = nullptr;
			}
		}
	}

	template<typename T>
	inline void SList<T>::Clear()
	{
		//delete all nodes
		while (mSize > 0)
		{
			PopFront();
		}
	}

	template<typename T>
	inline typename SList<T>::Iterator SList<T>::begin()
	{
		return SList<T>::Iterator(this, mFront);
	}

	template<typename T>
	inline typename SList<T>::ConstIterator SList<T>::begin() const
	{
		return cbegin();
	}

	template<typename T>
	inline typename SList<T>::ConstIterator SList<T>::cbegin() const
	{
		return SList<T>::ConstIterator(this, mFront);
	}

	template<typename T>
	inline typename SList<T>::Iterator SList<T>::end()
	{
		return SList<T>::Iterator(this, nullptr);	//nullptr = mBack + 1 (1 after end of array)
	}
	
	template<typename T>
	inline typename SList<T>::ConstIterator SList<T>::end() const
	{
		return cend();
	}


	template<typename T>
	inline typename SList<T>::ConstIterator SList<T>::cend() const
	{
		return SList<T>::ConstIterator(this, nullptr);	//nullptr = mBack + 1 (1 after end of array)
	}

	template<typename T>
	typename SList<T>::Iterator SList<T>::InsertAfter(Iterator it, const T& data)
	{
		if (it.mOwner != this) 
		{
			throw std::runtime_error("Iterator does not belong to this list");
		}

		//if iterator is pointing at the end or last element, do a push back
		if (it.mNode == nullptr || it.mNode->Next == nullptr)
		{
			PushBack(data);
			return Iterator(this, mBack);
		}

		Node* temp = it.mNode->Next;
		it.mNode->Next = new Node(data, temp);
		it++;
		mSize++;
		return it;
	}

	template<typename T>
	typename SList<T>::Iterator SList<T>::Find(const T& value, EqualityFunctor equalFunc)
	{
		Node* node = mFront;
		while (node != nullptr)
		{
			if (equalFunc(node->Data, value)) 
			{
				return Iterator(this, node);
			}
			node = node->Next;
		}
		
		//if here, did not find it. return an iterator pointing past the end of the list.
		return end();
	}

	template<typename T>
	typename SList<T>::ConstIterator SList<T>::Find(const T& value, EqualityFunctor equalFunc) const
	{
		Node* node = mFront;
		while (node != nullptr)
		{
			if (equalFunc(node->Data, value))
			{
				return ConstIterator(this, node);
			}
			node = node->Next;
		}

		//if here, did not find it. return an iterator pointing past the end of the list.
		return cend();
	}

	template<typename T>
	void SList<T>::Remove(Iterator it)
	{
		if (it.mOwner != this)
		{
			throw std::runtime_error("Iterator does not belong to this list");
		}

		//if pointing to end or if list is empty, do nothing
		if (it.mNode == nullptr || IsEmpty()) { return; }

		//if point to first element, pop front
		if (it.mNode == mFront)
		{
			PopFront();
			return;
		}

		//if pointing to last element, pop back
		if (it.mNode == mBack)
		{
			PopBack();
			return;
		}


		//find 
		Iterator prevIt = begin();
		while (prevIt.mNode->Next != nullptr)
		{
			//found the previous iterator, break out of loop.
			if (prevIt.mNode->Next == it.mNode) { break; }
			//if not found, increment
			prevIt++;
		}

		//did not find, do nothing. Unlikely to happen since it takes an iterator, but just in case.
		if (prevIt.mNode->Next != it.mNode) { return; }

		//found value, delete it.
		prevIt.mNode->Next = it.mNode->Next;
		it.mNode->Next = nullptr;	//just to be sure
		delete it.mNode;
		mSize--;
	}

	
	/************************************************************************/
	/*************************Iterator Functions*****************************/
	/************************************************************************/
	template<typename T>
	inline SList<T>::Iterator::Iterator(const SList<T>* owner)
	{
		if (owner == nullptr)
		{
			throw std::runtime_error("Attempted to construct an iterator without an owner");
		}
		mOwner = owner;
		mNode = mOwner->mFront;
	}

	template<typename T>
	inline SList<T>::Iterator::Iterator(const SList<T>* owner, Node* node)
	{
		if (owner == nullptr)
		{
			throw std::runtime_error("Attempted to construct an iterator without an owner");
		}
		mOwner = owner;
		mNode = node;
	}

	template<typename T>
	inline SList<T>::Iterator::Iterator(const Iterator& rhs)
	{
		if (rhs.mOwner == nullptr)
		{
			throw std::runtime_error("Attempted to construct an iterator without an owner");
		}
		mOwner = rhs.mOwner;
		mNode = rhs.mNode;
	}

	template<typename T>
	inline T& Library::SList<T>::Iterator::operator*() const
	{
		if (mNode == nullptr)
		{
			throw std::runtime_error("Attempted to dereference null pointer in SList iterator");
		}
		return mNode->Data;
	}

	template<typename T>
	inline bool SList<T>::Iterator::operator==(const Iterator& rhs) const noexcept
	{
		if (mOwner != rhs.mOwner) { return false; }
		return (mNode == rhs.mNode);	//if owners are the same, compare data
	}

	template<typename T>
	inline bool SList<T>::Iterator::operator!=(const Iterator& rhs) const noexcept
	{
		return !operator==(rhs);
	}

	//prefix
	template<typename T>
	inline typename SList<T>::Iterator& SList<T>::Iterator::operator++()
	{
		if (mNode == nullptr)
		{
			throw std::runtime_error("Incremented an iterator with a nullptr node");
		}
		mNode = mNode->Next;
		return *this;
	}

	//postfix
	template<typename T>
	inline typename SList<T>::Iterator SList<T>::Iterator::operator++(int)
	{
		Iterator temp = *this;
		operator++();
		return temp;
	}

	
	/************************************************************************/
	/***********************ConstIterator Functions**************************/
	/************************************************************************/

	template<typename T>
	inline SList<T>::ConstIterator::ConstIterator(const SList<T>* owner)
	{
		if (owner == nullptr)
		{
			throw std::runtime_error("Attempted to construct an iterator without an owner");
		}
		mOwner = owner;
		mNode = mOwner->mFront;
	}

	template<typename T>
	inline SList<T>::ConstIterator::ConstIterator(const SList<T>* owner, Node* node)
	{
		if (owner == nullptr)
		{
			throw std::runtime_error("Attempted to construct an iterator without an owner");
		}
		mOwner = owner;
		mNode = node;
	}

	template<typename T>
	inline SList<T>::ConstIterator::ConstIterator(const ConstIterator& rhs)
	{
		if (rhs.mOwner == nullptr)
		{
			throw std::runtime_error("Attempted to construct an iterator without an owner");
		}
		mOwner = rhs.mOwner;
		mNode = rhs.mNode;
	}

	template<typename T>
	inline SList<T>::ConstIterator::ConstIterator(const Iterator& rhs)
	{
		if (rhs.mOwner == nullptr)
		{
			throw std::runtime_error("Attempted to construct an iterator without an owner");
		}
		mOwner = rhs.mOwner;
		mNode = rhs.mNode;
	}

	template<typename T>
	inline const T& SList<T>::ConstIterator::operator*() const
	{
		if (mNode == nullptr)
		{
			throw std::runtime_error("Attempted to dereference null pointer in SList iterator");
		}
		return mNode->Data;
	}

	template<typename T>
	inline bool SList<T>::ConstIterator::operator==(const ConstIterator& rhs) const noexcept
	{
		if (mOwner != rhs.mOwner) { return false; }
		return (mNode == rhs.mNode);	//if owners are the same, compare data
	}

	template<typename T>
	inline bool SList<T>::ConstIterator::operator!=(const ConstIterator& rhs) const noexcept
	{
		return !operator==(rhs);
	}

	//prefix
	template<typename T>
	inline typename SList<T>::ConstIterator& SList<T>::ConstIterator::operator++()
	{
		if (mNode == nullptr)
		{
			throw std::runtime_error("Incremented an iterator with a nullptr node");
		}
		mNode = mNode->Next;
		return *this;
	}

	//postfix
	template<typename T>
	inline typename SList<T>::ConstIterator SList<T>::ConstIterator::operator++(int)
	{
		ConstIterator temp = *this;
		operator++();
		return temp;
	}
	
}