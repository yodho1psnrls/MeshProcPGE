#pragma once
//#include "mesh.hpp"
#include "../utilities.hpp"

// TODO: Implement this to work on generic MeshType, not only on Mesh or QuadMesh
//        and make it directly to create a quad mesh, not triangulized quad mesh !

namespace mesh {


template <typename MeshType>
inline MeshType generate_tri_plane(const uint div, const float screen_ratio = 1.0f) {
	MeshType mesh;
  using V = typename MeshType::vert_type;
  using vert_handle = typename MeshType::vert_handle;

  std::vector<vert_handle> added_verts;
  
  added_verts.reserve((div + 1U) * (div + 1U));
  mesh.reserve_verts(added_verts.capacity());
  mesh.reserve_indices(div * div * 6U);

	for (uint i = 0U; i < div + 1U; i++)
		for (uint j = 0U; j < div + 1U; j++) {
			V v;

			v.uv = olc::vf2d(float(i) / div, float(j) / div); // from 0 to 1
			v.pos = 2.0f * v.uv - olc::vf2d(1.0f, 1.0f);	  // from -1 to 1
			v.pos *= screen_ratio;

			v.pos = to_screen(v.pos);

			added_verts.push_back(mesh.add_vert(v));
		}

	for (uint i = 0U; i < div; i++) {
		for (uint j = 0U; j < div; j++) {
			uint index = i + j * (div + 1U);
     
      //    1---2
      //    |\  |
			//    | \ |
      //    |  \|
		  //    0---3

      mesh.add_face({
        added_verts[index],                   // 0
        added_verts[index + (div + 1U)],      // 1
        added_verts[index + 1U]               // 3
      });

      mesh.add_face({
        added_verts[index + 1],               // 1
        added_verts[index + (div + 1U)],      // 2
        added_verts[index + (div + 1U) + 1U]  // 3
      });
    }
	}

	return mesh;
}


template <typename MeshType>
inline MeshType generate_quad_plane(const uint div, const float screen_ratio = 1.0f) {
	MeshType mesh;
  using V = typename MeshType::vert_type;
  using vert_handle = typename MeshType::vert_handle;

  std::vector<vert_handle> added_verts;
  
  added_verts.reserve((div + 1U) * (div + 1U));
  mesh.reserve_verts(added_verts.capacity());
  mesh.reserve_indices(div * div * 4U);

	for (uint i = 0U; i < div + 1U; i++)
		for (uint j = 0U; j < div + 1U; j++) {
			V v;

			v.uv = olc::vf2d(float(i) / div, float(j) / div); // from 0 to 1
			v.pos = 2.0f * v.uv - olc::vf2d(1.0f, 1.0f);	  // from -1 to 1
			v.pos *= screen_ratio;

			v.pos = to_screen(v.pos);

			added_verts.push_back(mesh.add_vert(v));
		}

	for (uint i = 0U; i < div; i++) {
		for (uint j = 0U; j < div; j++) {
			uint index = i + j * (div + 1U);
     
      //    1---2
      //    |   |
      //    |   |
		  //    0---3

      mesh.add_face({
        added_verts[index],                   // 0
        added_verts[index + (div + 1U)],      // 1
        added_verts[index + 1U + (div + 1U)], // 2
        added_verts[index + 1U]               // 3
      });
    }
	}

	return mesh;
}



}
