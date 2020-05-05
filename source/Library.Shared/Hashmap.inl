#include "Hashmap.h"
//#include "SList.h"
#include "vector.h"
#include "DefaultHash.h"

namespace Library
{
	template<typename TKey, typename TData>
	inline Hashmap<TKey, TData>::Hashmap()
	{
		mBuckets.Resize(DEFAULT_CAPACITY);
	}

	template<typename TKey, typename TData>
	inline Hashmap<TKey, TData>::Hashmap(std::initializer_list<PairType> list)
	{
		mBuckets.Resize(list.size());

		for (const auto& value : list)
		{
			Insert(value);
		}
	}

	template<typename TKey, typename TData>
	inline Hashmap<TKey, TData>::Hashmap(Hashmap&& rhs) noexcept :
		mSize(rhs.mSize), mBuckets(std::move(rhs.mBuckets)), mHashFunc(std::move(rhs.mHashFunc))
	{
		rhs.mSize = 0;
	}

	template<typename TKey, typename TData>
	inline Hashmap<TKey, TData>& Hashmap<TKey, TData>::operator=(Hashmap&& rhs) noexcept
	{
		if (this != &rhs)
		{
			mSize = rhs.mSize;
			mBuckets = std::move(rhs.mBuckets);
			mHashFunc = std::move(rhs.mHashFunc);
			rhs.mSize = 0;
		}
		return *this;
	}

	template<typename TKey, typename TData>
	inline Hashmap<TKey, TData>::Hashmap(size_t capacity, HashFunctor hashFunc) :
		mHashFunc(hashFunc)
	{
		if (capacity == 0) { capacity = DEFAULT_CAPACITY; }
		mBuckets.Resize(capacity);
	}

	template<typename TKey, typename TData>
	inline TData& Hashmap<TKey, TData>::operator[](const TKey& key)
	{
		return Insert(std::pair<TKey, TData>(key, TData())).first->second;
	}

	template<typename TKey, typename TData>
	inline const TData& Hashmap<TKey, TData>::operator[](const TKey& key) const
	{
		return At(key);
	}

	template<typename TKey, typename TData>
	inline size_t Hashmap<TKey, TData>::Capacity() const noexcept
	{
		return mBuckets.Capacity();
	}
	
	template<typename TKey, typename TData>
	inline size_t Hashmap<TKey, TData>::Size() const noexcept
	{
		return mSize;
	}

	template<typename TKey, typename TData>
	inline float Hashmap<TKey, TData>::Load_Factor() const
	{
		if (Capacity() == 0) { return 0; }
		return static_cast<float>(mSize) / Capacity();
	}

	template<typename TKey, typename TData>
	inline bool Hashmap<TKey, TData>::ContainsKey(const TKey& key) const
	{
		//if find returns end iterator, the key was not found. 
		return(Find(key) != end());
	}

	template<typename TKey, typename TData>
	std::pair<typename Hashmap<TKey, TData>::Iterator, bool> Hashmap<TKey, TData>::Insert(const PairType& entry, EqualityFunctor equalFunc)
	{
		bool bEntryMade = false;
		if (Capacity() == 0) { return std::make_pair(end(), bEntryMade); }

		size_t hashIndex = (mHashFunc(entry.first)) % Capacity();

		auto chainIt = mBuckets[hashIndex].Find(entry, [&equalFunc](PairType left, PairType right) {return equalFunc(left.first, right.first); });

		//not found, push new entry
		if (chainIt == mBuckets[hashIndex].end()) 
		{
			chainIt = mBuckets[hashIndex].PushBack(entry);
			mSize++;
			bEntryMade = true;
		}

		return std::make_pair(Iterator(*this, hashIndex, chainIt), bEntryMade);
	}

