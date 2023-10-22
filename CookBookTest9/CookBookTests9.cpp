#pragma once
#include<format>
#include<iostream>
#include<ranges>
#include<map>
#include<vector>
#include<filesystem>
#include<chrono>
#include<future>
#include<list>

#define Nine_2 0
#define Nine_3 0
#define Nine_4 0
#define Nine_5 0
#define Nine_6 0
#define Nine_7 0
#define Nine_8 0
#define Nine_9 1

template < typename... Args>
void print(const std::string_view fmt_str, Args&&... args) {
	auto fmt_args{ std::make_format_args(args...) };
	std::string outstr{ std::vformat(fmt_str, fmt_args) };
	fputs(outstr.c_str(), stdout);
}

namespace fs = std::filesystem;
template<>
struct std::formatter<fs::path> {
	template<typename ParseContext>
	constexpr auto parse(ParseContext& ctx) {
		return ctx.begin();
	}
	template<typename FormatContext>
	auto format(const fs::path& p, FormatContext& ctx) {
		return std::format_to(ctx.out(), "{}", p.string());
	}
};

void print(std::ranges::range auto v) {
	print("size: {}  ", v.size());
	print("[ ");
	for (const auto& i : v)print("{} ", i);
	print("]\n");
}

template<class T, class T2>
void print(const std::map<T, T2>& map) {
	print("size: {} ", map.size());
	print("[ ");
	for (auto& [k, v] : map)print("{}:{} ", k, v);
	print("]\n");
}

template<class T, class T2>
void rprint(std::multimap<T, T2>& todo) {
	for (const auto& i : todo | std::views::reverse) {
		print("{}: {}\n", i.first, i.second);
	}
	print("\n");
}

void printc(const std::ranges::range auto& v, std::string_view s = "") {
	if (s.size())print("{}: ", s);
	for (const auto& i : v)print("[{}] ", i);
	print("\n");
}

void printr(const auto& r, std::string_view s = "") {
	auto rbegin = std::rbegin(r);
	auto rend = std::rend(r);
	for (auto it = rbegin; it != rend; ++it) {
		print("{} ", *it);
	}
	print("\n");
}

/**************************************9.2******************************************************************/
using namespace std::this_thread;
using namespace std::chrono_literals;
/**************************************9.3******************************************************************/
void sleepm(const unsigned ms)
{
	using std::chrono::milliseconds;
	std::this_thread::sleep_for(milliseconds(ms));
}

void fthread(const int n)
{
	print("this is t{}\n", n);
	for (size_t i{0}; i < 5; ++i) {
		sleepm(n*100);
		print("t{}: {}\n", n, i + 1);
	}
	print("Finishing t{}\n", n);
}
/**************************************9.4******************************************************************/
using secs = std::chrono::duration<double>;

struct prime_time
{
	secs dur{}; // duration of count
	uint64_t count{};// count the prime number
};

prime_time count_prime(const uint64_t& max)
{
	prime_time ret{};
	constexpr auto isprime = [&](const uint64_t& n)
		{	
			for (uint64_t i = 2; i < sqrt(n); ++i) {
				if (n % i == 0)
					return false;
			}
			return true;
		};

	uint64_t start{ 2 };
	uint64_t end{ max };

	auto t1 = std::chrono::steady_clock::now();
	for (uint64_t i{ start }; i <= end; ++i)
	{
		if (isprime(i))
			++ret.count;

	}
	auto t2 = std::chrono::steady_clock::now();
	ret.dur = t2 - t1;
	return ret;
}

void f(std::promise<int> value)
{
	print("this is f()\n");
	value.set_value(47);
}
/**************************************9.5******************************************************************/
#include<random>
#include<execution>
#include<algorithm>

using dur_t= std::chrono::duration<double,std::milli>;

/**************************************9.6******************************************************************/
#include<mutex>
#include<optional>

std::mutex animal_mutex;
class Animal
{
	using friend_t = std::list<Animal>;
	std::string_view s_name{ "unk" };
	friend_t l_friends{};
public:
	Animal() = delete;
	Animal(const std::string_view n) :s_name{ n } {}
	bool operator== (const Animal & o)const
	{
		return s_name.data() == o.s_name.data();
	}

	bool is_friend(const Animal& o)const
	{
		for (const auto& a : l_friends)
		{
			if (a == o)
				return true;
		}
		return false;
	}

	std::optional<friend_t::iterator>find_friend(const Animal& o)noexcept
	{
		for (auto it{ l_friends.begin() }; it != l_friends.end(); ++it)
		{
			if (*it == o)
				return it;
		}
		return {};
	}

	void print()const noexcept
	{
		std::lock_guard l{ animal_mutex };
		auto n_animals{ l_friends.size() };
		::print("Animal, {}, friends:", s_name);
		if (!n_animals)
			::print("none");
		else
		{
			for (auto& n : l_friends)
			{
				std::cout << n.s_name;
				if (--n_animals)
					std::cout << ", ";
			}
		}
		endl(std::cout);
	}

