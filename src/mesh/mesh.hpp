#pragma once

//#include "ngon_mesh.hpp"
#include "base_mesh.hpp"
#include "../point_cloud/point_vec.hpp"
#include "../utilities.hpp"	// Iterable and uint
#include "handlers.h" // we will reuse the common index_vert_handle and index_face_handle

#include <array>
#include <vector>
#include <initializer_list>

#include <iterator>
#include <stdexcept>
#include <type_traits>

//using uint = unsigned uint;
//using uint8 = unsigned char;

namespace mesh {

//template <typename V, typename PointCloud>
//class NgonMesh;

// @todo: Polygonal mesh
// (assumes that all polygons have the same number N of _vertices/_indices)

// Triangular Mesh // OpenGL Style
template <
	typename V,					// Vertex Type
	typename PointCloud = pc::PointVec<V>  // Vertex Container
>
class Mesh : public BaseMesh<V, PointCloud> {
public:
 
  using typename BaseMesh<V, PointCloud>::vert_handle;
	using BaseMesh<V, PointCloud>::add_vert;
 
  // Inherit the constructors
  // using PointCloud::PointCloud; 
//  using BaseMesh<V, PointCloud>::BaseMesh;
  
  using BaseMesh<V, PointCloud>::verts_size;
  using BaseMesh<V, PointCloud>::verts;
  using BaseMesh<V, PointCloud>::vert;

  using BaseMesh<V, PointCloud>::is_in_range;
  //using BaseMesh<V, PointCloud>::is_valid;
  using BaseMesh<V, PointCloud>::are_valid_indices;
  using face_handle = index_face_handle;

  // Non-Selected Size per Face
  static const uint INVALID_FACE_SIZE = -1;

	Mesh(
    const PointCloud& vertices = PointCloud(),
    const std::vector<vert_handle>& indices
      = std::vector<vert_handle>()
  );


  template <typename MeshType, typename = typename std::enable_if_t<
    std::is_base_of_v<BaseMesh<V, PointCloud>, MeshType> &&
    !std::is_same_v<MeshType, Mesh<V, PointCloud>>
  >>
  Mesh(const MeshType& mesh);


//  Mesh(const Mesh& other) 
//    : n(other.n), BaseMesh<V, PointCloud>::BaseMesh(other) {}


//  Mesh(const Mesh& mesh);


  void clear();
  using BaseMesh<V, PointCloud>::empty;
	bool is_valid() const;


  // @todo: Use enable_if so it is sure that it is an iterator !
	template <typename IterType>
  face_handle add_face(const IterType& begin_it, const IterType& end_it);

  template <uint N>
	face_handle add_face(const std::array<vert_handle, N>& new_indices);
	face_handle add_face(const std::vector<vert_handle>& new_indices);
	face_handle add_face(const std::initializer_list<vert_handle>& new_indices);


	uint faces_size() const; 
  uint face_size(const face_handle fh) const { return n; }
  //constexpr uint face_degree(const face_handle fh) const;
  //constexpr bool is_face_size(const face_handle fh, const uint n) const;
//  constexpr bool is_triangulated() const;

	Iterable<face_handle> faces() const;

  //std::array<vert_handle, N> verts(const face_handle face_id) const;    
  Iterable<typename std::vector<typename PointCloud::vert_handle>::const_iterator>
    verts(const face_handle fh) const;

//  Iterable<typename std::vector<typename PointCloud::vert_handle>::iterator>
//    verts(const face_handle fh);


  template <typename PC = PointCloud, typename = typename std::enable_if<std::is_base_of_v<pc::PointVec<V>, PC>>>
  Mesh<V, PC>& operator<<(const Mesh<V, PointCloud>& m);


protected:

  uint n;

//  using BaseMesh<V, PointCloud>::_indices;
//  using BaseMesh<V, PointCloud>::_vertices;
};



// =========================== IMPLEMENTATION ==============================//



template<typename V, typename PointCloud>
inline Mesh<V, PointCloud>::Mesh(
  const PointCloud& vertices,
  const std::vector<vert_handle>& indices)

