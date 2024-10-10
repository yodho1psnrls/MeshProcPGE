#pragma once
#include "mesh.hpp"
#include "../point_cloud/point_vec.hpp"
#include <type_traits>

namespace mesh {


template <
	typename V,						   // Vertex Type
	typename PointCloud = pc::PointVec<V>  // Vertex PointCloud
>
class TriMesh : public Mesh<V, PointCloud> {

public:

  //using typename Mesh<V, 3U, PointCloud>::vert_handle;
	using typename Mesh<V, PointCloud>::vert_handle;
  using typename Mesh<V, PointCloud>::face_handle;

 
  using Mesh<V, PointCloud>::add_vert;
	using Mesh<V, PointCloud>::add_face;

  using Mesh<V, PointCloud>::verts_size;
  using Mesh<V, PointCloud>::faces_size;

  using Mesh<V, PointCloud>::vert;
  using Mesh<V, PointCloud>::verts;
  using Mesh<V, PointCloud>::faces;
 
  //using Mesh<V, PointCloud>::Mesh;   // inherit the constructors
  
  bool is_valid() const {
    return Mesh<V, PointCloud>::is_valid() && this->n == 3U;
  }


	TriMesh(
    const PointCloud& vertices = PointCloud(),
    const std::vector<vert_handle>& indices
      = std::vector<vert_handle>())
    : Mesh<V, PointCloud>::Mesh(3U, vertices, indices) {

	  if(!is_valid())
      throw std::invalid_argument("Non valid indices count or invalid vert_handle in the indices");
  } 


  template <typename MeshType, typename = typename std::enable_if_t<
    std::is_base_of_v<BaseMesh<V, PointCloud>, MeshType> &&
    !std::is_same_v<MeshType, TriMesh>
  >>
  TriMesh(const MeshType& mesh) : Mesh<V, PointCloud>(mesh) { 
    if (this->n != 3U)
      throw std::invalid_argument("The converted MeshType should be consisted of triangle faces");
  }


//  TriMesh(const TriMesh& other)
//  : Mesh<V, PointCloud>::n(3U),
//    BaseMesh<V, PointCloud>::BaseMesh(other) {}



	face_handle add_face(const vert_handle vi1, const vert_handle vi2, const vert_handle vi3) {
		return Mesh<V, PointCloud>::add_face({ vi1, vi2, vi3});
	}

 
/*  template <uint N>
  TriMesh(const Mesh<V, N, PointCloud>& mesh) {

  }*/


};

}
