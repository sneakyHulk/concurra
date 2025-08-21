#pragma once

#include "RunnerSynchronous.h"
#include "function_traits_helper.h"

template <typename Type>
class RunnerSynchronousClass final : public Type, public RunnerSynchronous<std::remove_cvref_t<typename function_traits<decltype(&Type::run)>::template arg<0>>> {
   public:
	using Type::Type;

   private:
	void run(std::remove_cvref_t<typename function_traits<decltype(&Type::run)>::template arg<0>> const& arg) override { return Type::run(arg); }
};