  //: PointCloud(vertices) , _indices(new_indices) {
  : n(INVALID_FACE_SIZE), BaseMesh<V, PointCloud>::BaseMesh(vertices, indices) {

	if(!is_valid())
    throw std::invalid_argument("Non valid indices count or invalid vert_handle in the indices");
}


template<typename V, typename PointCloud>
inline void Mesh<V, PointCloud>::clear() {
  BaseMesh<V, PointCloud>::clear();
  n = uint(INVALID_FACE_SIZE);
}


template<typename V, typename PointCloud>
inline bool Mesh<V, PointCloud>::is_valid() const {
  return uint(this->_indices.size()) % n == 0U && BaseMesh<V, PointCloud>::is_valid();
}


// @todo: Use enable_if so it is sure that it is an iterator !
template<typename V, typename PointCloud>
template<typename IterType>
inline Mesh<V, PointCloud>::face_handle Mesh<V, PointCloud>::add_face(
  const IterType& begin_it,
  const IterType& end_it
) {

  uint new_face_size = std::distance(begin_it, end_it);
  
  if (n == INVALID_FACE_SIZE)
    n = new_face_size;
  else if (new_face_size != n)
    throw std::invalid_argument("The size of the new vector _indices should be equal to the constant face size N");

  if (!are_valid_indices(begin_it, end_it))
    throw std::invalid_argument("Non-valid vertex handle in the new indices");

	face_handle id(faces_size());

	this->_indices.insert(this->_indices.end(), begin_it, end_it);

	return id;
}


template<typename V, typename PointCloud>
inline typename Mesh<V, PointCloud>::face_handle Mesh<V, PointCloud>::add_face(
  const std::initializer_list<typename Mesh<V, PointCloud>::vert_handle>& new_indices) {

	return add_face(new_indices.begin(), new_indices.end());
}


template<typename V, typename PointCloud>
template <uint N>
inline typename Mesh<V, PointCloud>::face_handle Mesh<V, PointCloud>::add_face(
  const std::array<typename Mesh<V, PointCloud>::vert_handle, N>& new_indices) {

	return add_face(new_indices.begin(), new_indices.end());
}


template<typename V, typename PointCloud>
inline typename Mesh<V, PointCloud>::face_handle Mesh<V, PointCloud>::add_face(
  const std::vector<typename Mesh<V, PointCloud>::vert_handle>& new_indices) {

	return add_face(new_indices.begin(), new_indices.end());
}


template<typename V, typename PointCloud>
inline uint Mesh<V, PointCloud>::faces_size() const {
  return uint(this->_indices.size()) / n;
}


template<typename V, typename PointCloud>
inline Iterable<typename Mesh<V, PointCloud>::face_handle> Mesh<V, PointCloud>::faces() const {
  return Iterable<typename Mesh<V, PointCloud>::face_handle>(
    (face_handle)0U, (face_handle)faces_size());
}


template<typename V, typename PointCloud>
//std::array<typename Mesh<V, N, PointCloud>::vert_handle, N> Mesh<V, N, PointCloud>::verts
Iterable<typename std::vector<typename PointCloud::vert_handle>::const_iterator>
  Mesh<V, PointCloud>::verts(const typename Mesh<V, PointCloud>::face_handle fh) const { 
  
  using iter = typename std::vector<typename PointCloud::vert_handle>::const_iterator;

  iter begin_it = this->_indices.cbegin() + fh * n;

  // This would be problematic, if you add new vertices (iterator invalidation)
  return Iterable<iter>(begin_it, begin_it + n);

  // This way where you return an array will be more stable if you mutate the DS during iteration 
  //return std::array<typename Mesh<V, N, PointCloud>::vert_handle, N>(
  //  begin_it , begin_it + n
  //);

  //vert_handle* handle_ptr = _indices.data() + face_id;
//  std::array<typename Mesh<V, N, PointCloud>::vert_handle, N> vert_handles;
//  for(uint i = 0; i < N; ++i) {
  //  vh = *(handle_ptr++);
//    vert_handles[i] = _indices[fh * n + i];
//  }
//  return vert_handles;

}


template <typename V, typename PointCloud>
template <typename MeshType, typename>
Mesh<V, PointCloud>::Mesh(const MeshType& mesh) : BaseMesh<V, PointCloud>(mesh) { 
 
  face_handle first_face = *(mesh.faces().begin());
  n = mesh.face_size(first_face);

  for(face_handle fh : mesh.faces())
    if(mesh.face_size(fh) != n) {
        std::cout << face_size(fh) << "\n"; // ZERO FACE SIZE !!! 
        throw std::out_of_range("All faces should have a fixed size of incident vertices");
    }
}


template <typename V, typename PointCloud>
template <typename PC, typename>
inline Mesh<V, PC>& Mesh<V, PointCloud>::operator<<(const Mesh<V, PointCloud>& m) {

  if (n != m.n)
    throw std::invalid_argument("The two meshes that you want to concatenate, should be of the same face count for all faces");

  BaseMesh<V, PointCloud>::operator<<(m);

  return *this;

/*  uint new_verts_begin = this->_vertices.size();  
  this->_vertices.insert(this->_vertices.end(), m._vertices.begin(), m._vertices.end());

  std::vector<vert_handle> new_face;
  new_face.reserve(n);

  for (face_handle fh : m.faces()) {
    new_face.clear();

    for (vert_handle vh : m.verts(fh))
      new_face.push_back(vh + new_verts_begin);

    add_face(new_face);
  }

  return *this;*/

}



/*template<typename V, uint N, typename PointCloud>
//std::array<typename Mesh<V, N, PointCloud>::vert_handle, N> Mesh<V, N, PointCloud>::verts
Iterable<typename std::vector<typename PointCloud::vert_handle>::iterator>
  Mesh<V, N, PointCloud>::verts(const typename Mesh<V, N, PointCloud>::face_handle fh) { 
  
  using iter = typename std::vector<typename PointCloud::vert_handle>::iterator;

  iter begin_it = this->_indices.begin() + fh.begin_id;

  // This would be problematic, if you add new vertices (iterator invalidation)
  return Iterable<iter>(begin_it, begin_it + N);
}*/



/*template <typename V, uint N, typename PointCloud>
inline constexpr bool Mesh<V, N, PointCloud>::is_triangulated() const {
  return N == 3U;
}*/


/*template <typename V, uint N, typename PointCloud>
inline Mesh<V, N, PointCloud>::Mesh(const NgonMesh<V, PointCloud>& mesh)
  // Slicing constructor from BaseMesh // It directly copies the indices vector
  : BaseMesh<V, PointCloud>(mesh) {
  
  for(auto fh : mesh.faces())
    if (mesh.face_size(fh) != 3)
      throw std::out_of_range("The NgonMesh does not share the same face degree (number of vertices per face) as the Mesh<N>");  

}


template <typename V, uint N, typename PointCloud>
inline Mesh<V, N, PointCloud>::operator NgonMesh<V, PointCloud>() const {
  
  // It should use the constructor of BaseMesh
  // But it would NOT update the _face_sizes vector !!!
  
  //return NgonMesh<V, PointCloud>(*this);  
  
  NgonMesh<V, PointCloud> mesh;
  

  return mesh;
}*/



}
