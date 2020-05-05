#include "pch.h"
#include "Scope.h"

namespace Library
{
	RTTI_DEFINITIONS(Scope)

	Scope::Scope(size_t capacity) :
		mTable(capacity), mOrderVector(capacity)
	{
		if (capacity == 0)
		{
			throw std::runtime_error("Starting capacity cannot be 0");
		}
	}

	Scope::Scope(const Scope& rhs) :
		mTable(rhs.mTable), mParent(nullptr)
	{
		//populate order vector
		mOrderVector.Resize(rhs.Size());
		for (size_t i = 0; i < rhs.mOrderVector.Size(); ++i)
		{
			auto result = mTable.Find(rhs.mOrderVector[i]->first);
			assert(result != mTable.end());
			mOrderVector[i] = &(*result);

			Datum& dat = result->second;

			//deep copy any children scope
			if (dat.Type() == DatumType::Table)
			{
				for (size_t j = 0; j < dat.Size(); ++j)
				{
					//Scope* scope = new Scope(dat[j]);
					Scope* scope = dat[j].Clone();
					dat.Set(*scope, j);
					scope->mParent = this;
				}
			}
		}
	}

	Scope::Scope(Scope&& rhs) noexcept :
		mTable(std::move(rhs.mTable))
	{
		mOrderVector = std::move(rhs.mOrderVector);
		mParent = rhs.mParent;
		//reparent
		if (mParent != nullptr)
		{
			auto result = mParent->FindContainedScope(rhs);
			result.first->Set(*this, result.second);
			rhs.mParent = nullptr;
		}

		for (const auto& pair : mOrderVector)
		{
			Datum& dat = pair->second;
			if (dat.Type() == DatumType::Table)
			{
				for (size_t j = 0; j < dat.Size(); ++j)
				{
					Scope& scope = dat[j];
					assert(scope.mParent != this);
					scope.mParent = this;
				}
			}
		}
	}

	Scope::~Scope()
	{
		Orphan();
		Clear();
	}
	
	Scope& Scope::operator=(const Scope& rhs)
	{
		if (this != &rhs)
		{
			if (mParent == &rhs)
			{
				throw std::runtime_error("Cannot copy assign a child scope to its parent");
			}

			Clear();
			mTable = rhs.mTable;

			//populate order vector
			mOrderVector.Resize(rhs.Size());
			for (size_t i = 0; i < rhs.mOrderVector.Size(); ++i)
			{
				auto result = mTable.Find(rhs.mOrderVector[i]->first);
				assert(result != mTable.end());
				mOrderVector[i] = &(*result);

				Datum& dat = result->second;

				//deep copy any children scope
				if (dat.Type() == DatumType::Table)
				{
					for (size_t j = 0; j < dat.Size(); ++j)
					{
						//Scope* scope = new Scope(dat[j]);
						Scope* scope = dat[j].Clone();
						dat.Set(*scope, j);
						scope->mParent = this;
					}
				}
			}
		}
		return *this;
	}

	Scope& Scope::operator=(Scope&& rhs) noexcept
	{
		if (*this != rhs)
		{
			Clear();
			mTable = std::move(rhs.mTable);
			mOrderVector = std::move(rhs.mOrderVector);
			mParent = rhs.mParent;
			//reparent
			if (mParent != nullptr)
			{
				auto result = mParent->FindContainedScope(rhs);
				result.first->Set(*this, result.second);
				rhs.mParent = nullptr;
			}

			for (const auto& pair : mOrderVector)
			{
				Datum& dat = pair->second;
				if (dat.Type() == DatumType::Table)
				{
					for (size_t j = 0; j < dat.Size(); ++j)
					{
						Scope& scope = dat[j];
						assert(scope.mParent != this);
						scope.mParent = this;
					}
				}
			}
		}
		return *this;
	}

	bool Scope::operator==(const Scope& rhs) const
	{
		//early exits
		if (this == &rhs) { return true; }
		if (Size() != rhs.Size()) { return false; }

		//otherwise search for each and compare datums (order does not matter)
		for (auto it = mOrderVector.begin(); it != mOrderVector.end(); ++it)
		{
			if ((*it)->first == "this") { continue; }
			auto datum = rhs.Find((*it)->first);
			if (datum == nullptr) { return false; } //early exit. If nullptr, datum not found

			if ((*it)->second != *datum) { return false; } //early exit, if datums do not match, not equal
		}

		//if here, all datums match, are equal
		return true;
	}

	bool Scope::operator!=(const Scope& rhs)
	{
		return !operator==(rhs);
	}

	size_t Scope::Capacity() const noexcept
	{
		return mTable.Capacity();
	}

	size_t Scope::Size() const noexcept
	{
		return mTable.Size();
	}

	bool Scope::IsEmpty() const noexcept
	{
		return Size() == 0;
	}

	Scope* Scope::GetParent() const noexcept
	{
		return mParent;
	}