	bool add_friend(Animal& o)noexcept
	{
		::print("add_friend {} -> {}\n", s_name, o.s_name);
		if (*this == o)
			return false;
		std::lock_guard l(animal_mutex); // guard should be here
		if (!is_friend(o))
			l_friends.emplace_back(o);
		if (!o.is_friend((*this)))
			o.l_friends.emplace_back(*this);
		return true;
	}

	bool delete_friend(Animal& o)noexcept
	{
		::print("delete_friend{} -> {}\n", s_name, o.s_name);
		if (*this == o)
			return false;
		std::lock_guard l(animal_mutex); // guard should be here
		if (auto it = find_friend(o))
			l_friends.erase(it.value());
		if (auto it = find_friend(*this))
			o.l_friends.erase(it.value());
		return true;
	}
}
;
/**************************************9.7******************************************************************/
#include<atomic>
std::atomic<bool>ready{};
std::atomic<uint64_t>g_count{};
std::atomic_flag winner{};

constexpr int max_count{ 1000 * 1000 };
constexpr int max_threads{ 100 };

struct Trivial
{
	int a;
	int b;
};
std::atomic<Trivial>trivial;

void countem(int id)
{
	while (!ready)
		std::this_thread::yield();
	for (int i = 0; i < max_count; ++i)
		++g_count;
	if (!winner.test_and_set())
		std::cout << std::format("thread {:02} won!\n", id);
}

std::string make_commas(const uint64_t& num)
{
	std::string s{ std::to_string(num) };
	for (long l = s.length() - 3; l > 0; l -= 3)
	{	
		s.insert(l,",");
	}
	return s;
}
/**************************************9.8******************************************************************/
constexpr size_t max_threads_{ 25 };
std::once_flag init_flag;

void do_init(size_t id)
{
	print("do init ({}): ", id);
}

void do_print(size_t id)
{
	std::call_once(init_flag, do_init, id);
	print("{} ", id);
}
/**************************************9.9******************************************************************/
#include<deque>
using namespace std::chrono_literals;

constexpr size_t num_items{ 10 };
constexpr auto delay_time{ 200ms };
std::deque<size_t>q{};
std::mutex mtx{};
std::condition_variable cond{};
bool finished{false};

void producer()
{
	for (size_t i{}; i < num_items; ++i)
	{
		std::this_thread::sleep_for(delay_time); // attention
		std::lock_guard lock{ mtx };
		std::cout << "push: " << i <<" into queue" << std::endl;
		q.push_back(i);
		cond.notify_all();
	}

	std::lock_guard lock{ mtx };
	finished = true;
	cond.notify_all();
}

void consumer()
{
	while (!finished)
	{
		std::unique_lock lck{ mtx };
		cond.wait(lck, [] {return !q.empty() || finished; });
		std::cout << std::format("Got {} from the queue \n", q.front());
		q.pop_front();
	}
}

