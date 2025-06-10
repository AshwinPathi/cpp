# CPP Trivia

### Value Categories
- RVO cannot happen on subclasses

- rvalue ref can be implicitly casted to const lvalue ref
```
const std::vector<int> a = {1,2,3,4,5}; 
auto a2 = std::move(a);
```
- thats why in the above example, we just do a copy. The vector class may not have a `const vector&&` constructor, so when we call move on the vector, it gets casted, and then we can't find the right constructor for it. We then match to the copy constructor, which can bind to const rvalue ref, which takes in `const vector&`.


- `A&& b` - what is b here?
	- b is an lvalue (its named)


### Lambdas
Consider the following two snippets:
```
// (1)
std::function<...(...)> func = [](...){ ... };

// (2)
auto func = [](...) { ... };
```
Which is (technically) slower?
- The answer is: (1). `std::function` can store any arbitrary callable (like a lambda), but it might perform some actions such as type erasure or even heap allocations to store that lambda. With option (2), we just deduce the type of the returned lambda, which will just be some pure `closure` type, which is basically just a class/struct with `operator()` defined. This has the potential for more compiler optimization, inlining, less storage, etc.


### Misc
- most ABIs dictate that anything with non-trivial destructor (unique_ptr) cannot be placed in a register