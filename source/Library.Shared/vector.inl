#include "pch.h"
#include "vector.h"


namespace Library
{
	template<typename T>
	inline Vector<T>::Vector(std::initializer_list<T> list) :
		Vector(list.size())
	{
		for (const auto& value : list)
		{
			PushBack(value);
		}
	}

	template<typename T>
	Vector<T>::Vector(const Vector& rhs)
	{
		Reserve(rhs.mCapacity);
		for (size_t i = 0; i < rhs.Size(); ++i)
		{
			PushBack(rhs[i]);
		}
	}

	template<typename T>
	inline Vector<T>::Vector(Vector&& rhs) noexcept :
		mSize(rhs.mSize), mCapacity(rhs.mCapacity), mData(rhs.mData)
	{
		rhs.mSize = 0;
		rhs.mCapacity = 0;
		rhs.mData = nullptr;
	}

	template<typename T>
	inline Vector<T>::Vector(size_t capacity)
	{
		Reserve(capacity);
	}

	template<typename T>
	inline Vector<T>::~Vector()
	{
		for (size_t i = 0; i < mSize; i++)
		{
			mData[i].~T(); //destruct each element
		}
		free(mData);
	}

	template<typename T>
	Vector<T>& Vector<T>::operator=(const Vector& rhs)
	{
		if (this != &rhs)
		{
			Clear();
			ReCapacity(rhs.mCapacity);
			for (size_t i = 0; i < rhs.Size(); ++i)
			{
				PushBack(rhs[i]);
			}
		}

		return *this;
	}

	template<typename T>
	inline Vector<T>& Vector<T>::operator=(Vector&& rhs)
	{
		if (this != &rhs) 
		{
			Clear();
			free(mData);

			//copy
			mSize = rhs.mSize;
			mCapacity = rhs.mCapacity;
			mData = rhs.mData;

			//invalidate
			rhs.mSize = 0;
			rhs.mCapacity = 0;
			rhs.mData = nullptr;
		}

		return *this;
	}

	template<typename T>
	inline bool Vector<T>::operator==(const Vector& rhs) const noexcept
	{
		if (this == &rhs) { return true; }
		if (mSize != rhs.mSize) { return false; }
		for (size_t i = 0; i < mSize; i++)
		{
			if ((*this)[i] != rhs[i]) { return false; }
		}
		return true;
	}

	template<typename T>
	inline bool Vector<T>::operator!=(const Vector& rhs) const noexcept
	{
		return !operator==(rhs);
	}

	template<typename T>
	inline T& Vector<T>::operator[](size_t index)
	{
		if (index >= mSize)
		{
			throw std::runtime_error("Index out of bounds");
		}
		return mData[index];
	}

	template<typename T>
	inline const T& Vector<T>::operator[](size_t index) const
	{
		if (index >= mSize)
		{
			throw std::runtime_error("Index out of bounds");
		}
		return mData[index];
	}

	template<typename T>
	inline size_t Vector<T>::Size() const noexcept
	{
		return mSize;
	}

	template<typename T>
	inline bool Vector<T>::IsEmpty() const noexcept
	{
		return mSize == 0;
	}
	
	template<typename T>
	inline size_t Vector<T>::Capacity() const noexcept
	{
		return mCapacity;
	}

	template<typename T>
	void Vector<T>::Reserve(size_t capacity)
	{
		if (mCapacity >= capacity) { return; }	//can't shrink array
		ReCapacity(capacity);
	}

	template<typename T>
	void Vector<T>::ReCapacity(size_t capacity)
	{
		if (capacity == 0)
		{
			Clear();
			free(mData);
			mData = nullptr;
			mCapacity = 0;
			return;
		}

		//remove items that won't fit in new vector
		if (capacity < mSize)
		{
			for (size_t i = mSize - 1; i >= capacity; i--)
			{
				mData[i].~T(); //destruct each element
			}
		}

		T* newData = reinterpret_cast<T*>(realloc(mData, capacity * sizeof(T)));

		//protect pointer in the unlikely case realloc fails
		if (newData == nullptr)
		{
			throw std::runtime_error("realloc failed");
		}

		mData = newData;
		mSize = std::min(mSize, capacity);
		mCapacity = capacity;
	}

