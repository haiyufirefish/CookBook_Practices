 Preprocessing directives all start with the symbol "#". But at the same time, you should also realize that although it is all in one source file, 
 it does not belong to the C++ language. It uses the preprocessor and is not bound by the C++ grammar rules.
 
```
#if __linux__  // preprocessing checks if the macro exists
	# define HAS_LINUX 1 // macro definition, indented
#endif

```
"#include" can import source code, ordinary text, and even pictures, audio, and video
Therefore, when writing a header file, in order to prevent the code from being repeatedly included, 
it is usually necessary to add "Include Guard", that is, use "#ifndef/#define/#endif" to protect the entire header file
```
#ifndef _XXX_H_INCLUDED_
#define _XXX_H_INCLUDED_... //
  content of header
#endif // _XXX_H_INCLUDED_
```
When using macros, man must be cautious, and always keep in mind that the goal is to simplify the code, 
make it clear and understandable, and don't "abuse" it, so as to avoid confusing the source code and reducing readability.

Nginx example 
```
#define ngx_tolower(c) ((c >= 'A' && c <= 'Z') ? (c | 0x20) : c)
#define ngx_toupper(c) ((c >= 'a' && c <= 'z') ? (c & ~0x20) : c)
#define ngx_memzero(buf, n) (void) memset(buf, 0, n)
```
Macros have no concept of scope and are always in effect globally
```
#define CUBE(a) (a) * (a) * (a) // define a simple macro
cout << CUBE(10) << endl; // 
cout << CUBE(15) << endl; // 
#undef CUBE  // undefine it after completing the task
```
Other way is checking before compiling:
```
#ifdef AUTH_PWD // check if the macro exists
# undef AUTH_PWD // cancel macro
#endif // macro check end
#define AUTH_PWD "xxx"
```

From the book "effective c++", use macros as less as possible. But for some definitions, macros are conventions
```
#define MAX_BUF_LEN 65535
#define VERSION "1.0.18"
```

Condition compiling: #if/#else/#endif
g++ -E -dM - < /dev/null
in nginx, using shell to detect external environment
```
#if (NGX_FREEBSD)
#  include <ngx_freebsd.h>

#elif (NGX_LINUX)
#  include <ngx_linux.h>

#elif (NGX_SOLARIS)
#  include <ngx_solaris.h>

#elif (NGX_DARWIN)
#  include <ngx_darwin.h>
#endif
```
Some tricks:
```
#if 0 // 0 forbid the following code ... 1 execute the followind code// 
code....
#endif // 
```

Constexpr is a compile-time constant, which is equivalent to a preprocessing-time constant, but has a **different life cycle**.
```
#define PI (3.14) -> constexpr float PI = 3.14
```


