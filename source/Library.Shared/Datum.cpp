#include "pch.h"
#include "Datum.h"
#include "Scope.h"

#pragma warning(push)
#pragma warning(disable:4201)
#include <glm/gtx/string_cast.hpp>
#pragma warning(pop)

namespace Library
{
	Datum::Datum(DatumType type, size_t capacity) :
		mType(type), mCapacity(capacity)
	{
		if (capacity != 0)
		{
			void* newData = malloc(capacity * DatumSizes[static_cast<int>(mType)]);
			assert(newData != nullptr);
			mData.vo = newData;
		}
		else
		{
			mData.vo = nullptr;
		}
		mSize = 0;
	}

	Datum::Datum(const Datum& rhs) :
		mIsExternal(rhs.mIsExternal), mSize(rhs.mSize), mType(rhs.mType) 
	{
		if (mIsExternal)
		{
			mData.vo = rhs.mData.vo;
		}
		else
		{
			mData.vo = nullptr;
			ReallocData(rhs.mCapacity);
			if (rhs.mType == DatumType::String)
			{
				for (size_t i = 0; i < mSize; i++)
				{
					new(mData.s + i)std::string(rhs.Get<std::string>(i));
				}
			}
			else
			{
				memcpy(mData.vo, rhs.mData.vo, rhs.mSize * DatumSizes[static_cast<int>(mType)]); //deep copy
			}
		}
	}

	Datum::Datum(Datum&& rhs) noexcept :
		mSize(rhs.mSize), mIsExternal(rhs.mIsExternal), mData(rhs.mData), mCapacity(rhs.mCapacity), mType(rhs.mType)
	{
		rhs.mSize = 0;
		rhs.mCapacity = 0;
		rhs.mData.vo = nullptr;
	}

	Datum::~Datum()
	{
		if (!mIsExternal)
		{
			Clear();
			free(mData.vo);
		}
	}

	Datum& Datum::operator=(const Datum& rhs) 
	{
		if (this != &rhs)
		{
			if (!mIsExternal) { Clear(); }

			mSize = rhs.mSize;
			mCapacity = rhs.mCapacity;
			mType = rhs.mType;

			if (rhs.mIsExternal)
			{
				if (!mIsExternal) { free(mData.vo); }	//don't leak memory
				mData.vo = rhs.mData.vo;	//shallow copy
			}
			else
			{
				if (mIsExternal) { mData.vo = nullptr; } //protect external storage pointer
				ReallocData(rhs.mCapacity);
				if (rhs.mType == DatumType::String)
				{
					for (size_t i = 0; i < mSize; i++)
					{
						new(mData.s + i)std::string(rhs.Get<std::string>(i));
					}
				}
				else
				{
					memcpy(mData.vo, rhs.mData.vo, rhs.mSize * DatumSizes[static_cast<int>(mType)]); //deep copy
				}
			}
			mIsExternal = rhs.mIsExternal;
		}

		return *this;
	}

	Datum& Datum::operator=(Datum&& rhs) noexcept
	{
		if (this != &rhs)
		{
			//if have internal storage, clear and free it
			if (!mIsExternal)
			{
				Clear();
				free(mData.vo);
			}

			//copy
			mSize = rhs.mSize;
			mIsExternal = rhs.mIsExternal;
			mData = rhs.mData;
			mCapacity = rhs.mCapacity;
			mType = rhs.mType;

			//invalidate
			rhs.mSize = 0;
			rhs.mCapacity = 0;
			rhs.mData.vo = nullptr;
		}
		return *this;
	}

	Datum& Datum::operator=(int value)
	{
		SetType(DatumType::Integer); //will throw exception if type isn't this or unknown
		ReallocData(1);
		mData.i[0] = value;
		mSize = 1;

		return *this;
	}

	Datum& Datum::operator=(float value)
	{
		SetType(DatumType::Float);	//will throw exception if type isn't this or unknown
		ReallocData(1);
		mData.f[0] = value;
		mSize = 1;

		return *this;
	}

