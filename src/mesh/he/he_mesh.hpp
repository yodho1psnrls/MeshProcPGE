#pragma once

#include "../../utilities.hpp" 
#include "../ngon_mesh.hpp"

#include "../handlers.h"
#include "../../point_cloud/point_vec.hpp"

#include <iterator>
#include <vector>
#include <unordered_map>
#include <unordered_set>
//#include <queue>
//#include <functional>   // std::greater
#include <set>

#include <stdexcept>
#include <type_traits>

//#define HE_INVALID_INDEX -1
//#define ISOLATED_INDEX -2


namespace mesh {

namespace he {

// Min Heap Priority Queue   // Access to the smallest element
//template<typename T>
//using rpriority_queue = typename std::priority_queue<T, std::vector<T>, std::greater<T>>;

static const index_hedge_handle ISOLATED_INDEX = index_hedge_handle(uint(-2));
//constexpr index_hedge_handle ISOLATED_INDEX = index_hedge_handle(uint(-2));


struct HalfEdge {

  pc::index_vert_handle head_id;
  index_hedge_handle next_id;
  index_face_handle face_id;  // face or border id
  
/*  HalfEdge(
    pc::index_vert_handle head_id = pc::index_vert_handle::INVALID_INDEX,
    index_hedge_handle next_id = index_hedge_handle::INVALID_INDEX,
    index_face_handle face_id = index_face_handle::INVALID_INDEX
  ) : head_id(head_id), next_id(next_id), face_id(face_id) {}
*/

  HalfEdge() {}

  HalfEdge(
    pc::index_vert_handle head_id,
    index_hedge_handle next_id,
    index_face_handle face_id
  ) : head_id(head_id), next_id(next_id), face_id(face_id) {}

};


// Face or Border face, that only stores one of its half-edges
struct Face {

  index_hedge_handle begin_id;

  Face() {}
//  Face() : begin_id(index_hedge_handle::INVALID_INDEX) {}
  Face(index_hedge_handle begin_id) : begin_id(begin_id) {}

};


// =============================================================================================== //


template <typename V>
class HEMesh {
public:

  
  using vert_handle = typename pc::index_vert_handle;
  using hedge_handle = typename mesh::he::index_hedge_handle;
  using edge_handle = typename mesh::he::index_edge_handle;
  using face_handle = typename mesh::index_face_handle;

/*
  class vert_to_outer_hedges_loop;
  class vert_to_inner_hedges_loop;
  class vert_to_edges_loop;
  class vert_to_faces_loop;
  class face_to_verts_loop;
  class face_to_hedges_loop;
  class face_to_edges_loop;
  class face_to_faces_loop;


private:
  
  friend vert_to_outer_hedges_loop;
  friend vert_to_inner_hedges_loop;
  friend vert_to_edges_loop;
  friend vert_to_faces_loop;
  friend face_to_verts_loop;
  friend face_to_hedges_loop;
  friend face_to_edges_loop;
  friend face_to_faces_loop;

public:
*/

  // --------------------------------------------------------------------------------------------- //


  //using face_handle = typename face_handle;
  //using border_handle = typename face_handle;

  void check_validity() const;

  HEMesh() = default;

  //template <typename MeshType, typename = typename std::enable_if_t<std::is_base_of_v<BaseMesh<V, pc::PointVec<V>>, MeshType>>>
  //HEMesh(const MeshType& m);

  //template <typename MeshType, typename = typename std::enable_if_t<std::is_base_of_v<BaseMesh<V, pc::PointVec<V>>, MeshType>>>  
  //operator MeshType() const;

//  operator Mesh<V, 3U, pc::PointVec<V>>() const;

//  HEMesh(const NgonMesh<V, pc::PointVec<V>>& m);
//  operator NgonMesh<V, pc::PointVec<V>>() const;
  
  template <typename MeshType, typename = typename std::enable_if_t<
    std::is_base_of_v<BaseMesh<V, pc::PointVec<V>>, MeshType>
  //  && !std::is_same_v<MeshType, HEMesh<V>>
  >>
  HEMesh(const MeshType& m);

  template <typename MeshType, typename = typename std::enable_if_t<
    std::is_base_of_v<BaseMesh<V, pc::PointVec<V>>, MeshType>
  //  && !std::is_same_v<MeshType, HEMesh<V>>
  >>
  operator MeshType() const;


  vert_handle add_vert(const V& v);
  edge_handle add_edge(vert_handle a, vert_handle b);
  face_handle add_face(const std::vector<vert_handle>& new_indices);


  void remove_vert(vert_handle vh);
  void remove_edge(edge_handle eh);
  void remove_loop(face_handle lh);


  void clear();
  void shrink_verts();
  void shrink_edges();
  void shrink_loops();
  void shrink();

// --------------------------------------------------------------------------- //

  const V& vert(const vert_handle) const;
  V& vert(const vert_handle);

  // half-edge to vertex
  vert_handle head(const hedge_handle) const;
  vert_handle tail(const hedge_handle) const;

  // vertex to half-edge
  hedge_handle outgoing(const vert_handle) const;
  hedge_handle ingoing(vert_handle) const;

  // half-edge to incident half-edge
  hedge_handle next(const hedge_handle) const;
  hedge_handle prev(const hedge_handle) const;
  hedge_handle twin(const hedge_handle) const; // other-half/twin/opposite half-edge

  hedge_handle hedge(const edge_handle) const;
  hedge_handle hedge(const face_handle) const;
  edge_handle edge(const hedge_handle) const;  
  face_handle face(const hedge_handle) const; // get the face or border face of the half-edge

  hedge_handle hedge(const vert_handle, const vert_handle) const; // returns ISOLATED_INDEX if not found
  edge_handle edge(const vert_handle, const vert_handle) const;   // returns ISOLATED_INDEX if not found
  face_handle face(const vert_handle, const vert_handle) const;   // returns HE_INVALID_INDEX if not found


  bool is_removed_face(const face_handle) const;
  bool is_removed_hedge(const hedge_handle) const;
  bool is_removed_edge(const edge_handle) const;
  bool is_removed_vert(const vert_handle) const;

  bool is_border_face(const face_handle) const;
  bool is_border_hedge(const hedge_handle) const;
  bool is_border_edge(const edge_handle) const;
  bool is_border_vert(const vert_handle) const;

  bool is_isolated_loop(const hedge_handle) const; // face_handle or hedge_handle ??
  bool is_isolated_edge(const edge_handle) const;
  bool is_isolated_vert(const vert_handle) const;

  bool empty() const;
  bool are_verts_shrinked() const;
  bool are_edges_shrinked() const;
  bool are_faces_shrinked() const;
  bool is_shrinked() const;         // is the whole DS shrinked
 
  
  // Those do NOT give the actual count of each 
  //  element type(verts, edges, ...) if it is not shrinked ! 
  uint verts_size() const;
  uint edges_size() const;
  uint hedges_size() const;
  uint indices_size() const;  // Same as hedges_size, but added for generalization with the other mesh types
  uint faces_size() const;
  uint borders_size() const;


  void reserve_verts(const uint capacity);
  void reserve_hedges(const uint capacity);
  void reserve_edges(const uint capacity);
  void reserve_loops(const uint capacity); // reserve faces + borders count


