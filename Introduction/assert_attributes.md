 attributes formate
 ```
 [[...]]
 
[[noreturn]]              
int func(bool flag)       //
{
    throw std::runtime_error("XXX");
}
  // C++ 11 has only noreturn, carries_dependency, **useless**
  // C++ new keyword:deprecated

[[deprecated("deadline:2020-12-31")]]      // C++14 or later 
int old_func(); //when compiling: warning: ‘int old_func()’ is deprecated: deadline:2020-12-31 [-Wdeprecated-declarations]
```
In GCC standard: 
deprecated: Same as C++14, but can be used in C++11. \
unused: used for variables, types, functions, etc., indicating that although it is not used temporarily, \
it is best to keep it because it may be used in the future. \
constructor: The function will be executed before the main() function, and the effect is a bit like the constructor of the global object. \
destructor: The function will be executed after the main() function ends, a bit like the destructor of the global object.\
always_inline: Ask the compiler to force the inline function, which is stronger than the inline keyword.\
hot: Marks "hot" functions, asking the compiler to optimize more aggressively.\
```
[[gnu::unused]]      // declare the variable not used
int nouse;  
```
When the program (that is, the CPU) runs to the assert statement, the expression is evaluated, if it is false, an error message is output, \
and abort() is called to terminate the program execution.
```
assert(i > 0 && "i must be greater than zero");
assert(p != nullptr);
assert(!str.empty());
```
**static assert**
static_assert, but it's a specialized keyword, not a macro. Because it only takes effect at compile time and is\
invisible at runtime, it is "static".
```
static_assert(
  sizeof(long) >= 8, "must run on x64");
  
static_assert(
  sizeof(int)  == 4, "int must be 32bit");
```
False operation:
```
char* p = nullptr;
static_assert(p == nullptr, "some error."); // p is only valid at runtime 
```

template element:
```

// type_traits  assumed that T is template parameter, template<typename T>

static_assert(
  is_integral<T>::value, "int");

static_assert(
  is_pointer<T>::value, "ptr");

static_assert(
  is_default_constructible<T>::value, "constructible");
```