	Datum& Datum::operator=(const glm::vec4& value)
	{
		SetType(DatumType::Vector);	//will throw exception if type isn't this or unknown
		ReallocData(1);
		mData.v[0] = value;
		mSize = 1;

		return *this;
	}

	Datum& Datum::operator=(const glm::mat4& value)
	{
		SetType(DatumType::Matrix);	//will throw exception if type isn't this or unknown
		ReallocData(1);
		mData.m[0] = value;
		mSize = 1;

		return *this;
	}

	Datum& Datum::operator=(Scope& value)
	{
		SetType(DatumType::Table);	//will throw exception if type isn't this or unknown
		ReallocData(1);
		mData.t[0] = &value;
		mSize = 1;

		return *this;
	}
	
	Datum& Datum::operator=(const std::string& value)
	{
		SetType(DatumType::String);	//will throw exception if type isn't this or unknown
		Clear();
		ReallocData(1);
		new(mData.s)std::string(value);
		mSize = 1;

		return *this;
	}

	Datum& Datum::operator=(RTTI* value)
	{
		SetType(DatumType::Pointer);	//will throw exception if type isn't this or unknown
		ReallocData(1);
		mData.p[0] = value;
		mSize = 1;

		return *this;
	}
	
	bool Datum::operator==(const Datum& rhs) const noexcept
	{
		if (mData.vo == rhs.mData.vo && mType == rhs.mType) { return true; } //early exit
		if (mType == rhs.mType && mSize == rhs.mSize && mCapacity == rhs.mCapacity)
		{
			//if string, have to compare each type
			if (mType == DatumType::String)
			{
				for (size_t i = 0; i < mSize; i++)
				{
					if (mData.s[i] != rhs.mData.s[i]) { return false; }
				}
				return true; //if didn't early exit, then everything matched.
			}
			else if (mType == DatumType::Pointer || mType == DatumType::Table)
			{
				for (size_t i = 0; i < mSize; i++)
				{
					if (mData.p[i] != nullptr && !mData.p[i]->Equals(rhs.mData.p[i])) { return false; }
				}
				return true; //if didn't early exit, then everything matched.
			}

			//if not string, can just mem compare
			return !memcmp(mData.vo, rhs.mData.vo, mSize * DatumSizes[static_cast<int>(mType)]);	//memcmp returns 0 if same, non zero if not same
		}
		return false;	//if here, type size or capacity didn't match
	}

	void Datum::Resize(size_t capacity, bool defaultConstruct)
	{
		CheckTypeHasBeenSet();
		ExternalException();

		if (capacity == 0)
		{
			Clear();
			free(mData.vo);
			mData.vo = nullptr;
			mCapacity = 0;
			return;
		}

		//if shrinking and items are strings, destruct strings that won't fit anymore
		if (capacity < mSize && mType == DatumType::String)
		{
			for (size_t i = mSize - 1; i >= capacity; i--)
			{
				mData.s[i].~basic_string();
			}
		}


		void* newData = realloc(mData.vo, capacity * DatumSizes[static_cast<int>(mType)]);
		assert(newData != nullptr);
		mData.vo = newData;

		if (defaultConstruct)
		{
			//todo: change this so that it isn't checking type each loop.
			for (size_t i = mSize; i < capacity; i++)
			{
				switch (mType)
				{
				case DatumType::Integer:
					new(mData.i + i)int();
					break;
				case DatumType::Float:
					new(mData.f + i)float();
					break;
				case DatumType::Vector:
					new(mData.v + i)glm::vec4();
					break;
				case DatumType::Matrix:
					new(mData.m + i)glm::mat4();
					break;
				case DatumType::Table:
					throw std::runtime_error("cannot default construct Scope* objects");
					break;
				case DatumType::String:
					new(mData.s + i)std::string();
					break;
				case DatumType::Pointer:
					new(mData.p + i)RTTI*(nullptr);
					break;
				default:
					assert(false);
				}
			}
			mSize = capacity;
		}

		mSize = std::min(mSize, capacity);
		mCapacity = capacity;
	}

	void Datum::Reserve(size_t capacity)
	{
		CheckTypeHasBeenSet();	
		ExternalException();
		if (mCapacity >= capacity) { return; } //can't shrink array
		ReallocData(capacity);
	}
	
