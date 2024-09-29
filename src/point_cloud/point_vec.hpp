#pragma once
#include "../utilities.hpp"		// Iterable
#include <vector>


// ALL HANDLERS WILL BE INDEXES, THEY ARE JUST SEPARATED INTO DIFFERENT CATEGORIES
//  FOR TYPE SAFETY


using uint = unsigned int;

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

// A Defauld Vertex Cloud which is just an array of vertices
// Supports index based notation , but if you search closes point,
// it uses linear search, so it is not so optimal for that
template <typename V>
class PointVec {

protected:

	std::vector<V> _vertices;

public:	

	class vert_handle;

	// Type aliases for a valid PointVec
//	using vert_handle = vert_handle;
	using vert_type = V;


	PointVec() : _vertices() {};
	PointVec(const std::vector<V>& vertices) : _vertices(vertices) {}


	// Operations
	vert_handle add_vert(const V& vertex) {
		vert_handle vi = _vertices.size();
		
		_vertices.push_back(vertex);

		return vi;
	}


	// Getters
	V& vert(const vert_handle vert_id) {
		return _vertices[vert_id];
	}

	const V& vert(const vert_handle vert_id) const {
		return _vertices[vert_id];
	}


	// Iterables
	Iterable<vert_handle> verts() const {
		return Iterable<vert_handle>(0U, _vertices.size());
	}


  uint verts_size() const {
    return _vertices.size();
  }


	// Vertex Handler
	class vert_handle {
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

	};


};


/*
namespace std {

template <typename V>
struct hash<typename PointVec<V>::vert_handle> {
	size_t operator()(const typename PointVec<V>::vert_handle vert) const {
		return std::hash<int>()(vert.id);
	}
};

}
*/

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



