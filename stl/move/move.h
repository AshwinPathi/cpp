
namespace my {

// std::move is basically a cast from a type to an rvalue reference.
// see this for details: https://stackoverflow.com/questions/7510182/how-does-stdmove-transfer-values-into-rvalues
// Important thing here is the remove reference part, which we need because of C++
// reference collapsing rules:
// Object &  &  = Object &
// Object &  && = Object &
// Object && &  = Object &
// Object && && = Object &&
// If our type was Object&, then we end up having our static cast resolve to
// Object& (Object& && --> Object&), which means we would end up returning a 
// normal reference.
template <typename T>
auto move(T&& value) {
  return static_cast<typename std::remove_reference_t<T>&&>(value);
}

} // namespace my