int main()
{
	// two ways to deal with sleep 
#if Nine_2
	auto t1 = std::chrono::steady_clock::now();
	std::cout << "sleep for 1.3 seconds \n";
	sleep_for(1s + 300ms);

	std::cout << "sleep for 2 seconds \n";
	sleep_until(std::chrono::steady_clock::now() + 2s);
	std::chrono::duration<double> dur1 = std::chrono::steady_clock::now() - t1;
	// 3.303.... almost the same as sleeping time
	print("{:.5}s\n", dur1.count());

#endif
	// using std::thread for cocurrency
#if Nine_3
	std::thread t1(fthread, 1);
	t1.join();

	std::thread t2(fthread, 2);
	t2.detach();
	//sleepm(2000);// make sure after detach it has enough running time 

	//  ~jthread() {
	/*_Try_cancel_and_join();
	} // automatically join the caller at the end of its scope
	*/  

	std::jthread t3(fthread, 3);
	print("end of main()\n");

#endif
#if Nine_4
	//131071
	constexpr uint64_t MAX_PRIME{ 0x1FFFF };
	auto primes1 = std::async(count_prime, MAX_PRIME);
	auto pt = primes1.get();
	print("primes: {} {:.3}\n", pt.count, pt.dur);

	std::list<std::future<prime_time>>swarm;
	print("start parallel primes\n");
	auto t1{ std::chrono::steady_clock::now() };
	for (size_t i = 0; i < 15; ++i)
	{
		swarm.emplace_back(std::async(std::launch::async, count_prime, MAX_PRIME));
		//swarm.emplace_back(std::async(std::launch::deferred, count_prime, MAX_PRIME));
	}
	for (size_t i = 0; auto & f: swarm)
	{
		auto pt = f.get();
		print("primes({:02}): {} {:.5}\n", ++i, pt.count, pt.dur);
	}

	secs dur_total{ std::chrono::steady_clock::now() - t1 };
	print("total duration is {:.5}s\n", dur_total.count());

	std::promise<int>value_promise;
	std::future<int>value_future = value_promise.get_future();
	std::thread t2(f, std::move(value_promise)); // uncopyable, only move 
	t2.detach();
	print("value is {}\n", value_future.get());

	print("end of main()\n");
#endif
#if Nine_5
	std::vector<unsigned>v(10 * 1000 * 1000);
	std::random_device rng;
	for (auto& i : v)
		i = rng() % 0xFFFF;
	auto mul2 = [](int n)
		{
			return n * 2;
		};

	auto t1 = std::chrono::steady_clock::now();
	std::transform(v.begin(), v.end(), v.begin(), mul2);
	dur_t dur1 = std::chrono::steady_clock::now() - t1;
	print("no policy duration: {:3}ms \n", dur1.count());

	auto t2 = std::chrono::steady_clock::now();
	std::transform(std::execution::seq, v.begin(), v.end(), v.begin(), mul2);
	dur_t dur2 = std::chrono::steady_clock::now() - t2;
	print("std::execution::seq duration: {:3}ms \n", dur2.count());

	auto t3 = std::chrono::steady_clock::now();
	std::transform(std::execution::par, v.begin(), v.end(), v.begin(), mul2);
	dur_t dur3 = std::chrono::steady_clock::now() - t3;
	print("std::execution::par duration: {:3}ms \n", dur3.count());

	auto t4 = std::chrono::steady_clock::now();
	std::transform(std::execution::par_unseq, v.begin(), v.end(), v.begin(), mul2);
	dur_t dur4 = std::chrono::steady_clock::now() - t4;
	print("std::execution::par_seq duration: {:3}ms \n", dur4.count());

	// 400-600ms
	auto t5 = std::chrono::steady_clock::now();
	std::sort(v.begin(), v.end());
	dur_t dur5 = std::chrono::steady_clock::now() - t5;
	print("sort duration: {:3}ms \n", dur5.count());

	// 100-150 ms
	/*auto t6 = std::chrono::steady_clock::now();
	std::sort(std::execution::par,v.begin(), v.end());
	dur_t dur6 = std::chrono::steady_clock::now() - t6;
	print("sort duration: {:3}ms \n", dur6.count());*/

	// 120 ms about
	//auto t7 = std::chrono::steady_clock::now();
	//std::sort(std::execution::par_unseq,v.begin(), v.end());
	//dur_t dur7 = std::chrono::steady_clock::now() - t7;
	//print("sort duration: {:3}ms \n", dur7.count());

#endif
#if Nine_6
	auto cat1 = std::make_unique<Animal>("Felix");
	auto tiger1 = std::make_unique<Animal>("Hobbes");
	auto dog1 = std::make_unique<Animal>("Astro");
	auto rabbit1 = std::make_unique<Animal>("Bugs");

	auto a1 = std::async([&] {cat1->add_friend(*tiger1); });
	auto a2 = std::async([&] {tiger1->add_friend(*rabbit1); });
	auto a3 = std::async([&] {dog1->add_friend(*dog1); });
	auto a4 = std::async([&] {rabbit1->add_friend(*cat1); });

	a1.wait();
	a2.wait();
	a3.wait();
	a4.wait();

	auto p1 = std::async([&] {cat1->print(); });
	auto p2 = std::async([&] {tiger1->print(); });
	auto p3 = std::async([&] {dog1->print(); });
	auto p4 = std::async([&] {rabbit1->print(); });
	p1.wait();
	p2.wait();
	p3.wait();
	p4.wait();

	auto a5 = std::async([&] {cat1->delete_friend(*rabbit1); });
	a5.wait();
	auto p5 = std::async([&] {cat1->print(); });
	auto p6 = std::async([&] {rabbit1->print(); });
#endif
#if Nine_7
	//using atomic to share label and value
	std::vector<std::thread>swarm;
	std::cout << std::format("spawn {} threads\n", max_threads);
	for (int i{}; i < max_threads; ++i)
		swarm.emplace_back(countem, i);

	ready = true;
	for (auto& t : swarm)
		t.join();
	std::cout << std::format("global count: {} \n", make_commas(g_count));

	std::cout << std::format("is g_count lock-free? {}\n", g_count.is_lock_free());

#endif
#if Nine_8
	std::list<std::thread>spawn;
	for (size_t id = 0; id < max_threads_; id++)
	{
		spawn.emplace_back(do_print, id);
	}
	for (auto& t : spawn)
		t.join();
	std::cout << '\n';
#endif
#if Nine_9
	std::thread t1{ producer };
	std::thread t2{ consumer };

	t1.join();
	t2.join();
	print("finished!");
#endif
}

