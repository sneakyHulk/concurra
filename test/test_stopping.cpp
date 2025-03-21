#include <functional>
#include <iostream>
#include <tuple>

template <typename... T>
[[maybe_unused]] std::ostream& operator<<(std::ostream& stream, std::tuple<T...> const& tup) {
	static auto print_tuple = []<std::size_t... I>(std::ostream& stream, std::tuple<T...> const& tup, std::index_sequence<I...>) {
		char space[]{0, 0, 0};
		(... << (stream << space << std::get<I>(tup), *space = ',', *(space + 1) = ' '));
	};

	stream << '(';
	print_tuple(stream, tup, std::make_index_sequence<sizeof...(T)>());
	stream << ')';

	return stream;
}
template <typename... T>
[[maybe_unused]] std::ostream& operator<<(std::ostream& stream, std::tuple<T...>&& tup) {
	return operator<<(stream, tup);
}

#include <chrono>
#include <iostream>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

template <std::size_t I0, std::size_t I1, std::size_t I, typename T>
requires(I0 <= I && I < I1) [[maybe_unused]] constexpr std::tuple<T> get_single_tuple_part();

template <std::size_t I0, std::size_t I1, std::size_t I, typename T>
requires(!(I0 <= I && I < I1)) [[maybe_unused]] constexpr std::tuple<> get_single_tuple_part();

template <typename>
struct tuple_part_from_parameter_pack_helper2;

template <typename... Ts>
struct tuple_part_from_parameter_pack_helper2<std::tuple<Ts...>> {
	using type = std::tuple<Ts...>;
};

template <std::size_t I0, std::size_t I1, typename... T>
struct tuple_part_from_parameter_pack_helper1;

template <std::size_t I0, std::size_t I1, std::size_t... Is, typename... T>
struct tuple_part_from_parameter_pack_helper1<I0, I1, std::index_sequence<Is...>, T...> : public tuple_part_from_parameter_pack_helper2<decltype(std::tuple_cat(get_single_tuple_part<I0, I1, Is, T>()...))> {};

template <std::size_t I0, std::size_t I1, typename... T>
using tuple_part_from_parameter_pack = tuple_part_from_parameter_pack_helper1<I0, I1, std::make_index_sequence<sizeof...(T)>, T...>::type;

template <std::size_t split, typename... INPUTS_OUTPUTS>
class Node {
   public:
	Node() = default;
	bool function(tuple_part_from_parameter_pack<0, split, INPUTS_OUTPUTS...> const& inputs, tuple_part_from_parameter_pack<split, sizeof...(INPUTS_OUTPUTS), INPUTS_OUTPUTS...>& outputs) {
		std::cout << "HI" << std::endl;

		return false;
	}

	void connect();
};

enum class RETURN_VALUE { CONTINUE, SKIP, LAST, STOP, ERROR };
enum class INFO { NORMAL, FIRST, LAST };

class NODE {
	static std::vector<NODE*> nodes;

   protected:
	NODE() { nodes.push_back(this); }

   public:
	virtual void name() { std::cout << typeid(*this).name() << std::endl; }
	virtual void stop() = 0;
	virtual void start() = 0;

	static void exit() {
		for (auto& node : nodes) node->stop();
	}
};

template <typename OUTPUT>
class PUSHER;

template <typename INPUT, typename OUTPUT>
class PROCESSOR;

template <typename INPUT>
class RUNNER;

template <typename OUTPUT>
class PUSHER : public NODE {
	std::vector<std::function<void(OUTPUT const&)>> synchronous_functions;
	std::vector<std::function<void(std::shared_ptr<OUTPUT const>)>> asynchronous_functions;

	bool synchronous_call() {
		static OUTPUT output;
		static RETURN_VALUE return_value;

		if (stop_requested()) {
			RETURN_VALUE value = push_last(output);
		}

		if (static bool first = true; std::exchange(first, false)) {
			RETURN_VALUE value = push_once(output);

			if (value == RETURN_VALUE::SKIP) [[unlikely]]
				first = true;
		} else {
			RETURN_VALUE value = push(output);
		}

		if (return_value == RETURN_VALUE::CONTINUE) [[likely]] {
			if (!asynchronous_functions.empty()) {
				for (std::shared_ptr<OUTPUT const> output_ptr = std::make_shared<OUTPUT const>(output); auto& connection : asynchronous_functions) connection(output_ptr);
			}
			for (auto& connection : synchronous_functions) connection(output);
		}

		if (return_value == RETURN_VALUE::STOP) {
			~PUSHER();
		}
	}

	std::stop_source stop_source;
	std::thread thread;
	bool running = false;

   protected:
	PUSHER() = default;
	bool stop_requested() { return stop_source.stop_requested(); }

   public:
	void start() override {
		if (std::exchange(running, true))
			thread = std::thread([this] {
				while (synchronous_call())
					;
			});
	}

	 ~PUSHER() = default;

	virtual RETURN_VALUE push_first(OUTPUT& output) { return push(output); }
	virtual RETURN_VALUE push(OUTPUT& output) = 0;
	virtual RETURN_VALUE push_last(OUTPUT& output) { return RETURN_VALUE::STOP; }
};

