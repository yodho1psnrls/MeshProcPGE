#pragma once
//#include "../utilities.hpp"		// Iterable
#include "handlers.h"
#include <vector>
//#include <iterator> // For iterator tags


// ALL HANDLERS WILL BE INDEXES, THEY ARE JUST SEPARATED INTO DIFFERENT CATEGORIES
//  FOR TYPE SAFETY



// DIFFERENT POINT CLOUD REPRESENTATIONS SPECIALIZE THE VERT_CLOUD TAG/VERT_TRAITS

// Policy // Traits class for a vertex container, should be specialized by the user to
// be used with custom point cloud data structures

// A Generic way to store Point Cloud Data (Only the vertices)


// Either make all different Vertex Containers to specialize PointVec/PointCloud traits class
//	or just make all VertexCloud data structures to have the same policy/interface
//  and make the meshes directly inherit that, instead of templating Container
//  which specializes PointVec


// Vertex Handler
/*class vert_handle {
	int id;

	friend class std::hash<vert_handle>;

public:

	static const int INVALID_INDEX = -1;

	vert_handle(const int id = INVALID_INDEX) : id(id) {}


	operator int& () { return id; }
	operator int() const { return id; }


	// We make this so the Iterable works and returns a vertex handler
	vert_handle& operator*() { return *this; }
	vert_handle operator*() const { return *this; }

};*/


// (KEET IT AS A SPETIALIZATION) THIS WILL GIVE ME MORE FREEDOM FOR THE INDIVIDIAL
//  POINT CLOUD DATA STRUCTURES AND WILL MAKE THE VERTCLOUD SPECIALIZATION TO HANDLE
//  THE GENERIC INTERFACE FOR THE MESH CLASSES
// 
// SHOULD I MAKE THIS A CLASS, INSTEAD OF A SPECIALIZATION ?
// 
// (MAKE ALL DIFFERENT PC WITH SAME INTERFACE) THIS WILL GIVE ME A LOT OF
//  CONSTRAINTS FOR DIFFERENT TYPES OF POINT CLOUDS


// VertexHandle ???

namespace pc {


// A Default Vertex Cloud which is just an array of vertices
// Supports index based notation , but if you search closes point,
// it uses linear search, so it is not so optimal for that
//template <typename VertType>
template <typename V>
class PointVec {
public:	

  // The other classes that inherit PointCloud as a policy dont nececarily need
  //  to specify the vertex type V again
  //using V = VertType;
	using vert_handle = index_vert_handle;

	// Type aliases for a valid PointVec
//	using vert_handle = vert_handle;
	using vert_type = V;
  using vert_container = PointVec<V>;


	PointVec() : _vertices() {};
	PointVec(const std::vector<V>& vertices) : _vertices(vertices) {}
  operator std::vector<V>&() { return _vertices; }
  operator const std::vector<V>&() const { return _vertices; }


  template<typename IterType>
  bool are_valid_indices(IterType begin_it, IterType end_it) const {
    for(IterType it = begin_it; it != end_it; ++it)
      if (!is_in_range(*it))
        return false;

    return true;
  }
 

  bool is_in_range(const vert_handle vh) const {
    //return *vh >= 0 && *vh < verts_size();
    return uint(vh) >= 0 && uint(vh) < verts_size();
  }



	// Operations
	vert_handle add_vert(const V& vertex) {
		vert_handle vi(_vertices.size());
		
		_vertices.push_back(vertex);

		return vi;
	}


	// Getters
	V& vert(const vert_handle vert_id) {
    if (!is_in_range(vert_id))
      throw std::out_of_range("Out of range vertex in PointVec");   
   
    return _vertices[vert_id];
	}

	const V& vert(const vert_handle vert_id) const {
    if (!is_in_range(vert_id))
      throw std::out_of_range("Out of range vertex in PointVec");

    return _vertices[vert_id];
	}


	// Iterables
	Iterable<vert_handle> verts() const {
		return Iterable<vert_handle>(
      (vert_handle)0U, (vert_handle)_vertices.size());
	}


  uint verts_size() const {
    return _vertices.size();
  }

  void reserve_verts(const uint new_capacity) {
    _vertices.reserve(new_capacity);
  }


protected:

	std::vector<V> _vertices;


};


/*template <typename V>
class PointVec<V, std::vector<V>> : protected std::vector<V> {
protected:

	std::vector<V> _vertices;


public:

	void add_vert(const V& v) {
		_vertices.push_back(v);
	}

//	template <typename VertHandle, typename = std::enable_if<std::is_base_of_v<mesh::Handle, VertHandle>>
	void remove_vert(const uint vi) {

	}

};
*/



}

