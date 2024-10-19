#pragma once

#include "he_mesh.hpp"


namespace mesh {


namespace he {


template <typename V>
inline float edge_length(const HEMesh<V> &hm,
                             typename HEMesh<V>::hedge_handle hedge_id) {

  const V &a = hm.vert(hm.head(hedge_id));
  const V &b = hm.vert(hm.head(hm.twin(hedge_id)));

  return (a.pos - b.pos).mag();
}


template <typename V>
//inline V edge_center(const HEMesh<V>& hm, typename HEMesh<V>::hedge_handle hedge_id) {
inline V edge_center(const HEMesh<V>& hm, typename HEMesh<V>::hedge_handle hedge_id, const float h = 0.5f) {
  //return 0.5f * (hm.vert(hm.head(hedge_id)) + hm.vert(hm.head(hedge_id)));
 
	const V& va = hm.vert(hm.head(hedge_id));	// head vertex
	const V& vb = hm.vert(hm.tail(hedge_id)); // tail vertex

	return h * va + (1.0f - h) * vb;
}


template <typename V>
inline V vert_center(const HEMesh<V>& hm, typename HEMesh<V>::vert_handle vert_id) {
  using hedge_handle = typename HEMesh<V>::hedge_handle;

  hedge_handle he = hm.outgoing(vert_id);
  hedge_handle begin_id = he;
    
  V center{};
  uint n = 0U;

  do {
    center += hm.vert(hm.head(he));

    ++n;
    he = hm.next(hm.twin(he));
  } while(he != begin_id);
    
  return center /= n;
}


// 
//template <typename V>
//inline V tangent_vert_center(const HEMesh<V>& hm, typename HEMesh<V>::vert_handle vert_id)



template <typename V>
inline V face_center(const HEMesh<V>& hm, const typename HEMesh<V>::hedge_handle hedge_id) {
//  check_hedge_id(hedge_id);

  using hedge_handle = typename HEMesh<V>::hedge_handle;

  V v{};
  int n = 0;

  hedge_handle e = hedge_id;

  do {
    v += hm.vert(hm.head(e));

    ++n;
    e = hm.next(e);
  } while(e != hedge_id);

  return v /= n; 
}


template <typename V>
inline V face_center(const HEMesh<V>& hm, const typename HEMesh<V>::face_handle face_id) {
  return face_center(hm.hedge(face_id));
}



// TODO:
// vertex normal
// edge dir
// vertex sharpness
// edge sharpness








}


}
