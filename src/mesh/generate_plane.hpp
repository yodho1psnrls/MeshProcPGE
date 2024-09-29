#pragma once
#include "tri_mesh.hpp"
#include "../utilities.hpp"

namespace mesh {


template <typename V, typename PointCloud>
inline TriMesh<V, PointCloud> generate_plane(const int div, const float screen_ratio = 1.0f) {
	TriMesh<V, PointCloud> mesh;

	for (int i = 0; i < div + 1; i++)
		for (int j = 0; j < div + 1; j++) {
			V v;

			v.uv = olc::vf2d(float(i) / div, float(j) / div); // from 0 to 1
			v.pos = 2.0f * v.uv - olc::vf2d(1.0f, 1.0f);	  // from -1 to 1
			v.pos *= screen_ratio;

			v.pos = to_screen(v.pos);

			mesh.add_vert(v);

		}

	for (int i = 0; i < div; i++) {
		for (int j = 0; j < div; j++) {

			int index = i + j * (div + 1);
     
      //    1---3
      //    |\  |
			//    | \ |
      //    |  \|
		  //    0---2

      //            (0)      (1)           (2)
      //mesh.add_face(index, index + 1, index + (div + 1));
      mesh.add_face(index, index + (div + 1), index + 1);

      //               (1)             (3)                 (2)
      //mesh.add_face(index + 1, index + (div + 1) + 1, index + (div + 1));
		  mesh.add_face(index + 1, index + (div + 1), index + (div + 1) + 1);

    }
	}

	return mesh;
}



}
