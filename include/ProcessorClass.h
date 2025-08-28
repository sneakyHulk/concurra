#pragma once

#include <type_traits>

#include "Processor.h"
#include "function_traits_helper.h"

template <typename Type>
class ProcessorClass final : public Type,
                             public Processor<std::remove_cvref_t<typename function_traits<decltype(&Type::process)>::template arg<0>>,
                                 std::invoke_result_t<decltype(&Type::process), Type&, std::remove_cvref_t<typename function_traits<decltype(&Type::process)>::template arg<0>> const&>> {
   public:
	using Type::Type;

   private:
	std::invoke_result_t<decltype(&Type::process), Type&, std::remove_cvref_t<typename function_traits<decltype(&Type::process)>::template arg<0>> const&> process(
	    std::remove_cvref_t<typename function_traits<decltype(&Type::process)>::template arg<0>> const& arg) override {
		return Type::process(arg);
	}
};