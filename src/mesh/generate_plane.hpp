#pragma once
#include "tri_mesh.hpp"
#include "../utilities.hpp"

// TODO: Implement this to work on generic MeshType, not only on TriMesh or QuadMesh
//        and make it directly to create a quad mesh, not triangulized quad mesh !

namespace mesh {


template <typename V, typename PointCloud>
inline TriMesh<V, PointCloud> generate_plane(const uint div, const float screen_ratio = 1.0f) {
	TriMesh<V, PointCloud> mesh;

  using vert_handle = typename TriMesh<V, PointCloud>::vert_handle;

	for (uint i = 0U; i < div + 1U; i++)
		for (uint j = 0U; j < div + 1U; j++) {
			V v;

			v.uv = olc::vf2d(float(i) / div, float(j) / div); // from 0 to 1
			v.pos = 2.0f * v.uv - olc::vf2d(1.0f, 1.0f);	  // from -1 to 1
			v.pos *= screen_ratio;

			v.pos = to_screen(v.pos);

			mesh.add_vert(v);

		}

	for (uint i = 0U; i < div; i++) {
		for (uint j = 0U; j < div; j++) {

			uint index = i + j * (div + 1U);
     
      //    1---3
      //    |\  |
			//    | \ |
      //    |  \|
		  //    0---2

      mesh.add_face(
        vert_handle(index),                   // 0
        vert_handle(index + (div + 1U)),      // 1
        vert_handle(index + 1U)               // 2
      );

      mesh.add_face(
        vert_handle(index + 1),               // 1
        vert_handle(index + (div + 1U)),      // 3
        vert_handle(index + (div + 1U) + 1U)  // 2
      );
      
    }
	}

	return mesh;
}



}
