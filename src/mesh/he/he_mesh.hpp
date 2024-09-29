#pragma once
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <functional>   // std::greater


#include "../ngon_mesh.hpp"


#define HE_INVALID_INDEX -1
#define HE_ISOLATED_INDEX -2


namespace he {

// Min Heap Priority Queue   // Access to the smallest element
template<typename T>
using rpriority_queue = typename std::priority_queue<T, std::vector<T>, std::greater<T>>;


// TODO: Hash for the handlers



template <typename V> class HEMesh {
public:

  
class loop_handle {
  int _id;

  friend class std::hash<loop_handle>;
public:

  loop_handle(const int id) : _id(id) {}
  operator const int() const {return _id;}
  operator int&() {return _id;}

};


class vert_handle {
  int _id;

  friend class std::hash<vert_handle>;
public:

  vert_handle(const int id) : _id(id) {}
  operator const int() const {return _id;}
  operator int&() {return _id;}

};


class hedge_handle {
  int _id;

  friend class edge_handle;
  friend class std::hash<hedge_handle>;
public:

  hedge_handle(const int id) : _id(id) {}
  operator const int() const {return _id;}
  operator int&() {return _id;}

};


class edge_handle {
	int _hedge_id;

	friend class hedge_handle;
	friend class std::hash<edge_handle>;
public:

	edge_handle() : _hedge_id(HE_INVALID_INDEX) {}
	edge_handle(const int id) : _hedge_id(id << 1) {}
	edge_handle(const hedge_handle he);

	operator int() const { return _hedge_id >> 1; }

	// To give the id when iterated
	edge_handle& operator*() { return *this; }
	edge_handle operator*() const { return *this; }

	edge_handle& operator++() { _hedge_id += 2; return *this; }
	edge_handle operator++(int) { edge_handle temp = *this; _hedge_id += 2; return temp; }
	edge_handle& operator--() { _hedge_id -= 2; return *this; }
	edge_handle operator--(int) { edge_handle temp = *this; _hedge_id -= 2; return temp; }

	// Operators with integers

	edge_handle& operator+=(const int x) { _hedge_id += (x << 1); return *this; }
	edge_handle& operator-=(const int x) { _hedge_id -= (x << 1); return *this; }
	edge_handle& operator*=(const int x) { _hedge_id *= (x << 1); return *this; }
	edge_handle& operator/=(const int x) { _hedge_id /= (x << 1); _hedge_id &= ~1; return *this; }

	friend edge_handle operator+(edge_handle lhs, const int rhs) { return lhs += rhs; }
	friend edge_handle operator-(edge_handle lhs, const int rhs) { return lhs -= rhs; }
	friend edge_handle operator*(edge_handle lhs, const int rhs) { return lhs *= rhs; }
	friend edge_handle operator/(edge_handle lhs, const int rhs) { return lhs /= rhs; }

	// Operators with other EdgeIDs

	edge_handle& operator+=(const edge_handle other) { _hedge_id += other._hedge_id; return *this; }
	edge_handle& operator-=(const edge_handle other) { _hedge_id -= other._hedge_id; return *this; }
	edge_handle& operator*=(const edge_handle other) { _hedge_id *= other._hedge_id; return *this; }
	edge_handle& operator/=(const edge_handle other) { _hedge_id /= other._hedge_id; _hedge_id &= ~1; return *this; }

	friend edge_handle operator+(edge_handle lhs, const edge_handle rhs) { return lhs += rhs; }
	friend edge_handle operator-(edge_handle lhs, const edge_handle rhs) { return lhs -= rhs; }
	friend edge_handle operator*(edge_handle lhs, const edge_handle rhs) { return lhs *= rhs; }
	friend edge_handle operator/(edge_handle lhs, const edge_handle rhs) { return lhs /= rhs; }

};


  //using face_handle = typename loop_handle;
  //using border_handle = typename loop_handle;






protected:



private:

  std::vector<vert_handle> _vertices;
  std::vector<hedge_handle> _hedges;

  // All loops that form a face or border
  // Faces span from 0 to _faces_cap - 1 // Borders span from _faces_cap to _loops.size() 
  std::vector<loop_handle> _loops;  
  uint _faces_cap;

  // Garbage collection
  rpriority_queue<vert_handle> _removed_verts;
  rpriority_queue<edge_handle> _removed_edges;
  rpriority_queue<loop_handle> _removed_loops;

};


} // namespace he
