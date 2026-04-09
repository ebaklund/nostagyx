
#pragma once

namespace nyx
{
    struct MdxVector {
    float coord[3];
    };

    struct MdxVertex {
        int16_t coord[3];
        uint8_t normal[2];

        static float* get_coord_f3(const int16_t* in, float* out) {
            *(out++) = *(in++);
            *(out++) = *(in++);
            *(out++) = *(in++);
            return out;
        }

        static float* get_normal_f3(const uint8_t* in, float* out) {
            const static float delta_deg = 2.f*M_PI/255.f;
            float    alpha = in[0] * delta_deg;
            float     beta = in[1] * delta_deg;
            float sinAlpha = sinf(alpha);
            *(out++) = cosf(beta)*sinAlpha;
            *(out++) = sinf(beta)*sinAlpha;
            *(out++) = cosf(alpha);
            return out;
        }

        void get_vertex_f3(float* coord_out, float* normal_out) {
            get_coord_f3(coord, coord_out);
            get_normal_f3(normal, normal_out);
        }
    };

    struct MdxTexCoord {
        float coord[2];
    };

    struct MdxTriangle {
        int32_t vertex[3];

        template<typename T>
        static T* get_indices(const int32_t* in, T* out) {
            *(out++) = (T)*(in++);
            *(out++) = (T)*(in++);
            *(out++) = (T)*(in++);
            return out;
        }
    };

    struct MdxRotation {
        float matrix[9];
    };

    struct MdxTransform {
        MdxVector position;
        MdxRotation rotation;
    };

    struct MdxAnimation {
        int32_t first_frame;
        int32_t last_frame;
        int32_t back_frame;
        int32_t fps;
    };

    struct MdxMesh {
    std::vector<MdxVertex> vertices;
    std::vector<MdxTexCoord> tex_coords;
    std::vector<MdxTriangle> triangles;
    };


    // class GLBone: public DMReference {
    // public:
    //   int framesCount;

    //   int animationsCount;
    //   GLAnimationData* animations;

    //   int shapesCount;
    //   GLAnimatedShape* shapes;

    //   int linksCount;
    //   char** linkNames;

    //   int transformsCount;
    //   MD3Transform* transforms;
    // };

    struct MdxBone
    {
        int frame_count;
        std::vector<MdxAnimation> animations;
        std::vector<MdxTransform> transforms;
        std::vector<MdxMesh> meshes;
    };

    // class GLModel: public GLObject, public GLBasicMaterialOwner {
    // private:
    //   static float animationTime;
    //   static DMCache* staticCache;
    //   DMCache* cache;
    // #ifdef USE_OGLES
    // #else // !USE_OGLES
    //   static GLVertexProgram* lerpVP;
    //   GLVertexBuffer* vertexBuffer;
    // #endif // !USE_OGLES
    // public:
    // public:
    // 	bool playBackward;
    //   int currFrame;
    //   int nextFrame;
    //   float lastUpdate;
    //   float interpolation;
    //   int currAnimation;
    //   int stopAnimation;
    //   GLBone* bone;
    //   GLObject** links;
    //   float scale;
    // public:
    //   float yawAngle;
    //   float pitchAngle;
    //   float getYawAngle() {return yawAngle;}
    //   float getPitchAngle() {return pitchAngle;}
    //   void setYawAngle(float a) {yawAngle = a; updateRotations();}
    //   void setPitchAngle(float a) {pitchAngle = a; updateRotations();}
    //   float addYawAngle(float a, float maxA = 0, float minA = 0);
    //   float addPitchAngle(float a, float maxA = 0, float minA = 0);
    // };

    // GLModel::GLModel(bool ownsCache):
    //   currFrame(-1), nextFrame(-1), interpolation(0), lastUpdate(0), bone(NULL),
    //   links(NULL), yawAngle(0), pitchAngle(0), scale(1/64.0f), cache(NULL),
    //   currAnimation(-1), stopAnimation(-1), playBackward(false)

    struct MdxModel
    {
        std::vector<std::string> link_names;
        MdxBone bone;
    };
}
