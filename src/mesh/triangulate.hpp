#pragma once

#include "ngon_mesh.hpp"
#include "he/he_mesh.hpp"

namespace mesh {


template <typename MeshType>
inline bool is_triangulated(const MeshType& m) {
  
  for (typename MeshType::face_handle fh : m.faces())
    //if (m.face_size(fh) != 3)
    if (m.is_face_size(fh, 3))
      return false;

  return true;
}


template <typename V, uint N, typename PointCloud>  
constexpr bool is_triangulated(const Mesh<V, N, PointCloud>& m) {
  return N == 3U;
}

// Triangulates only one facet of the mesh
// There will be no such method to triangulate only one face of a NgonMesh !!!
//template <typename V, typename PointCloud>
//void triangulate(NgonMesh<V, PointCloud>& m, const NgonMesh<V, PointCloud>::face_handle fh) {}


/*
template <typename V, typename PointCloud>
void triangulate(NgonMesh<V, PointCloud>& m) {
  
  using hedge_handle = typename NgonMesh<V, PointCloud>::hedge_handle;

  // Copy the vertices
  NgonMesh<V> tm = pc::PointVec<V>(m);
  
	hedge_handle he[3];

	for (hedge_handle fh : m.faces()) {
		if (is_n_poly(fh, 2))
			continue;

		he[0] = fh;
		he[1] = next(he[0]);
		he[2] = next(he[1]);

		do {
			for (int i = 0; i < 3; ++i)
				m.indices.push_back(_hedges[he[i]].head_id);

			he[1] = he[2];
			he[2] = next(he[2]);

		} while (he[2] != he[0]);
	}

  m = tm;
}


template <typename V>
void triangulate(<V>& m) {
  
  using hedge_handle = typename NgonMesh<V, PointCloud>::hedge_handle;

  // Copy the vertices
  NgonMesh<V> tm = pc::PointVec<V>(m);
  
	hedge_handle he[3];

	for (hedge_handle fh : m.faces()) {
		if (is_n_poly(fh, 2))
			continue;

		he[0] = fh;
		he[1] = next(he[0]);
		he[2] = next(he[1]);

		do {
			for (int i = 0; i < 3; ++i)
				m.indices.push_back(_hedges[he[i]].head_id);

			he[1] = he[2];
			he[2] = next(he[2]);

		} while (he[2] != he[0]);
	}

  m = tm;
}
*/

} // namespace mesh
