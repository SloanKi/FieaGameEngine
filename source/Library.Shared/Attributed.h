#pragma once
#include "RTTI.h"
#include "Scope.h"


namespace Library
{
	class Attributed : public Scope
	{
		RTTI_DECLARATIONS(Attributed, Scope)
		
	public:
		Attributed() = delete;
		virtual ~Attributed() = default;

		/// <summary>
		/// Constructor: Uses the given typeId to retrieve signatures from the type registry
		/// Will explode if type register is not created or if type is not registered.
		/// </summary>
		/// <param name="typeID">the rtti type id of the class</param>
		Attributed(RTTI::IdType typeID);

		/// <summary>
		/// Copy Constructor
		/// </summary>
		/// <param name="rhs">the attributed scope to copy</param>
		Attributed(const Attributed& rhs);

		/// <summary>
		/// Move constructor
		/// </summary>
		/// <param name="rhs"> the attributed scope to move into this one</param>
		/// <returns></returns>
		Attributed(Attributed&& rhs) noexcept;

		/// <summary>
		/// Copy assignment operator
		/// </summary>
		/// <param name="rhs">the attributed scope to copy</param>
		/// <returns>a deep copy of rhs</returns>
		Attributed& operator=(const Attributed& rhs);

		/// <summary>
		/// Move assignment operator
		/// </summary>
		/// <param name="rhs">the attributed scope to move into this one</param>
		/// <returns>this with rhs moved into it</returns>
		Attributed& operator=(Attributed&& rhs) noexcept;

		/// <summary>
		/// Checks to see if an attribute with the given name exists within this scope
		/// </summary>
		/// <param name="name">the name of the attribute to search for</param>
		/// <returns>True if an attribute with the given name exists in this scope, false otherwise</returns>
		/// <remarks>Uses scope::find</remarks>
		bool IsAttribute(std::string name) const;

		/// <summary>
		/// Checks to see if an attribute with the given name is a prescribed attribute
		/// </summary>
		/// <param name="name">The name of the attribute to search for</param>
		/// <returns>True if an attribute with the given name is prescribed, false otherwise</returns>
		bool IsPrescribedAttribute(std::string name) const;

		/// <summary>
		/// Checks to see if attribute exists within scope and is not prescribed.
		/// </summary>
		/// <param name="name">The name of the attribute to search for </param>
		/// <returns>True if the attribute exists and is not prescribed, false otherwise</returns>
		bool IsAuxiliaryAttribute(std::string name) const;

		/// <summary>
		/// Populates the scope that mirrors this class. 
		/// Populates string datum pairs from signatures retrieved from TypeManager
		/// NOTE: IF TYPE IS NOT REGISTERED IN TYPEMANAGER, EVERYTHING WILL EXPLODE.
		/// </summary>
		/// <param name="typeID">The typeID of this instance (used to get signatures)</param>
		void Populate(RTTI::IdType typeID);

		/// <summary>
		/// Appends an auxiliary attribute
		/// Will create a new datum to store attribute if it does not yet exist.
		/// </summary>
		/// <param name="name">the name of the attribute</param>
		/// <returns>A reference to the datum with the given name</returns>
		/// <exception cref="std::runtime_error">Throws if attribute is a prescribed attribute, or if string is empty</exception>
		/// <remarks>Is a thin wrapper to Scope::Append</remarks>
		Datum& AppendAuxiliaryAttribute(std::string name);

		/// <summary>
		/// Appends an auxiliary attribute
		/// Will create a new datum to store attribute if it does not yet exist.
		/// </summary>
		/// <param name="name">the name of the attribute</param>
		/// <param name="EntryCreated">A bool out parameter indicating whether a new entry was created or not</param>
		/// <returns>A reference to the datum with the given name</returns>
		/// <exception cref="std::runtime_error">Throws if attribute is a prescribed attribute, or if string is empty</exception>
		/// <remarks>Is a thin wrapper to Scope::Append</remarks>
		Datum& AppendAuxiliaryAttribute(std::string name, bool& EntryCreated);

		/// <summary>
		/// RTTI Equals overload
		/// </summary>
		/// <param name="rhs">the RTTI pointer to compare to</param>
		/// <returns>True if equal, false otherwise</returns>
		bool Equals(const RTTI* rhs) const override;

		/// <summary>
		/// Returns all attributes
		/// </summary>
		/// <returns>All attributes</returns>
		/// Ask paul: does this violate constness since the Datum could be changed?
		const Vector<std::pair<const std::string, Datum>*>& Attributes() const;

		/// <summary>
		/// Returns all prescribed attributes
		/// </summary>
		/// <returns>a vector of all attributes</returns>
		Vector<std::pair<const std::string, Datum>*> PrescribedAttributes() const;

		/// <summary>
		/// Returns all auxiliary attributes
		/// </summary>
		/// <returns>a vector of all attributes</returns>
		Vector<std::pair<const std::string, Datum>*> AuxiliaryAttributes() const;

	private:
		void UpdateExternalStorage(RTTI::IdType typeID);
	};
}