// template <typename INPUT>
// class RUNNER : public NODE {
// 	std::stop_source stop_source;
// 	std::thread thread;
// 	bool running = false;
//
// 	bool synchronous_call(INPUT const&) {
// 		static RETURN_VALUE return_value;
//
// 		if (stop_requested()) {
// 		}
// 		RETURN_VALUE value = push(output);
//
//
// 		if (return_value == RETURN_VALUE::CONTINUE) [[likely]] {
// 			if (!asynchronous_functions.empty()) {
// 				for (std::shared_ptr<OUTPUT const> output_ptr = std::make_shared<OUTPUT const>(output); auto& connection : asynchronous_functions) connection(output_ptr);
// 			}
// 			for (auto& connection : synchronous_functions) connection(output);
// 		}
//
// 		if (return_value == RETURN_VALUE::STOP) {
// 			~PUSHER();
// 		}
// 	}
//
//
//
// 	void start() override {
// 		if (std::exchange(running, true))
// 			thread = std::thread([this] {
// 				while (synchronous_call())
// 					;
// 			});
// 	}
//
//    public:
// 	virtual ~RUNNER() = default;
//
// 	virtual RETURN_VALUE run(INPUT& input) = 0;
// 	virtual RETURN_VALUE run_last(INPUT& input) { return RETURN_VALUE::STOP; }
//
// 	bool stop_requested() { return stop_source.stop_requested(); }
// };

class t1 {

   public:
	~t1() {
		std::cout << "~t1()" << std::endl;
	}
};

class t2 : public t1 {
   public:
	~t2() {
		std::cout << "~t2()" << std::endl;
	}
};

int main() {
	t2 t22;
	tuple_part_from_parameter_pack<0, 2, double, int> x;
	std::get<0>(x) = 0.1;
	std::cout << std::get<0>(x) << std::endl;
	std::cout << std::get<1>(x) << std::endl;

	Node<0, double, int, int> t;

	std::cout << std::boolalpha << std::is_same_v<tuple_part_from_parameter_pack<0, 2, double, int>, std::tuple<double, int>> << std::endl;

	std::tuple<double, int> z = static_cast<std::tuple<double, int>>(tuple_part_from_parameter_pack<0, 2, double, int>{});

	std::cout << z << std::endl;

	std::tuple<> const i;
	std::tuple<double, int, int> o;
	t.function(i, o);
}

// template <std::size_t split, std::size_t... I, typename... INPUTS_OUTPUT>
// using INPUT =

// #include <tuple>
// #include <type_traits>
//
// template <typename...>
// struct TypeA {};
//
// template <std::size_t Imin, std::size_t Imax, std::size_t I, typename T>
// std::enable_if_t<(Imin <= I) && (I < Imax), std::tuple<T>> getTpl();
//
// template <std::size_t Imin, std::size_t Imax, std::size_t I, typename>
// std::enable_if_t<(I < Imin) || (Imax <= I), std::tuple<>> getTpl();
//
// template <typename>
// struct pta_helper2;
//
// template <typename... Ts>
// struct pta_helper2<std::tuple<Ts...>> {
// 	using type = TypeA<Ts...>;
// };
//
// template <std::size_t, std::size_t, typename... Ts>
// struct pta_helper1;
//
// template <std::size_t I0, std::size_t I1, std::size_t... Is, typename... Ts>
// struct pta_helper1<I0, I1, std::index_sequence<Is...>, Ts...> : public pta_helper2<decltype(std::tuple_cat(getTpl<I0, I1, Is, Ts>()...))> {};
//
// template <std::size_t I0, std::size_t I1, typename... Ts>
// using proTypeA = typename pta_helper1<I0, I1, std::make_index_sequence<sizeof...(Ts)>, Ts...>::type;
//
// template <typename... Ts>
// struct TypeB : public proTypeA<0u, sizeof...(Ts) / 2u, Ts...>, public proTypeA<sizeof...(Ts) / 2u, sizeof...(Ts), Ts...> {};
//
// int main() {
// 	using tb = TypeB<char, short, int, long, long long>;
// 	using ta1 = TypeA<char, short>;
// 	using ta2 = TypeA<int, long, long long>;
//
// 	static_assert(std::is_base_of<ta1, tb>::value, "!");
// 	static_assert(std::is_base_of<ta2, tb>::value, "!");
// }

#include <tuple>

using namespace std::chrono_literals;

class TEST {
   public:
	TEST() = default;

	std::jthread* thread = nullptr;
	[[nodiscard("The thread would stop immediately!")]] std::jthread operator()() {
		std::jthread tmp = std::jthread([this, &tmp](std::stop_token const& _stop_token) {
			thread = &tmp;

			std::this_thread::sleep_for(std::chrono::seconds(2));
		});
		std::this_thread::sleep_for(std::chrono::seconds(1));
		thread->request_stop();

		return tmp;
	}
};

// int main() {
//	TEST test;
//
//	auto thread = test();
//
//	std::this_thread::sleep_for(std::chrono::seconds(3));
// }

// class Push1 : public Pusher<int> {
//	int i = 0;
//
//    public:
//	Push1() { std::cout << "push1()" << std::endl; }
//	int push() final {
//		std::cout << ++i << " push1.push()" << std::endl;
//
//		Pusher<int>::thread->request_stop();
//		return i;
//	}
//	int push_once() final {
//		std::cout << ++i << " push1.push_once()" << std::endl;
//		return i;
//	}
// };
//
// class Run1 : public Runner<int> {
//    public:
//	Run1() { std::cout << "run1()" << std::endl; }
//	void run_once(int const &data) final { return run(data); }
//	void run(int const &data) final { std::cout << "run1.run(" << data << ")" << std::endl; }
// };
//
// int main() {
//	Push1 push1;
//	Run1 run1;
//
//	push1.synchronously_connect(run1);
//
//	auto t1 = push1();
//
//	std::this_thread::sleep_for(1000ms);
// }