  Iterable<vert_handle> verts() const;
  Iterable<edge_handle> edges() const;
  Iterable<hedge_handle> hedges() const;
  Iterable<face_handle> faces() const;
  Iterable<face_handle> borders() const;


  //bool is_triangulated() const; // maybe do this as a generic function that works both on HEMesh, NgonMesh and other non-statically triangular Mesh classes
  
  //bool is_face_size(const hedge_handle hedge_id, const uint n) const; // true if the face_size(hedge_id) == n, use this if you want to check for a size of the face, because its cheaper if it is a big face and you just want to check if it is a triangle for example
  //uint face_size(const hedge_handle hedge_id) const; // the number of adjacent edges/vertices to the face or border face of the half-edge
  
  bool is_face_size(const face_handle face_id, const uint n) const; // true if the face_size(hedge_id) == n, use this if you want to check for a size of the face, because its cheaper if it is a big face and you just want to check if it is a triangle for example
  uint face_size(const face_handle face_id) const; // the number of adjacent edges/vertices to the face or border face of the half-edge
 
  uint valence(const vert_handle vert_id) const;      // the number of adjacent edges to the vertex
  bool is_extremal(const vert_handle vert_id) const;  // a vertex is extremal if it is adjacent to only one edge


// --------------------------------------------------------------------------- //

  // These are not Iterators !!!, they are more like Loops, which iterate, until they reach the begining again
  // So always use them with a do-while loop !
  
  // Also, all of them are like constant iterators, you cannot change the handle value that they point to
  // The point is that from them you can get adjacency info, but you cannot change those values directly
  // The only way to manipulate the Data Structure is to use its local vert/edge/face operations

  // And all of them are made bidirectional for convenience, 
  //  but for performance reasons, prefer to iterate them forward most of the times ! 

/*
  class vert_to_verts_loop {
      
    friend HEMesh<V>;

    const HEMesh<V>& hm;
    hedge_handle he;

    vert_to_verts_loop(
      const vert_handle vh,
      const HEMesh<V>& hm
    ) : he(hm.outgoing(vh)), hm(hm) {}

  public:

    vert_to_verts_loop& operator++() { he = hm.next(hm.twin(he)); return *this; }
    vert_to_verts_loop operator++(int) {auto temp = *this; ++(*this); return temp; }
    
    vert_to_verts_loop& operator--() { he = hm.twin(hm.prev(he)); return *this; }
    vert_to_verts_loop operator--(int) {auto temp = *this; --(*this); return temp; }

    vert_handle operator*() const { return hm.head(he); }

    bool operator==(const vert_to_verts_loop& other) const { return he == other.he; }
    bool operator!=(const vert_to_verts_loop& other) const { return he != other.he; }

  };


  class vert_to_outgoing_hedges_loop {
    
    friend HEMesh<V>;

    const HEMesh<V>& hm;
    hedge_handle he;

    vert_to_outgoing_hedges_loop(
      const vert_handle vh,
      const HEMesh<V>& hm
    ) : he(hm.outgoing(vh)), hm(hm) {}

  public:

    vert_to_outgoing_hedges_loop& operator++() { he = hm.next(hm.twin(he)); return *this; }
    vert_to_outgoing_hedges_loop operator++(int) {auto temp = *this; ++(*this); return temp; }
    
    vert_to_outgoing_hedges_loop& operator--() { he = hm.twin(hm.prev(he)); return *this; }
    vert_to_outgoing_hedges_loop operator--(int) {auto temp = *this; --(*this); return temp; }

    hedge_handle operator*() const { return he; }

    bool operator==(const vert_to_outgoing_hedges_loop& other) const { return he == other.he; }
    bool operator!=(const vert_to_outgoing_hedges_loop& other) const { return he != other.he; }

  };


  class vert_to_ingoing_hedges_loop {
    friend HEMesh<V>;

    const HEMesh<V>& hm;
    hedge_handle he;

    vert_to_ingoing_hedges_loop(
      const vert_handle vh,
      const HEMesh<V>& hm
    ) : he(hm.ingoing(vh)), hm(hm) {}

  public:

    vert_to_ingoing_hedges_loop& operator++() { he = hm.twin(hm.next(he)); return *this; }
    vert_to_ingoing_hedges_loop operator++(int) {auto temp = *this; ++(*this); return temp; }
    
    vert_to_ingoing_hedges_loop& operator--() { he = hm.prev(hm.twin(he)); return *this; }
    vert_to_ingoing_hedges_loop operator--(int) {auto temp = *this; --(*this); return temp; }

    hedge_handle operator*() const { return he; }

    bool operator==(const vert_to_ingoing_hedges_loop& other) const { return he == other.he; }
    bool operator!=(const vert_to_ingoing_hedges_loop& other) const { return he != other.he; }
 
  };


  class vert_to_edges_loop {
    friend HEMesh<V>;
   
    const HEMesh<V>& hm;
    hedge_handle he;

    vert_to_edges_loop(
      const vert_handle vh,
      const HEMesh<V>& hm
    ) : he(hm.outgoing(vh)), hm(hm) {}

  public:

    vert_to_edges_loop& operator++() { he = hm.next(hm.twin(he)); return *this; }
    vert_to_edges_loop operator++(int) {auto temp = *this; ++(*this); return temp; }
    
    vert_to_edges_loop& operator--() { he = hm.twin(hm.prev(he)); return *this; }
    vert_to_edges_loop operator--(int) {auto temp = *this; --(*this); return temp; }

//    edge_handle operator*() const { return hm.edge(he); }
    edge_handle operator*() const { return edge_handle(he); }

    bool operator==(const vert_to_edges_loop& other) const { return he == other.he; }
    bool operator!=(const vert_to_edges_loop& other) const { return he != other.he; }
 
  };


  class vert_to_faces_loop {
    friend HEMesh<V>;
  
    HEMesh<V>& hm;
    hedge_handle he;

    vert_to_faces_loop(
      const vert_handle vh,
      const HEMesh<V>& hm
    ) : he(hm.outgoing(vh)), hm(hm) {}

  public:

    vert_to_faces_loop& operator++() { he = hm.next(hm.twin(he)); return *this; }
    vert_to_faces_loop operator++(int) {auto temp = *this; ++(*this); return temp; }
    
    vert_to_faces_loop& operator--() { he = hm.twin(hm.prev(he)); return *this; }
    vert_to_faces_loop operator--(int) {auto temp = *this; --(*this); return temp; }

//    edge_handle operator*() const { return hm.edge(he); }
    face_handle operator*() const { return hm.face(he); }

    bool operator==(const vert_to_faces_loop& other) const { return he == other.he; }
    bool operator!=(const vert_to_faces_loop& other) const { return he != other.he; }

  };



  class face_to_verts_loop {
    friend HEMesh<V>;
 
    const HEMesh<V>& hm;
    hedge_handle he;

    face_to_verts_loop(
      const face_handle fh,
      const HEMesh<V>& hm
    ) : he(hm.hedge(fh)), hm(hm) {}

  public:

    face_to_verts_loop& operator++() { he = hm.next(he); return *this; }
    face_to_verts_loop operator++(int) {auto temp = *this; ++(*this); return temp; }
    
    face_to_verts_loop& operator--() { he = hm.prev(he); return *this; }
    face_to_verts_loop operator--(int) {auto temp = *this; --(*this); return temp; }

//    edge_handle operator*() const { return hm.edge(he); }
    vert_handle operator*() const { return hm.head(he); }

    bool operator==(const face_to_verts_loop& other) const { return he == other.he; }
    bool operator!=(const face_to_verts_loop& other) const { return he != other.he; }

  };


  class face_to_hedges_loop {
    friend HEMesh<V>;
  
    const HEMesh<V>& hm;
    hedge_handle he;

    face_to_hedges_loop(
      const face_handle fh,
      const HEMesh<V>& hm
    ) : he(hm.hedge(fh)), hm(hm) {}

  public:

    face_to_hedges_loop& operator++() { he = hm.next(he); return *this; }
    face_to_hedges_loop operator++(int) {auto temp = *this; ++(*this); return temp; }
    
    face_to_hedges_loop& operator--() { he = hm.prev(he); return *this; }
    face_to_hedges_loop operator--(int) {auto temp = *this; --(*this); return temp; }

//    edge_handle operator*() const { return hm.edge(he); }
    hedge_handle operator*() const { return he; }

    bool operator==(const face_to_hedges_loop& other) const { return he == other.he; }
    bool operator!=(const face_to_hedges_loop& other) const { return he != other.he; }

  };


  class face_to_edges_loop {
    friend HEMesh<V>;
  
    const HEMesh<V>& hm;
    hedge_handle he;

    face_to_edges_loop(
      const face_handle fh,
      const HEMesh<V>& hm
    ) : he(hm.hedge(fh)), hm(hm) {}

  public:

    face_to_edges_loop& operator++() { he = hm.next(he); return *this; }
    face_to_edges_loop operator++(int) {auto temp = *this; ++(*this); return temp; }
    
    face_to_edges_loop& operator--() { he = hm.prev(he); return *this; }
    face_to_edges_loop operator--(int) {auto temp = *this; --(*this); return temp; }

    edge_handle operator*() const { return hm.edge(he); }

    bool operator==(const face_to_edges_loop& other) const { return he == other.he; }
    bool operator!=(const face_to_edges_loop& other) const { return he != other.he; }

  };


  class face_to_faces_loop {
    friend HEMesh<V>;
  
    const HEMesh<V>& hm;
    hedge_handle he;

    face_to_faces_loop(
      const face_handle fh,
      const HEMesh<V>& hm
    ) : he(hm.hedge(fh)), hm(hm) {}

  public:

    face_to_faces_loop& operator++() { he = hm.next(he); return *this; }
    face_to_faces_loop operator++(int) {auto temp = *this; ++(*this); return temp; }
    
    face_to_faces_loop& operator--() { he = hm.prev(he); return *this; }
    face_to_faces_loop operator--(int) {auto temp = *this; --(*this); return temp; }

    face_handle operator*() const { return hm.face(hm.twin(he)); }

    bool operator==(const face_to_faces_loop& other) const { return he == other.he; }
    bool operator!=(const face_to_faces_loop& other) const { return he != other.he; }

  };
*/


// --------------------------------------------------------------------------- //

private:

