
#pragma once

#include <raylib.h>
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


class MdxRaylib
{
public:
    static int get_raylib_vertices(const MdxModel& mdx_model, int i, float*& raylib_vertices) {
        auto mdx_vertices = mdx_model.bone.meshes[i].vertices;
        int vertex_count = mdx_vertices.size();
        raylib_vertices = new float[vertex_count * 3];
        float* rvp = raylib_vertices;

        for (const MdxVertex& mdx_vertex : mdx_vertices)
            rvp = MdxVertex::get_coord_f3(mdx_vertex.coord, rvp);

        return vertex_count;
    }

    static void init_single_raylib_mesh(const MdxModel& mdx_model, int i, Mesh& raylib_mesh) {
        raylib_mesh.vertexCount = get_raylib_vertices(mdx_model, i, raylib_mesh.vertices);
    }

    static int get_raylib_meshes(const MdxModel& mdx_model, Mesh*& meshes_out) {
        auto mesh_count = mdx_model.bone.meshes.size();
        meshes_out = new Mesh[mesh_count];

        for (int i = 0; i < mesh_count; ++i)
            init_single_raylib_mesh(mdx_model, i, *(meshes_out + i * sizeof(Model)));

        return mesh_count;
    }

    static Model get_raylib_model_from(const MdxModel& mdx_model) {
        Model raylib_model;
        raylib_model.meshCount = get_raylib_meshes(mdx_model, raylib_model.meshes);

        return raylib_model;
    }
};
