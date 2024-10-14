#pragma once

#include "he/he_mesh.hpp"
#include "tri_mesh.hpp"
#include "base_mesh.hpp"
#include "point_vec.hpp"

#include <type_traits>


namespace mesh {


template <typename MeshType, typename V, typename PointCloud, typename = typename std::enable_if_t<std::is_base_of_v<mesh::BaseMesh<V, PointCloud>, MeshType>>>  
constexpr bool is_triangulated(const MeshType& m) {
  for (typename MeshType::face_handle fh : m.faces())
    if (m.face_size(fh) != 3)
    //if (!m.is_face_size(fh, 3))
      return false;

  return true;
}


namespace he {


template <typename V>
void triangulate(HEMesh<V>& hm, typename HEMesh<V>::face_handle face_id) {
//	check_hedge_id(hedge_id);
  hm.is_removed_face(face_id);
  
  using hedge_handle = typename HEMesh<V>::hedge_handle;
  hedge_handle hedge_id = hm.hedge(face_id);

	if (hm.is_face_size(face_id, 2U))
		//throw std::invalid_argument("The face should be non-degrade");
		return;

	// if it is a triangle, it will terminate at the beginning, so no need to check explicidly
	hedge_handle e = hm.next(hm.next(hedge_id));
	while (hm.next(e) != hedge_id) {
		hedge_handle ne = hm.next(e);

		hm.split_face_at(hedge_id, e);

		e = ne;
	}

}


template<typename V>
inline void triangulate(HEMesh<V>& hm) {
	auto old_faces = hm.faces();
	for (const auto& f : old_faces)
		triangulate(hm, f);

}


//template <typename V, typename PointCloud>
template <typename V>
inline TriMesh<V, pc::PointVec<V>> triangulated(const HEMesh<V>& hm) {
//  if (!hm.are_verts_shrinked())
//    throw std::invalid_argument("The HalfEdge vertices should be shrinked in order to be triangulated");

  using vert_handle = typename HEMesh<V>::vert_handle;  
  using hedge_handle = typename HEMesh<V>::hedge_handle;
  using face_handle = typename HEMesh<V>::face_handle;


 	TriMesh<V, pc::PointVec<V>> m;

  m.reserve_verts(hm.verts_size());
	for(vert_handle vh : hm.verts())
    if (!hm.is_removed_vert(vh))
      m.add_vert(hm.vert(vh));
  
  m.reserve_indices(hm.hedges_size());

	hedge_handle he[3];

	for (face_handle f : hm.faces()) {
    if (hm.is_removed_face(f) || hm.is_face_size(f, 2U))
			continue;

		he[0] = hm.hedge(f);
		he[1] = hm.next(he[0]);
		he[2] = hm.next(he[1]);

		do {
  		m.add_face(hm.head(he[0]), hm.head(he[1]), hm.head(he[2]));
      
			he[1] = he[2];
			he[2] = hm.next(he[2]);

		} while (he[2] != he[0]);
	}

	return m; 

}



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
