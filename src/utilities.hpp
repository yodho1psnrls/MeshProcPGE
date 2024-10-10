#pragma once
#include "../olc/olcPixelGameEngine.h"
#include <cmath>
#include <iterator>
#include <stdexcept>
#include <type_traits> // std::is_base_of_v

//const float scrW = 640.0f;
//const float scrH = 420.0f;
const float scrW = 380.0f;
const float scrH = 340.0f;

typedef unsigned int uint;

// Isnt this std::fmodf(x, 1.0f) ??
inline float fract(const float x) {
	return x - (x >= 0.0f ? floor(x) : ceil(x));
	//return x - floor(x);
}

// from x(-1, 1) and y(-1, 1) to x(0, scrW) and y(0, scrH), and reverts Y axis 
inline olc::vf2d to_screen(olc::vf2d v) {
	v.y = -v.y;
	v = (v + olc::vf2d(1.0f, 1.0f)) * 0.5f; // to (0, 1)
	return v * olc::vf2d(scrW, scrH);
}

// template specialization for std::hash to work with std::pair
//namespace std {
	template<typename T, typename Y> struct std::hash<std::pair<T, Y>> {
		size_t operator()(const std::pair<T, Y>& p) const {
			size_t h1 = std::hash<T>()(p.first);
			size_t h2 = std::hash<Y>()(p.second);
			return h1 ^ (h2 << 1);
		}
	};
//}


// https://stackoverflow.com/questions/5423246/how-to-detect-if-a-type-is-an-iterator-or-const-iterator
template<class IT, class T=decltype(*std::declval<IT>())>    
constexpr bool is_const_iterator() {   
  return !std::is_assignable<decltype(*std::declval<IT>()), T>::value;
}


//struct base_handle {};


// TODO: Use SFINAE to work only with iterators
template <typename IterType>	// Iterator Type
class Iterable {
protected:

	IterType begin_iter;
	IterType end_iter;

public:

	Iterable(const IterType& begin_iterator, const IterType& end_iterator)
		: begin_iter(begin_iterator), end_iter(end_iterator) {}


  const IterType& begin() const { return begin_iter; }
	const IterType& end() const { return end_iter; }


  IterType::difference_type size() const { return std::distance(begin_iter, end_iter); }
  bool empty() const { return begin_iter == end_iter; }

  // If you use std::enable_if_t that depends on the template type T,
  //  on a method from some Class<T> , then if the condition fails,
  //  you cannot use the whole class, but if you do another type, 
  //  that defaults to T and check the condition on it, 
  //  only the method is not declared

  //template <typename = typename std::enable_if_t<
  template <typename IT = IterType, typename = typename std::enable_if_t<
    std::is_base_of_v<std::random_access_iterator_tag, typename IT::iterator_category> && 
    !is_const_iterator<IT>() &&
    !std::is_same_v<IT, std::remove_reference_t<std::remove_const_t<typename IT::value_type>>>   
  >> 
  IT::value_type& operator[](const IT::difference_type index) {
    if (index < 0 || begin_iter + index >= end_iter)
      throw std::out_of_range("Out of range Iterable");
   
    //return begin_iter[index];
    return *(begin_iter + index);
  }


  //template <typename = typename std::enable_if_t<
  template <typename IT = IterType, typename = typename std::enable_if_t<
    std::is_base_of_v<std::random_access_iterator_tag, typename IT::iterator_category> &&
    is_const_iterator<IT>() &&
    !std::is_same_v<IT, std::remove_reference_t<std::remove_const_t<typename IT::value_type>>>   
  >> 
  const IT::value_type& operator[](const IT::difference_type index) const {
    if (index < 0 || begin_iter + index >= end_iter)
      throw std::out_of_range("Out of range Iterable");
   
    //return begin_iter[index];
    return *(begin_iter + index);
  }



  //template <typename = typename std::enable_if_t<
  template <typename IT = IterType, typename = typename std::enable_if_t<
    std::is_base_of_v<std::random_access_iterator_tag, typename IT::iterator_category> && 
  //  std::is_base_of_v<base_handle, IT>
    std::is_same_v<IT, std::remove_reference_t<std::remove_const_t<typename IT::value_type>>>
  >>
  IT::value_type operator[](const IT::difference_type index) const {
  //const IT::value_type operator[](IT::difference_type index) const {
    if (index < 0 || begin_iter + index >= end_iter)
      throw std::out_of_range("Out of range Iterable");
  
    //return begin_iter[index];
    return IT(begin_iter + index);
  }


};


