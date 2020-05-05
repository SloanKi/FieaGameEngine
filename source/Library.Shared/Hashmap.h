#pragma once
#include <functional>
#include "SList.h" //have to include to get iterator?
#include "DefaultHash.h"

namespace Library
{
	//forward declaration
	template <typename T>
	class Vector;
	/// <summary>
	/// Hashmap that uses Vector<SList<std::pair<const TKey, TData>>>
	/// The vector holds the buckets, and the SLists hold chains of key-data pairs.
	/// </summary>
	template <typename TKey, typename TData>
	class Hashmap
	{
	public:
		using PairType = std::pair<const TKey, TData>;
		using ChainType = SList<PairType>;
		using BucketType = Vector<ChainType>;
		using EqualityFunctor = std::function<bool(const TKey& lhs, const TKey& rhs)>;
		using HashFunctor = std::function<size_t(const TKey& key)>;
		class Iterator;			//forward declaration 
		class ConstIterator;	//forward declaration 

		static const size_t DEFAULT_CAPACITY = 5;	//default capacity

		/// <summary>
		/// Default constructor, sets all values to default initializations
		/// </summary>
		Hashmap();

		Hashmap(std::initializer_list<PairType> list);

		/// <summary>
		/// copy constructor (defaulted)
		/// </summary>
		/// <param name="rhs">the hashmap to copy</param>
		Hashmap(const Hashmap& rhs) = default;

		Hashmap(Hashmap&& rhs) noexcept;

		/// <summary>
		/// Destructor
		/// </summary>
		/// <remarks>
		/// Can be defaulted because it will automatically call the destructor of each member variable.
		/// We already implemented destructors for Vector and SList that prevent memory leaks.
		/// </remarks>
		virtual ~Hashmap() = default;

		/// <summary>
		/// assignment operator (defaulted)
		/// </summary>
		/// <param name="rhs">the hashmap to copy</param>
		/// <returns>reference to the copy of the hashmap</returns>
		Hashmap& operator=(const Hashmap& rhs) = default;

		Hashmap& operator=(Hashmap&& rhs) noexcept;

		/// <summary>
		/// Constructor that allows the user to specify the number of buckets for the hashmap
		/// As well as the HashFunctor they would like to use.
		/// </summary>
		/// <param name="capacity">The number of buckets the hashmap will have</param>
		/// <param name="hashFunc">The hash functor to use </param>
		/// <remarks>Hashfunctor is defaulted to an additive hash. </remarks>
		Hashmap(size_t capacity, HashFunctor hashFunc = DefaultHash<TKey>{});

		/// <summary>
		/// Index Operator: returns a reference to the TData of the entry with given key. 
		/// If associated key does not exist, create entry with default constructed TData
		/// </summary>
		/// <param name="key"> the key to look for</param>
		/// <returns>The reference to the TData from the entry with the given key</returns>
		TData& operator[](const TKey& key);

		/// <summary>
		/// Gets the data for a given key
		/// </summary>
		/// <param name="key">the key to get the data from</param>
		/// <returns>a refreence to the data for the given key</returns>
		/// <exception cref="std::runtime_error">Throws exception if key does not exist</exception>
		/// <remarks> Just calls At()</remarks>
		const TData& operator[](const TKey& key) const;

		/// <summary>
		/// Gets mCapacity
		/// </summary>
		/// <returns>The capacity (how many buckets there are)</returns>
		size_t Capacity() const noexcept;

		/// <summary>
		/// Gets mSize
		/// </summary>
		/// <returns>The size (how many entries are in the hashmap)</returns>
		size_t Size() const noexcept;

		/// <summary>
		/// Returns the average number of elements per bucket
		/// Size() divided by Capacity()
		/// </summary>
		/// <returns>The average number of elements per bucket</returns>
		float Load_Factor() const;

		/// <summary>
		/// Searches for a given key in the hashmap
		/// </summary>
		/// <param name="key">the key to search for</param>
		/// <returns>true if hashmap contains the key, false otherwise</returns>
		bool ContainsKey(const TKey& key) const;

		/// <summary>
		/// Inserts a given entry at the correct spot in the hashmap and returns an iterator pointing to it.
		/// If an entry with the given key already exists, just returns the iterator pointing to it
		/// </summary>
		/// <param name="entry">The key,data pair to insert into the hashmap</param>
		/// <returns>An iterator pointing to the entry with given key in the hashmap and a bool indicating whether an entry was created.</returns>
		std::pair<Iterator, bool> Insert(const PairType& entry, EqualityFunctor equalFunc = DefaultEquality<TKey>{});