	template<typename TKey, typename TData>
	typename Hashmap<TKey, TData>::Iterator Hashmap<TKey, TData>::Find(const TKey& key, EqualityFunctor equalFunc)
	{
		if (Capacity() == 0 || mSize == 0) { return end(); }

		size_t hashIndex = (mHashFunc(key)) % Capacity();

		std::pair<TKey, TData> placeHolder(key, TData());
		auto chainIt = mBuckets[hashIndex].Find(placeHolder, [&equalFunc](PairType left, PairType right) {return equalFunc(left.first, right.first); });
		
		//not found, return end()
		if (chainIt == mBuckets[hashIndex].end()) { return end(); }

		return Iterator(*this, hashIndex, chainIt);
	}

	template<typename TKey, typename TData>
	typename Hashmap<TKey, TData>::ConstIterator Hashmap<TKey, TData>::Find(const TKey& key, EqualityFunctor equalFunc) const
	{
		if (Capacity() == 0) { return end(); }
		 
		size_t hashIndex = (mHashFunc(key)) % Capacity();

		std::pair<TKey, TData> placeHolder(key, TData());
		auto chainIt = mBuckets[hashIndex].Find(placeHolder, [&equalFunc](PairType left, PairType right) {return equalFunc(left.first, right.first); });

		//not found, return end()
		if (chainIt == mBuckets[hashIndex].end()) { return end(); }

		return ConstIterator(*this, hashIndex, chainIt);
	}

	template<typename TKey, typename TData>
	inline TData& Hashmap<TKey, TData>::At(const TKey& key)
	{
		Hashmap<TKey, TData>::Iterator it = Find(key);

		//key not found, throw exception
		if (it == end())
		{
			throw std::runtime_error("Key not found");
		}

		return it->second;

	}

	template<typename TKey, typename TData>
	inline const TData& Hashmap<TKey, TData>::At(const TKey& key) const
	{
		Hashmap<TKey, TData>::ConstIterator it = Find(key);

		//key not found, throw exception
		if (it == end())
		{
			throw std::runtime_error("Key not found");
		}

		return it->second;
	}

	template<typename TKey, typename TData>
	inline void Hashmap<TKey, TData>::Clear()
	{
		for (size_t i = 0; i < Capacity(); i++)
		{
			mBuckets[i].Clear();
		}
		mSize = 0;
	}

	template<typename TKey, typename TData>
	void Hashmap<TKey, TData>::Remove(const TKey& key, EqualityFunctor equalFunc)
	{
		if (Capacity() == 0 || mSize == 0) { return; }
		size_t hashIndex = (mHashFunc(key)) % Capacity();

		if (mBuckets[hashIndex].IsEmpty()) { return; }

		//find entry
		std::pair<TKey, TData> placeHolder(key, TData());
		auto chainIt = mBuckets[hashIndex].Find(placeHolder, [&equalFunc](PairType left, PairType right) {return equalFunc(left.first,right.first); });

		if (chainIt == mBuckets[hashIndex].end()) { return; }

		mBuckets[hashIndex].Remove(chainIt);
		mSize--;
	}

	template<typename TKey, typename TData>
	typename Hashmap<TKey,TData>::Iterator Hashmap<TKey, TData>::begin()
	{
		//if list is empty, begin = end
		if (mSize == 0) { return this->end(); }

		//search for first bucket with something in it
		size_t i;
		for (i = 0; i < Capacity(); i++)
		{
			if (!mBuckets[i].IsEmpty())
			{
				break;
			}
		}

		return Iterator(*this, i, mBuckets[i].begin());
	}

	template<typename TKey, typename TData>
	inline typename Hashmap<TKey, TData>::ConstIterator Hashmap<TKey, TData>::begin() const
	{
		return cbegin();
	}

	template<typename TKey, typename TData>
	typename Hashmap<TKey, TData>::ConstIterator Hashmap<TKey, TData>::cbegin() const
	{
		//if list is empty, begin = end
		if (mSize == 0) { return this->end(); }

		//search for first bucket with something in it
		size_t i;
		for (i = 0; i < Capacity(); i++)
		{
			if (!mBuckets[i].IsEmpty())
			{
				break;
			}
		}

		return ConstIterator(*this, i, mBuckets[i].begin());
	}

