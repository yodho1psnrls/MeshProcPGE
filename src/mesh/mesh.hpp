#pragma once

#include "base_mesh.hpp"
#include "../point_cloud/point_vec.hpp"
#include "../utilities.hpp"	// Iterable

#include <array>
#include <vector>
#include <initializer_list>
#include <iterator>

#include <cassert>

using uint = unsigned int;
//using uint8 = unsigned char;

namespace mesh {

// @todo: Polygonal mesh
// (assumes that all polygons have the same number N of _vertices/_indices)

// Triangular Mesh // OpenGL Style
template <
	typename V,					// Vertex Type
	uint N,							// number of _vertices/edges per face
	typename PointCloud = pc::PointVec<V>  // Vertex Container
>
class Mesh : public BaseMesh<V, PointCloud> {
public:

  
  using typename BaseMesh<V, PointCloud>::vert_handle;
	using BaseMesh<V, PointCloud>::add_vert;
  //using PointCloud::PointCloud; // inherit the constructors
  using BaseMesh<V, PointCloud>::verts_size;
  using BaseMesh<V, PointCloud>::verts;
  using BaseMesh<V, PointCloud>::vert;

  class face_handle;


  // Constructor
	Mesh(
    const PointCloud& vertices = PointCloud(),
		const std::vector<vert_handle>& indices
      = std::vector<vert_handle>()
  );


	bool is_valid() const;


  // @todo: Use enable_if so it is sure that it is an iterator !
	template <typename IterType>
	uint add_face(const IterType& begin_it, const IterType& end_it);


	face_handle add_face(const std::initializer_list<vert_handle>& new_indices);

	face_handle add_face(const std::array<vert_handle, N>& new_indices);

	face_handle add_face(const std::vector<vert_handle>& new_indices);


	uint faces_size() const;

	Iterable<face_handle> faces() const;

  //Iterable<typename std::vector<vert_handle>::iterator> verts(const face_handle face_id);
  std::array<vert_handle, N> verts(const face_handle face_id) const;

  // ----------------------- FACE HANDLE ---------------------------- //


	class face_handle {
  protected:
    friend class Mesh<V, N, PointCloud>;
    // this is the id of the beginning of the face vertex _indices !! 
		// so the actual face id is equal to id / N
		int begin_id;		// !!!  _vertices[_indices[id]]  !!!

	public:

    static const int INVALID_INDEX = -1;

		face_handle(const int face_id = INVALID_INDEX) : begin_id(face_id* N) {}
		operator int() const { return begin_id / N; }

		// To work with iterators
	//	face_handle& operator*() { return *this; }
		face_handle operator*() const { return *this; }

		face_handle& operator++() { begin_id += N; return *this; }
		face_handle& operator--() { begin_id -= N; return *this; }

		face_handle& operator++(int) { face_handle temp(*this); begin_id += N; return temp; }
		face_handle& operator--(int) { begin_id -= N; return *this; }
	};


protected:

  using BaseMesh<V, PointCloud>::are_valid_indices;
  
  using BaseMesh<V, PointCloud>::_indices;
  using BaseMesh<V, PointCloud>::_vertices;
};


  // ======================= IMPLEMENTATION =========================//


template<typename V, uint N, typename PointCloud>
inline Mesh<V, N, PointCloud>::Mesh(
  const PointCloud& vertices,
  const std::vector<typename Mesh<V, N, PointCloud>::vert_handle>& indices
//) : PointCloud(vertices) , _indices(new_indices) {
) : BaseMesh<V, PointCloud>::BaseMesh(vertices, indices) {

	assert(is_valid() && "Non valid indices count or invalid vert_handle in the indices");

}


template<typename V, uint N, typename PointCloud>
inline bool Mesh<V, N, PointCloud>::is_valid() const {
  return uint(this->_indices.size()) % N == 0U
      && are_valid_indices(_indices.begin(), _indices.end());
}


// @todo: Use enable_if so it is sure that it is an iterator !
template<typename V, uint N, typename PointCloud>
template<typename IterType>
inline uint Mesh<V, N, PointCloud>::add_face(
  const IterType& begin_it,
  const IterType& end_it
) {
	assert(std::distance(begin_it, end_it) == N &&
		"The size of the new vector _indices should be equal to the constant face size N");

  assert(are_valid_indices(begin_it, end_it) && "Non-valid vertex handle in the new indices");

	uint id = faces_size();

	this->_indices.insert(this->_indices.end(), begin_it, end_it);

	return id;
}


template<typename V, uint N, typename PointCloud>
inline typename Mesh<V, N, PointCloud>::face_handle Mesh<V, N, PointCloud>::add_face(
  const std::initializer_list<typename Mesh<V, N, PointCloud>::vert_handle>& new_indices
) {
	return add_face(new_indices.begin(), new_indices.end());
}


template<typename V, uint N, typename PointCloud>
inline typename Mesh<V, N, PointCloud>::face_handle Mesh<V, N, PointCloud>::add_face(
  const std::array<typename Mesh<V, N, PointCloud>::vert_handle, N>& new_indices
) {
	return add_face(new_indices.begin(), new_indices.end());
}


template<typename V, uint N, typename PointCloud>
inline typename Mesh<V, N, PointCloud>::face_handle Mesh<V, N, PointCloud>::add_face(
  const std::vector<typename Mesh<V, N, PointCloud>::vert_handle>& new_indices
) {
	return add_face(new_indices.begin(), new_indices.end());
}


template<typename V, uint N, typename PointCloud>
inline uint Mesh<V, N, PointCloud>::faces_size() const {
  return uint(this->_indices.size()) / N;
}


template<typename V, uint N, typename PointCloud>
inline Iterable<typename Mesh<V, N, PointCloud>::face_handle> Mesh<V, N, PointCloud>::faces() const {
  return Iterable<typename Mesh<V, N, PointCloud>::face_handle>(0, faces_size());
}


template<typename V, uint N, typename PointCloud>
std::array<typename Mesh<V, N, PointCloud>::vert_handle, N> Mesh<V, N, PointCloud>::verts
  (const typename Mesh<V, N, PointCloud>::face_handle face_id) const {
  
  //auto begin_it = this->_indices.begin() + face_id;

  // This would be problematic, if you add new vertices (iterator invalidation)
  //return Iterable(begin_it, begin_it + N);

  //return std::array<typename Mesh<V, N, PointCloud>::vert_handle, N>(
  //  begin_it , begin_it + N
  //);

  //vert_handle* handle_ptr = _indices.data() + face_id;
  std::array<typename Mesh<V, N, PointCloud>::vert_handle, N> vert_handles;
  for(int i = 0; i < N; ++i) {
  //  vh = *(handle_ptr++);
    vert_handles[i] = _indices[face_id.begin_id + i];
  }

  return vert_handles;
}



}