	void Datum::Clear()
	{
		ExternalException();
		if (mType == DatumType::String)
		{
			for (size_t i = 0; i < mSize; i++)
			{
				mData.s[i].~basic_string();
			}
		}

		mSize = 0;
	}

	void Datum::Set(int value, size_t index)
	{
		TypeCheck(DatumType::Integer);
		VerifyIndexInBounds(index);
		mData.i[index] = value;
	}

	void Datum::Set(float value, size_t index)
	{
		TypeCheck(DatumType::Float);
		VerifyIndexInBounds(index);
		mData.f[index] = value;
	}

	void Datum::Set(const glm::vec4& value, size_t index)
	{
		TypeCheck(DatumType::Vector);
		VerifyIndexInBounds(index);
		mData.v[index] = value;
	}

	void Datum::Set(const glm::mat4& value, size_t index)
	{
		TypeCheck(DatumType::Matrix);
		VerifyIndexInBounds(index);
		mData.m[index] = value;
	}

	void Datum::Set(Scope& value, size_t index)
	{
		TypeCheck(DatumType::Table);
		VerifyIndexInBounds(index);
		mData.t[index] = &value;
	}

	void Datum::Set(const std::string& value, size_t index)
	{
		TypeCheck(DatumType::String);
		VerifyIndexInBounds(index);
		mData.s[index] = value;
	}

	void Datum::Set(RTTI* value, size_t index)
	{
		TypeCheck(DatumType::Pointer);
		VerifyIndexInBounds(index);
		mData.p[index] = value;
	}

	void Datum::SetFromStringDanger(const std::string& str, size_t index)
	{
		CheckTypeHasBeenSet();
		switch (mType)
		{
		case DatumType::Integer:
			SetFromString<int>(str, index);
			break;
		case DatumType::Float:
			SetFromString<float>(str, index);
			break;
		case DatumType::Vector:
			SetFromString<glm::vec4>(str, index);
			break;
		case DatumType::Matrix:
			SetFromString<glm::mat4>(str, index);
			break;
		case DatumType::String:
			SetFromString<std::string>(str, index);
			break;
		}
	}

	void Datum::PushBack(int data)
	{
		SetType(DatumType::Integer);	//will throw exception if datum has a type other than int or unknown
		ExternalException();
		IncrementCapacityIfFull();
		mData.i[mSize] = data;
		mSize++;
	}

	void Datum::PushBack(float data)
	{
		SetType(DatumType::Float);	//will throw exception if datum has a type other than int or unknown
		ExternalException();
		IncrementCapacityIfFull();
		mData.f[mSize] = data;
		mSize++;
	}

	
	void Datum::PushBack(const glm::vec4& data)
	{
		SetType(DatumType::Vector);	//will throw exception if datum has a type other than int or unknown
		ExternalException();
		IncrementCapacityIfFull();
		mData.v[mSize] = data;
		mSize++;
	}

	void Datum::PushBack(const glm::mat4& data)
	{
		SetType(DatumType::Matrix);	//will throw exception if datum has a type other than int or unknown
		ExternalException();
		IncrementCapacityIfFull();
		mData.m[mSize] = data;
		mSize++;
	}

	void Datum::PushBack(Scope& data)
	{
		SetType(DatumType::Table);	//will throw exception if datum has a type other than int or unknown
		ExternalException();
		IncrementCapacityIfFull();
		mData.t[mSize] = &data;
		mSize++;
	}

	void Datum::PushBack(const std::string& data)
	{
		SetType(DatumType::String);	//will throw exception if datum has a type other than int or unknown
		ExternalException();
		IncrementCapacityIfFull();
		new(mData.s + mSize)std::string(data);
		mSize++;
	}

	void Datum::PushBack(RTTI* data)
	{
		SetType(DatumType::Pointer);	//will throw exception if datum has a type other than int or unknown
		ExternalException();
		IncrementCapacityIfFull();
		mData.p[mSize] = data;
		mSize++;
	}

