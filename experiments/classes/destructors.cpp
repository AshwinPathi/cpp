#include <iostream>
#include <memory>

// Normal classes with normal inheritence, no virtual destructors.
struct Base {
  Base() {
    std::cout << "Base ctor\n";
  }
  ~Base() {
    std::cout << "Base dtor\n";
  }
};

struct Derived : Base {
  Derived() {
    std::cout << "Derived ctor\n";
  }
  ~Derived() {
    std::cout << "Derived dtor\n";
  }
};

// Virtual classes with virtual inheritence.
struct VirtualBase {
  VirtualBase() {
    std::cout << "VirtualBase ctor\n";
  }
  virtual ~VirtualBase() {
    std::cout << "VirtualBase dtor\n";
  }
};

struct VirtualDerived : VirtualBase {
  VirtualDerived() {
    std::cout << "VirtualDerived ctor\n";
  }
  virtual ~VirtualDerived() {
    std::cout << "VirtualDerived dtor\n";
  }
};

// Tests for non-virtual classes.
void NonVirtualTests() {
  std::cout << "Constructing Base\n";
  {
    Base b;
  }
  std::cout << '\n';

  std::cout << "Constructing Derived\n";
  {
    Derived d;
  }
  std::cout << '\n';

  std::cout << "Constructing Derived and setting Base to it\n";
  {
    std::cout << "Constructing Derived.\n";
    Derived d;
    std::cout << "Setting Base to Derived.\n";
    Base b = d;
  }
  std::cout << '\n';
}

void VirtualTests() {
  // For virtual tests, wrap everything in unique ptrs to have
  // auto destruction.
  std::cout << "Constructing virtual base.\n";
  {
    auto base = std::make_unique<VirtualBase>();
  }
  std::cout << '\n';

  std::cout << "Constructing virtual derived.\n";
  {
    auto derived = std::make_unique<VirtualDerived>();
  }
  std::cout << '\n';

  std::cout << "Constructing Virtual Derived and setting Virtual Base to it\n";
  {
    std::unique_ptr<VirtualBase> base_as_derived{std::make_unique<VirtualDerived>()};
  }
  std::cout << '\n';
}

int main() {
  NonVirtualTests();
  std::cout << "-------------------------------\n";
  VirtualTests();
  return 0;
}
