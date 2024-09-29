#pragma once
#include "../point_cloud/point_vec.hpp"
// #include "../utilities.hpp"	// Iterable

typedef unsigned int vert_handle;

// The Vertex Container should support operator[] and std::vector syntax

namespace mesh {


// Base Mesh class that will be the foundation of the other mesh classes
template <
	typename V,							 // Vertex Type
	typename PointCloud = pc::PointVec<V>
>
class BaseMesh : public PointCloud {
public:


  using typename PointCloud::vert_handle;
  using PointCloud::add_vert;
  using PointCloud::verts_size;
  //using PointCloud::PointCloud;

/*
  template <typename IterType>
  inline face_handle add_face(
    IterType begin_new_indices,
    IterType end_new_indices
  );
*/

protected:
 
	std::vector<vert_handle> _indices{};


	BaseMesh(
    const PointCloud& vertices = PointCloud(),
    const std::vector<vert_handle>& indices
      = std::vector<vert_handle>()
  );


  template<typename IterType>
  bool are_valid_indices(IterType begin_it, IterType end_it) const;


};


  // ======================= IMPLEMENTATION ========================= //


template <typename V, typename PointCloud>
BaseMesh<V, PointCloud>::BaseMesh(
    const PointCloud& vertices,
    const std::vector<vert_handle>& indices
) : PointCloud(vertices), _indices(indices) {}


template<typename V, typename PointCloud>
template<typename IterType>
inline bool BaseMesh<V, PointCloud>::are_valid_indices(IterType begin_it, IterType end_it) const {

  for(IterType it = begin_it; it != end_it; ++it)
    if (*it < 0 || *it >= verts_size())
      return false;

  return true;
}


/*
template<typename V, typename PointCloud>
template <typename IterType>
inline typename BaseMesh<V, PointCloud>::face_handle BaseMesh<V, PointCloud>::add_face(IterType begin_new_indices, IterType end_new_indices) {
	return add_face(new_indices.begin(), new_indices.end());
}
*/


}