	void Datum::PopBack()
	{
		ExternalException();
		if (mSize == 0) { return; } //if list is empty, just return

		//have to destruct strings
		if (mType == DatumType::String)
		{
			mData.s[mSize - 1].~basic_string();
		}

		mSize--;
	}

	size_t Datum::Find(int value) const
	{
		TypeCheck(DatumType::Integer);
		for (size_t i = 0; i < mSize; i++)
		{
			if (mData.i[i] == value)
			{
				return i;
			}
		}

		//if here, did not find it
		return mSize;
	}

	size_t Datum::Find(float value) const
	{
		TypeCheck(DatumType::Float);
		for (size_t i = 0; i < mSize; i++)
		{
			if (mData.f[i] == value)
			{
				return i;
			}
		}

		//if here, did not find it
		return mSize;
	}

	size_t Datum::Find(const glm::vec4& value) const
	{
		TypeCheck(DatumType::Vector);
		for (size_t i = 0; i < mSize; i++)
		{
			if (mData.v[i] == value)
			{
				return i;
			}
		}

		//if here, did not find it
		return mSize;
	}

	size_t Datum::Find(const glm::mat4& value) const
	{
		TypeCheck(DatumType::Matrix);
		for (size_t i = 0; i < mSize; i++)
		{
			if (mData.m[i] == value)
			{
				return i;
			}
		}

		//if here, did not find it
		return mSize;
	}

	size_t Datum::Find(const Scope& value) const
	{
		TypeCheck(DatumType::Table);
		for (size_t i = 0; i < mSize; i++)
		{
			if (mData.t[i] != nullptr && mData.t[i]->Equals(&value))
			{
				return i;
			}
		}

		//if here, did not find it
		return mSize;
	}

	size_t Datum::Find(const std::string& value) const
	{
		TypeCheck(DatumType::String);
		for (size_t i = 0; i < mSize; i++)
		{
			if (mData.s[i] == value)
			{
				return i;
			}
		}

		//if here, did not find it
		return mSize;
	}

	size_t Datum::Find(RTTI* value) const
	{
		TypeCheck(DatumType::Pointer);
		for (size_t i = 0; i < mSize; i++)
		{
			if (mData.p[i] != nullptr && mData.p[i]->Equals(value))
			{
				return i;
			}
		}

		//if here, did not find it
		return mSize;
	}

	bool Datum::RemoveAt(size_t index)
	{
		ExternalException();
		if (index >= mSize) { return false; }
		size_t dataToShift = mSize - index - 1;
		if (mType == DatumType::String) { mData.s[index].~basic_string(); }
		memmove(mData.byte + (index * DatumSizes[static_cast<int>(mType)]), mData.byte + ((index + 1) * DatumSizes[static_cast<int>(mType)]), dataToShift* DatumSizes[static_cast<int>(mType)]);
		mSize--;
		return true;
	}

	std::string Datum::ToString(size_t index) const
	{
		CheckTypeHasBeenSet();
		VerifyIndexInBounds(index);
		switch (mType)
		{
		case DatumType::Integer:
			return std::to_string(mData.i[index]);
		case DatumType::Float:
			return std::to_string(mData.f[index]);
		case DatumType::Vector:
			return glm::to_string(mData.v[index]);
		case DatumType::Matrix:
			return glm::to_string(mData.m[index]);
		case DatumType::String:
			return mData.s[index];
		case DatumType::Pointer:
			return mData.p[index]->ToString();
		default:
			assert(false);
		}
		return ""; //won't do this, used to suppress warning
	}

	

	/************************************************************************/
	/**************************Helper Functions******************************/
	/************************************************************************/
	void Datum::SetStorageHelper(void* array, DatumType type, size_t size)
	{
		if (array == nullptr)
		{
			throw std::runtime_error("External Storage array cannot be nullptr");
		}
		if (mCapacity > 0)
		{
			throw std::runtime_error("Cannot make a datum external after it has already been allocated memory");
		}
		SetType(type); //will also throw exception if type is already set to something other than type
		mIsExternal = true;
		mSize = size;
		mCapacity = size;
		mData.vo = array;
	}
}