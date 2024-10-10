#pragma once
//#include <vector>   // for std::hash
//#include <iterator> // for iterator tags
#include "../utilities.hpp" // for uint

// Some common handler types that u can reuse, so you dont need to rewrite them
//  if you need them in different data structures


namespace pc {


//class index_vert_handle : public base_handle {
class index_vert_handle {
  uint _id;

  friend class std::hash<index_vert_handle>;
public:

    using iterator_category = std::contiguous_iterator_tag;
    using difference_type   = int;
    using value_type        = index_vert_handle;
    using pointer           = index_vert_handle*;  // or also value_type*
    using reference         = index_vert_handle&;  // or also value_type&

  //using handler_tag;

  static const uint INVALID_INDEX = -1;
//  static const index_vert_handle INVALID = index_vert_handle(uint(-1));

  index_vert_handle() : _id(INVALID_INDEX) {}
  explicit index_vert_handle(const uint id) : _id(id) {}
  operator uint() const {return _id;}
  operator uint&() {return _id;}

	// To give the id when iterated
	index_vert_handle& operator*() { return *this; }
	const index_vert_handle operator*() const { return *this; } 


  //index_vert_handle operator[](const difference_type index) const { return _id + index; }
  //index_vert_handle& operator[](const uint index) { return _id + index; }


};


}


template <>
struct std::hash<pc::index_vert_handle> {
	size_t operator()(const pc::index_vert_handle& vh) const {	
    return std::hash<int>()(vh._id);
	}
};


