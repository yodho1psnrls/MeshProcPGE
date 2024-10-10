#include "engine.h"

/*
#include <iostream>
#include "utilities.hpp"
#include "point_cloud/handlers.h"
#include <vector>

#include "vertex/vertex.h"
#include "mesh/tri_mesh.hpp"
#include "mesh/ngon_mesh.hpp"
#include "mesh/he/he_mesh.hpp"
#include "mesh/generate_plane.hpp"
*/

int main() {

  Example demo;
  if (demo.Construct(scrW, scrH, 2, 2, false, true))
    demo.Start();

/*
  //using vf2d = typename olc::vf2d;
  using V = char;
  using Mesh = typename mesh::TriMesh<V, pc::PointVec<V>>;
  //using vert_handle = pc::index_vert_handle;
  using vert_handle = Mesh::vert_handle;
  using face_handle = Mesh::face_handle;
  using iter = typename std::vector<vert_handle>::const_iterator;

  using HEMesh = mesh::he::HEMesh<V>;

//  std::cout << is_const_iterator<iter>() << "\n";

  Mesh m;
  m.add_vert('a');
  m.add_vert('b');
  m.add_vert('c');
  m.add_vert('d');

//  m.add_face({0, 1, 2});
//  m.add_face({2, 1, 3});
  m.add_face({vert_handle(0), vert_handle(1), vert_handle(2)});
  m.add_face({vert_handle(2), vert_handle(1), vert_handle(3)});


//  const auto indices = m.verts(face_handle(0));
//  const Mesh& mesh = m; 
//  const V& v = mesh.vert(indices[1]);
//  std::cout << v.pos << "\n";


  HEMesh hm;
  mesh::NgonMesh<V> temp; 

  // Converting from Mesh to HEMesh seems fine
  temp = m;
  hm = temp;

  // But this doesnt show the faces right (It may be from the iterators/handlers)
  temp = hm; 
  m = temp;   // Here it doesnt copy the _indices !!

  const Mesh& mesh = m;

//  auto indices = mesh.verts(face_handle(0));
//  const V& a = mesh.vert(indices[0]);
//  const V& b = mesh.vert(indices[1]);
//  std::cout << (a.pos.x + b.pos.x) / 2.0f << "\n";

//  for(auto fh : hm.faces())
//    std::cout << hm.face_size(fh) << "\n";

  for (Mesh::face_handle fh : mesh.faces()) {
    for (Mesh::vert_handle vh : mesh.verts(fh)) {
      std::cout << mesh.vert(vh) << "\n";
    }
    std::cout << "===================\n"; 
  }   
*/



/*
  std::vector<vert_handle> face_verts;
  for (uint i = 0; i < 15; ++i)
    face_verts.push_back(i);

   
  Iterable<vert_handle> verts(0, 15);
  //Iterable<iter> verts(face_verts.begin(), face_verts.end());

//  verts[7] = 69;

  std::cout << verts[7] << "\n";

  //std::cout << verts.size() << "\n";

  //std::cout << *vert_handle(verts.begin() + 7U) << "\n";

  std::cout << "Shllqlql\n";*/


  return 0;
}
