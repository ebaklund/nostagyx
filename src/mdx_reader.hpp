
#pragma once

#include <iostream>
#include "buffer_reader.hpp"
#include "mdx_model.hpp"

class MdxReader
{
private:
    struct MdxHeader
    {
        int32_t num_tags;
        int32_t num_meshes;
        int32_t num_animations;
    };

public:

    static std::unique_ptr<MdxModel> read_mdx_model(const std::vector<uint8_t>& buff)
    {
        BufferReader reader(buff.data(), buff.size());

        auto mdx_model = std::make_unique<MdxModel>();
        auto mdx_bone = std::make_unique<MdxBone>();
        reader.read_next_or_throw(&(mdx_bone->frame_count));

        MdxHeader mdx_header;
        reader.read_next_or_throw(&mdx_header);

        mdx_bone->animations.resize(mdx_header.num_animations);
        reader.read_next_or_throw(mdx_bone->animations);

        mdx_bone->link_names.resize(mdx_header.num_tags);
        for (std::string& name : mdx_bone->link_names)
            reader.read_next_or_throw(name, 16);

        mdx_bone->transforms.resize(mdx_bone->frame_count * mdx_header.num_tags);
        reader.read_next_or_throw(mdx_bone->transforms);

        mdx_bone->meshes.resize(mdx_header.num_meshes);
        for (MdxMesh& mesh : mdx_bone->meshes) {
            int32_t num_frames;
            int32_t num_triangles;
            int32_t num_tex_coords;

            reader.read_next_or_throw(&num_frames);
            reader.read_next_or_throw(&num_triangles);
            reader.read_next_or_throw(&num_tex_coords);
            int32_t num_vertices = num_tex_coords * num_frames;

            mesh.triangles.resize(num_triangles);
            mesh.tex_coords.resize(num_tex_coords);
            mesh.vertices.resize(num_vertices);

            reader.read_next_or_throw(mesh.triangles);
            reader.read_next_or_throw(mesh.tex_coords);
            reader.read_next_or_throw(mesh.vertices);
        }

        if (reader.bytes_left() != 0) {
            constexpr auto fmt = "Buffer is not empty after reading MDX model. Bytes left: {}\n";
            throw std::runtime_error(std::format(fmt, reader.bytes_left()));
        }

        return mdx_model;
    }
};