	template<typename TKey, typename TData>
	inline typename Hashmap<TKey, TData>::Iterator Hashmap<TKey, TData>::end()
	{
		return Iterator(*this, Capacity(), ChainType::Iterator());
	}

	template<typename TKey, typename TData>
	inline typename Hashmap<TKey, TData>::ConstIterator Hashmap<TKey, TData>::end() const
	{
		return ConstIterator(*this, Capacity(), ChainType::ConstIterator());
	}

	template<typename TKey, typename TData>
	inline typename Hashmap<TKey, TData>::ConstIterator Hashmap<TKey, TData>::cend() const
	{
		return ConstIterator(*this, Capacity(), ChainType::ConstIterator());
	}


	/************************************************************************/
	/*************************Iterator Functions*****************************/
	/************************************************************************/
	template<typename TKey, typename TData>
	inline Hashmap<TKey, TData>::Iterator::Iterator(Hashmap<TKey, TData>& owner, size_t bucketIndex, typename ChainType::Iterator chainIt) :
		mOwner(&owner), mBucketIndex(bucketIndex)
	{
		mChainIterator = chainIt; //not sure why it breaks on init line
	}

	template<typename TKey, typename TData>
	inline bool Hashmap<TKey, TData>::Iterator::operator==(const Iterator& rhs) const noexcept
	{
		return ((mOwner == rhs.mOwner) && (mBucketIndex == rhs.mBucketIndex) && (mChainIterator == rhs.mChainIterator));
	}

	template<typename TKey, typename TData>
	inline bool Hashmap<TKey, TData>::Iterator::operator!=(const Iterator& rhs) const noexcept
	{
		return !operator==(rhs);
	}

	template<typename TKey, typename TData>
	inline std::pair<const TKey, TData>& Hashmap<TKey, TData>::Iterator::operator*() const
	{
		if (mOwner == nullptr)
		{
			throw std::runtime_error("Invalid iterator owner (nullptr)");
		}
		if (mBucketIndex >= mOwner->Capacity())
		{
			throw std::runtime_error("Attempted to dereference null pointer");
		}

		/*return (*mChainIterator).second;*/
		return *mChainIterator;
	}

	template<typename TKey, typename TData>
	inline std::pair<const TKey, TData>* Hashmap<TKey, TData>::Iterator::operator->() const
	{
		if (mOwner == nullptr)
		{
			throw std::runtime_error("Invalid iterator owner (nullptr)");
		}
		if (mBucketIndex >= mOwner->Capacity())
		{
			throw std::runtime_error("Attempted to dereference null pointer");
		}

		return &(*mChainIterator);
	}

	//prefix
	template<typename TKey, typename TData>
	inline typename Hashmap<TKey, TData>::Iterator& Hashmap<TKey, TData>::Iterator::operator++()
	{
		if (mOwner == nullptr)
		{
			throw std::runtime_error("Invalid iterator owner");
		}
		
		if (mBucketIndex >= mOwner->Capacity())
		{
			throw std::runtime_error("Iterator out of bounds");
		}
		
		//if currently in an SList with more elements, increment that iterator
		if (mChainIterator != mOwner->mBuckets[mBucketIndex].end())
		{
			mChainIterator++;
			//if not at end of list, return
			if (mChainIterator != mOwner->mBuckets[mBucketIndex].end())
			{
				return *this;
			}
			else //Search for next bucket with data
			{
				//increment mBucketIndex until reach end of map or find a non-empty SList
				do 
				{
					mBucketIndex++;
				} while (mBucketIndex < mOwner->Capacity() && mOwner->mBuckets[mBucketIndex].IsEmpty());

				//either found a non-empty SList, or reached end of list
				if (mBucketIndex >= mOwner->Capacity())
				{
					*this = mOwner->end();
					return *this;
				}

				//if did not return, found a valid sList
				mChainIterator = mOwner->mBuckets[mBucketIndex].begin();
			}
		}

		return *this;
	}

