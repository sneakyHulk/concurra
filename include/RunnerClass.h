#pragma once

#include "Runner.h"
#include "function_traits_helper.h"

template <typename Type>
class RunnerClass final : public Type, public Runner<std::remove_cvref_t<typename function_traits<decltype(&Type::run)>::template arg<0>>> {
   public:
	using Type::Type;

   private:
	void run(std::remove_cvref_t<typename function_traits<decltype(&Type::run)>::template arg<0>> const& arg) override { return Type::run(arg); }
};