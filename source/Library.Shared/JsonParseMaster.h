#pragma once
#include "vector.h"
#include <json/json.h>
#include "RTTI.h"

namespace Library
{
	class IJsonParseHelper;
	class JsonParseMaster final
	{
	public:
		class SharedData : public RTTI
		{
			friend JsonParseMaster;
			RTTI_DECLARATIONS(SharedData, RTTI)
		public:
			/// <summary>
			/// Default constructor
			/// </summary>
			SharedData() = default;

			/// <summary>
			/// Copy Constructor
			/// </summary>
			/// <param name="rhs"> the sharedData to copy</param>
			SharedData(const SharedData& rhs) = default;

			/// <summary>
			/// Move Constructor
			/// </summary>
			/// <param name="rhs">the shared data to move into this</param>
			SharedData(SharedData&& rhs) noexcept = default;

			/// <summary>
			/// Destructor
			/// </summary>
			virtual ~SharedData() = default;

			/// <summary>
			/// Copy Assignment Operator
			/// </summary>
			/// <param name="rhs">the shared data to copy</param>
			/// <returns>a copy of rhs</returns>
			SharedData& operator=(const SharedData& rhs) = default;

			/// <summary>
			/// Move assignment operator
			/// </summary>
			/// <param name="rhs">the shared data to move into this</param>
			/// <returns>a shared data that rhs was moved into</returns>
			SharedData& operator=(SharedData&& rhs) noexcept = default;

			/// <summary>
			/// Equality. True if parsemaster and depth are the same
			/// </summary>
			/// <param name="rhs">the data to compare to</param>
			/// <returns>True if parsemaster and depth are the same</returns>
			bool operator==(const SharedData& rhs) const;

			/// <summary>
			/// Not equals. True if parsemaster or depth are different
			/// </summary>
			/// <param name="rhs">the data to compare to</param>
			/// <returns>True if parsemaster or depth are different</returns>
			bool operator!=(const SharedData& rhs) const;

			/// <summary>
			/// Virtual Constructor: Pure virtual
			/// </summary>
			virtual SharedData* Create() const = 0; //pure virtual

			/// <summary>
			/// Initialize this shared data (sets mdepth to 0)
			/// </summary>
			virtual void Initialize();

			/// <summary>
			/// Get the ParseMaster that is using this shareddata
			/// </summary>
			/// <returns>The ParseMaster that is using this SharedData</returns>
			JsonParseMaster* GetJsonParseMaster() const;

			/// <summary>
			/// Increases Depth by one
			/// </summary>
			void IncrementDepth();

			/// <summary>
			/// Decreases Depth
			/// </summary>
			void DecrementDepth();

			/// <summary>
			/// Get the Depth of this SharedData
			/// </summary>
			/// <returns>The depth of this SharedData</returns>
			size_t Depth() const;

			/// <summary>
			/// RTTI override, calls ==
			/// </summary>
			/// <param name="rhs">the RTTI ptr to compare to</param>
			/// <returns>True if is SharedData and is equal, false otherwise</returns>
			bool Equals(const RTTI* rhs) const override;

		private:
			void SetJsonParseMaster(JsonParseMaster& master);
			JsonParseMaster* mParseMaster = nullptr;
			size_t mDepth = 0;
		};

		/// <summary>
		/// Default constructor: Does not exist
		/// </summary>
		JsonParseMaster() = delete;

		/// <summary>
		/// Constructor: Initialize this with given SharedData
		/// </summary>
		/// <param name="data">The SharedData this Master will use</param>
		explicit JsonParseMaster(SharedData& data);

		/// <summary>
		/// Copy Constructor: Does not Exist. (See Clone)
		/// </summary>
		JsonParseMaster(const JsonParseMaster& rhs) = delete;

		/// <summary>
		/// Move constructor: moves rhs into this. Reparents SharedData to this. Invalidates rhs
		/// </summary>
		/// <param name="rhs">the ParseMaster to move into this one</param>
		JsonParseMaster(JsonParseMaster&& rhs) noexcept;

		/// <summary>
		/// Destructor: If this is a clone, deletes sharedData and helpers.
		/// </summary>
		~JsonParseMaster();

		/// <summary>
		/// Copy Assignment: Does not exist. See Clone
		/// </summary>
		JsonParseMaster& operator=(const JsonParseMaster& rhs) = delete;

		/// <summary>
		/// Move Assignment: moves rhs into this. Reparents SharedData to this. Invalidates rhs
		/// </summary>
		/// <param name="rhs">the Master to move into this one</param>
		/// <returns>This master with RHS moved into it</returns>
		JsonParseMaster& operator=(JsonParseMaster&& rhs) noexcept;

		void Initialize();

		/// <summary>
		/// Clones this ParseMaster.
		/// Creates new SharedData and Helpers that are the same kind this one has.
		/// The clone now owns all its data, and will free it at destruction.
		/// The clone is heap allocated and *Must* be deleted when done.
		/// </summary>
		/// <returns>A new heap allocated clone of this</returns>
		JsonParseMaster* Clone() const;

		/// <summary>
		/// Add Helper
		/// </summary>
		/// <param name="helper">the helper to add</param>
		/// <remarks>If this is a clone, helper must be heap allocated, otherwise everything explodes.</remarks>
		void AddHelper(IJsonParseHelper& helper);

		/// <summary>
		/// Remove Helper
		/// </summary>
		/// <param name="helper">The helper to remove</param>
		/// <remarks> 
		/// If helpers does not exist, nothing happens.
		/// NOTE: If this is a clone, you become responsible for any helper you remove from this list.
		/// You must manually delete any helper that you remove from a clone.
		/// </remarks>
		void RemoveHelper(IJsonParseHelper& helper);

		/// <summary>
		/// Parse string data
		/// </summary>
		/// <param name="data">The string data to parse</param>
		/// <returns>True if parse was successful, false otherwise</returns>
		bool Parse(const std::string& data);

		/// <summary>
		/// Parse stream Data
		/// </summary>
		/// <param name="data">The stream data to parse</param>
		/// <returns>true if parse was successful, false otherwise</returns>
		bool Parse(std::istream& data);

		/// <summary>
		/// Parse from file 
		/// </summary>
		/// <param name="filename">The filename of the file to parse from</param>
		/// <returns>true if parse was successful, false otherwise</returns>
		bool ParseFromFile(const std::string& filename);

		/// <summary>
		/// Gets the filename from the last file parsed.
		/// </summary>
		/// <returns>The filename from the last file parsed. If no file was parsed, an empty string</returns>
		const std::string& GetFileName() const;

		/// <summary>
		/// Get the shared data
		/// </summary>
		/// <returns>The address of the shared data</returns>
		SharedData* GetSharedData();

		/// <summary>
		/// Get the shared data (const)
		/// </summary>
		/// <returns>pointer to sharedData (const)</returns>
		const SharedData* GetSharedData() const;

		/// <summary>
		/// Set the shared data
		/// </summary>
		/// <param name="data">The data this master will use</param>
		/// <remarks> 
		/// NOTE: If this is a clone you *must* first retrieve the current SharedData and delete it, otherwise mem leak.
		/// </remarks>
		void SetSharedData(SharedData& data);
	private:
		bool ParseMembers(Json::Value& root, bool isArr = false, size_t index = 0);
		bool Parse(const std::string& key, Json::Value& value, bool isArr = false, size_t index = 0);
		std::string mFileName = "";
		SharedData* mData = nullptr;
		Vector<IJsonParseHelper*> mHelpers;
		bool bIsClone = false;
	};
}

