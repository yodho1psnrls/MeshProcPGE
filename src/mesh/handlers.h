#pragma once

//#include <vector> // for std::hash
#include "../utilities.hpp" // for uint
#include <iterator>   // for iterator tags

namespace mesh {


class index_face_handle {
  uint _id;

  friend class std::hash<index_face_handle>;
public:

    using iterator_category = std::contiguous_iterator_tag;
    using difference_type   = uint;
    using value_type        = index_face_handle;
    using pointer           = index_face_handle*;  // or also value_type*
    using reference         = index_face_handle&;  // or also value_type&


  static const uint INVALID_INDEX = -1;

  index_face_handle() : _id(INVALID_INDEX) {}
  explicit index_face_handle(const uint id) : _id(id) {}
  operator uint() const {return _id;}
  operator uint&() {return _id;}

	// To give the id when iterated
	index_face_handle& operator*() { return *this; }
	const index_face_handle operator*() const { return *this; }

};


namespace he {


class index_hedge_handle {
  uint _id;

  friend class std::hash<index_hedge_handle>;
public:

    using iterator_category = std::contiguous_iterator_tag;
    using difference_type   = uint;
    using value_type        = index_hedge_handle;
    using pointer           = index_hedge_handle*;  // or also value_type*
    using reference         = index_hedge_handle&;  // or also value_type&

  static const uint INVALID_INDEX = -1;

  index_hedge_handle() : _id(INVALID_INDEX) {}
  explicit index_hedge_handle(const uint id) : _id(id) {}
  operator uint() const {return _id;}
  operator uint&() {return _id;}

	// To give the id when iterated
	index_hedge_handle& operator*() { return *this; }
	const index_hedge_handle operator*() const { return *this; }


};


class index_edge_handle {
	uint _hedge_id;

	friend class std::hash<index_edge_handle>;
public:


    using iterator_category = std::contiguous_iterator_tag;
    using difference_type   = uint;
    using value_type        = index_edge_handle;
    using pointer           = index_edge_handle*;  // or also value_type*
    using reference         = index_edge_handle&;  // or also value_type&


  // NOTE: Be aware of convertions from an Invalid Half-Edge Handler to Edge Handler !!!

  static const uint INVALID_INDEX = -1; 

	index_edge_handle() : _hedge_id(INVALID_INDEX) {}
	explicit index_edge_handle(const uint id) : _hedge_id(id << 1U) {}
	operator uint() const { return _hedge_id >> 1U; }

  // Makes sure that the internal half-edge index that is stored inside edge_handle is always an even number,
  //  because if its not, then some very nasty bugs may appear, like an infinite iterator
  index_edge_handle(const index_hedge_handle hh) : _hedge_id(uint(hh) & ~1U) {}  
  operator index_hedge_handle() const { return index_hedge_handle(_hedge_id); }


	// To give the id when iterated
	index_edge_handle& operator*() { return *this; }
	const index_edge_handle operator*() const { return *this; }


	index_edge_handle& operator++() { _hedge_id += 2U; return *this; }
	index_edge_handle operator++(int) { index_edge_handle temp = *this; _hedge_id += 2U; return temp; }
	index_edge_handle& operator--() { _hedge_id -= 2U; return *this; }
	index_edge_handle operator--(int) { index_edge_handle temp = *this; _hedge_id -= 2U; return temp; }

	// Operators with integers

	index_edge_handle& operator+=(const uint x) { _hedge_id += (x << 1U); return *this; }
	index_edge_handle& operator-=(const uint x) { _hedge_id -= (x << 1U); return *this; }
	index_edge_handle& operator*=(const uint x) { _hedge_id *= (x << 1U); return *this; }
	index_edge_handle& operator/=(const uint x) { _hedge_id /= (x << 1U); _hedge_id &= ~1U; return *this; }

	friend index_edge_handle operator+(index_edge_handle lhs, const uint rhs) { return lhs += rhs; }
	friend index_edge_handle operator-(index_edge_handle lhs, const uint rhs) { return lhs -= rhs; }
	friend index_edge_handle operator*(index_edge_handle lhs, const uint rhs) { return lhs *= rhs; }
	friend index_edge_handle operator/(index_edge_handle lhs, const uint rhs) { return lhs /= rhs; }

	// Operators with other handlers

	index_edge_handle& operator+=(const index_edge_handle other) { _hedge_id += other._hedge_id; return *this; }
	index_edge_handle& operator-=(const index_edge_handle other) { _hedge_id -= other._hedge_id; return *this; }
	index_edge_handle& operator*=(const index_edge_handle other) { _hedge_id *= other._hedge_id; return *this; }
	index_edge_handle& operator/=(const index_edge_handle other) { _hedge_id /= other._hedge_id; _hedge_id &= ~1U; return *this; }

	friend index_edge_handle operator+(index_edge_handle lhs, const index_edge_handle rhs) { return lhs += rhs; }
	friend index_edge_handle operator-(index_edge_handle lhs, const index_edge_handle rhs) { return lhs -= rhs; }
	friend index_edge_handle operator*(index_edge_handle lhs, const index_edge_handle rhs) { return lhs *= rhs; }
	friend index_edge_handle operator/(index_edge_handle lhs, const index_edge_handle rhs) { return lhs /= rhs; }

  // Comparison operators
  /* Ambiguous because of operator uint() !!! 
  friend bool operator>(const index_edge_handle lhs, const index_edge_handle rhs) { return lhs._hedge_id > rhs._hedge_id; }
  friend bool operator>=(const index_edge_handle lhs, const index_edge_handle rhs) { return lhs._hedge_id >= rhs._hedge_id; }
  friend bool operator<(const index_edge_handle lhs, const index_edge_handle rhs) { return lhs._hedge_id < rhs._hedge_id; }
  friend bool operator<=(const index_edge_handle lhs, const index_edge_handle rhs) { return lhs._hedge_id <= rhs._hedge_id; }
  friend bool operator==(const index_edge_handle lhs, const index_edge_handle rhs) { return lhs._hedge_id == rhs._hedge_id; }
  friend bool operator!=(const index_edge_handle lhs, const index_edge_handle rhs) { return lhs._hedge_id != rhs._hedge_id; }
  */

};


}


}


template <>
struct std::hash<mesh::index_face_handle> {
	size_t operator()(const mesh::index_face_handle& fh) const {	
    return std::hash<int>()(fh._id);
	}
};


template <>
struct std::hash<mesh::he::index_hedge_handle> {
	size_t operator()(const mesh::he::index_hedge_handle& hh) const {	
    return std::hash<uint>()(hh._id);
	}
};


template <>
struct std::hash<mesh::he::index_edge_handle> {
	size_t operator()(const mesh::he::index_edge_handle& eh) const {	
    return std::hash<uint>()(eh._hedge_id);
	}
};

