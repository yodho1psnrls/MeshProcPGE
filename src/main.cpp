#include "engine.hpp"
#include "mesh/mesh.hpp"
#include "mesh/tri_mesh.hpp"
#include "point_cloud/point_vec.hpp"
#include "vertex/vertex.h"
// #include "../olc/olcPixelGameEngine.h"

int main() {

  Example<Vertex> demo;

  if (demo.Construct(scrW, scrH, 2, 2, false, true))
    demo.Start();


  /*
          using namespace mesh;
          using namespace olc;

    //vc::VertVec<Vertex> vertices;
    //auto vh = vertices.add_vert(vf2d(-70, 45)); // vertex handle
    //std::cout << vertices.vert(vh).pos << "\n";

    using vert_handle = typename TriMesh<Vertex>::vert_handle;
    using face_handle = typename TriMesh<Vertex>::face_handle;


    TriMesh<Vertex> m;
  //	Mesh<Vertex, 3U> m;

          m.add_vert(vf2d(100, 100));
          m.add_vert(vf2d(100, 100));
          m.add_vert(vf2d(100, 100));
          m.add_vert(vf2d(100, 100));
          m.add_vert(vf2d(100, 100));

          face_handle fh;

          m.add_face({ 0, 1, 2 });
          //m.add_face({ 2, 3, 14 });
          fh = m.add_face({ 0, 1, 2 });


          std::cout << "=============\n";
          //std::cout << m.faces_size() << "\n";

          //for (auto vi : m.verts())

    for(face_handle fh : m.faces())
      for(vert_handle vh : m.verts(fh))
                    std::cout << m.vert(vh).pos << "\n";

  */

  return 0;
}