	void Scope::Adopt(Scope& child, const std::string& name)
	{
		if (child.GetParent() != this)
		{
			if (name.empty()) { throw std::runtime_error("Name cannot be empty"); }
			Datum& dat = Append(name);
			dat.PushBack(child); //will throw exception if datum is not type table or unknown
			child.Orphan(); //always orphan to prevent memory leaks
			child.mParent = this;
		}
	}

	void Scope::Orphan()
	{
		if (mParent == nullptr) { return; }
		auto result = mParent->FindContainedScope(*this);
		assert(result.first != nullptr);
		result.first->RemoveAt(result.second);	//remove this scope from the datum
		mParent = nullptr;
	}

	Datum& Scope::Append(const std::string& name)
	{
		bool throwaway;
		return Append(name, throwaway);
	}

	Datum& Scope::Append(const std::string& name, bool& EntryCreated)
	{
		if (name.empty()) { throw std::runtime_error("Name cannot be empty"); }
		auto result = mTable.Insert(std::make_pair(name, Datum())); //returns pair<It, bool>
		auto& pair = *result.first;
		EntryCreated = result.second;
		if (EntryCreated)
		{
			mOrderVector.PushBack(&pair);
		}
		return pair.second;
	}

	Scope& Scope::AppendScope(const std::string& name)
	{
		bool throwaway;
		return AppendScope(name, throwaway);
	}

	Scope& Scope::AppendScope(const std::string& name, bool& EntryCreated)
	{
		if (name.empty()) { throw std::runtime_error("Name cannot be empty"); }
		Scope* child;
		Datum& dat = Append(name, EntryCreated);
		dat.TypeCheckOrUnknown(DatumType::Table); //makes sure type is either unknown or scope
		child = new Scope();
		child->mParent = this;
		dat.PushBack(*child);	//will throw exception if datum is not type scope or unknown
		return *child;
	}

	Datum* Scope::Find(const std::string& name)
	{
		auto tableIt = mTable.Find(name);
		if (tableIt == mTable.end()) { return nullptr; }
		return &(*tableIt).second;
	}

	const Datum* Scope::Find(const std::string& name) const
	{
		auto tableIt = mTable.Find(name);
		if (tableIt == mTable.end()) { return nullptr; }
		return &(*tableIt).second;
	}

	std::pair<Datum*, size_t> Scope::FindContainedScope(const Scope& child)
	{
		bool found = false;
		size_t index = 0;
		auto it = mTable.begin();
		Datum* datum;

		while (it != mTable.end() && !found)
		{
			datum = &it->second;
			//if datum is a table, search table for this scope
			//Ask Paul about Find() in datum->mine calls Equals, but we are looking for a specific pointer (sameness), not equality?
			if (datum->Type() == DatumType::Table)
			{
				for (index = 0; index < datum->Size(); ++index)
				{
					if (&(datum->Get<Scope>(index)) == &child)
					{
						found = true;
						break;
					}
				}
			}
			++it;
		}

		if (!found) { return std::make_pair(nullptr, 0); }
		return std::make_pair(datum, index);
	}

	Datum& Scope::operator[](const std::string& name)
	{
		return Append(name);
	}

	const Datum* Scope::operator[](const std::string& name) const
	{
		return Find(name);
	}

	Datum& Scope::operator[](size_t index)
	{
		return mOrderVector[index]->second;
	}

	const Datum& Scope::operator[](size_t index) const
	{
		return mOrderVector[index]->second;
	}

	Datum* Scope::Search(const std::string& key, Scope** foundScope)
	{
		auto result = Find(key);
		if (result != nullptr)
		{
			if (foundScope != nullptr) { *foundScope = this; }
			return result;
		}
		
		//if here not found in this scope, search parent
		if (mParent != nullptr) { return mParent->Search(key, foundScope); }
		if (foundScope != nullptr) { *foundScope = nullptr; }
		return nullptr;
	}

	const Datum* Scope::Search(const std::string& key, Scope** foundScope) const
	{
		return (const_cast<Scope*>(this))->Search(key, foundScope);
	}
	
	bool Scope::Equals(const RTTI* rhs) const
	{
		if (rhs == nullptr) { return false; }
		const Scope* other = rhs->As<Scope>();
		//if other is a scope, compare, else return false
		return (other != nullptr ? *this == *other : false);
	}

	gsl::owner<Scope*> Scope::Clone() const
	{
		return new Scope(*this);
	}

	void Scope::Clear()
	{
		for (auto it = mTable.begin(); it != mTable.end(); ++it)
		{
			Datum& datum = it->second;
			//if datum is a table, iterate through and delete children
			if (datum.Type() == DatumType::Table)
			{
				for (size_t i = 0; i < datum.Size(); ++i)
				{
					Scope* scope = &datum.Get<Scope>(i);
					assert(scope->mParent == this);
					scope->mParent = nullptr;
					delete(scope);
				}
			}
		}
	}
}