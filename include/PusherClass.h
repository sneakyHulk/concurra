#pragma once

#include "Pusher.h"

template <typename Type>
class PusherClass final : public Type, public Pusher<std::invoke_result_t<decltype(&Type::push), Type&>> {
   public:
	using Type::Type;

   private:
	std::invoke_result_t<decltype(&Type::push), Type&> push() override { return Type::push(); }
};