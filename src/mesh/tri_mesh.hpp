#pragma once
#include "mesh.hpp"
#include "../point_cloud/point_vec.hpp"


namespace mesh {


template <
	typename V,						   // Vertex Type
	typename PointCloud = pc::PointVec<V>  // Vertex PointCloud
>
class TriMesh : public Mesh<V, 3U, PointCloud> {

public:

  using Mesh<V, 3U, PointCloud>::Mesh;   // inherit the constructors

	//using typename Mesh<V, 3U, PointCloud>::vert_handle;
	using typename Mesh<V, 3U, PointCloud>::vert_handle;
  using typename Mesh<V, 3U, PointCloud>::face_handle;

 
  using Mesh<V, 3U, PointCloud>::add_vert;
	using Mesh<V, 3U, PointCloud>::add_face;

  using Mesh<V, 3U, PointCloud>::verts_size;
  using Mesh<V, 3U, PointCloud>::faces_size;

  using Mesh<V, 3U, PointCloud>::vert;
  using Mesh<V, 3U, PointCloud>::verts;
  using Mesh<V, 3U, PointCloud>::faces;


	uint add_face(const vert_handle vi1, const vert_handle vi2, const vert_handle vi3) {
		return this->add_face({ vi1, vi2, vi3});
	}


 
  template <uint N>
  TriMesh(const Mesh<V, N, PointCloud>& mesh) {


  }


  TriMesh(const Mesh<V, 4U, PointCloud>& mesh) {


  }






};


}
