//#include "pch.h"
#include <glm/glm.hpp>
#include <string>
#include "Datum.h"


namespace Library
{
	inline Datum::Datum()
	{
		mData.vo = nullptr;
	}
	
	inline bool Datum::operator==(int value) const noexcept
	{
		return (mType == DatumType::Integer && mSize == 1 && mData.i[0] == value);
	}

	inline bool Datum::operator==(float value) const noexcept
	{
		return (mType == DatumType::Float && mSize == 1 && mData.f[0] == value);
	}

	inline bool Datum::operator==(const glm::vec4& value) const noexcept
	{
		return (mType == DatumType::Vector && mSize == 1 && mData.v[0] == value);
	}

	inline bool Datum::operator==(const glm::mat4& value) const noexcept
	{
		return (mType == DatumType::Matrix && mSize == 1 && mData.m[0] == value);
	}

	inline bool Datum::operator==(const std::string& value) const noexcept
	{
		return (mType == DatumType::String && mSize == 1 && mData.s[0] == value);
	}

	inline bool Datum::operator==(RTTI* value) const noexcept
	{
		return (mType == DatumType::Pointer && mSize == 1 && mData.p != nullptr && mData.p[0]->Equals(value));
	}

	inline bool Datum::operator!=(const Datum& rhs) const noexcept
	{
		return !operator==(rhs);
	}

	inline bool Datum::operator!=(int value) const noexcept
	{
		return !operator==(value);
	}

	inline bool Datum::operator!=(float value) const noexcept
	{
		return !operator==(value);
	}

	inline bool Datum::operator!=(const glm::vec4& value) const noexcept
	{
		return !operator==(value);
	}

	inline bool Datum::operator!=(const glm::mat4& value) const noexcept
	{
		return !operator==(value);
	}

	inline bool Datum::operator!=(const std::string& value) const noexcept
	{
		return !operator==(value);
	}

	inline bool Datum::operator!=(RTTI* value) const noexcept
	{
		return !operator==(value);
	}

	inline DatumType Datum::Type() const noexcept
	{
		return mType;
	}

	inline size_t Datum::Size() const noexcept
	{
		return mSize;
	}

	inline size_t Datum::Capacity() const noexcept
	{
		return mCapacity;
	}

	inline bool Datum::IsEmpty() const noexcept
	{
		return mSize == 0;
	}

	inline void Datum::SetType(const DatumType type)
	{
		TypeCheckOrUnknown(type);	//throw exception if mType != type or unknown
		mType = type;
	}

	inline void Datum::SetStorage(int* array, size_t size)
	{
		SetStorageHelper(reinterpret_cast<void*>(array), DatumType::Integer, size);
	}

	inline void Datum::SetStorage(float* array, size_t size)
	{
		SetStorageHelper(reinterpret_cast<void*>(array), DatumType::Float, size);
	}

	inline void Datum::SetStorage(glm::vec4* array, size_t size)
	{
		SetStorageHelper(reinterpret_cast<void*>(array), DatumType::Vector, size);
	}

	inline void Datum::SetStorage(glm::mat4* array, size_t size)
	{
		SetStorageHelper(reinterpret_cast<void*>(array), DatumType::Matrix, size);
	}

	inline void Datum::SetStorage(std::string* array, size_t size)
	{
		SetStorageHelper(reinterpret_cast<void*>(array), DatumType::String, size);
	}

	inline void Datum::SetStorage(RTTI** array, size_t size)
	{
		SetStorageHelper(reinterpret_cast<void*>(array), DatumType::Pointer, size);
	}

	inline void Datum::SetStorage(void* array, size_t size)
	{
		CheckTypeHasBeenSet();
		SetStorageHelper(array, mType, size);
	}

	template<>
	inline int& Datum::Get<int>(size_t index)
	{
		TypeCheck(DatumType::Integer);
		VerifyIndexInBounds(index);
		return mData.i[index];
	}

	template<>
	inline const int& Datum::Get<int>(size_t index) const
	{
		return (const_cast<Datum*>(this))->Get<int>(index);
	}

	template<>
	inline float& Datum::Get<float>(size_t index)
	{
		TypeCheck(DatumType::Float);
		VerifyIndexInBounds(index);
		return mData.f[index];
	}

	template<>
	inline const float& Datum::Get<float>(size_t index) const
	{
		return (const_cast<Datum*>(this))->Get<float>(index);
	}

	template<>
	glm::vec4& Datum::Get<glm::vec4>(size_t index)
	{
		TypeCheck(DatumType::Vector);
		VerifyIndexInBounds(index);
		return mData.v[index];
	}

	template<>
	inline const glm::vec4& Datum::Get<glm::vec4>(size_t index) const
	{
		return (const_cast<Datum*>(this))->Get<glm::vec4>(index);
	}

	template<>
	glm::mat4& Datum::Get<glm::mat4>(size_t index)
	{
		TypeCheck(DatumType::Matrix);
		VerifyIndexInBounds(index);
		return mData.m[index];
	}

	template<>
	inline const glm::mat4& Datum::Get<glm::mat4>(size_t index) const
	{
		return (const_cast<Datum*>(this))->Get<glm::mat4>(index);
	}

	template<>
	inline Scope& Datum::Get<Scope>(size_t index)
	{
		TypeCheck(DatumType::Table);
		VerifyIndexInBounds(index);
		return *(mData.t[index]);
	}

