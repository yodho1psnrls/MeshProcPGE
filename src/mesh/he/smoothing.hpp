#pragma once

#include "he_mesh.hpp"
#include "utilities.hpp"  // vert_center

// #include <functional>
#include <vector>


namespace mesh {


namespace he {


// TODO: It may be a good idea to make this function to receive a function
//        as an argument for the vertex positioning, then you may choose
//        different positioning strategies
template <typename V>
inline void smooth_laplancian(
    HEMesh<V> &hm, const uint times = 1U,
//    const std::function<V(const HEMesh<V> &, typename HEMesh<V>::vert_handle)>
//        vert_func = vert_center) {
    V (*vert_func)(const HEMesh<V>&, typename HEMesh<V>::vert_handle) = vert_center) {

  using vert_handle = typename HEMesh<V>::vert_handle;
  using hedge_handle = typename HEMesh<V>::hedge_handle;

  std::vector<bool> skip_verts(hm.verts_size());
  for (const vert_handle vh : hm.verts())
    skip_verts[vh] =
      hm.is_removed_vert(vh) || 
      hm.is_isolated_vert(vh) ||
      hm.is_border_vert(vh);

  std::vector<V> new_verts(hm.verts_size());

  for (uint i = 0U; i < times; ++i) {
    for (const vert_handle vh : hm.verts())
      if (!skip_verts[vh])
        new_verts[vh] = vert_func(hm, vh);

    for (const vert_handle vh : hm.verts())
      if (!skip_verts[vh])
        hm.vert(vh) = new_verts[vh];
  }
}



} // namespace he


} // namespace mesh