		/// <summary>
		/// Searches for a given key in the hashmap
		/// </summary>
		/// <param name="key">The key to search for</param>
		/// <returns>
		/// An iterator pointing to the entry with given key in the hashmap, or end() if not found
		/// </returns>
		Iterator Find(const TKey& key, EqualityFunctor equalFunc = DefaultEquality<TKey>{});

		/// <summary>
		/// Searches for a given key in the hashmap
		/// </summary>
		/// <param name="key">The key to search for</param>
		/// <returns>
		/// A ConstIterator pointing to the entry with given key in the hashmap, or cend() if not found
		/// </returns>
		ConstIterator Find(const TKey& key, EqualityFunctor equalFunc = DefaultEquality<TKey>{}) const;

		/// <summary>
		/// Gets the data for a given key
		/// </summary>
		/// <param name="key">the key to get the data from</param>
		/// <returns>a refreence to the data for the given key</returns>
		/// <exception cref="std::runtime_error">Throws exception if key does not exist</exception>
		TData& At(const TKey& key);

		/// <summary>
		/// Gets the data for a given key
		/// </summary>
		/// <param name="key">the key to get the data from</param>
		/// <returns>a constant reference to the data for the given key</returns>
		/// <exception cref="std::runtime_error">Throws exception if key does not exist</exception>
		const TData& At(const TKey& key) const;

		/// <summary>
		/// Empties the table
		/// </summary>
		void Clear();

		/// <summary>
		/// Removes the entry matching the given key. If element does not exist, does nothing.
		/// </summary>
		/// <param name="key">the key of the entry to remove</param>
		void Remove(const TKey& key, EqualityFunctor equalFunc = DefaultEquality<TKey>{});

		/// <summary>
		/// Get an iterator pointing to the starting contents of the hashmap
		/// </summary>
		/// <returns>An iterator pointing to the first item in the hashmap</returns>
		Iterator begin();

		/// <summary>
		/// Get a constIterator pointing to the starting contents of the hashmap
		/// </summary>
		/// <returns>A constIterator pointing to the first item in the hashmap</returns>
		ConstIterator begin() const;

		/// <summary>
		/// Get a constIterator pointing to the starting contents of the hashmap
		/// </summary>
		/// <returns>A constIterator pointing to the first item in the hashmap</returns>
		ConstIterator cbegin() const;

		/// <summary>
		/// Get an iterator that can be used to determine when a loop is done
		/// </summary>
		/// <returns>An iterator with a bucketIndex = mCapacity </returns>
		Iterator end();

		/// <summary>
		/// Get a constIterator that can be used to determine when a loop is done
		/// </summary>
		/// <returns>A constIterator with a bucketIndex = mCapacity </returns>
		ConstIterator end() const;

		/// <summary>
		/// Get a constIterator that can be used to determine when a loop is done
		/// </summary>
		/// <returns>A constIterator with a bucketIndex = mCapacity </returns>
		ConstIterator cend() const;

	private:
		HashFunctor mHashFunc = DefaultHash<TKey>{};
		size_t mCapacity = 0;	//how many buckets we have
		size_t mSize = 0;		//how many buckets have items
		BucketType mBuckets;	//Vector<SList<std::pair<const TKey, TData>>>


	public:
		class Iterator
		{
			friend Hashmap;
			friend ConstIterator;

		public:
			/// <summary>
			/// Default constructor
			/// </summary>
			Iterator() = default;

			/// <summary>
			/// Copy constructor: copies member variables from rhs
			/// </summary>
			/// <param name="rhs">the iterator to copy</param>
			Iterator(const Iterator& rhs) = default;

			/// <summary>
			/// Destructor
			/// </summary>
			~Iterator() = default;

			/// <summary>
			/// Assignment operator: copies member variables from rhs
			/// </summary>
			/// <param name="rhs">the iterator to copy</param>
			/// <returns>A reference of a copy of rhs</returns>
			Iterator& operator=(const Iterator& rhs) = default;

			/// <summary>
			/// Comparison Operator
			/// </summary>
			/// <returns>True if owner, index, and chainIt are equal, false otherwise.</returns>
			bool operator==(const Iterator& rhs) const noexcept;

