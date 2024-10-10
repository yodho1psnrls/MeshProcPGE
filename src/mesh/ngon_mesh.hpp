#pragma once

#include "base_mesh.hpp"
#include "../point_cloud/point_vec.hpp"
#include "handlers.h"

#include <vector>
#include <initializer_list>
#include <array>

#include <iterator>
//#include <stdexcept>
#include <type_traits>


namespace mesh {


// This class will support file handling and every other mesh class should be able to
// convert from/to it, if they dont want to implement their own file handling

template <
	typename V,							 // Vertex Type
	typename PointCloud = pc::PointVec<V>  // Vertex PointCloud
>
class NgonMesh : public BaseMesh<V, PointCloud> {
public: 

  using face_handle = index_face_handle;
  //using hedge_handle = index_hedge_handle;

//  using BaseMesh<V, PointCloud>::BaseMesh;

  using BaseMesh<V, PointCloud>::are_valid_indices;
  using BaseMesh<V, PointCloud>::is_in_range;
  using BaseMesh<V, PointCloud>::is_valid;


  NgonMesh(const PointCloud& verts = PointCloud())
    : BaseMesh<V, PointCloud>::BaseMesh(verts)
    , _faces() {}


  template <typename MeshType, typename = typename std::enable_if_t<
    std::is_base_of_v<BaseMesh<V, PointCloud>, MeshType> &&
    !std::is_same_v<MeshType, NgonMesh<V, PointCloud>>
  >>
  NgonMesh(const MeshType& mesh);


//  NgonMesh(const NgonMesh& other)
//  : _faces(other._faces),
//    BaseMesh<V, PointCloud>::BaseMesh(other) {}


//  template <uint N>
//  NgonMesh(const Mesh<V, N, PointCloud>& mesh);
  
//  template <uint N>
//  operator Mesh<V, N, PointCloud>() const;
  

  using typename BaseMesh<V, PointCloud>::vert_handle;

  
  using BaseMesh<V, PointCloud>::verts_size;
  //uint get_n_poly(const face_handle fh) const;
  uint face_size(const face_handle fh) const;
  uint faces_size() const;
  
//  bool is_triangulated() const;

  using BaseMesh<V, PointCloud>::add_vert;


  // @todo: use SFINAE (enable_if its iterator tag is a base of std::iterator) 
  template <typename IterType>
	face_handle add_face(const IterType& begin_it, const IterType& end_it);
  
  template <uint N>
  face_handle add_face(const std::array<vert_handle, N>& new_indices);
  face_handle add_face(const std::vector<vert_handle>& new_indices);
  face_handle add_face(const std::initializer_list<vert_handle>& new_indices);

	//template <size_t L>
	//uint add_face(const std::array<uint, L>& new_indices);
  

  using BaseMesh<V, PointCloud>::verts;
  Iterable<typename std::vector<vert_handle>::const_iterator> verts(const face_handle fh) const;
//  Iterable<typename std::vector<vert_handle>::iterator> verts(const face_handle fh);
  Iterable<face_handle> faces() const;	
	

protected:

  // stores the number of vertices/edges in each face
  // first is the beginning(in _indices) of the face vertex indices
  // second is the size of the face (the number of adjacent vertices or edges)
	std::vector<std::pair<uint, uint>> _faces;
  //std::vector<uint> _face_sizes;
};


// ========================================================================== //


template <typename V, typename PointCloud>
inline uint NgonMesh<V, PointCloud>::face_size(
  const typename NgonMesh<V, PointCloud>::face_handle fh) const {

  return _faces[fh].second;
}


template <typename V, typename PointCloud>
inline uint NgonMesh<V, PointCloud>::faces_size() const {
  return _faces.size();
}


template <typename V, typename PointCloud>
inline typename NgonMesh<V, PointCloud>::face_handle
  NgonMesh<V, PointCloud>::add_face(const std::vector<vert_handle>& new_indices) {
  
  return add_face(new_indices.begin(), new_indices.end());
}


template <typename V, typename PointCloud>
inline typename NgonMesh<V, PointCloud>::face_handle
	NgonMesh<V, PointCloud>::add_face(const std::initializer_list<vert_handle>& new_indices) {

  return add_face(new_indices.begin(), new_indices.end());
}


template <typename V, typename PointCloud>
template <uint N>
inline typename NgonMesh<V, PointCloud>::face_handle
  NgonMesh<V, PointCloud>::add_face(const std::array<vert_handle, N>& new_indices) {
  
  return add_face(new_indices.begin(), new_indices.end());
}



template<typename V, typename PointCloud>
template<typename IterType>
inline typename NgonMesh<V, PointCloud>::face_handle
  NgonMesh<V, PointCloud>::add_face(const IterType& begin_it, const IterType& end_it) {

  if (!are_valid_indices(begin_it, end_it))
    throw std::invalid_argument("Non-valid vertex handle in the new indices");

  face_handle fh(_faces.size());
  uint n = std::distance(begin_it, end_it);

	_faces.push_back({ this->_indices.size(), n });
	this->_indices.insert(this->_indices.end(), begin_it, end_it);

	return fh;
}


template <typename V, typename PointCloud>
Iterable<typename std::vector<typename NgonMesh<V, PointCloud>::vert_handle>::const_iterator> 
  NgonMesh<V, PointCloud>::verts(const typename NgonMesh<V, PointCloud>::face_handle fh) const {
  
  using iter = typename std::vector<typename NgonMesh<V, PointCloud>::vert_handle>::const_iterator;
  iter begin_it = this->_indices.cbegin() + _faces[fh].first;
  
  return Iterable<iter>(
    begin_it, 
    begin_it + _faces[fh].second
  );
}


/*template <typename V, typename PointCloud>
Iterable<typename std::vector<typename NgonMesh<V, PointCloud>::vert_handle>::iterator> 
  NgonMesh<V, PointCloud>::verts(const typename NgonMesh<V, PointCloud>::face_handle fh) {
  
  using iter = typename std::vector<typename NgonMesh<V, PointCloud>::vert_handle>::iterator;
  iter begin_it = this->_indices.begin() + _faces[fh].first;
  
  return Iterable<iter>(
    begin_it, 
    begin_it + _faces[fh].second
  );
}*/


template <typename V, typename PointCloud>
inline Iterable<typename NgonMesh<V, PointCloud>::face_handle> 
  NgonMesh<V, PointCloud>::faces() const {
  
  return Iterable((face_handle)0U, (face_handle)_faces.size());
}


/*template <typename V, typename PointCloud>
inline bool NgonMesh<V, PointCloud>::is_triangulated() const {
  for(const auto& pairr : _faces)
    if (pairr.second != 3U)
      return false;
  
  return true;
}*/

/*
template <typename V, typename PointCloud>
template <uint N>
inline NgonMesh<V, PointCloud>::NgonMesh(const Mesh<V, N, PointCloud>& mesh)
  : BaseMesh<V, PointCloud>(mesh) {
  
  uint n = mesh.faces_size();
  _faces.reserve(n);

  //_face_sizes.assign(mesh.faces_size(), N);
  for (uint i = 0; i < n; ++i)
    _faces.push_back({i * N, N});
    
}
*/

template <typename V, typename PointCloud>
template <typename MeshType, typename>
inline NgonMesh<V, PointCloud>::NgonMesh(const MeshType& mesh)
: BaseMesh<V, PointCloud>::BaseMesh(mesh) {

  _faces.reserve(mesh.faces_size());

  uint last_id = 0U;
  uint n;

  for(typename MeshType::face_handle fh : mesh.faces()) {
    n = mesh.face_size(fh);
    _faces.push_back({last_id, n});
    last_id += n;
  }
      
}

/*
template <typename V, typename PointCloud>
template <typename MeshType>
inline NgonMesh<V, PointCloud>::NgonMesh(const MeshType& mesh) {
  
  this->_vertices.reserve(mesh.verts_size());
  this->_indices.reserve(mesh.indices_size());
  this->_faces.reserve(mesh.faces_size());

  for(typename MeshType::vert_handle vh : mesh.verts())
    this->_vertices.push_back(mesh.vert(vh));
  
  for(typename MeshType::face_handle fh : mesh.faces()) {
    auto face_verts = mesh.verts(fh);
    add_face(face_verts.begin(), face_verts.end());
  }

}*/


/*
template <typename V, typename PointCloud>
template <uint N>
inline NgonMesh<V, PointCloud>::operator Mesh<V, N, PointCloud>() const {

  for(face_handle fh : faces())
    if(face_size(fh) != N)
      throw std::out_of_range("All faces should have a fixed size of incident vertices, that should be equal to N");
  
  //return Mesh<V, N, PointCloud>::BaseMesh(*this);
  //return Mesh<V, N, PointCloud>(*this);
  return *this;
}*/







}