	template<>
	const Scope& Datum::Get<Scope>(size_t index) const
	{
		return (const_cast<Datum*>(this))->Get<Scope&>(index);
	}

	template<>
	std::string& Datum::Get<std::string>(size_t index)
	{
		TypeCheck(DatumType::String);
		VerifyIndexInBounds(index);
		return mData.s[index];
	}

	template<>
	inline const std::string& Datum::Get<std::string>(size_t index) const
	{
		return (const_cast<Datum*>(this))->Get<std::string>(index);
	}

	template<>
	RTTI*& Datum::Get<RTTI*>(size_t index)
	{
		TypeCheck(DatumType::Pointer);
		VerifyIndexInBounds(index);
		return mData.p[index];
	}

	template<>
	RTTI* const& Datum::Get<RTTI*>(size_t index) const
	{
		return (const_cast<Datum*>(this))->Get<RTTI*>(index);
	}

	inline bool Datum::Remove(int value)
	{
		//will return false if value doesn't exist, will throw exception if type doesn't match
		return RemoveAt(Find(value));
	}

	inline bool Datum::Remove(float value)
	{
		//will return false if value doesn't exist, will throw exception if type doesn't match
		return RemoveAt(Find(value));
	}

	inline bool Datum::Remove(const glm::vec4& value)
	{
		//will return false if value doesn't exist, will throw exception if type doesn't match
		return RemoveAt(Find(value));
	}

	inline bool Datum::Remove(const glm::mat4& value)
	{
		//will return false if value doesn't exist, will throw exception if type doesn't match
		return RemoveAt(Find(value));
	}

	inline bool Datum::Remove(const std::string& value)
	{
		//will return false if value doesn't exist, will throw exception if type doesn't match
		return RemoveAt(Find(value));
	}

	inline bool Datum::Remove(RTTI* value)
	{
		//will return false if value doesn't exist, will throw exception if type doesn't match
		return RemoveAt(Find(value));
	}

	

	template<>
	inline void Datum::SetFromString<int>(const std::string& str, size_t index)
	{
		Set(std::stoi(str, nullptr), index);
	}

	template<>
	inline void Datum::SetFromString<float>(const std::string& str, size_t index)
	{
		Set(std::stof(str, nullptr), index);
	}

	template<>
	void Datum::SetFromString<glm::vec4>(const std::string& str, size_t index)
	{
		glm::vec4 values;
		int count = sscanf_s(str.c_str(), "vec4(%f, %f, %f, %f)", &values[0], &values[1], &values[2], &values[3]);
		if (count != 4)
		{
			throw std::runtime_error("invalid string input");
		}

		Set(values, index);
	}

	template<>
	void Datum::SetFromString<glm::mat4>(const std::string& str, size_t index)
	{
		glm::mat4 val;
		const char* matrixTemplate = "mat4x4((%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f))";
		int count = sscanf_s(str.c_str(), matrixTemplate, &val[0][0], &val[0][1], &val[0][2], &val[0][3], &val[1][0], &val[1][1], &val[1][2], &val[1][3], &val[2][0], &val[2][1], &val[2][2], &val[2][3], &val[3][0], &val[3][1], &val[3][2], &val[3][3]);
		if (count != 16)
		{
			throw std::runtime_error("invalid string input");
		}

		Set(val, index);
	}

	template<>
	inline void Datum::SetFromString<std::string>(const std::string& str, size_t index)
	{
		Set(str,index);
	}

	inline Scope& Datum::operator[](size_t index)
	{
		return Get<Scope>(index);
	}
	/************************************************************************/
	/**************************Helper Functions******************************/
	/************************************************************************/
	inline void Datum::ReallocData(size_t capacity)
	{
		if (capacity == 0) 
		{
			free(mData.vo);
			mData.vo = nullptr;
		}
		else
		{
			void* newData = realloc(mData.vo, capacity * DatumSizes[static_cast<int>(mType)]);
			assert(newData != nullptr);
			mData.vo = newData;
		}
		mCapacity = capacity;
	}

	inline void Datum::TypeCheck(DatumType type) const
	{
		if (mType != type)
		{
			throw std::runtime_error("Datum type does not match operation type");
		}
	}

	inline void Datum::TypeCheckOrUnknown(DatumType type) const
	{
		if (!(mType == type || mType == DatumType::Unknown))
		{
			throw std::runtime_error("Datum type cannot change after it is set");
		}
	}

	inline void Datum::CheckTypeHasBeenSet() const
	{
		if (mType == DatumType::Unknown)
		{
			throw std::runtime_error("Datum must have type to complete this operation");
		}
	}

	inline void Datum::VerifyIndexInBounds(size_t index) const
	{
		if (index >= mSize)
		{
			throw std::runtime_error("Index out of bounds");
		}
	}
	
	inline void Datum::IncrementCapacityIfFull()
	{
		size_t newCapacity = 0;
		if (mSize == mCapacity)
		{
			newCapacity = mCapacity + (mCapacity / 2) + 1; //Add half the current capacity + 1 each capacity increment
		}
		Reserve(newCapacity);
	}

	inline void Datum::ExternalException()
	{
		if (mIsExternal)
		{
			throw std::runtime_error("Cannot add/remove elements or otherwise change the size/capacity of a datum with external memory.");
		}
	}
}