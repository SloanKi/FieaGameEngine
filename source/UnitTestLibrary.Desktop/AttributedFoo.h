#pragma once
#include "RTTI.h"
#include "vector.h"
#include "Signature.h"
#include "Attributed.h"
#include <glm/glm.hpp>
#include "Foo.h"
#include "Scope.h"

using namespace Library;

namespace UnitTests
{
	class AttributedFoo final : public Library::Attributed
	{
		RTTI_DECLARATIONS(AttributedFoo, Attributed)

	public:
		AttributedFoo();
		AttributedFoo(const AttributedFoo& rhs) = default;
		/*AttributedFoo(const AttributedFoo& rhs) :
			Attributed(rhs)*/
		AttributedFoo(AttributedFoo&& rhs) noexcept = default;
		~AttributedFoo() = default;
		AttributedFoo& operator=(const AttributedFoo& rhs) = default;
		AttributedFoo& operator=(AttributedFoo&& rhs) noexcept = default;

		bool AttributedFoo::Equals(const RTTI* rhs) const override;

		virtual gsl::owner<AttributedFoo*> Clone() const override;

		static Vector<Signature> Signatures();


		int ExternalInteger = 4;
		float ExternalFloat = 52.0f;
		glm::vec4 ExternalVector = glm::vec4(123.234f);
		glm::mat4 ExternalMatrix = glm::mat4(1.234f);
		std::string ExternalString = "Meow";
		RTTI* ExternalRTTI = &foo;
		Scope InternalScope;

		int ExternalIntArr[5] { 1, 2, 3, 4, 5 };
		float ExternalFloatArr[5] { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f };
		glm::vec4 ExternalVecArr[5] { glm::vec4(1), glm::vec4(2), glm::vec4(3), glm::vec4(4), glm::vec4(5) };
		glm::mat4 ExternalMatArr[5] { glm::mat4(1), glm::mat4(2), glm::mat4(3), glm::mat4(4), glm::mat4(5) };
		std::string ExternalStringArr[5] { "Meow", "Woof", "So", "much", "testing" };
		Scope InternalScopeArr[5];

	private:
		Foo foo = Foo(2);
	};
}

