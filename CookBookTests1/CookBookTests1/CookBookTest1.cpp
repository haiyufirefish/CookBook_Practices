// CookBookTests.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include<iostream>
#include<string_view>
#include<format>

//****************************1.2**************************************//
template <typename... Args>
void print(const std::string_view fmt_str, Args&&... args)
{
    auto fmt_args{ std::make_format_args(args...) };
    std::string outstr{ std::vformat(fmt_str,fmt_args) };
   // fputs(outstr.c_str, stdout); here c_str is not a pointer
    fputs(outstr.c_str(), stdout);
}

struct Frac
{
    int a, b;
};

// a formatter overloaded is required here for Frac
template<>
struct std::formatter<Frac>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }

    template<typename ParseContext>
    auto format(const Frac &f,ParseContext& ctx)
    {
        return std::format_to(ctx.out(), "{0:d}/{1:d}", f.a, f.b);
    }
};

//****************************1.3**************************************//
#include<vector>
#include<array>

constexpr auto f3()
{
    std::vector<int>v { 1,2,3 };

    //return std::array<int, 3>{1, 2, 3};//a fixed-size array
    return v;
}

//****************************1.3**************************************//
template<class T,class U>
constexpr bool cmp_less(T t, U u) noexcept
{
    using UT = std::make_unsigned_t<T>;
    using UU = std::make_unsigned_t<U>;
    // is_signed_v == is_signed<>::value
    //if constexpr (std::is_signed<T>::value == std::is_signed<U>::value)
    if constexpr (std::is_signed_v<T> == std::is_signed_v<U>)
        return t < u;
    else if constexpr (std::is_signed_v<T>)
        return t < 0 ? true : UT(t) < u;
    else
        return u < 0 ? false : t < UU(u);
}

//****************************1.4**************************************//

constexpr auto f2()
{
    int* p = new int{ 10 };
    // not delete p
    return *p;
}

//****************************1.5**************************************//
struct X
{
    int a{};
    double b{};
    char c{};
    // it doesnt't need overload
    friend auto operator<=>(const X&, const X&) = default;
};

struct Y
{
    int a = 6;
};
// self defined
auto operator<=>(const Y& y1, const Y& y2)noexcept->int
{
    if (y1.a == y2.a)
        return 0;
    if (y1.a > y2.a)
        return 1;
    if (y1.a < y2.a)
        return -1;
}

auto operator==(const Y& y1, const Y& y2)noexcept->bool
{
    return y1.a == y2.a;
}

//****************************1.6**************************************//
#include<version>

#ifdef __cpp_lib_three_way_comparison
#include<compare>
#else
# error no_header
#endif

#ifdef __cpp_lib_format
#include<format>
#else
# error no_header
#endif //

//#if __has_include(<iostream>)
//    #include<iostream>
//#endif

//****************************1.7**************************************//
template<std::integral T>
void f6(T t)
{}

template<class T>
    requires std::integral<T> || std::is_pointer_v<T>
struct X2
{};

template<class T>
    requires std::is_integral_v<T>
T n7{};

template<class T>
concept love = std::is_integral_v<T> && (std::is_same_v<int, T> || std::is_same_v<uint32_t, T>);

void f7(love auto) {}

//****************************1.8**************************************//
import test1;

//****************************1.9**************************************//
#include<ranges>
#include<vector>

namespace stdv = std::views;
namespace stdr = std::ranges;

void print3(stdr::range auto v)
{
    for (const auto& i : v)
        std::cout << i << ' ';
    endl(std::cout);
}

int main()
{   
    // 1.2
    //Frac f{ 1,10 };
    //print("{}", f);

    // 1.3
    constexpr auto n1 = f3().size(); // ok
    //constexpr auto n2 = f3();  // not ok, as f3() is not a const value
    //constexpr auto n3 = f2();  // not ok, as f2() is not a const value

    // 1.4
    //std::cout << std::boolalpha << (5u < -1) << '\n'; // true
    //std::cout << std::boolalpha << ::cmp_less(5u, -1) << '\n'; // false
    //std::cout << std::boolalpha << (5u < 2u) << '\n'; // false
    //std::cout << std::boolalpha << ::cmp_less(5u,2u) << '\n'; // false

    // 1.5
    //X x{ 10,1.2,'*' };
    //X x2{ 10,1,'*' };
    //std::cout << std::boolalpha << (x == x2) << '\n'; // false
    //std::cout << std::boolalpha << (x <= x2) << '\n'; // false
    //std::cout << std::boolalpha << (x != x2) << '\n'; // true
    //std::cout << std::boolalpha << (x >= x2) << '\n'; // true
    //Y y{ 1 };
    //Y y2{ 2 };
    //std::cout << (y <=> y2) << '\n';				//-1
    //std::cout << std::boolalpha << (y > y2) << '\n';//false
    //std::cout << std::boolalpha << (y < y2) << '\n';//true
    //std::cout << std::boolalpha << (y != y2) << '\n';//true

    // 1.6
    //std::cout << __cpp_lib_three_way_comparison << '\n'; // 201907, from 2019.07 is accepted
    //std::cout << __cpp_lib_format << '\n';// 202207, from 2022.07 is accepted

    //1.7
    //f6(1);
    //f6('*');
    ////f6(1.2); // invalid
    //X2<int>x1;
    ////X2<double>x2;// invalid
    //X2<double*>x3; 
    //n7<int> = 3;
    ////n7<double> = 3.0;//invalid?
    //std::cout << n7<int> << '\n';
    //f7(1);
    //f7(1u);
    ////f7(1l);//invalid

    // 1.8
    // import test1; define test1 is not global invalid
   // std::cout << mylib::add_(1, 2) << '\n';
   //// std::cout << mylib::PI << '\n'; invalid
   // std::cout << PI1 << '\n'; // ok
   // tt();

    // 1.9
    //std::vector nums{ 1,2,3,4,5,6,7,8,9,10 };
    //auto ret = nums | stdv::take(5) | stdv::reverse;
    //print3(ret); // 5 4 3 2 1
    //auto ret2 = nums | stdv::filter([](int i) {return i > 6; });
    //print3(ret2); // 7, 8 ,9 , 10
    //auto ret3 = nums | stdv::transform([](int i) {return i * i; });
    //print3(ret3); // 1,4,9 ....
    //print3(nums); // view doesnt' change orignal
    //auto ret5 = nums | stdv::filter([](int i) {return i % 2 != 0; }) | stdv::transform([](int i) {return i * 2; });
    //print3(ret5);
    //auto nums_ = stdv::iota(1, 10);
    //print3(nums_);
    //// 1 to 200
    //auto rnums = stdv::iota(1) | stdv::take(200);
    //print3(rnums);
   // stdr::copy(nums_ | stdv::reverse | stdv::drop(2), std::ostream_iterator<std::string>(std::cout, " "));
    return 0;

}