  void invalidate_vert(const vert_handle vert_id);
  void invalidate_edge(const edge_handle edge_id);
  void invalidate_loop(const face_handle face_id);

  void free_vert(const vert_handle vert_id);
  void free_edge(const edge_handle edge_id);
  void free_loop(const face_handle face_id);

  bool is_in_range_vert(const vert_handle vert_id) const;
  bool is_in_range_hedge(const hedge_handle hedge_id) const;
  //bool is_in_range_edge(const edge_handle edge_id) const;
  bool is_in_range_loop(const face_handle face_id) const;
  //bool is_in_range_face(const face_handle face_id) const;
  //bool is_in_range_border(const face_handle face_id) const;


  void check_vert_id(const vert_handle vert_id) const;
  void check_hedge_id(const hedge_handle hedge_id) const;
  //void check_edge_id(const edge_handle& edge_id) const;
  void check_face_or_border_id(const face_handle face_id) const; // check if it is a valid face or border handler
  //void check_face_id(const face_handle face_id) const;
  //void check_border_id(const face_handle border_id) const;


  // Checks if two half-edges share the same face , iterating through the next half-edges, it does not just compare the loop_ids
  bool are_same_loop(const hedge_handle hedge_id1, const hedge_handle hedge_id2) const;

  void shift_begin(const face_handle face_id, const uint how_much = 1);
  //void set_loop_id(const hedge_handle hedge_id);
  //void set_begin_id(const hedge_handle hedge_id, const hedge_handle begin_id);
  void set_loop_id(const hedge_handle hedge_id, const face_handle face_id);
  void set_vert_id(const hedge_handle hedge_id, const vert_handle new_head_vert_id); 
  

  face_handle find_common_face_around_head(hedge_handle& he1, hedge_handle& he2);
  face_handle find_border_around_head(hedge_handle& hedge_id);

  hedge_handle ltwin(const hedge_handle hedge_id) const;
  hedge_handle rtwin(const hedge_handle hedge_id) const;

  hedge_handle new_edge();
  hedge_handle new_edge(const vert_handle tail_id, const vert_handle head_id); 

// ------------------------------------------------------------------------ //


  std::vector<V> _vertices;
  std::vector<hedge_handle> _vert_to_hedge;

  std::vector<HalfEdge> _hedges;

  // All loops that form a face or border
  // Faces span from 0 to _faces_cap - 1 // Borders span from _faces_cap to _faces_and_borders.size() 
  std::vector<Face> _faces_and_borders;  
  uint _faces_cap{};

