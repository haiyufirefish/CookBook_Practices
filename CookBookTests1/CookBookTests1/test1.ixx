module;

#define PI 3.14 // cannot access, is not module

export module test1;
export import <iostream>;
export import test2;
export const double PI1 = 3.14; //ok

namespace mylib
{
	export auto add_(std::integral auto a, std::integral auto b)
	{
		return a + b;
	}

	auto print2(auto t)
	{
		std::cout << t << '\n';
	}
}