	//postfix
	template<typename TKey, typename TData>
	inline typename Hashmap<TKey, TData>::Iterator Hashmap<TKey, TData>::Iterator::operator++(int)
	{
		Iterator temp = *this;
		operator++();
		return temp;
	}

	
	/************************************************************************/
	/***********************ConstIterator Functions**************************/
	/************************************************************************/
	template<typename TKey, typename TData>
	inline Hashmap<TKey, TData>::ConstIterator::ConstIterator(const Hashmap<TKey, TData>& owner, size_t bucketIndex, typename ChainType::ConstIterator chainIt) :
		mOwner(&owner), mBucketIndex(bucketIndex)
	{
		mChainIterator = chainIt;
	}

	template<typename TKey, typename TData>
	inline Hashmap<TKey, TData>::ConstIterator::ConstIterator(const Iterator& rhs) : 
		mOwner(rhs.mOwner), mBucketIndex(rhs.mBucketIndex)
	{
		mChainIterator = rhs.mChainIterator;
	}

	template<typename TKey, typename TData>
	inline bool Hashmap<TKey, TData>::ConstIterator::operator==(const ConstIterator& rhs) const noexcept
	{
		return ((mOwner == rhs.mOwner) && (mBucketIndex == rhs.mBucketIndex) && (mChainIterator == rhs.mChainIterator));
	}

	template<typename TKey, typename TData>
	inline bool Hashmap<TKey, TData>::ConstIterator::operator!=(const ConstIterator& rhs) const noexcept
	{
		return !operator==(rhs);
	}

	template<typename TKey, typename TData>
	inline const std::pair<const TKey, TData>& Hashmap<TKey, TData>::ConstIterator::operator*() const
	{
		if (mOwner == nullptr)
		{
			throw std::runtime_error("Invalid ConstIterator owner (nullptr)");
		}
		if (mBucketIndex >= mOwner->Capacity())
		{
			throw std::runtime_error("Attempted to dereference null pointer");
		}

		//return (*mChainIterator).second;
		return *mChainIterator;

	}

	template<typename TKey, typename TData>
	inline const std::pair<const TKey, TData>* Hashmap<TKey, TData>::ConstIterator::operator->() const
	{
		if (mOwner == nullptr)
		{
			throw std::runtime_error("Invalid iterator owner (nullptr)");
		}
		if (mBucketIndex >= mOwner->Capacity())
		{
			throw std::runtime_error("Attempted to dereference null pointer");
		}

		return &(*mChainIterator);
	}

	//prefix
	template<typename TKey, typename TData>
	inline typename Hashmap<TKey, TData>::ConstIterator& Hashmap<TKey, TData>::ConstIterator::operator++()
	{
		if (mOwner == nullptr)
		{
			throw std::runtime_error("Invalid ConstIterator owner");
		}

		if (mBucketIndex >= mOwner->Capacity())
		{
			throw std::runtime_error("ConstIterator out of bounds");
		}

		//if currently in an SList with more elements, increment that ConstIterator
		if (mChainIterator != mOwner->mBuckets[mBucketIndex].end())
		{
			mChainIterator++;
			//if not at end of list, return
			if (mChainIterator != mOwner->mBuckets[mBucketIndex].end())
			{
				return *this;
			}
			else //Search for next bucket with data
			{
				//increment mBucketIndex until reach end of map or find a non-empty SList
				do
				{
					mBucketIndex++;
				} while (mBucketIndex < mOwner->Capacity() && mOwner->mBuckets[mBucketIndex].IsEmpty());

				//either found a non-empty SList, or reached end of list
				if (mBucketIndex >= mOwner->Capacity())
				{
					*this = mOwner->end();
					return *this;
				}

				//if did not return, found a valid sList
				mChainIterator = mOwner->mBuckets[mBucketIndex].begin();
			}
		}

		return *this;
	}

	//postfix
	template<typename TKey, typename TData>
	inline typename Hashmap<TKey, TData>::ConstIterator Hashmap<TKey, TData>::ConstIterator::operator++(int)
	{
		ConstIterator temp = *this;
		operator++();
		return temp;
	}
}