	template<typename T>
	inline void Vector<T>::Resize(size_t size)
	{
		if (size == mSize) { return; }	//early exit, no resizing needed if already at desired size
		
		//will shrink/grow the allocated memory
		//Note: mSize is only changed in reCapacity if it shrunk. 
		ReCapacity(size);

		//default construct items to fill space if size grows
		for (size_t i = mSize; i < size; i++)
		{
			new(mData + i)T();
		}

		mSize = size;

	}

	template<typename T>
	void Vector<T>::PushBack(const T& data)
	{
		size_t newCapacity = 0;

		//if no memory is currently allocated, set to default capacity.
		if (mCapacity == 0) { newCapacity = Vector::DEFAULT_CAPACITY; }

		//if vector is full, increase capacity
		else if (mSize == mCapacity)
		{
			newCapacity = mCapacity + (mCapacity / 2)  + 1;	//Add half the current capacity + 1 each capacity increment
		}
		Reserve(newCapacity);
		
		new(mData + mSize)T(data);	//place new into vector
		mSize++;
	}

	template<typename T>
	inline void Vector<T>::PushBack(T&& data)
	{
		size_t newCapacity = 0;

		//if no memory is currently allocated, set to default capacity.
		if (mCapacity == 0) { newCapacity = Vector::DEFAULT_CAPACITY; }

		//if vector is full, increase capacity
		else if (mSize == mCapacity)
		{
			newCapacity = mCapacity + (mCapacity / 2) + 1;	//Add half the current capacity + 1 each capacity increment
		}
		Reserve(newCapacity);

		new(mData + mSize)T(std::move(data));	//place new into vector
		mSize++;

	}

	template<typename T>
	inline void Vector<T>::PopBack()
	{
		if (IsEmpty()) { return; }
		mData[mSize - 1].~T();	//destruct last element
		mSize--;
	}
	
	template<typename T>
	inline void Vector<T>::Clear()
	{
		while (mSize > 0)
		{
			PopBack();
		}
	}

	template<typename T>
	inline typename Vector<T>::Iterator Vector<T>::begin()
	{
		return Vector<T>::Iterator(*this, 0);
	}
	
	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::begin() const
	{
		return cbegin();
	}
	
	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::cbegin() const
	{
		return Vector<T>::ConstIterator(*this, 0);
	}

	template<typename T>
	inline typename Vector<T>::Iterator Vector<T>::end()
	{
		return Vector<T>::Iterator(*this, mSize);
	}
	
	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::end() const
	{
		return cend();
	}
	
	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::cend() const
	{
		return Vector<T>::ConstIterator(*this, mSize);
	}

	template<typename T>
	inline typename Vector<T>::Iterator Vector<T>::Find(const T& value)
	{
		Iterator it = begin();
		for (; it != end(); ++it)
		{
			if (*it == value) { break; }
		}
		return it;
	}

	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::Find(const T& value) const
	{
		ConstIterator it = begin();
		for (; it != end(); ++it)
		{
			if (*it == value) { break; }
		}
		return it;
	}

	template<typename T>
	void Vector<T>::Remove(const Iterator& it)
	{
		if (it.mOwner != this)
		{
			throw std::runtime_error("Iterator does not belong to this list");
		}

		if (IsEmpty() || it == end() || it.mIndex >= mSize) { return; }

		if (it.mIndex == mSize - 1)
		{
			PopBack();
			return;
		}

		//destruct element we are removing
		mData[it.mIndex].~T();

		//shift all elements after the element we remove
		size_t dataToShift = mSize - it.mIndex - 1;
		memmove((mData + it.mIndex), (mData + it.mIndex + 1), (dataToShift * sizeof(T)));
		mSize--;
	}


	/************************************************************************/
	/*************************Iterator Functions*****************************/
	/************************************************************************/
	template<typename T>
	inline Vector<T>::Iterator::Iterator(Vector& owner, size_t index)
	{
		if (index > owner.Size())
		{
			throw std::runtime_error("Iterator out of bounds");
		}

		mOwner = &owner;
		mIndex = index;
	}

	template<typename T>
	inline T& Vector<T>::Iterator::operator*() const
	{
		if (mOwner == nullptr)
		{
			throw std::runtime_error("Invalid iterator owner (nullptr)");
		}
		if (mIndex >= mOwner->Size())
		{
			throw std::runtime_error("Attempted to dereference null pointer");
		}
		return (*mOwner)[mIndex];
	}

