# CPP Study Guide

#cpp #studyguide #studymaterials 

Systems/Low-level C++ 
Review OS stuff *Important*: [http://pages.cs.wisc.edu/~remzi/Classes/537/Spring2018/Discussion/videos.html](http://pages.cs.wisc.edu/~remzi/Classes/537/Spring2018/Discussion/videos.html) 

Memory: [https://people.freebsd.org/~lstewart/articles/cpumemory.pdf](https://people.freebsd.org/~lstewart/articles/cpumemory.pdf) 

- Implement unique/shared ptr / (maybe weak_ptr?) 
- virtual functions/destructors + vtables/overhead
- delete[], new[], operators, constructors, destructors
- delegating constructors
- move constructors / move assignment / copy / copy assignment
- rule of 5
- lvalue/rvalue/xvalues
	- ```TR   R
	  T&   &  -> T&  // lvalue reference to cv TR -> lvalue reference to T
	  T&   && -> T&  // rvalue reference to cv TR -> TR (lvalue reference to T)
	  T&&  &  -> T&  // lvalue reference to cv TR -> lvalue reference to T
	  T&&  && -> T&& // rvalue reference to cv TR -> TR (rvalue reference to T)```
- template review!!
	- The example problem had a std::tuple of indeterminate size as the only template argument. This problem had an indeterminate number of template arguments of another provided template type (no std library functions to help me manipulate it), each of those having an indeterminate number of non-type template arguments, adding many levels of complication
- implement statically typed heterogeneous tuple
- CRTP / SFINAE
- std
- map, unordered_map, multimap, set, unordered_set, multiset
- std array
- static initialization of vecs, sets, array
- string 

- When to use noexcept/explicit?
- Types of caching, and how they work
- Data hazards and control hazards… modern processors handle many instructions at a time - how do you deal with that and performance issues that arise?
- Memory allocators
- Memory access patterns
- when/how to parallelize loops
- universal references (and when they differ from rvalue references)

- Review perf stuff Levels of caches/latencies (L1, L2, L3) x86 calling conventions [https://codearcana.com/posts/2013/05/21/a-brief-introduction-to-x86-calling-conventions.html](https://codearcana.com/posts/2013/05/21/a-brief-introduction-to-x86-calling-conventions.html) semaphores/condvars + implementation Relevant Extra Readings: Effective Modern C++ Agner Fog's Optimizing software in C++


**Interview Prep:**
-   Modern C++ intricacies (move semantics in depth, resource management, concurrency primitives, static computation, template meta-programming)
-   Data structures, algorithms, complexity, threading, concurrency etc..
-   System design
-   Basic Networking
-   Linux
-   Brainteasers/logic puzzles

**Good websites to look at:**

-   Interview Guide - [https://hired.com/blog/candidates/technical-onsite-interview-guide/](https://hired.com/blog/candidates/technical-onsite-interview-guide/)
-   Coding Interview - [https://github.com/xianzhez/Coding-Interview-101](https://github.com/xianzhez/Coding-Interview-101)
-   **Interview Practice,** highly recommended from many programmers to prepare technical interviews **-** [**https://leetcode.com/**](https://leetcode.com/) **(I cant emphasise enough how important it is to practice the Medium & Hard tasks on Leetcode. Also a premium account will unlock some specific tasks known to be asked by firm).**
-   Interview Prep Material - [https://app.codesignal.com/interview-practice](https://app.codesignal.com/interview-practice)
-   “good website to understand why those complexities are the case, rather than just learning complexities of structures” - [http://bigocheatsheet.com/](http://bigocheatsheet.com/)
-   [https://www.geeksforgeeks.org/](https://www.geeksforgeeks.org/)  
-   Brainteaser type questions - [www.techinterview.org](http://www.techinterview.org/)
-   C++ specific - [https://google.github.io/styleguide/cppguide.html](https://google.github.io/styleguide/cppguide.html)
-   Template specific - [https://www.slamecka.cz/posts/2021-03-17-cpp-metaprogramming-exercises-1/](https://www.slamecka.cz/posts/2021-03-17-cpp-metaprogramming-exercises-1/)
- https://www.agner.org/optimize/ 
- https://en.algorithmica.org/hpc

**Example of Questions asked:**

**C++**

-   C++ (predominantly modern language features):
-   std::move
-   RAII and its impact
-   std::unique_ptr
-   memory layout of a C++ struct, empty, with some members and with virtual functions. Alignment consequences and so forth.
-   floating point numbers - layout, use cases, weaknesses.
-   initialiser lists and constructor initialisation list
-   details wrt vector, deque, unordered_map (implementation, O(x) guarantees, use cases, ...)
-   hashmaps, collision handling
-   Networking (TCP vs UDP, Throttling & delivery guarantees in TCP)

-   C++ what is move semantics?
-   C++ what are its benefits? which things become possible with introduction of move semantics, which weren't possible in previous versions of C++?
-   C++ what is unique_ptr?
-   C++ what is size of "struct {}" (empty structure) in memory?
	- 1 byte, this is to make sure that two structs don't have the same addresss
-   C++ what is size of "class { int a; long b; }" in memory in case of 64-bit architecture?
-   C++ what is size of the class if we add a virtual method to it?
-   C++ how virtual methods work i.e. what is the underlying mechanism behind them?
-   C++ what is RAII (hint: resource acquisition is initialization)? how is it used in C++?
-   Network what are differences between TCP/UDP? what are benefits of TCP and of UDP in comparison to each other?
-   Network what is multicast? in general, how does it work?

-   Cache. Layers, size, performance guarantees.
-   Memory access latency, cache, TLB, multithreading impacts.
-   Multithreading vs Multiprocessing
-   ++ (predominantly C++11 focused, so quite modern language features):
-   std::unique_ptr, shared_pointer, reference counting,multithreading impacts of shared_pointers, and comparisons to Java garbage collection
-   memory layout of virtual functions, performance impacts of calling overriden virtual functions
-   floating point numbers - layout, use cases, weaknesses.
-   hash maps, linear proping, rehashing and performance guarantees.

-   unordered_map (detailed implementation)
-   push_back() vs emplace_back()
-   difference between struct and class

**Question Examples:**

Please find below some example of questions that have been asked to various clients (please keep in mind that it is not specifically targeted to a specific language but hopefully will be useful):

-   How does Python (or an equivalent language) deal with concurrency (what primitives are there etc)
-   A scatter graph of points on a page, draw a horizontal line across the page such the perpendicular y distance to the line from all points in aggregate is minimised. Describe an algorithm for placing this line optimally?
-   How to reverse a string?
-   A question on using arrays to build trees and how to optimize the performance
-   Implement a string to int function
-   Implement a string to double function. This went on to discuss the accuracy of doubles, how they're implemented, and how this affects the implementation of my function.
-   Given a tree, print its contents in order.
-   When would this function error? This then went on to discuss stack overflows, the implementation of a stack in memory, what the fact that the function overflowed can tell you about the tree.
-   I was shown some code with some problems and asked to explain what the function did and how to improve it
-   What is the complexity of the typical fibbonaci function
-   How would you improve this?
-   Given a node in a tree, the head of a tree and an integer, return a list of nodes that are that distance away from the node (including above).
-   What is the complexity of the answer? How can this be improved?
-   Question on heap vs stack memory and analysis of code causing a stack overflow
-   Given an arithmetic expression in a string format (e.g. "(2*3)+((1+2)*7)") create a function that evaluates the expression, taking operator precedence into account, and returns the result as an integer
-   Find pythagorean triplet in list of nums
-   How to find a median in a streaming list (5m)
-   How many bits is a long

