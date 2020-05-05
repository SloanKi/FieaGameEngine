#include "pch.h"
#include "Datum.h"
#include "Signature.h"
#include "TypeRegistry.h"
#include "Attributed.h"

namespace Library
{
	RTTI_DEFINITIONS(Attributed)

	Attributed::Attributed(RTTI::IdType typeID)
	{
		(*this)["this"] = this;
		Populate(typeID);
	}

	Attributed::Attributed(const Attributed& rhs) :
		Scope(rhs)
	{
		(*this)["this"] = this;
		UpdateExternalStorage(rhs.TypeIdInstance());
	}

	Attributed::Attributed(Attributed&& rhs) noexcept :
		Scope(rhs)
	{
		(*this)["this"] = this;
		UpdateExternalStorage(rhs.TypeIdInstance());
	}

	Attributed& Attributed::operator=(const Attributed& rhs)
	{
		if (this != &rhs)
		{
			Scope::operator=(rhs);
			(*this)["this"] = this;
			UpdateExternalStorage(rhs.TypeIdInstance());
		}
		return *this;
	}

	Attributed& Attributed::operator=(Attributed&& rhs) noexcept
	{
		if (this != &rhs)
		{
			Scope::operator=(rhs);
			(*this)["this"] = this;
			UpdateExternalStorage(rhs.TypeIdInstance());
		}
		return *this;
	}

	bool Attributed::IsAttribute(std::string name) const
	{
		return (Find(name) != nullptr);
	}

	bool Attributed::IsPrescribedAttribute(std::string name) const
	{
		const auto& signatures = TypeRegistry::GetSignatures(TypeIdInstance());
		for (const auto& signature : signatures)
		{
			if (signature.mName == name) { return true; }
		}
		return false; //if here, not found in signatures.
	}

	bool Attributed::IsAuxiliaryAttribute(std::string name) const
	{
		return (IsAttribute(name) && !IsPrescribedAttribute(name));
	}

	void Attributed::Populate(RTTI::IdType typeID)
	{
		const auto& signatures = TypeRegistry::GetSignatures(typeID);
		for (const auto& signature : signatures)
		{
			Datum& dat = Append(signature.mName);
			dat.SetType(signature.mType);
			if (signature.mType != DatumType::Table)
			{
				dat.SetStorage(reinterpret_cast<uint8_t*>(this) + signature.mOffset, signature.mSize);
			}
			else
			{
				dat.Reserve(signature.mSize);
			}
		}
	}

	Datum& Attributed::AppendAuxiliaryAttribute(std::string name)
	{
		if (IsPrescribedAttribute(name)) { throw std::runtime_error("Cannot append prescribed scope"); }
		return Append(name);
	}

	Datum& Attributed::AppendAuxiliaryAttribute(std::string name, bool& EntryCreated)
	{
		if (IsPrescribedAttribute(name)) { throw std::runtime_error("Cannot append prescribed scope"); }
		return Append(name, EntryCreated);
	}

	bool Attributed::Equals(const RTTI* rhs) const
	{
		if (rhs == nullptr) { return false; }
		const Attributed* other = rhs->As<Attributed>();
		//if other is an attributed, compare, else return false
		return (other != nullptr ? *this == *other : false);
	}

	const Vector<std::pair<const std::string, Datum>*>& Attributed::Attributes() const
	{
		return mOrderVector;
	}

	Vector<std::pair<const std::string, Datum>*> Attributed::PrescribedAttributes() const
	{
		Vector<std::pair<const std::string, Datum>*> list;
		auto numPrescribed = TypeRegistry::GetSignatures(TypeIdInstance()).Size() + 1; //+1 because "this"
		if (Size() == numPrescribed) { list = mOrderVector; } //no auxiliary, so just pass mordervector
		else 
		{
			list.Reserve(numPrescribed);
			for (size_t i = 0; i < numPrescribed; i++)
			{
				list.PushBack(mOrderVector[i]);
			}
		}
		return list;
	}

	Vector<std::pair<const std::string, Datum>*> Attributed::AuxiliaryAttributes() const
	{
		Vector<std::pair<const std::string, Datum>*> list;
		auto numPrescribed = TypeRegistry::GetSignatures(TypeIdInstance()).Size() + 1; //+1 because "this"
		if (Size() > numPrescribed)
		{
			for (size_t i = numPrescribed; i < mOrderVector.Size(); i++)
			{
				list.PushBack(mOrderVector[i]);
			}
		}
		return list;
	}

	void Attributed::UpdateExternalStorage(RTTI::IdType typeID)
	{
		const auto& signatures = TypeRegistry::GetSignatures(typeID);
		for (const auto& signature : signatures)
		{
			Datum& dat = Append(signature.mName);
			dat;
			if (signature.mType != DatumType::Table)
			{
				dat.SetStorage(reinterpret_cast<uint8_t*>(this) + signature.mOffset, signature.mSize);
			}
		}
	}
}