  // Garbage collection
  std::set<vert_handle> _removed_verts;
  std::set<edge_handle> _removed_edges;
  std::set<face_handle> _removed_faces;



};


// ========================= IMPLEMENTATION ==================================== //

/*
template <typename V>
inline Iterable<vert_handle> HEMesh<V>::verts() const {
	return Iterable<vert_handle>(vert_handle(0), verts_size());
}

template <typename V>
inline Iterable<hedge_handle> HEMesh<V>::hedges() const {
	return Iterable<hedge_handle>(hedge_handle(0), hedges_size());
}

template <typename V>
inline Iterable<edge_handle> HEMesh<V>::edges() const {
	return Iterable<edge_handle>(edge_handle(0), edges_size());
}

template <typename V>
inline const std::unordered_set<hedge_handle>& HEMesh<V>::faces() const {
	return _faces;
}

template <typename V>
inline const std::unordered_set<hedge_handle>& HEMesh<V>::borders() const {
	return _borders;
}
*/


// ----------------------------------------------------------------------- //


template<typename V>
inline void HEMesh<V>::check_validity() const {
	//std::cout << "======================================================\n";

	for (face_handle f : faces()) {
    if(is_removed_face(f))
      continue;  

		hedge_handle e = hedge(f);

    do {
			if (is_removed_hedge(e)) {
				std::cout << "Invalid hedge " << e << " with begin of face " << f << "\n";
				break;
			}
			else if (face(e) != f) {
        std::cout << "Hedge " << e << " points to another face " << face(e) << " rather than its face " << f << "\n";

        for(face_handle other_face : faces())
          if (hedge(other_face) == e)
            std::cout << "Dublicated face hedge " << e << " in face " << f << "\n";

        for(face_handle other_face : borders())
          if (hedge(other_face) == e)
            std::cout << "Dublicated border hedge " << e << " in face " << f << "\n";
			 
      }

			e = next(e);
		} while (e != hedge(f));
	}


	for (face_handle b : borders()) {
    if(is_removed_face(b))
      continue;

		hedge_handle e = hedge(b);
	
    do {
			if (is_removed_hedge(e)) {
				std::cout << "Invalid hedge " << e << " at border " << b << "\n";
				break;
			}
			else if (face(e) != b) {
        std::cout << "Hedge " << e << " points to another face " << face(e) << " rather than its border " << b << "\n";

        for(face_handle other_face : faces())
          if (hedge(other_face) == e)
            std::cout << "Dublicated face hedge " << e << " in border " << b << "\n";

        for(face_handle other_face : borders())
          if (hedge(other_face) == e)
            std::cout << "Dublicated border hedge " << e << " in border " << b << "\n";
			 
      }
			e = next(e);
		} while (e != hedge(b));
	}


	for (vert_handle vi : verts()) {
		hedge_handle e = _vert_to_hedge[vi];

		if (e == hedge_handle() || e == ISOLATED_INDEX)
			continue;

		if (is_removed_hedge(e))
			std::cout << "Tail vert " << vi << " points to removed hedge " << e << "\n";
		else if (tail(e) != vi)
			std::cout << "Tail vert " << vi << " points to outgoing hedge " << e << ", but its tail is " << tail(e) << "\n";

		e = twin(e);
		hedge_handle beg = e;

		do {
			if (head(e) != vi)
				//std::cout << "Vert " << vi << " has an incident ingoing hedge " << e << ", but the hedge points to a head vertex " << head(e) << "\n";
				std::cout << "Head vert " << vi << " points to ingoing hedge " << e << ", but its head is " << head(e) << "\n";

			e = twin(next(e));
		} while (e != beg);
	}

	// Check if it points to a valid face or border index
	for (hedge_handle e : hedges()) {
		if (is_removed_hedge(e)) {
		//if (_hedges[e] == HalfEdge())
		
			// Edges are managed as pairs of half-edges
			// so if a half-edge is removed, its twin should also be marked as removed
			if (!is_removed_hedge(twin(e)))
				std::cout << "Hedge " << e << " is invalidated, but its twin is not\n";

			if (_removed_edges.find(ltwin(e)) == _removed_edges.end())
				std::cout << "Hedge " << e << " is invalidated, but is not in the garbage list of removed hedges\n";
			
			continue;
		}

		if (head(twin(e)) != head(prev(e)))
			//std::cout << "Hedge " << e << "'s tail vertex is inconsistent\n";
			std::cout << "The twin and prev hedges of hedge " << e << " , should have the same head vertex\n";

    

		if (e == next(e))
			std::cout << "Infinite loop: hedge " << e << "'s next hedge is itself\n";

		if (head(e) == tail(e))
			std::cout << "Degrade edge " << e << "(its head and tail vertices are the same)\n";

		if (next(next(e)) == e)
			std::cout << "Degrade face " << face(e) << "\n";

		if (outgoing(head(e)) == ISOLATED_INDEX)
			std::cout << "Hedge " << e << " points to a head vertex " << head(e) << " , which is marked as isolated\n";
		else if (outgoing(head(e)) == hedge_handle(hedge_handle::INVALID_INDEX))
			std::cout << "Hedge " << e << " points to a head vertex " << head(e) << " , which is marked as removed\n";
		else if (head(e) != tail(e)) { // if not degrade (because a degrade edge will always fail this test)

			hedge_handle head_hedge = twin(outgoing(head(e)));
			hedge_handle ee = head_hedge;
			bool isValid = false;

			do {
				if (ee == e)
					isValid = true;

				ee = twin(next(ee));
			} while (ee != head_hedge);

			if (!isValid)
				std::cout << "Hedge " << e << " points to an adjacent vertex from which you cannot find adjacency with the hedge\n";
		}
	}

	for (edge_handle e : _removed_edges) {
		hedge_handle he(e);

    if (he % 2 != 0)
			std::cout << "Only even half-edge indexes should be in the garbage set of half-edges !\n";
		
		//if (_hedges[he] != HalfEdge() || _hedges[he + 1] != HalfEdge())
		if (!is_removed_hedge(he) || !is_removed_hedge(twin(he)))
			std::cout << "There is an pair of hedges " << he << " and " << he + 1 << " which are garbaged, but not invalidated\n";
	
	}
		
}



// ------------------------------------------------------------------------ //


template<typename V>
inline void HEMesh<V>::clear() {
	_vertices.clear();
	_vert_to_hedge.clear();
	_hedges.clear();
	_faces_and_borders.clear();

	_removed_verts.clear();
	_removed_edges.clear();
  _removed_faces.clear();

  _faces_cap = 0;
}


template<typename V>
inline void HEMesh<V>::shrink_verts() {
	for (auto it = _removed_verts.rbegin(); it != _removed_verts.rend(); ++it)
		free_vert(*it);

	_removed_verts.clear();
}

template<typename V>
inline void HEMesh<V>::shrink_edges() {
	for (auto it = _removed_edges.rbegin(); it != _removed_edges.rend(); ++it)
		free_edge(*it);

	_removed_edges.clear();
}


template<typename V>
inline void HEMesh<V>::shrink_loops() {
	for (auto it = _removed_faces.rbegin(); it != _removed_faces.rend(); ++it)
		free_loop(*it);

	_removed_faces.clear();
}


template<typename V>
inline void HEMesh<V>::shrink() {
	//std::cout << "removed verts: " << _removed_verts.size() << "\n";
	//std::cout << "removed edges: " << _removed_edges.size() << "\n";
  //std::cout << "removed faces or borders: " << _removed_faces.size() << "\n";

	shrink_verts();
	shrink_edges();
  shrink_loops();

}


template<typename V>
inline bool HEMesh<V>::empty() const {
	//return _faces.empty() && _borders.empty()
	return _vertices.size() == _removed_verts.size()
		&& _hedges.size() == (_removed_edges.size() << 1ULL);
  //  && _faces_and_borders.size() == _removed_faces.size();
	//	&& _hedges.size() == 2ULL * _removed_edges.size();
}


template<typename V>
inline bool HEMesh<V>::are_verts_shrinked() const {
	return _removed_verts.empty();
}

template<typename V>
inline bool HEMesh<V>::are_edges_shrinked() const {
	return _removed_edges.empty();
}

template<typename V>
inline bool HEMesh<V>::are_faces_shrinked() const {
	return _removed_faces.empty();
}


template<typename V>
inline bool HEMesh<V>::is_shrinked() const {
	return _removed_verts.empty() && _removed_edges.empty() && _removed_faces.empty();
}


template<typename V>
inline uint HEMesh<V>::verts_size() const {
//	return _vertices.size() - _removed_vertices.size();
	return _vertices.size();
}

template<typename V>
inline uint HEMesh<V>::hedges_size() const {
//	return _hedges.size() - _removed_edges.size() * 2ULL;
	return _hedges.size();
}

template<typename V>
inline uint HEMesh<V>::indices_size() const {
	return _hedges.size();

//  uint n = 0;
//  for(face_handle b : borders())
//    n += face_size(b);
//  return _hedges.size() - n; 
}

template<typename V>
inline uint HEMesh<V>::edges_size() const {
	//return _hedges.size() / 2ULL - _removed_hedges.size();
	return uint(_hedges.size()) >> 1;
}

template<typename V>
inline uint HEMesh<V>::faces_size() const {
	return _faces_cap;
}

template<typename V>
inline uint HEMesh<V>::borders_size() const {
	return _faces_and_borders.size() - _faces_cap;
}


template<typename V>
inline void HEMesh<V>::reserve_verts(const uint capacity) {
	_vertices.reserve(capacity);
}

template<typename V>
inline void HEMesh<V>::reserve_hedges(const uint capacity) {
	_hedges.reserve(capacity);
}

template<typename V>
inline void HEMesh<V>::reserve_edges(const uint capacity) {
	_hedges.reserve(capacity << 1);
}

template<typename V>
inline void HEMesh<V>::reserve_loops(const uint capacity) {
	_faces_and_borders.reserve(capacity);
}

/*template<typename V>
inline void HEMesh<V>::reserve_faces(const uint capacity) {
	uint cap = capacity + borders_size();
  _faces_and_borders.reserve(cap);
}

template<typename V>
inline void HEMesh<V>::reserve_borders(const uint capacity) {
	uint cap = capacity + faces_size();
  _faces_and_borders.reserve(cap);
}*/


template <typename V>
inline Iterable<typename HEMesh<V>::vert_handle> HEMesh<V>::verts() const {
  return Iterable(vert_handle(0), vert_handle(_vertices.size()));
}


template <typename V>
inline Iterable<typename HEMesh<V>::edge_handle> HEMesh<V>::edges() const {
  return Iterable(edge_handle(0), edge_handle(edges_size()));
}


template <typename V>
inline Iterable<typename HEMesh<V>::hedge_handle> HEMesh<V>::hedges() const {
  return Iterable(hedge_handle(0), hedge_handle(_hedges.size()));
}


template <typename V>
inline Iterable<typename HEMesh<V>::face_handle> HEMesh<V>::faces() const {
  return Iterable(face_handle(0), face_handle(_faces_cap));
}


template <typename V>
inline Iterable<typename HEMesh<V>::face_handle> HEMesh<V>::borders() const {
  return Iterable(face_handle(_faces_cap), face_handle(_faces_and_borders.size()));
}



// ----------------------------------------------------------- //


template <typename V>
inline V& HEMesh<V>::vert(const vert_handle vert_id) {
	check_vert_id(vert_id);
  return _vertices[vert_id];
}

template <typename V>
inline const V& HEMesh<V>::vert(const vert_handle vert_id) const {
  check_vert_id(vert_id);
  return _vertices[vert_id];
}


template<typename V>
inline HEMesh<V>::vert_handle HEMesh<V>::head(const hedge_handle hedge_id) const {
	return _hedges[hedge_id].head_id;
}

template<typename V>
inline HEMesh<V>::vert_handle HEMesh<V>::tail(const hedge_handle hedge_id) const {
	return _hedges[twin(hedge_id)].head_id;
}

template<typename V>
inline HEMesh<V>::hedge_handle HEMesh<V>::outgoing(const vert_handle vert_id) const {
	return _vert_to_hedge[vert_id];
}

template<typename V>
inline HEMesh<V>::hedge_handle HEMesh<V>::ingoing(const vert_handle vert_id) const {
	hedge_handle he = _vert_to_hedge[vert_id];

	//if (!is_in_range(he))
	if (he != hedge_handle::INVALID_INDEX && he != ISOLATED_INDEX)
    he = twin(he);

	return he;
}

template<typename V>
inline HEMesh<V>::hedge_handle HEMesh<V>::next(const hedge_handle hedge_id) const {
	return _hedges[hedge_id].next_id;
}

template<typename V>
inline HEMesh<V>::hedge_handle HEMesh<V>::prev(const hedge_handle hedge_id) const {
	hedge_handle e = hedge_id;

	while (_hedges[e].next_id != hedge_id)
		e = _hedges[e].next_id;

	return e;
}

template<typename V>
inline HEMesh<V>::hedge_handle HEMesh<V>::twin(const hedge_handle hedge_id) const {
	//uint j = hedge_id % 2;
	//return hedge_id - j + (j + 1) % 2;
	return hedge_handle(hedge_id ^ 1U);
}


template <typename V>
inline HEMesh<V>::hedge_handle HEMesh<V>::hedge(const edge_handle edge_id) const {
	//return hedge_handle(edge_id << 1);
	return hedge_handle(edge_id);
}

template <typename V>
inline HEMesh<V>::hedge_handle HEMesh<V>::hedge(const face_handle face_id) const {
	return _faces_and_borders[face_id].begin_id;
}

template <typename V>
inline HEMesh<V>::edge_handle HEMesh<V>::edge(const hedge_handle hedge_id) const {
	//return edge_handle(hedge_id >> 1);
	return edge_handle(hedge_id);
}

template <typename V>
inline HEMesh<V>::face_handle HEMesh<V>::face(const hedge_handle hedge_id) const {
	//return edge_handle(hedge_id >> 1);
	return _hedges[hedge_id].face_id;
}


template<typename V>
inline HEMesh<V>::hedge_handle HEMesh<V>::hedge(const vert_handle tail_vert_id, const vert_handle head_vert_id) const {
	hedge_handle et = _vert_to_hedge[tail_vert_id];
	hedge_handle eh = _vert_to_hedge[head_vert_id];

	if (et != ISOLATED_INDEX && eh != ISOLATED_INDEX) {
		hedge_handle e = et;

		do {
			if (head(e) == head_vert_id)
				return e;

			e = next(twin(e));
		} while (e != et);
	}

	return ISOLATED_INDEX;
}

template<typename V>
inline HEMesh<V>::edge_handle HEMesh<V>::edge(const vert_handle tail_vert_id, const vert_handle head_vert_id) const {
  //return edge(hedge(tail_vert_id, head_vert_id));
  hedge_handle he = hedge(tail_vert_id, head_vert_id);
  
  if (he == ISOLATED_INDEX)
    return ISOLATED_INDEX;

  return edge_handle(he);
}

template<typename V>
inline HEMesh<V>::face_handle HEMesh<V>::face(const vert_handle vert_id1, const vert_handle vert_id2) const {
	const hedge_handle& beg1 = _vert_to_hedge[vert_id1];
	const hedge_handle& beg2 = _vert_to_hedge[vert_id2];

  // If one of the vertices is isolated, then there is
  //  no way they share the same face or border face
  if (beg1 == ISOLATED_INDEX || beg2 == ISOLATED_INDEX)
//    return face_handle::INVALID_INDEX;
    return face_handle();

	hedge_handle he1 = beg1;
	hedge_handle he2 = beg2;

	do {
		do {

			if (face(he1) == face(he2))
				return face(he1);

			he2 = next(twin(he2));
		} while (he2 != beg2);

		he1 = next(twin(he1));
	} while (he1 != beg1);

//	return face_handle::INVALID_INDEX;
  return face_handle();
}

//template<typename V>
//inline const uint& HEMesh<V>::hedge(const uint& tail_vert_id) const {
//	return _vert_to_hedge[tail_vert_id];
//}






template<typename V>
inline bool HEMesh<V>::is_removed_vert(const vert_handle vert_id) const {
	return _vert_to_hedge[vert_id] == hedge_handle::INVALID_INDEX;
}

template<typename V>
inline bool HEMesh<V>::is_removed_hedge(const hedge_handle hedge_id) const {
	return _hedges[hedge_id].next_id == hedge_handle::INVALID_INDEX;
}

template<typename V>
inline bool HEMesh<V>::is_removed_edge(const edge_handle edge_id) const {
	return _hedges[uint(edge_id) << 1].next_id == hedge_handle::INVALID_INDEX;
	//return _hedges[hedge(edge_id)].next_id == HE_INVALID_INDEX;
	//return _hedges[hedge_id] == HalfEdge();
}


template <typename V>
inline bool HEMesh<V>::is_removed_face(const face_handle face_id) const {
  //return _faces_and_borders[face_id].begin_id == hedge_handle::INVALID_INDEX;
  return face_id == face_handle::INVALID_INDEX;
}


//template<typename V>
//inline bool HEMesh<V>::is_begin_hedge(const hedge_handle hedge_id) const {
//  return _faces_and_borders[hedge_id].begin_id == hedge_id;	
//}


template <typename V>
inline bool HEMesh<V>::is_border_face(const face_handle face_id) const {
  check_face_or_border_id(face_id);
  return face_id >= _faces_cap;
}


template<typename V>
inline bool HEMesh<V>::is_border_hedge(const hedge_handle hedge_id) const {
	check_hedge_id(hedge_id);
  return _hedges[hedge_id].face_id >= _faces_cap;
}

template<typename V>
inline bool HEMesh<V>::is_border_edge(const edge_handle edge_id) const {
	hedge_handle hedge_id(edge_id);
	check_hedge_id(hedge_id);
	return is_border_hedge(hedge_id) || is_border_hedge(twin(hedge_id));
}

template<typename V>
inline bool HEMesh<V>::is_border_vert(const vert_handle vert_id) const {
	check_vert_id(vert_id);
	hedge_handle begin_id = _vert_to_hedge[vert_id];
	hedge_handle e = begin_id;

	do {

		//if (is_border_edge(e))
		if (is_border_hedge(e))
			return true;

		e = next(twin(e));
	} while (e != begin_id);

	return false;
}


template<typename V>
inline bool HEMesh<V>::is_isolated_vert(const vert_handle vert_id) const {
	check_vert_id(vert_id);
	return _vert_to_hedge[vert_id] == ISOLATED_INDEX;
}

template<typename V>
inline bool HEMesh<V>::is_isolated_edge(const edge_handle edge_id) const {
	hedge_handle hedge_id = hedge(edge_id);
	check_hedge_id(hedge_id);
	hedge_handle t = twin(hedge_id);
	return next(hedge_id) == t && next(t) == hedge_id;
}

template<typename V>
inline bool HEMesh<V>::is_isolated_loop(const hedge_handle hedge_id) const {
	check_hedge_id(hedge_id);

	face_handle f = face(hedge_id);
	
  hedge_handle t = twin(hedge_id);
	hedge_handle e = t;

	for (hedge_handle e = next(t); e != t; e = next(e))
		if (face(twin(e)) != f)
			return false;

	return true;
}


// ---------------------------------


template<typename V>
inline bool HEMesh<V>::is_face_size(const face_handle face_id, const uint n) const {
//inline bool HEMesh<V>::is_face_size(const hedge_handle hedge_id, const uint n) const {
	//check_hedge_id(hedge_id);
  check_face_or_border_id(face_id);
	hedge_handle hedge_id = hedge(face_id);
  hedge_handle e = hedge_id;

	for (uint i = 0; i < n; ++i)
		e = next(e);

	return e == hedge_id;
}

/*template<typename V>
inline bool HEMesh<V>::is_triangulated() const {
	for (const hedge_handle& f : _faces)
		if (!is_face_size(f, 3))
			return false;

	return true;
}*/


template<typename V>
inline uint HEMesh<V>::face_size(const face_handle face_id) const {
//inline uint HEMesh<V>::face_size(const hedge_handle hedge_id) const {
	//check_hedge_id(hedge_id);
	check_face_or_border_id(face_id);
  hedge_handle hedge_id = hedge(face_id);
  hedge_handle e = hedge_id;
	uint n = 0;

	do {
		++n;
		e = next(e);
	} while (e != hedge_id);

	return n;
}

template<typename V>
inline uint HEMesh<V>::valence(const vert_handle vert_id) const {
	check_vert_id(vert_id);

	const hedge_handle& beg = _vert_to_hedge[vert_id];
	if (beg == ISOLATED_INDEX)
		return 0;

	hedge_handle e = beg;
	uint n = 0;

	do {
		++n;
		e = next(twin(e));
	} while (e != beg);

	return n;
}

template<typename V>
inline bool HEMesh<V>::is_extremal(const vert_handle vert_id) const {
	check_vert_id(vert_id);
	const hedge_handle& he = _vert_to_hedge[vert_id];

	if (he == ISOLATED_INDEX)
		return false;

	return next(twin(he)) == he;
}


// ------------------------------------------------------------------- //


template<typename V>
inline void HEMesh<V>::invalidate_vert(const vert_handle vert_id) {
	_vert_to_hedge[vert_id] = hedge_handle(hedge_handle::INVALID_INDEX);
	_removed_verts.insert(vert_id);
}

template<typename V>
inline void HEMesh<V>::invalidate_edge(const edge_handle edge_id) {
	hedge_handle he(edge_id);

  _hedges[he] = HalfEdge();
	_hedges[he + 1] = HalfEdge();
  _removed_edges.insert(edge_id);
}

template<typename V>
inline void HEMesh<V>::invalidate_loop(const face_handle face_id) {
  //_faces_and_borders[face_id].begin_id = hedge_handle::INVALID_INDEX;
  _faces_and_borders[face_id].begin_id = hedge_handle();
  _removed_faces.insert(face_id);
}


/*template <typename V>
inline void HEMesh<V>::free_vert(const vert_handle vert_id) {

}

template <typename V>
inline void HEMesh<V>::free_edge(const edge_handle edge_id) {

}

template <typename V>
inline void HEMesh<V>::free_loop(const face_handle face_id) {

}*/


template <typename V>
inline bool HEMesh<V>::is_in_range_vert(const vert_handle vert_id) const {
  return vert_id >= 0 && vert_id < _vertices.size();
}

template <typename V>
inline bool HEMesh<V>::is_in_range_hedge(const hedge_handle hedge_id) const {
  return hedge_id >= 0 && hedge_id < _hedges.size();
}

//template<typename V>
//inline bool HEMesh<V>::is_in_range_edge(const edge_handle edge_id) const {
//	return 0 <= edge_id && edge_id < edges_size();
//}

template <typename V>
inline bool HEMesh<V>::is_in_range_loop(const face_handle face_id) const {
  return face_id >= 0 && face_id < _faces_and_borders.size();
}

/*template <typename V>
inline bool HEMesh<V>::is_in_range_face(const face_handle face_id) const {
  return face_id >= 0 && face_id < _faces_cap;
}

template <typename V>
inline bool HEMesh<V>::is_in_range_border(const face_handle face_id) const {
  return face_id >= _faces_cap && face_id < _faces_and_borders.size();
}*/


template<typename V>
inline void HEMesh<V>::check_vert_id(const vert_handle vert_id) const {
	if (!is_in_range_vert(vert_id))
		throw std::out_of_range("Vertex index out of range");
	if (is_removed_vert(vert_id))
		throw std::invalid_argument("Removed vertex");
}

template<typename V>
inline void HEMesh<V>::check_hedge_id(const hedge_handle hedge_id) const {
	if (!is_in_range_hedge(hedge_id))
		throw std::out_of_range("Half edge index out of range");
	if (is_removed_hedge(hedge_id))
		throw std::invalid_argument("Removed half edge");
}

template<typename V>
inline void HEMesh<V>::check_face_or_border_id(const face_handle face_id) const {
	if (!is_in_range_loop(face_id))
		throw std::out_of_range("Half edge index out of range");
	if (is_removed_face(face_id))
		throw std::invalid_argument("Removed half edge");
}

/*template<typename V>
inline void HEMesh<V>::check_face_id(const face_handle face_id) const {
	if (!is_in_range_face(face_id))
		throw std::out_of_range("Half edge index out of range");
	if (is_removed_face(face_id))
		throw std::invalid_argument("Removed half edge");
}

template<typename V>
inline void HEMesh<V>::check_border_id(const face_handle border_id) const {
	if (!is_in_range_border(border_id))
		throw std::out_of_range("Half edge index out of range");
	if (is_removed_face(border_id))
		throw std::invalid_argument("Removed half edge");
}*/


template<typename V>
inline bool HEMesh<V>::are_same_loop(const hedge_handle hedge_id1, const hedge_handle hedge_id2) const {
	check_hedge_id(hedge_id1);
	check_hedge_id(hedge_id2);
	hedge_handle e = hedge_id1;

	do {
		if (e == hedge_id2)
			return true;

		e = next(e);
	} while (e != hedge_id1);

	return false;
}


template<typename V>
inline void HEMesh<V>::shift_begin(const face_handle face_id, const uint how_much) {
	check_face_or_border_id(face_id);

  hedge_handle nc = _faces_and_borders[face_id].begin_id;
  for (uint i = 0; i < how_much; ++i)
    nc = next(nc);

  _faces_and_borders[face_id].begin_id = nc;
}

template<typename V>
inline void HEMesh<V>::set_loop_id(const hedge_handle hedge_id, const face_handle face_id) {
  hedge_handle e = hedge_id;
  
  //while (_hedges[e].face_id != face_id) {
  do {
		_hedges[e].face_id = face_id;

		e = next(e);
	} while (e != hedge_id);

  //_faces_and_borders[face_id].begin_id = hedge_id;
} 

template<typename V>
inline void HEMesh<V>::set_vert_id(const hedge_handle hedge_id, const vert_handle new_head_vert_id) {
	hedge_handle beg = hedge_id;	// beggining
	hedge_handle e = beg;

	do {
		_hedges[e].head_id = new_head_vert_id;
		e = twin(next(e));
	} while (e != beg);

	//for (uint e = hedge_id; head(e) != new_head_vert_id; e = twin(next(e)))
	//	_hedges[e].head_id = new_head_vert_id;

//	_vert_to_hedge[new_head_vert_id] = next(hedge_id);	// should this method also update the _vert_to_hedge vector ??
}

template<typename V>
inline HEMesh<V>::face_handle HEMesh<V>::find_common_face_around_head(HEMesh<V>::hedge_handle& he1, HEMesh<V>::hedge_handle& he2) {

  face_handle f;
	hedge_handle hedge1 = he1;
	hedge_handle hedge2 = he2;

//	face_handle b = face_handle::INVALID_INDEX;
  face_handle b;
  hedge_handle be1 = he1;
	hedge_handle be2 = he2;

	// brute force
	do {
		do {
			f = face(he1);

			if (f == face(he2)) {
				if (!is_border_face(f))
					return f;

				b = f;
				be1 = he1;
				be2 = he2;
			}

			he1 = twin(next(he1));	// rotates around the head vertex
		} while (he1 != hedge1);

		he2 = twin(next(he2));	// rotates around the head vertex
	} while (he2 != hedge2);

	he1 = be1;
	he2 = be2;

	return b;
}

template<typename V>
inline HEMesh<V>::face_handle HEMesh<V>::find_border_around_head(hedge_handle& hedge_id) {
	hedge_handle begin_id = hedge_id;

	do {
		face_handle b = face(hedge_id);
		if (is_border_face(b))
			return b;

		hedge_id = twin(next(hedge_id));
	} while (hedge_id != begin_id);

//	return face_handle::INVALID_INDEX;
  return face_handle();
}

template<typename V>
inline HEMesh<V>::hedge_handle HEMesh<V>::ltwin(const hedge_handle hedge_id) const {
	//return (hedge_id / 2) * 2;
	//return hedge_handle((hedge_id >> 1) << 1); // make the number even
	return hedge_handle(uint(hedge_id) & ~1);
}

template<typename V>
inline HEMesh<V>::hedge_handle HEMesh<V>::rtwin(const hedge_handle hedge_id) const {
	//return hedge_handle(((hedge_id >> 1) << 1) + 1); // make the number odd
	return hedge_handle(uint(hedge_id) | 1); // make the least significant bit equal 1
}


template<typename V>
inline HEMesh<V>::hedge_handle HEMesh<V>::new_edge() {
	hedge_handle ne;		// new half-edge index

	if (_removed_edges.empty()) {
		ne = hedges_size();
		_hedges.push_back(HalfEdge());
		_hedges.push_back(HalfEdge());
	}
	else {
		auto front_iter = _removed_edges.begin();
		ne = *front_iter;

		//if (ne % 2 != 0 || _hedges[ne] != HalfEdge() || _hedges[ne + 1] != HalfEdge())
		//	throw std::exception("There is an edge that is garbaged, but not invalidated");

		_removed_edges.erase(front_iter);
	}

	return ne;
}

template<typename V>
inline HEMesh<V>::hedge_handle HEMesh<V>::new_edge(const vert_handle tail_id, const vert_handle head_id) {
	hedge_handle ne;		// new half-edge index
	hedge_handle net;	// new twin half-edge index

  // Should this method create a new face ??

//  _hedges[ne] = HalfEdge(head_id, net, face_handle::INVALID_INDEX);
//  _hedges[net] = HalfEdge(tail_id, ne, face_handle::INVALID_INDEX);	
  _hedges[ne] = HalfEdge(head_id, net, face_handle());
  _hedges[net] = HalfEdge(tail_id, ne, face_handle());	

	return ne;
}




// ------------------------------------------------------------------------ //



template <typename V>
//inline HEMesh<V>::HEMesh(const NgonMesh<V, pc::PointVec<V>>& m) {
template <typename MeshType, typename>
inline HEMesh<V>::HEMesh(const MeshType& m) {
//template <typename V>
//template <typename MeshType, typename>
//inline HEMesh<V>::HEMesh(const MeshType& m) {

  _vertices = m; // NgonMesh<V, PointVec> -> PointVec<V> -> std::vector<V>
  //_vertices.insert(other_verts.begin(), other_verts.end());

  _vert_to_hedge.assign(_vertices.size(), ISOLATED_INDEX);
	_faces_and_borders.reserve(m.faces_size());
	_hedges.reserve(m.indices_size());
	//_hedges.assign(m.indices.size(), HalfEdge());
	//_hedges.resize(indices.size());

	std::unordered_map<std::pair<uint, uint>, hedge_handle> vert_pair_to_hedge;

	//uint found_edges = 0U;

	hedge_handle curr_hedge_id;
	hedge_handle prev_hedge_id;
	hedge_handle curr_begin_id;
  face_handle curr_face_id;

  for (face_handle fh : m.faces()) {
		//_faces.push_back(i);
  
    auto face_verts = m.verts(fh);
    curr_face_id = (face_handle)_faces_and_borders.size();
    //curr_face_id = _faces_cap++;

    auto it = face_verts.begin();

    do { 
      auto nit = it;
      ++nit;

      if (nit == face_verts.end())
        nit = face_verts.begin();

      uint id0 = *it;
      uint id1 = *nit;     
     
     	if (vert_pair_to_hedge.find({ id0, id1 }) != vert_pair_to_hedge.end())
				throw std::invalid_argument("Trying to convert a Non-Consistently-Orientated or Non-2-Manifold mesh to a half edge data structure !");

			auto mp = vert_pair_to_hedge.find({ id1, id0 });
			if (mp == vert_pair_to_hedge.end()) {
				//curr_hedge_id = 2ULL * found_edges;
				//++found_edges;

				curr_hedge_id = hedge_handle(_hedges.size());
				_hedges.push_back(HalfEdge());
				_hedges.push_back(HalfEdge());

				vert_pair_to_hedge[{id0, id1}] = curr_hedge_id;
			}
			else {
				curr_hedge_id = hedge_handle(mp->second + 1ULL);
				vert_pair_to_hedge.erase(mp); // erase, so later we are left only with the border half edges
			}

			_vert_to_hedge[id0] = curr_hedge_id;

//	    if (j != 0ULL)
      if (it != face_verts.begin())
        _hedges[prev_hedge_id].next_id = curr_hedge_id;
      else
				curr_begin_id = curr_hedge_id;

			prev_hedge_id = curr_hedge_id;

			_hedges[curr_hedge_id].face_id = curr_face_id;

			//_hedges[curr_hedge_id] = HalfEdge(id1);
			_hedges[curr_hedge_id].head_id = vert_handle(id1);

      it = nit;
		} while (it != face_verts.begin());

		_faces_and_borders.push_back(Face(curr_begin_id));
		_hedges[prev_hedge_id].next_id = curr_begin_id;

	}

  _faces_cap = _faces_and_borders.size();

	// Assign Borders ========================================================== //

	std::unordered_set<hedge_handle> inner_border_hedges;
	for (const auto& it : vert_pair_to_hedge)
		inner_border_hedges.insert(it.second);

	// Traverses the boundary inner HalfEdges, to create the outer HalfEdges
	while (!inner_border_hedges.empty()) {
		hedge_handle begin_he = *inner_border_hedges.begin();   // inner edge border begin
		hedge_handle e = begin_he;								// inner border edge
		hedge_handle prev = hedge_handle(hedge_handle::INVALID_INDEX);                // previous inner border edge

		do {

			inner_border_hedges.erase(inner_border_hedges.find(e));

			// If not first iteration
			//if (e != begin_he)
			if (prev != hedge_handle::INVALID_INDEX) {
				//_hedges[twin(e)].next_id = prev;
				_hedges[twin(e)].next_id = twin(prev);
				_hedges[twin(e)].head_id = _hedges[prev].head_id;
				//_hedges[twin(e)].begin_id = curr_begin_id;  
			  _hedges[twin(e)].face_id = curr_face_id;
      }
			else {
        curr_begin_id = twin(e);
        curr_face_id = (face_handle)_faces_and_borders.size(); // Here in the border assignment we will use the curr_face_id as curr_border_id
      }
				

			prev = e;

			// Find Next Inner Edge
			e = _hedges[e].next_id;
			while (e != begin_he) {
				//if (next(twin(e)) == HE_INVALID_INDEX)					// !!!!
				if (_hedges[twin(e)].next_id == hedge_handle::INVALID_INDEX)
					break;

				e = _hedges[twin(e)].next_id;
			}

		} while (e != begin_he);

		_hedges[twin(e)].next_id = twin(prev);
		_hedges[twin(e)].head_id = _hedges[prev].head_id;
		//_hedges[twin(e)].begin_id = curr_begin_id;
    _hedges[twin(e)].face_id = curr_face_id;

		_faces_and_borders.push_back(Face(curr_begin_id));
	}

}


template <typename V>
//inline HEMesh<V>::operator NgonMesh<V, pc::PointVec<V>>() const {
template <typename MeshType, typename> 
inline HEMesh<V>::operator MeshType() const {
  //if (!is_shrinked())
  if (!are_verts_shrinked() || !are_faces_shrinked())
    throw std::invalid_argument("Shrink the HEMesh's vertices and face loops before converting to another Mesh Type");

	//NgonMesh<V, pc::PointVec<V>> m(_vertices);
	MeshType m(_vertices);
  //m.indices.reserve(_hedges.size());
  m.reserve_indices(_hedges.size());

  hedge_handle e;
  hedge_handle begin_id;
  
  std::vector<vert_handle> curr_face_indices;

	for (face_handle fh : faces()) {
    //he = hedge(f);
    begin_id = _faces_and_borders[fh].begin_id;

//    if (is_face_size(begin_id, 2))
//			continue;
    
    curr_face_indices.clear();

    e = begin_id;
    do {
      curr_face_indices.push_back(head(e));

      e = next(e);
    } while (e != begin_id);

    m.add_face(curr_face_indices);
	}

	return m;
}


/*
template <typename V>
inline HEMesh<V>::operator Mesh<V, 3U, pc::PointVec<V>>() const {
  
		if (!are_verts_shrinked() || !are_faces_shrinked())
			throw std::invalid_argument("Cannot convert a Half-Edge HEMesh to a Triangular HEMesh, if the vertices and faces are not shrinked");

	Mesh<V, 3U, pc::PointVec<V>> m = pc::PointVec<V>(_vertices);
	//m.indices.reserve(_hedges.size());
  m.reserve_indices(_hedges.size());

	hedge_handle he[3];

	for (face_handle fh : faces()) {
		if (is_face_size(fh, 2))
			continue;
 
    he[0] = hedge(fh);
		he[1] = next(he[0]);
		he[2] = next(he[1]);

		do {
			for (uint i = 0; i < 3; ++i)
				m.indices.push_back(_hedges[he[i]].head_id);

			he[1] = he[2];
			he[2] = next(he[2]);

		} while (he[2] != he[0]);
	}

	return m;
}
*/





} // namespace he


} // namespace mesh

