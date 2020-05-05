#pragma once
#include "RTTI.h"
#include "Datum.h"
#include "Hashmap.h"
#include "vector.h"
#include <string>
#include <gsl/gsl>


namespace Library
{
	class Scope : public RTTI
	{
		RTTI_DECLARATIONS(Scope, RTTI)

		//using PairType = std::pair<const std::string, Datum>;
	public:
		static const size_t DEFAULT_CAPACITY = 5;	//default capacity

		/// <summary>
		/// Constructor
		/// Allocates memory for capacity elements.
		/// If no capacity is given, allocates memory for DEFAULT_CAPACITY elements.
		/// </summary>
		/// <param name="capacity">The number of elements to reserve memory for</param>
		/// <exception cref="std::runtime_error">Throws exception if capacity == 0</exception>
		explicit Scope(size_t capacity = DEFAULT_CAPACITY);

		/// <summary>
		/// Copy Constructor: Deep copies rhs into this scope
		/// </summary>
		/// <param name="rhs">the scope to deep copy</param>
		Scope(const Scope& rhs);

		/// <summary>
		/// Move Copy Constructor
		/// </summary>
		/// <param name="rhs">the scope to take memory from</param>
		Scope(Scope&& rhs) noexcept;

		/// <summary>
		/// Destructor
		/// </summary>
		virtual ~Scope();

		/// <summary>
		/// Copy Assignment Operator: Deep copies rhs into this scope
		/// </summary>
		/// <param name="rhs">the scope to deep copy</param>
		/// <returns>A reference to this scope (a deep copy of rhs)</returns>
		Scope& operator=(const Scope& rhs);

		/// <summary>
		/// Move Assignment Operator
		/// </summary>
		/// <param name="rhs">the scope to move memory from</param>
		/// <returns>a reference to this scope after moving data from rhs into it</returns>
		Scope& operator=(Scope&& rhs) noexcept;

		/// <summary>
		/// Equality: Returns true if the contents of this scope = scope.rhs, false otherwise
		/// </summary>
		/// <param name="rhs">the scope to compare to</param>
		/// <returns>true if the contents of this scope = scope.rhs, false otherwise</returns>
		bool operator==(const Scope& rhs) const;

		/// <summary>
		/// Not Equals: Returns false if the contents of this scope = scope.rhs, true otherwise
		/// </summary>
		/// <param name="rhs">the scope to compare to</param>
		/// <returns>false if the contents of this scope = scope.rhs, true otherwise</returns>
		bool operator!=(const Scope& rhs);

		/// <summary>
		/// Returns the capacity of the table
		/// </summary>
		/// <returns>The capacity of the table</returns>
		size_t Capacity() const noexcept;

		/// <summary>
		/// Returns the size of the table (how many elements are in it)
		/// </summary>
		/// <returns>The size of the table (how many elements are in it)</returns>
		size_t Size() const noexcept;

		/// <summary>
		/// Returns true if there are no string, datum pairs in the scope, false otherwise
		/// </summary>
		/// <returns>Returns true if there are no elements in the scope, false otherwise</returns>
		bool IsEmpty() const noexcept;

		/// <summary>
		/// Returns the address of the parent of this scope
		/// </summary>
		/// <returns>The address of the parent of this scope (nullptr if no parent) </returns>
		Scope* GetParent() const noexcept;

		/// <summary>
		/// Makes the given scope a child of this scope
		/// Inserts an entry into the table with the given name and given scope. If entry already exists, adds scope to end of that datum.
		/// NOTE: PROVIDED SCOPE MUST BE HEAP ALLOCATED WITH NEW
		/// </summary>
		/// <param name="child">the HEAP APPLOCATED scope to adopt</param>
		/// <param name="name">the name of the entry for this scope</param>
		/// <remarks>If child already exists in this scope, nothing happens.</remarks>
		void Adopt(Scope& child, const std::string& name);

		/// <summary>
		/// Removes this scope from another scope.
		/// Sets this scope's parent to nullptr and removes self from the parent entry.
		/// NOTE: Because children are heap allocated, this means this scope must now be adopted by another scope, or must later be deleted to prevent memory leaks
		/// </summary>
		void Orphan();

		/// <summary>
		/// Returns a reference to the datum with the associated name.
		/// If datum does not already exist, creates a new datum and returns a reference to it.
		/// </summary>
		/// <param name="name">The name of the datum to append</param>
		/// <returns>A reference to the datum with the given name</returns>
		/// <exception cref="std::runtime_error">Throws exception if key is empty</exception>
		Datum& Append(const std::string& name);

		/// <summary>
		/// Returns a reference to the datum with the associated name.
		/// If datum does not already exist, creates a new datum and returns a reference to it.
		/// </summary>
		/// <param name="name">The name of the datum to append</param>
		/// <param name="EntryCreated">Out parameter bool representing whether an entry was created</param>
		/// <returns>A reference to the datum with the given name</returns>
		/// <exception cref="std::runtime_error">Throws exception if key is empty</exception>
		Datum& Append(const std::string& name, bool& EntryCreated);

