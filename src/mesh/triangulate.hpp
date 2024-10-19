#pragma once

#include "he/he_mesh.hpp"
#include "tri_mesh.hpp"
#include "base_mesh.hpp"
#include "point_vec.hpp"

#include <type_traits>
#include <stack>        // triangulate_min_diagonal


namespace mesh {


//template <typename MeshType, typename V, typename PointCloud, typename = typename std::enable_if_t<std::is_base_of_v<mesh::BaseMesh<V, PointCloud>, MeshType>>>  
template <typename MeshType>
inline bool is_triangulated(const MeshType& m) {
  for (typename MeshType::face_handle fh : m.faces())
    if (m.face_size(fh) != 3U)
      return false;

  return true;
}


template <typename V, typename PointCloud>
inline bool is_triangulated(const Mesh<V, PointCloud>& m) {
  using face_handle = typename Mesh<V, PointCloud>::face_handle;
  return m.face_size(face_handle(0U)) == 3U;
}


namespace he {


template <typename V>
inline bool is_triangulated(const HEMesh<V>& hm) {
  for (typename HEMesh<V>::face_handle fh : hm.faces()) {
    if (hm.is_removed_face(fh))
      continue;

    if (!hm.is_face_size(fh, 3U))
      return false;
  }
 
  return true;
}


template <typename V>
inline void triangulate(HEMesh<V>& hm, typename HEMesh<V>::face_handle face_id) {
//	check_hedge_id(hedge_id);
  if (hm.is_removed_face(face_id))
    throw std::invalid_argument("Removed face index given to a triangulate function");
  
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


// This triangulation works best with planar convex faces
template <typename V>
inline TriMesh<V, pc::PointVec<V>> triangulated(const HEMesh<V>& hm) {
//  if (!hm.are_verts_shrinked())
//    throw std::invalid_argument("The HalfEdge vertices should be shrinked in order to be triangulated");

  using vert_handle = typename HEMesh<V>::vert_handle;  
  using hedge_handle = typename HEMesh<V>::hedge_handle;
  using face_handle = typename HEMesh<V>::face_handle;

 	TriMesh<V, pc::PointVec<V>> m;

  // NOTE: Think about a generalized way to directly copy the vertices vector, because they both share such a vector
  m.reserve_verts(hm.verts_size());
	for(vert_handle vh : hm.verts()) {
    if (!hm.is_removed_vert(vh))  
      m.add_vert(hm.vert(vh));
    else
      // if you dont add the removed vertices, then you
      // would have to remap all of them in the half-edges
      // So for now, just add a dummy vertex, so that
      // the rest of the vertex indices are mapped correctly
      m.add_vert(V());      
  }
  
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


template <typename V>
inline void triangulate_min_diagonal(HEMesh<V>& hm, typename HEMesh<V>::hedge_handle hedge_id) {

  using hedge_handle = typename HEMesh<V>::hedge_handle;
  using face_handle = typename HEMesh<V>::hedge_handle;
  using vert_handle = typename HEMesh<V>::hedge_handle;

//  if (hm.is_face_size(hedge_id, 2U))
//    return;

  std::stack<hedge_handle> stakk;
  stakk.push(hedge_id);

  while(!stakk.empty()) {
    hedge_handle begin_id = stakk.top();
    stakk.pop();

    if (hm.is_face_size(begin_id, 3U))
      continue;

    hedge_handle he1 = begin_id;

    float min_diag_len = std::numeric_limits<float>::max();
    hedge_handle min_diag_he1;
    hedge_handle min_diag_he2;

    // NOTE: This diagonal iteration iterates every possible diagonal twice
    //        try to optimize it to iterate all diagonals uniquely (only once)
    do {
      hedge_handle he2 = hm.next(hm.next(he1));
      while (hm.next(he2) != he1) {
        const V& a = hm.vert(hm.head(he1));
        const V& b = hm.vert(hm.head(he2));
        float diag_len = (a.pos - b.pos).mag();
        
        if (diag_len < min_diag_len) {
          min_diag_len = diag_len;
          min_diag_he1 = he1;
          min_diag_he2 = he2;
        }

        he2 = hm.next(he2);
      }

      he1 = hm.next(he1);
    } while (he1 != begin_id);


    hedge_handle e = hm.split_face_at(min_diag_he1, min_diag_he2);
    
    stakk.push(hm.twin(e));
    stakk.push(e);
  }
}


template <typename V>
inline void triangulate_min_diagonal(HEMesh<V>& hm, typename HEMesh<V>::face_handle face_id) {
  triangulate_min_diagonal(hm, hm.hedge(face_id));
}


template <typename V>
inline void triangulate_min_diagonal(HEMesh<V>& hm) {
  using face_handle = typename HEMesh<V>::face_handle;

  for(face_handle fh : hm.faces())
    triangulate_min_diagonal(hm, hm.hedge(fh));
}



}


// Very similar to triangulate_min_diagonal, but splits the faces only at diagonals which endpoints are at a two edges distance
template <typename MeshType, typename = typename std::enable_if_t<std::is_base_of_v<BaseMesh<typename MeshType::vert_type, typename MeshType::vert_container>, MeshType>>>
inline void triangulate_min_corner(MeshType& m) {

  using face_handle = typename MeshType::face_handle;
  using vert_handle = typename MeshType::vert_handle;

  using V = typename MeshType::vert_type;
  using PointCloud = typename MeshType::vert_container;

  MeshType nm = PointCloud(m); // Copy the vertices
  
  for(face_handle fh : m.faces()) {

    //std::stack<face_handle> stakk;
    auto face_verts = m.verts(fh);
    
    std::vector<vert_handle> vertices(face_verts.begin(), face_verts.end());

    while(vertices.size() > 3ULL) {
      float min_corner_len = std::numeric_limits<float>::max();
      uint min_corner_id;
      uint n = vertices.size();

      // NOTE: This diagonal iteration iterates every possible diagonal twice
      //        try to optimize it to iterate all corner diagonals uniquely (only once)
      for(uint i = 0U; i < n; ++i) {
        const V& a = m.vert(vertices[(i + 1U) % n]);
        const V& b = m.vert(vertices[(i + n - 1U) % n]);
        
        float corner_len = (a.pos - b.pos).mag();

        if (corner_len < min_corner_len) {
          min_corner_len = corner_len;
          min_corner_id = i;
        }
      } 
      
      nm.add_face({
        vertices[(min_corner_id + n - 1U) % n], 
        vertices[min_corner_id],
        vertices[(min_corner_id + 1U) % n],
      });

      vertices.erase(vertices.begin() + min_corner_id);
    }

    // At the end we are left with the last triangle of the face,
    //  which we also have to add
    nm.add_face(vertices);
  }

  m = nm;
}



// ===================================================================================== //


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


// ============================================================================== //


/*
// Triangulates the face of the given hedge_id
template <typename V>
inline void triangulate_radial_and_delanay(HEMesh<V>& hm, typename HEMesh<V>::hedge_handle hedge_id) {
  // Treat the boundary of the nPoly face as the boundary of the triangulation
  // (perform a delanay edge flip locally, only within the initial nPoly face)
  // The goal is to keep the topology of the geometry after the triangulation (if the nPoly face is flat/planar)
}


// Triangulates the whole mesh
void triangulate_radial_and_delanay() {

}


// Triangulates the face of the given hedge_id
void triangulate_min_diagonal(const hedge_handle hedge_id) {

}


// Triangulates the whole mesh
void triangulate_min_diagonal() {

}


void triangulate_by_min_angle_corner_clipping() {

}


void triangulate() {
  // http://www.polygontriangulation.com/2018/07/triangulation-algorithm.html
}


vector<face_handle> get_triangulated_face_indices(const hedge_handle hedge_id) const {

}

*/


} // namespace mesh
