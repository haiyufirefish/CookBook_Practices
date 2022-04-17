### auto type deduction
C++ is a statically strongly typed language, Because there are many template parameters\
in generic programming, and some types have internal subtypes, it complicates the originally \
concise type system of C++ at once.\
auto is just like placeholder:
```
auto  i = 0;          // int
auto  x = 1.0;        // double

auto  str = "hello";  // char[]

std::map<int, std::string> m = {{1,"a"}, {2,"b"}};  // 

auto  iter = m.begin();  // iterator

auto  f = bind1st(std::less<int>(), 2);  //


auto        x = 10L;    // auto is long£¬x is long

auto&       x1 = x;      // auto is long£¬x1 long&
auto*       x2 = &x;    // auto is long£¬x2 long*
const auto& x3 = x;        // auto is long£¬x3 is const long&
auto        x4 = &x3;    // auto is const long*£¬x4 is const long*

```

**decltyp** auto needs deduction from experssion, without it, using decltype(like sizeof)
```

int x = 0;          // int type

decltype(x)     x1;      // int£¬x1 is int
decltype(x)&    x2 = x;    // x int£¬x2 int&£¬reference given value
decltype(x)*    x3;      // x int£¬x3 int*
decltype(&x)    x4;      // &x int*£¬x4 is int*
decltype(&x)*   x5;      // &x int*£¬x5 is int**
decltype(x2)    x6 = x2;  // x2 int&£¬x6 int&£¬reference must be assigned

```
decltype can deduce not only the value type, but also the **reference type**, which is the "primitive type" of\
the expression.

C++14: decltyp(auto):
```
int x = 0;            // int type

decltype(auto)     x1 = (x);  // int&£¬expr is reference
decltype(auto)     x2 = &x;   // int*
decltype(auto)     x3 = x1;   // int&
```
**auto** used in ¡°range-based for¡±:
```
for(auto &i : vec){
...
}
```
**decltype** applied in define of **funtion pointer**:
```

// UNIX signal function prototype
void (*signal(int signo, void (*func)(int)))(int)

// using decltype can get the type easily
using sig_func_ptr_t = decltype(&signal) ;
```
or define a class, auto is forbidden in declaration of class:
```

class DemoClass final
{
public:
    using set_type      = std::set<int>;  // alias of collection
private:
    set_type      m_set;                   // using alias for variable

    // decltype express alias
    using iter_type = decltype(m_set.begin());

    iter_type     m_pos;                   // define member variable
};
```



