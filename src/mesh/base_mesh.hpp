#pragma once
#include "../point_cloud/point_vec.hpp"
// #include "../utilities.hpp"	// Iterable

// The Vertex Container should support operator[] and std::vector syntax

namespace mesh {


// Base Mesh class that will be the foundation of the other mesh classes
template <
	typename V,							 // Vertex Type
	typename PointCloud = pc::PointVec<V>
>
//template <typename PointCloud>
class BaseMesh : public PointCloud {
public:


//  using typename PointCloud::V;
  using typename PointCloud::vert_type;

  using typename PointCloud::vert_handle;
  using PointCloud::add_vert;
  using PointCloud::verts_size;
  //using PointCloud::PointCloud;
  using PointCloud::vert;

  void clear();

  bool empty() const;
  bool is_valid() const;

  using PointCloud::are_valid_indices;
  using PointCloud::is_in_range;
  using PointCloud::reserve_verts;

  /*template <typename IterType>
  inline face_handle add_face(
    IterType begin_new_indices,
    IterType end_new_indices
  );*/


	BaseMesh(
    const PointCloud& vertices = PointCloud(),
    const std::vector<vert_handle>& indices
      = std::vector<vert_handle>()
  );

  // The count of all face adjacent edges (net face degree or half-edges count)
  uint indices_size() const;

  void reserve_indices(size_t new_capacity);

  template <typename PC = PointCloud, typename = typename std::enable_if<std::is_base_of_v<pc::PointVec<V>, PC>>>
  BaseMesh<V, PC>& operator<<(const BaseMesh<V, PointCloud>& m);


protected:
 
	std::vector<vert_handle> _indices{};

};


  // ======================= IMPLEMENTATION ========================= //


template <typename V, typename PointCloud>
inline void BaseMesh<V, PointCloud>::clear() {
  PointCloud::clear();
  _indices.clear();
}


template <typename V, typename PointCloud>
inline bool BaseMesh<V, PointCloud>::empty() const {
  return PointCloud::empty() && _indices.empty();
}


template <typename V, typename PointCloud>
inline bool BaseMesh<V, PointCloud>::is_valid() const {
  for(vert_handle vh : _indices)
    if (!is_in_range(vh))
      return false;
  
  return true;
}


template <typename V, typename PointCloud>
BaseMesh<V, PointCloud>::BaseMesh(
    const PointCloud& vertices,
    const std::vector<vert_handle>& indices
) : PointCloud(vertices), _indices(indices) {
  
  if (!is_valid())
    throw std::out_of_range("Some of the indices contain out of range vertex ids");

}


template <typename V, typename PointCloud>
inline uint BaseMesh<V, PointCloud>::indices_size() const {
  return _indices.size();
}


template <typename V, typename PointCloud>
inline void BaseMesh<V, PointCloud>::reserve_indices(size_t new_capacity) {
  _indices.reserve(new_capacity);
}


/*
template<typename V, typename PointCloud>
template <typename IterType>
inline typename BaseMesh<V, PointCloud>::face_handle BaseMesh<V, PointCloud>::add_face(IterType begin_new_indices, IterType end_new_indices) {
	return add_face(new_indices.begin(), new_indices.end());
}
*/


template <typename V, typename PointCloud>
template <typename PC, typename>
inline BaseMesh<V, PC>& BaseMesh<V, PointCloud>::operator<<(const BaseMesh<V, PointCloud>& m) {

  uint new_verts_begin = this->_vertices.size();  
  this->_vertices.insert(this->_vertices.end(), m._vertices.begin(), m._vertices.end());

  _indices.reserve(_indices.size() + m._indices.size());

  for (vert_handle vh : m._indices)
    _indices.push_back(vh + new_verts_begin);
  
  return *this;
}



}

