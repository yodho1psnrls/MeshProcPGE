#pragma once
#include "he_mesh.hpp"
#include <vector>

// http://15462.courses.cs.cmu.edu/fall2016/article/15


namespace mesh {


namespace he {


template <typename V>
//inline typename HEMesh<V>::vert_handle sub_face_by_vert(
inline typename HEMesh<V>::vert_handle sqrt3_subdivision(
  HEMesh<V>& hm, 
    const typename HEMesh<V>::hedge_handle hedge_id,
    const V& v
) {
  
  using hedge_handle = typename HEMesh<V>::hedge_handle;
  using vert_handle = typename HEMesh<V>::vert_handle;

  hedge_handle ne = hm.next(hedge_id);
  hedge_handle first_hedge = hm.add_edge(hedge_id, v);
  hedge_handle e = ne;

  while(e != hedge_id) {
    ne = hm.next(e);

    hm.split_face_at(e, first_hedge);     

    e = ne;
  }  

  return hm.head(first_hedge);
}


template <typename V>
//inline typename HEMesh<V>::vert_handle sub_face_by_vert(
inline typename HEMesh<V>::vert_handle sqrt3_subdivision(
  HEMesh<V>& hm, 
    const typename HEMesh<V>::hedge_handle hedge_id
) {
//  return sub_face_by_vert(hm, hedge_id, hm.face_center(hedge_id));
  return sqrt3_subdivision(hm, hedge_id, face_center(hm, hedge_id));
}


// sqrt(3) subdivision
template <typename V>
//inline void sub_faces_by_vert(HEMesh<V>& hm) {
inline void sqrt3_subdivision(HEMesh<V>& hm) {
  // https://www.redalyc.org/pdf/4115/411534379004.pdf

	hm.shrink();

  using edge_handle = typename HEMesh<V>::edge_handle;
  using face_handle = typename HEMesh<V>::face_handle;

  std::vector<edge_handle> old_edges;
	old_edges.reserve(hm.edges_size());

  hm.reserve_hedges(2U * hm.hedges_size());
	hm.reserve_verts(hm.verts_size() + hm.faces_size());

	for (edge_handle e : hm.edges())
		if (!hm.is_border_edge(e))
			old_edges.push_back(e);

  auto old_faces = hm.faces();
	for (const face_handle f : old_faces)
//		sub_face_by_vert(hm, hm.hedge(f));
		sqrt3_subdivision(hm, hm.hedge(f));

	for (const edge_handle e : old_edges)
		hm.flip_edge(e);
}


// ============================================================================== //


/*Subdivides the face by refining / subdividing its edges and then clipping the old corners
	  The result looks like a camera lens shutter on big poly faces
	  It creates n - 1 new triangle faces and a middle one, which is a smaller version of the original face
	  This is useful (but not limited to) when subdividing Triangle faces*/
template <typename V>
inline void corner_subdivision(HEMesh<V>& hm) {
  // Refine All / Clip All
  // https://cs418.cs.illinois.edu/website/text/halfedge.html

// NOTE: You can also implement it by splitting all edges in whatever order
//        and then flipping the edges who have a new vertex and an old vertex at both ends

  using vert_handle = typename HEMesh<V>::vert_handle;
  using hedge_handle = typename HEMesh<V>::hedge_handle;
  using edge_handle = typename HEMesh<V>::edge_handle;

  hm.shrink();
  hm.reserve_verts(hm.verts_size() + hm.hedges_size() / 2U);  
  hm.reserve_hedges(3U * hm.hedges_size());

	for(const edge_handle e : hm.edges())
    hm.refine_edge(e);

	for (const vert_handle vh : hm.verts()) {
		hedge_handle begin_id = hm.ingoing(vh);
		hedge_handle he = begin_id;

		do {
			if (!hm.is_border_hedge(he))
				hm.clip_corner(he);

			he = hm.twin(hm.next(he));
		} while (he != begin_id);
	}
}

template <typename V>
inline void loop_subdivision(HEMesh<V>& hm) {

  using vert_handle = typename HEMesh<V>::vert_handle;
  using hedge_handle = typename HEMesh<V>::hedge_handle;
  using edge_handle = typename HEMesh<V>::edge_handle;
  using face_handle = typename HEMesh<V>::face_handle;

  


}


// ============================================================================== //


// Linear Subdivision
template <typename V>
//inline void quad_subdivision(HEMesh<V>& hm) {
inline Iterable<typename HEMesh<V>::vert_handle> quad_subdivision(HEMesh<V>& hm) {
//inline Iterable<typename HEMesh<V>::vert_handle> linear_subdivision(HEMesh<V>& hm) {

  using vert_handle = typename HEMesh<V>::vert_handle;
  using hedge_handle = typename HEMesh<V>::hedge_handle;
  using edge_handle = typename HEMesh<V>::edge_handle;
  using face_handle = typename HEMesh<V>::face_handle;

  // The faces and verts should be shrinked, because if not, the id of every face,
  //  won't map with the exact new center vertex index, 
  //  if there was a removed face or vertex
  hm.shrink_faces();
  hm.shrink_verts();

  hm.reserve_verts(hm.verts_size() + hm.faces_size() + hm.edges_size());  
  hm.reserve_edges(4U * hm.edges_size());
  //hm.reserve_faces(4U * hm.edges_size());

  vert_handle vb(hm.verts_size());  // face center vertices begin 
 
  for(const face_handle fh : hm.faces())
    hm.add_vert(face_center(hm, fh));

  vert_handle ve(hm.verts_size()); // face center vertices end

  for(const hedge_handle e : hm.edges()) {
    if (hm.is_removed_hedge(e))
      continue;

    //hedge_handle t = hm.twin(e);

    // When we refine the edge, head(e) 
    //  should be the vertex that is result of the refinement
    hm.refine_edge(e);
    
    //if (hm.begin(e) == hm.next(e))
    //  hm.shift_begin(e);
   
    //if (hm.begin(t) == t)
    //  hm.shift_begin(t);
  
  }

  for(const face_handle fh : hm.faces()) {
    vert_handle cv(fh + vb);

    hedge_handle begin_id = hm.hedge(fh);
    
    // If the head vertex of begin hedge 
    //  is not the result of the refined edges
    if (hm.head(begin_id) < ve)
      begin_id = hm.next(begin_id);

    hedge_handle ne = hm.next(hm.next(begin_id));
    hedge_handle first_hedge = hm.add_edge(begin_id, cv); 
    hedge_handle e = ne;
    
    while (e != begin_id) {
      ne = hm.next(hm.next(e));
      
      hm.split_face_at(e, first_hedge);

      e = ne;
    }
  }

  return Iterable<vert_handle>(vb, ve);
}


// NOTE: You can try to implement catmull_clark by first using quad_subdivision(hm)
//        and then work with the returned range of vertices, which represent the
//        centers of the old faces


// https://en.wikipedia.org/wiki/Catmull%E2%80%93Clark_subdivision_surface
template <typename V>
inline void catmull_clark(HEMesh<V>& hm) {

  using vert_handle = typename HEMesh<V>::vert_handle;
  using hedge_handle = typename HEMesh<V>::hedge_handle;
  using edge_handle = typename HEMesh<V>::edge_handle;
  using face_handle = typename HEMesh<V>::face_handle;

  hm.shrink();
  hm.reserve_verts(hm.verts_size() + hm.faces_size() + hm.edges_size());  
  hm.reserve_edges(4U * hm.edges_size());
  //hm.reserve_faces(4U * hm.edges_size());

  // Original vertices to original adjacent faces
//  std::vector<face_handle> vert_to_faces()
  
  vert_handle vb(hm.verts_size());  // face center vertices begin 
  for(const face_handle fh : hm.faces())
    hm.add_vert(hm.face_center(hm.hedge(fh)));   
  vert_handle ve(hm.verts_size()); // face center vertices end

  
  for(const hedge_handle e : hm.edges()) {
    hedge_handle t = hm.twin(e);
    V v{};
    
    uint n = 2U;

    face_handle f = hm.face(e);
    face_handle tf = hm.face(t);

    if (!hm.is_border_face(f) && !hm.is_border_face(tf)) {
      v += hm.vert(vert_handle(vb + f));
      v += hm.vert(vert_handle(vb + tf));

      n += 2U;
    }

    v += hm.vert(hm.head(e));
    v += hm.vert(hm.head(t));
    
    v /= n;
    hm.refine_edge(e, v);      
  }


  for(const face_handle fh : hm.faces()) {
    hedge_handle begin_id = hm.hedge(fh);
    vert_handle cv(fh + vb);
    
    // If the head vertex of begin hedge 
    //  is not the result of the refined edges
    if (hm.head(begin_id) < ve)
      begin_id = hm.next(begin_id);

    hedge_handle ne = hm.next(hm.next(begin_id));
    hedge_handle first_hedge = hm.add_edge(begin_id, cv); 
    hedge_handle e = ne;
    
    while (e != begin_id) {
      ne = hm.next(hm.next(e));
      
      hm.split_face_at(e, first_hedge);

      e = ne;
    }
  }

  // TODO: Make it such that if it is a border edge, then to only take the average of the two
  //  border edges (no matter if there are more adjacent edges) and the original point, for the
  //  new position formula

  for(vert_handle vh(0U); vh < vb; ++vh) {
    if (hm.is_isolated_vert(vh))
      continue;

    if (hm.is_border_vert(vh))
      continue;

    V avr_faces{};
    V avr_edges{}; // that is actually, twice the average edges

    uint nf = 0U;
    uint ne = 0U;

    hedge_handle begin_id = hm.outgoing(vh);
    hedge_handle he = begin_id;

    do {
      //face_handle f = hm.face(he);
      //if (!hm.is_border_face(f)) {
      //  vf += hm.vert(vert_handle(vb + f));
      //  ++nf;
      //}

      if (!hm.is_border_hedge(he)) {
        avr_faces += hm.vert(hm.head(hm.next(he)));
        ++nf;
      }
      
      avr_edges += hm.vert(hm.head(he));
      //v += hm.vert(hm.tail(he));
      avr_edges += hm.vert(vh);


      ++ne;
      he = hm.next(hm.twin(he));
    } while (he != begin_id);

    if (nf != 0U)
      avr_faces /= nf;
    
    avr_edges /= ne;

//    V temp = hm.vert(vh);

    hm.vert(vh) = (avr_faces + avr_edges + (ne - 3U) * hm.vert(vh)) / ne;
    //hm.vert(vh) = (avr_edges + (ne - 3U) * hm.vert(vh)) / ne;
    //hm.vert(vh) = V(olc::vf2d(1.0f/0.0f, 1.0f/0.0f));

//    if (hm.vert(vh).pos.mag() > 1e+6f)
//      hm.vert(vh) = temp;

    //std::cout << hm.vert(vh).pos << "\n";

  }

}


// ============================================================================== //


// https://en.wikipedia.org/wiki/Doo%E2%80%93Sabin_subdivision_surface
template <typename V>
inline void doo_sabin(HEMesh<V>& hm) {



}









}


}

