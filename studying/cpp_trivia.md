# CPP Trivia

- RVO cannot happen on subclasses
- most ABIs dictate that anything with non-trivial destructor (unique_ptr) cannot be placed in a register
- rvalue ref can be implicitly casted to const lvalue ref
	- ```const std::vector<int> a = {1,2,3,4,5}; 
	  auto a2 = std::move(a);```
	  - thats why in this example, we just do a copy. The vector class may not have a `const vector&&` constructor, so when we call move on the vector, it gets casted, and then we can't find the right constructor for it. We then match to the copy constructor, which can bind to const rvalue ref, which takes in `const vector&`.
  - `A&& b` - what is b here?
	  - b is an lvalue (its named)