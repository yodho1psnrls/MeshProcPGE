#pragma once

#include "smoothing.hpp"          // laplancian smoothing
#include "edge_flip.hpp"

#include "utilities.hpp"          // edge_length
#include "he_mesh.hpp"

#include "triangulate.hpp"        // is_triangulated
#include <stdexcept>
#include <cmath>                  // std::abs


namespace mesh {


namespace he {


template <typename V>
inline bool valence_flip_condition(
    const HEMesh<V>& hm,
    typename HEMesh<V>::hedge_handle hedge_id
) {

//  if (!hm.is_face_size(hedge_id, 3U) || !hm.is_face_size(hm.twin(hedge_id), 3U))
//    throw std::invalid_argument("The two adjacent faces of the edge should be triangles to get valence_flip_condition");

  using hedge_handle = typename HEMesh<V>::hedge_handle;
  
  hedge_handle t = hm.twin(hedge_id);
  
  int i = hm.valence(hm.head(hedge_id));
  int j = hm.valence(hm.head(t));
  int k = hm.valence(hm.head(hm.next(hedge_id)));
  int l = hm.valence(hm.head(hm.next(t)));

  using namespace std;

  int before_flip = abs(i - 6) + abs(j - 6) + abs(k - 6) + abs(l - 6);
  int after_flip = abs(i - 1 - 6) + abs(j - 1 - 6) + abs(k + 1 - 6) + abs(l + 1 - 6);

  return after_flip - before_flip >= 0;
//  return after_flip - before_flip <= 0;
}



// Isotropic Remeshing:
// Based on the paper: A Remeshing Approach to Multiresolution Modeling (Section 4)
template <typename V>
inline void remesh_isotropic(HEMesh<V>& hm, const uint smoothing_iterations = 10U) {
  // http://15462.courses.cs.cmu.edu/fall2016/article/15
  // https://www.graphics.rwth-aachen.de/media/papers/remeshing1.pdf

  if (!is_triangulated(hm))
    throw std::invalid_argument("Cannot perform Isotropic Remeshing on a NON triangulated Half-Edge Mesh");

  using hedge_handle = typename HEMesh<V>::hedge_handle;
  using edge_handle = typename HEMesh<V>::edge_handle;


  float average_edge_len = 0.0f;
  uint n = 0;
  
  for(hedge_handle e : hm.edges()) {
    if (hm.is_removed_hedge(e))
      continue;

    //if (hm.is_isolated_edge(e))
    //  continue;

    average_edge_len += (hm.vert(hm.head(e)).pos - hm.vert(hm.tail(e)).pos).mag();
    ++n;
  }
  
  average_edge_len /= float(n);

  float split_edge_len = 4.0f / 3.0f * average_edge_len;
  float collapse_edge_len = 4.0f / 5.0f * average_edge_len;

  for(hedge_handle e : hm.edges()) {
    if (hm.is_removed_hedge(e))
      continue;

    if (edge_length(hm, e) > split_edge_len)
      hm.split_edge(e);
  }

  for(hedge_handle e : hm.edges()) {
    //if (hm.is_removed_hedge(e))
    //if (hm.is_border_edge(e))
  
    // TODO: Try to mark old and new vertices simultaniously
    //  if they are border, so you dont check 
    //  if they are border multiple times (memoization)
 
    if (hm.is_removed_hedge(e) ||
        hm.is_border_vert(hm.head(e)) ||
        hm.is_border_vert(hm.tail(e))
    ) continue;


    if (edge_length(hm, e) < collapse_edge_len)
      hm.collapse_edge(e);
  }  


//  conditional_edge_flip(hm, valence_flip_condition); 
//  delanay_edge_flip(hm);

  for(hedge_handle e : hm.edges()) {
    if (hm.is_removed_hedge(e) || hm.is_border_edge(e))
      continue;

    if (!valence_flip_condition(hm, e))
      hm.flip_edge(e);
  }


  //smooth_laplancian(hm, 10U, tangent_center_vert);  // TODO: IN 3D
  smooth_laplancian(hm, smoothing_iterations);

}



}


}