			/// <summary>
			/// Comparison Operator (not equal)
			/// </summary>
			/// <returns>True if not equal, false if equal</returns>
			bool operator!=(const Iterator& rhs) const noexcept;

			/// <summary>
			/// Dereference operator
			/// </summary>
			/// <returns>The data held where the iterator points to</returns>
			/// <exception cref="std::runtime_error">
			/// Throws exception if bucketIndex >= capacity or if mChainIterator is invalid.
			/// </exception>
			PairType& operator*() const;

			PairType* operator->() const;

			/// <summary>
			/// Prefix Increment operator-increments to the next element
			/// </summary>
			/// <returns>This Iterator incremented to the next element</returns>
			/// <exception cref="std::runtime_error">Throws exception if incrementing out of list bounds</exception>
			Iterator& operator++();

			/// <summary>
			/// Postfix Increment operator-increments to the next element
			/// </summary>
			/// <returns>This Iterator incremented to the next element</returns>
			/// <remarks>For performance reasons, actually calls the prefix increment operator.</remarks>
			/// <exception cref="std::runtime_error">Throws exception if incrementing out of list bounds</exception>
			Iterator operator++(int);

		private:
			Iterator(Hashmap<TKey, TData>& owner, size_t bucketIndex, typename ChainType::Iterator chainIt);

			Hashmap<TKey, TData>* mOwner = nullptr;
			size_t mBucketIndex = 0;
			typename ChainType::Iterator mChainIterator;
		};


		class ConstIterator
		{
			friend Hashmap;
		public:
			/// <summary>
			/// Default constructor
			/// </summary>
			ConstIterator() = default;

			/// <summary>
			/// Copy constructor: copies member variables from rhs
			/// </summary>
			/// <param name="rhs">the ConstIterator to copy</param>
			ConstIterator(const ConstIterator& rhs) = default;

			/// <summary>
			/// Constructor that creates a ConstIterator from an Iterator
			/// </summary>
			/// <param name="rhs"> The Iterator to copy into a ConstIterator </param>
			ConstIterator(const Iterator& rhs);

			/// <summary>
			/// Destructor
			/// </summary>
			~ConstIterator() = default;

			/// <summary>
			/// Assignment operator: copies member variables from rhs
			/// </summary>
			/// <param name="rhs">the ConstIterator to copy</param>
			/// <returns>A reference of a copy of rhs</returns>
			ConstIterator& operator=(const ConstIterator& rhs) = default;

			/// <summary>
			/// Comparison Operator
			/// </summary>
			/// <returns>True if owner, index, and chainIt are equal, false otherwise.</returns>
			bool operator==(const ConstIterator& rhs) const noexcept;

			/// <summary>
			/// Comparison Operator (not equal)
			/// </summary>
			/// <returns>True if not equal, false if equal</returns>
			bool operator!=(const ConstIterator& rhs) const noexcept;

			/// <summary>
			/// Dereference operator
			/// </summary>
			/// <returns>The data held where the ConstIterator points to</returns>
			/// <exception cref="std::runtime_error">
			/// Throws exception if bucketIndex >= capacity or if mChainConstIterator is invalid.
			/// </exception>
			const PairType& operator*() const;

			const PairType* operator->() const;

			/// <summary>
			/// Prefix Increment operator-increments to the next element
			/// </summary>
			/// <returns>This ConstIterator incremented to the next element</returns>
			/// <exception cref="std::runtime_error">Throws exception if incrementing out of list bounds</exception>
			ConstIterator& operator++();

			/// <summary>
			/// Postfix Increment operator-increments to the next element
			/// </summary>
			/// <returns>This ConstIterator incremented to the next element</returns>
			/// <remarks>For performance reasons, actually calls the prefix increment operator.</remarks>
			/// <exception cref="std::runtime_error">Throws exception if incrementing out of list bounds</exception>
			ConstIterator operator++(int);

		private:
			ConstIterator(const Hashmap<TKey, TData>& owner, size_t bucketIndex, typename ChainType::ConstIterator chainIt);

			const Hashmap<TKey, TData>* mOwner = nullptr;
			size_t mBucketIndex = 0;
			typename ChainType::ConstIterator mChainIterator;
		};
	};
}

#include "Hashmap.inl"