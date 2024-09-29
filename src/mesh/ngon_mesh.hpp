#pragma once

#include "mesh.hpp"
#include "base_mesh.hpp"
#include "../point_cloud/point_vec.hpp"

#include <vector>
#include <array>
#include <initializer_list>
#include <iterator>


// THIS WILL NOT INHERIT FROM BASE MESH !!!


// This class will support file handling and every other mesh class should be able to
// convert from/to it, if they dont want to implement their own file handling

namespace mesh {


template <
	typename V,							 // Vertex Type
	typename PointCloud = pc::PointVec<V>  // Vertex PointCloud
>
class NgonMesh : public BaseMesh<V, PointCloud> {
public:

	// Inherit the constructors
	
  NgonMesh(const PointCloud& verts = PointCloud())
    : BaseMesh<V, PointCloud>::BaseMesh(verts)
    , _faces() {}


  
  using typename BaseMesh<V, PointCloud>::vert_handle;
  class face_handle;
  
  using BaseMesh<V, PointCloud>::verts_size;
  uint face_size(const face_handle face) const;
  uint faces_size(const face_handle face) const;


  using BaseMesh<V, PointCloud>::add_vert;

  // @todo: use SFINAE (enable_if its iterator tag is a base of std::iterator) 
  template <typename IterType>
	face_handle add_face(const IterType& begin_it, const IterType& end_it);

	face_handle add_face(const std::vector<uint>& new_indices);
  
	//template <size_t L>
	//uint add_face(const std::array<uint, L>& new_indices);
  

  using BaseMesh<V, PointCloud>::verts;
  
  Iterable<typename std::vector<vert_handle>::iterator> verts(const face_handle face) const;

	
  class face_handle {
  protected:
    int id;

  public:
    face_handle(const int id) : id(id) {}
    
    operator int() const { return id; }
    operator int&() { return id; }


  };
	


protected:

  // stores the number of vertices/edges in each face
  // first is the beginning(in _indices) of the face vertex indices
  // second is the size of the face (the number of adjacent vertices or edges)
	std::vector<std::pair<uint, uint>> _faces;

};


template <typename V, typename PointCloud>
inline uint NgonMesh<V, PointCloud>::face_size(
  const typename NgonMesh<V, PointCloud>::face_handle face) const {

  return _faces[face].second;
}


template <typename V, typename PointCloud>
inline uint NgonMesh<V, PointCloud>::faces_size(
  const typename NgonMesh<V, PointCloud>::face_handle face) const {
  return _faces.size();
}


template <typename V, typename PointCloud>
inline typename NgonMesh<V, PointCloud>::face_handle
  NgonMesh<V, PointCloud>::add_face(const std::vector<uint>& new_indices) {
	
  uint f = _faces.size();
  
	_faces.push_back({ this->_indices.size(), new_indices.size() });

  this->_indices.assign(this->_indices.end(), new_indices.begin(), new_indices.end());

	return f;
}


/*template <typename V, typename PointCloud>
template <size_t L>
inline uint NgonMesh<V, PointCloud>::add_face(const std::array<uint, L>& new_indices) {
	uint f = _face_sizes.size();

	face_sizes.push_back(new_indices.size());
	this->_indices.assign(this->_indices.end(), new_indices.begin(), new_indices.end());

	return f;
}*/

template<typename V, typename PointCloud>
template<typename IterType>
inline typename NgonMesh<V, PointCloud>::face_handle
  NgonMesh<V, PointCloud>::add_face(const IterType& begin_it, const IterType& end_it) {
	
  uint f = _faces.size();
  uint n = std::distance(begin_it, end_it);

	_faces.push_back({ n, this->_indices.size() });
	this->_indices.assign(this->_indices.end(), begin_it, end_it);

	return f;
}


template <typename V, typename PointCloud>
Iterable<typename std::vector<typename NgonMesh<V, PointCloud>::vert_handle>::iterator> 
  NgonMesh<V, PointCloud>::verts(const typename NgonMesh<V, PointCloud>::face_handle face) const {
  
  auto begin_id = this->_indices.begin() + _faces[face].first;
  
  return Iterable<typename std::vector<typename NgonMesh<V, PointCloud>::vert_handle>::iterator>(
    begin_id, 
    begin_id + _faces[face].second
  );
}





}
