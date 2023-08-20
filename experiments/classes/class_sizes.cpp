#include <iostream>
#include <cassert>

/*************************************************
 * 
 *    Various test classes to experiment with
 * 
 * ***********************************************/

struct EmptyStruct {};

class EmptyClass {};

class IntAndLong {
  int a;
  long b;
};

// From here on out, all classes are structs since there is no difference
// between the two wrt size.

#pragma pack(push, 1) // pack == 1 has no alignment.
struct PackedIntAndLong {
  int a;
  long b;
};
#pragma pack(pop)

struct HasFunction {
  int test(int x) { return x; }
};

struct VirtualFunctionBase {
  virtual int test(int x) = 0;
};

struct HasVirtualFunctionDerived : VirtualFunctionBase {
  int test(int x) override { return x + 5; }
};

struct HasVirtualFunctionDerived2 : VirtualFunctionBase {
  int test(int x) override { return x + 5; }
  int test2(int y) { return y + 10; }
};

struct HasFunctionWithIntMember {
  int a;
  int test(int x) { return x; }
};

struct HasVirtualFunctionWithIntMember : VirtualFunctionBase {
  int a;
  int test(int x) override { return x + 7; }
};


/*************************************************
 * 
 *    Static assert tests to prove class sizes
 * 
 * ***********************************************/

//////// Empty classes
// An empty struct has size 1 since classes must have at least some footprint
// so that it has a unique memory address.
static_assert(sizeof(EmptyStruct) == 1);

// Classes are functionally the same as structs except class members are by default
// private, while struct members are by default public. Otherwise, they behave
// the exact same.
static_assert(sizeof(EmptyClass) == 1);


//////// Packing and alignment
// Note that even though int and long are size 4 and 8 respectively, the sizeof
// the IntAndLong struct is 16 since it has to introduce padding for the longest
// type in the struct (the long).
static_assert(sizeof(int) == 4);
static_assert(sizeof(long) == 8);
static_assert(sizeof(IntAndLong) == 16);

// If we pack the struct with push(pack, 1), it aligns on 1 byte, meaning its
// fully packed, so we get the size we "expect" for a struct with just an int
// and a long in it.
static_assert(sizeof(PackedIntAndLong) == 12);

//////// Functions
// A function does not contribute any "size" to a class, as we can see with this
// static assert for a class with only 1 method. Its effectively an empty class,
// and has the same rules as an empty class, i.e its size is 0. Therefore, we can
// conclude that a function contributes 0 bytes to the function size (directly).
static_assert(sizeof(HasFunction) == 1);
// Virtual functions add a vptr to the class, which has a sizeof(pointer). This makes
// the class size at least 8 bytes by default to accomodate for the vptr.
static_assert(sizeof(VirtualFunctionBase) == 8);
// Derived classes from the virtual class also have a vptr, as expected.
static_assert(sizeof(HasVirtualFunctionDerived) == 8);
// However, note that the virtual function itself is also 0 bytes, just like a regular
// function. The space that the virtual function contributes is not from the function
// itself, but the vptr that the function introduces by virtue of being virtual.
static_assert(sizeof(HasVirtualFunctionDerived2) == 8);
// We can see the above example clearer with this, where a class with a function
// and an int member is only sizeof(int), since the function contributes nothing.
static_assert(sizeof(HasFunctionWithIntMember) == 4);
// A virtual function with an int member increases the class size to 16(!) since we
// have to add a vptr for the virtual function addition, and we have to still obey
// padding rules (since the class isn't packed). This is one reason why we might
// want to watch out for using virtual functions, since this can affect struct
// alignment.
static_assert(sizeof(HasVirtualFunctionWithIntMember) == 16);

int main() {
  return 0;
}