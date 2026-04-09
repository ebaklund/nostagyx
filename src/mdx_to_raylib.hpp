
#pragma once

#include <raylib.h>
#include <cstring>
#include "mdx_model.hpp"


// struct MdxModel
// {
//     std::vector<MdxAnimation> animations;
//     std::vector<std::string> link_names;
//     std::vector<MdxTransform> transforms;
//     std::vector<MdxMesh> meshes;
// };

// typedef struct Model {
//     Matrix transform;       // Local transform matrix

//     int meshCount;          // Number of meshes
//     int materialCount;      // Number of materials
//     Mesh *meshes;           // Meshes array
//     Material *materials;    // Materials array
//     int *meshMaterial;      // Mesh material number

//     // Animation data
//     ModelSkeleton skeleton; // Skeleton for animation

//     // Runtime animation data (CPU/GPU skinning)
//     ModelAnimPose currentPose; // Current animation pose (Transform[])
//     Matrix *boneMatrices;   // Bones animated transformation matrices
// } Model;


// typedef struct Mesh {
//     int vertexCount;        // Number of vertices stored in arrays
//     int triangleCount;      // Number of triangles stored (indexed or not)

//     // Vertex attributes data
//     float *vertices;        // Vertex position (XYZ - 3 components per vertex) (shader-location = 0)
//     float *texcoords;       // Vertex texture coordinates (UV - 2 components per vertex) (shader-location = 1)
//     float *texcoords2;      // Vertex texture second coordinates (UV - 2 components per vertex) (shader-location = 5)
//     float *normals;         // Vertex normals (XYZ - 3 components per vertex) (shader-location = 2)
//     float *tangents;        // Vertex tangents (XYZW - 4 components per vertex) (shader-location = 4)
//     unsigned char *colors;  // Vertex colors (RGBA - 4 components per vertex) (shader-location = 3)
//     unsigned short *indices; // Vertex indices (in case vertex data comes indexed)

//     // Skin data for animation
//     int boneCount;          // Number of bones (MAX: 256 bones)
//     unsigned char *boneIndices; // Vertex bone indices, up to 4 bones influence by vertex (skinning) (shader-location = 6)
//     float *boneWeights;     // Vertex bone weight, up to 4 bones influence by vertex (skinning) (shader-location = 7)

//     // Runtime animation vertex data (CPU skinning)
//     // NOTE: In case of GPU skinning, not used, pointers are NULL
//     float *animVertices;    // Animated vertex positions (after bones transformations)
//     float *animNormals;     // Animated normals (after bones transformations)

//     // OpenGL identifiers
//     unsigned int vaoId;     // OpenGL Vertex Array Object id
//     unsigned int *vboId;    // OpenGL Vertex Buffer Objects id (default vertex data)
// } Mesh;

namespace nyx
{
    class MdxRaylib
    {
    public:
        static int get_raylib_texcoords(const std::vector<MdxTexCoord>& mdx_texcoords, float*& raylib_texcoords) {
            raylib_texcoords = new float[mdx_texcoords.size() * 2];
            std::memcpy(raylib_texcoords, (float*)mdx_texcoords.data(), mdx_texcoords.size() * 2);

            return mdx_texcoords.size();
        }

        static int get_raylib_indices(const std::vector<MdxTriangle>& mdx_triangles, uint16_t*& raylib_indices) {
            raylib_indices = new uint16_t[mdx_triangles.size() * 3];
            uint16_t* outp = raylib_indices;

            for (const MdxTriangle& mdx_triangle : mdx_triangles)
                outp = MdxTriangle::get_indices(mdx_triangle.vertex, outp);

            return mdx_triangles.size();
        }

        static int get_raylib_vertices(const std::vector<MdxVertex>& mdx_vertices, float*& raylib_vertices) {
            raylib_vertices = new float[mdx_vertices.size() * 3];
            float* outp = raylib_vertices;

            for (const MdxVertex& mdx_vertex : mdx_vertices)
                outp = MdxVertex::get_coord_f3(mdx_vertex.coord, outp);

            return mdx_vertices.size();
        }

        static void init_single_raylib_mesh(const MdxModel& mdx_model, int i, Mesh& raylib_mesh) {
            raylib_mesh.vertexCount   = get_raylib_vertices(mdx_model.bone.meshes[i].vertices, raylib_mesh.vertices);
            raylib_mesh.triangleCount = get_raylib_indices(mdx_model.bone.meshes[i].triangles, raylib_mesh.indices);
                                        get_raylib_texcoords(mdx_model.bone.meshes[i].tex_coords, raylib_mesh.texcoords);
        }

        static int get_raylib_meshes(const MdxModel& mdx_model, Mesh*& meshes_out) {
            auto mesh_count = mdx_model.bone.meshes.size();
            meshes_out = new Mesh[mesh_count];

            for (int i = 0; i < mesh_count; ++i)
                init_single_raylib_mesh(mdx_model, i, *(meshes_out + i * sizeof(Model)));

            return mesh_count;
        }

        // Model

        static Model get_raylib_model_from(const MdxModel& mdx_model) {
            Model raylib_model;
            raylib_model.meshCount = get_raylib_meshes(mdx_model, raylib_model.meshes);

            return raylib_model;
        }
    };
}
