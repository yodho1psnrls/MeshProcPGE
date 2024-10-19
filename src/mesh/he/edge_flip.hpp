#pragma once

#include "he_mesh.hpp"

#include "triangulate.hpp"  // is triangulated
#include <stdexcept>

// #include <functional>


namespace mesh {


namespace he {


/*template <typename V>
inline bool is_delanay(const HEMesh<V>& hm, const typename HEMesh<V>::hedge_handle hedge_id) {
  const V& a0 = hm.vert(hm.head(hedge_id));		// Error after used with collapse edge 
  const V& b0 = hm.vert(hm.head(hm.twin(hedge_id)));

  const V& a1 = hm.vert(hm.head(hm.next(hedge_id)));
  const V& b1 = hm.vert(hm.head(hm.next(hm.twin(hedge_id))));

  return (a0.pos - b0.pos).mag2() <= (a1.pos - b1.pos).mag2();
}*/

// Can be used for delanay condition in 3D !!!
// https://www.cs.jhu.edu/~misha/Fall09/7-delaunay.pdf
template <typename V>
inline float get_cotan_weight(const HEMesh<V> &hm,
                              typename HEMesh<V>::hedge_handle hedge_id) {
  float w = 0.0f;
  int n = 0;

  const V &i = hm.vert(hm.head(hedge_id));
  const V &j = hm.vert(hm.head(hm.twin(hedge_id)));

  if (!isBorderHedge(hedge_id)) {
    const V &k = hm.vert(hm.head(hm.next(hedge_id)));

    olc::vf2d ki_norm = (i.pos - k.pos).norm();
    olc::vf2d kj_norm = (j.pos - k.pos).norm();

    //			cosAngleK sinAngleK
    w += ki_norm.dot(kj_norm) /
         abs(ki_norm.dot(
             kj_norm.perp())); // Division by zero at flat faces after flip !!!
    ++n;
  }

  if (!hm.is_border_hedge(hm.twin(hedge_id))) { // !!!
    // if (!isBorder(hedge_id)) {
    const V &l = hm.vert(hm.head(hm.next(hm.twin(hedge_id))));

    olc::vf2d li_norm = (i.pos - l.pos).norm();
    olc::vf2d lj_norm = (j.pos - l.pos).norm();

    //			cosAngleL sinAngleL
    w += li_norm.dot(lj_norm) /
         abs(li_norm.dot(
             lj_norm.perp())); // Division by zero at flat faces after flip !!!
    // w -= liNorm.dot(ljNorm) / liNorm.dot(ljNorm.perp());
    ++n;
  }

  if (n == 2)
    w *= 0.5f;

  return w;
}


// https://www.cs.jhu.edu/~misha/Fall09/7-delaunay.pdf
template <typename V>
inline float intrinsic_cotan_weight(const HEMesh<V> &hm,
                                    typename HEMesh<V>::hedge_handle hedge_id) {
  float w = 0.0f;
  int n = 0;

  const V &i = hm.vert(hm.head(hedge_id));
  // const vf2d& jPos = vertices[head(next(next(hedge_id)))].pos;
  const V &j = hm.vert(hm.head(hm.twin(hedge_id)));

  float a = (i.pos - j.pos).mag();

  for (int iter = 0; iter < 2; ++iter)
    if (!hm.is_border_hedge(hedge_id)) {
      const V &k = hm.vert(hm.head(hm.next(hedge_id)));

      float b = (i.pos - k.pos).mag();
      float c = (j.pos - k.pos).mag();

      float tan_half_alpha_squared =
          (a - b + c) * (a + b - c) / ((a + b + c) * (-a + b + c));
      float cot_alpha =
          (1.0f - tan_half_alpha_squared) * 0.5f / sqrt(tan_half_alpha_squared);

      w += cot_alpha;
      ++n;
      hedge_id = hm.twin(hedge_id);
    }

  if (!hm.is_border_hedge(hm.twin(hedge_id))) { // !!!
    const V &l = hm.vert(hm.head(hm.next(hm.twin(hedge_id))));

    float b = (i.pos - l.pos).mag();
    float c = (j.pos - l.pos).mag();

    float tan_half_alpha_squared =
        (a - b + c) * (a + b - c) / ((a + b + c) * (-a + b + c));
    float cot_alpha =
        (1.0f - tan_half_alpha_squared) * 0.5f / sqrt(tan_half_alpha_squared);

    w += cot_alpha;
    ++n;
  }

  if (n == 2)
    w *= 0.5f;

  return w;
}


template <typename V>
inline bool is_delanay(const HEMesh<V> &hm,
                       typename HEMesh<V>::hedge_handle hedge_id) {
  // The cotangent weight is non-negative if the sum of the angles is less than
  // or equal to PI if the sum of the two angles is less than PI, then the edge
  // is locally delanay !! (no shared circumcircle)

  // return get_cotan_wweight(hm, hedge_id) >= 0.0f;
  return intrinsic_cotan_weight(hm, hedge_id) >= 0.0f;
}



// Triangles Only
template <typename V>
inline void conditional_edge_flip(
  HEMesh<V>& hm,
//  std::function<bool(
//    const HEMesh<V>&, typename HEMesh<V>::hedge_handle)> valid_edge_condition) {
  bool (*valid_edge_condition)(const HEMesh<V>&, typename HEMesh<V>::hedge_handle)) {

  // https://www.cise.ufl.edu/~ungor/delaunay/delaunay/node5.html

  if(!is_triangulated(hm))
    //throw std::invalid_argument("Cannot perform delanay_edge_flip on a NON triangular mesh");
    throw std::invalid_argument("The mesh should be triangulated, before performing flip_edges");

  using hedge_handle = typename HEMesh<V>::hedge_handle; 
  using edge_handle = typename HEMesh<V>::edge_handle; 


  // !!! Very important to do before edge iteration by twin half edges in memory
  hm.shrink();

  std::vector<edge_handle> stak; // stores half edge ids !
  stak.reserve(hm.edges_size());

  for (edge_handle e : hm.edges())
    if (!hm.is_border_edge(e)) {
      stak.push_back(e);
    }

  // We may not mark them, but if we mark them we save unnecessary iterations
  std::vector<bool> marked(hm.edges_size(), true); // stores booleans per edge ids !!!

  while (!stak.empty()) {
    edge_handle edge_id = stak.back();
    stak.pop_back();

    marked[edge_id] = false; // unmark

//    if (!is_delanay(hm, edge_id)) {
    if (!valid_edge_condition(hm, edge_id)) {
      hm.flip_edge(edge_id);

      hedge_handle others[4];
      others[0] = hm.next(edge_id);
      others[1] = hm.next(others[0]);
      others[2] = hm.next(hm.twin(edge_id));
      others[3] = hm.next(others[2]);

      for (uint i = 0U; i < 4U; ++i)
        // if (!marked[hm.edge(others[i])] && !hm.is_border_edge(others[i]))
        if (!marked[hm.edge(others[i])] && !hm.is_border_hedge(hm.twin(others[i])))
          stak.push_back(others[i]);
    }
  }
}


template <typename V>
inline void delanay_edge_flip(HEMesh<V>& hm) {
  conditional_edge_flip(hm, is_delanay);
}



} // namespace he


} // namespace mesh