	template<typename T>
	inline T* Vector<T>::Iterator::operator->() const
	{
		if (mOwner == nullptr)
		{
			throw std::runtime_error("Invalid iterator owner (nullptr)");
		}
		if (mIndex >= mOwner->Size())
		{
			throw std::runtime_error("Attempted to dereference null pointer");
		}
		return &(*mOwner)[mIndex];
	}

	template<typename T>
	inline bool Vector<T>::Iterator::operator==(const Iterator& rhs) const noexcept
	{
		return ((mOwner == rhs.mOwner) && (mIndex == rhs.mIndex));
	}

	template<typename T>
	inline bool Vector<T>::Iterator::operator!=(const Iterator& rhs) const noexcept
	{
		return !operator==(rhs);
	}

	//prefix
	template<typename T>
	inline typename Vector<T>::Iterator& Vector<T>::Iterator::operator++()
	{
		if (mOwner == nullptr)
		{
			throw std::runtime_error("Invalid iterator owner");
		}
		if (mIndex >= mOwner->Size())
		{
			throw std::runtime_error("Iterator out of bounds");
		}

		++mIndex;
		return *this;
	}

	//postfix
	template<typename T>
	inline typename Vector<T>::Iterator Vector<T>::Iterator::operator++(int)
	{
		Iterator temp = *this;
		operator++();
		return temp;
	}

	template<typename T>
	inline size_t Vector<T>::Iterator::Index() const
	{
		return mIndex;
	}


	/************************************************************************/
	/***********************ConstIterator Functions**************************/
	/************************************************************************/
	template<typename T>
	inline Vector<T>::ConstIterator::ConstIterator(const Vector& owner, size_t index)
	{
		if (index > owner.Size())
		{
			throw std::runtime_error("ConstIterator out of bounds");
		}

		mOwner = &owner;
		mIndex = index;
	}

	template<typename T>
	inline Vector<T>::ConstIterator::ConstIterator(const Iterator& rhs)
	{
		if (rhs.mOwner == nullptr)
		{
			throw std::runtime_error("Attempted to construct a ConstIterator without an owner");
		}
		if (rhs.mIndex > rhs.mOwner->Size())
		{
			throw std::runtime_error("ConstIterator out of bounds");
		}

		mOwner = rhs.mOwner;
		mIndex = rhs.mIndex;
	}

	template<typename T>
	inline const T& Library::Vector<T>::ConstIterator::operator*() const
	{
		if (mOwner == nullptr)
		{
			throw std::runtime_error("Invalid iterator owner (nullptr)");
		}
		if (mIndex >= mOwner->Size())
		{
			throw std::runtime_error("Attempted to dereference null pointer");
		}
		return (*mOwner)[mIndex];
	}

	template<typename T>
	inline const T* Vector<T>::ConstIterator::operator->() const
	{
		if (mOwner == nullptr)
		{
			throw std::runtime_error("Invalid iterator owner (nullptr)");
		}
		if (mIndex >= mOwner->Size())
		{
			throw std::runtime_error("Attempted to dereference null pointer");
		}
		return &(*mOwner)[mIndex];
	}

	template<typename T>
	inline bool Vector<T>::ConstIterator::operator==(const ConstIterator& rhs) const noexcept
	{
		return ((mOwner == rhs.mOwner) && (mIndex == rhs.mIndex));
	}

	template<typename T>
	inline bool Vector<T>::ConstIterator::operator!=(const ConstIterator& rhs) const noexcept
	{
		return !operator==(rhs);
	}

	//prefix
	template<typename T>
	inline typename Vector<T>::ConstIterator& Vector<T>::ConstIterator::operator++()
	{
		if (mOwner == nullptr)
		{
			throw std::runtime_error("Invalid ConstIterator owner");
		}
		if (mIndex >= mOwner->Size())
		{
			throw std::runtime_error("ConstIterator out of bounds");
		}

		++mIndex;
		return *this;
	}

	//postfix
	template<typename T>
	inline typename Vector<T>::ConstIterator Vector<T>::ConstIterator::operator++(int)
	{
		ConstIterator temp = *this;
		operator++();
		return temp;
	}

	template<typename T>
	inline size_t Vector<T>::ConstIterator::Index() const
	{
		return mIndex;
	}
}
