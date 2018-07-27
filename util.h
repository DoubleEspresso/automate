#ifndef UTIL_H
#define UTIL_H


// std::make_unique is part of c++14
template<typename T, typename... Args>
  std::unique_ptr<T> make_unique(Args&&... args) {
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}


#endif
