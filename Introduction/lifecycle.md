# waterfall model
coding, pre-processing, compiling, running.
pre-processing:#include¡¢#define¡¢#if...

compiling: Linking and compiling. High level technology: template metaprogramming
running: GDB debugging, log tracking, performance analysis, etc., and then collect dynamic data and adjust design ideas
The earlier a bug is found and fixed, the more valuable it is; the later a bug is found and fixed, the more expensive it is.

Paradigms: "procedural, "object-oriented," "generic," "template", "functional"
Procedural: Its core idea is commands, which are usually statements and subprograms (functions) executed in sequence. 
The task is decomposed into several steps to execute, and finally the goal is achieved.
OO: The core idea is abstract and encapsulation
generic programming: Everything is a type, or a parameterized type. Reuse code using templates instead of inheritance: **template**
template metaprogramming: type operation, Code can only be executed by the compiler. **type_traits¡¢enable_if**
functional: The core idea is "everything can be called", and data processing is realized through a series of consecutive or nested function calls. **lambda""

**readability and maintainability are the first principles.**

CamelCase: mainly used in java: MyJobClass¡¢tryToLock etc.
snake_case: in c and c++: all in lowercase: vector¡¢unordered_set

Take the best of all:
Use snake_case for variable, function names, and namespaces, and prefix global variables with "g_";
Custom class names use CamelCase, member functions use snake_case, and member variables are prefixed with "m_";
Macros and constants should be all uppercase, with underscores between words;
Try not to use underscore as a prefix or suffix for variables (such as _local, name_), it is difficult to identify.

```
#define MAX_PATH_LEN 256 //constants£¬all uppercase

int g_sys_flag; // global variable£¬g_ prefix

namespace linux_sys // namespace, all lowercase
{  
	void get_rlimit_core(); // function£¬all lowercase
}

class FilePath final // class£¬first alphabet uppercase
{
public: void set_path(const string& str); // function all lowercase

private: 
		string m_path; // member variable£¬m_prefix
		int m_level; // 
};

```
The length of a variable/function name is proportional to its scope.

**comment**:
```
// author : fish
// date : 2020-xx-xx
// purpose : get inner counter value of generic T
// notice : T must have xxx member
// notice : return value maybe -1, means xxx, you should xxx
templateint get_value(const T& v);
```

Should function comments be written at the declaration or at the definition?
Answer: If it is released externally, it is written in the declaration, and if it is used internally, it is in the definition.
But it must be noted that it is best not to have comments in both places, otherwise once there are changes, 
it will be troublesome to maintain synchronization.

plugin in vscode to define your code style:**astyle**