		/// <summary>
		/// Append the scope with the given name. If entry does not exist, creates it.
		/// </summary>
		/// <param name="name">The name of the scope/entry</param>
		/// <returns>A reference to the scope with the given name</returns>
		/// <exception cref="std::runtime_error">Throws exception if entry exists with a datum type other than unknown or table or if key is empty</exception>
		/// <remarks>This will always make a new scope (not return the existing) </remarks>
		Scope& AppendScope(const std::string& name);

		/// <summary>
		/// Append the scope with the given name. If entry does not exist, creates it.
		/// </summary>
		/// <param name="name">The name of the scope/entry</param>
		/// <param name="EntryCreated">Out parameter bool representing whether an entry was created</param>
		/// <returns>A reference to the scope with the given name</returns>
		/// <exception cref="std::runtime_error">Throws exception if entry exists with a datum type other than unknown or table or if key is empty</exception>
		/// <remarks>This will always make a new scope (not return the existing) </remarks>
		Scope& AppendScope(const std::string& name, bool& EntryCreated);

		/// <summary>
		/// Finds the address of the datum with the given name in this scope.
		/// </summary>
		/// <param name="name">the name of the datum to find</param>
		/// <returns>the address of the datum with the given name in this scope, or nullptr if not found</returns>
		Datum* Find(const std::string& name);

		/// <summary>
		/// Finds the address of the datum with the given name in this scope. (const)
		/// </summary>
		/// <param name="name">the name of the datum to find</param>
		/// <returns>the address to a const datum with the given name in this scope, or nullptr if not found</returns>
		const Datum* Find(const std::string& name) const;

		/// <summary>
		/// Finds the child scope in this scope and returns the datum and the index the scope is at
		/// </summary>
		/// <param name="child">the scope to look for</param>
		/// <returns>
		/// A std::pair containing a pointer to the datum containing child, and the index child is at
		/// If scope not found, returns a std::pair contain a nullptr, and 0
		/// </returns>
		std::pair<Datum*, size_t> FindContainedScope(const Scope& child);

		/// <summary>
		/// Returns a reference to the datum with the associated name.
		/// If datum does not already exist, creates a new datum and returns a reference to it.
		/// </summary>
		/// <param name="name">the name of the datum to append</param>
		/// <returns>A reference to the datum with the given name</returns>
		/// <remarks>This is a thin wrapper to the append function</remarks>
		Datum& operator[](const std::string& name);

		/// <summary>
		/// Returns a pointer to the a constant datum with the associated name.
		/// If datum does not already exist, returns nullptr
		/// </summary>
		/// <param name="name">the name of the datum to append</param>
		/// <returns>A pointer to a constant datum with the associated name, or nullptr of does not exist</returns>
		/// <remarks>This is a thin wrapper to the const Find() function</remarks>
		const Datum* operator[](const std::string& name) const;

		/// <summary>
		/// Returns a reference to a datum at the given index
		/// (Index values correspond to the order in which items were added)
		/// </summary>
		/// <param name="index">the index of the datum in the order which items were added</param>
		/// <returns>a reference to the datum at the given index in the order vector</returns>
		/// <exception cref="std::runtime_error">Throws exception if index is out of bounds</exception>
		Datum& operator[](size_t index);

		/// <summary>
		/// Returns a const reference to a datum at the given index
		/// (Index values correspond to the order in which items were added)
		/// </summary>
		/// <param name="index">the index of the datum in the order which items were added</param>
		/// <returns>a const reference to the datum at the given index in the order vector</returns>
		/// <exception cref="std::runtime_error">Throws exception if index is out of bounds</exception>
		const Datum& operator[](size_t index) const;

		/// <summary>
		/// Searches for and returns the address of the most closely nested Datum associated with the given name in this scope or its ancestors.
		/// </summary>
		/// <param name="key">The name of the datum to search for</param>
		/// <param name="foundScope">Out parameter for the scope the datum was found in</param>
		/// <returns>The address of the most closely nested Datum associated with the given name, or nullptr if it does not exist</returns>
		Datum* Search(const std::string& key, Scope** foundScope = nullptr);

		/// <summary>
		/// Searches for and returns the address of the most closely nested Datum associated with the given name in this scope or its ancestors.
		/// </summary>
		/// <param name="key">The name of the datum to search for</param>
		/// <param name="foundScope">Out parameter for the scope the datum was found in</param>
		/// <returns>The address of the most closely nested Datum associated with the given name, or nullptr if it does not exist</returns>
		const Datum* Search(const std::string& key, Scope** foundScope = nullptr) const;

		/// <summary>
		/// Overload of RTTI Equals. 
		/// </summary>
		/// <param name="rhs">The RTTI pointer to compare to</param>
		/// <returns>True if RTTI* is a scope and is equal to this one, false otherwise</returns>
		bool Equals(const RTTI* rhs) const override;

		/// <summary>
		/// Creates a new heap allocated copy of this
		/// </summary>
		/// <returns>a heap allocated copy of this</returns>
		virtual gsl::owner<Scope*> Clone() const;

	protected:
		using MapType = Hashmap<const std::string, Datum>;
		using PairType = MapType::PairType;
		MapType mTable;
		Vector<PairType*> mOrderVector; //holds pointers to mTable entries in the order they are appended
		Scope* mParent = nullptr;

		//delete all memory allocated by this object
		void Clear();
	};
}

