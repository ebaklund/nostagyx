
#pragma once

#include "mdx_model_loader.hpp"

namespace nyx
{
    class RaylibModelLoader
    {
    private:
        MdxModelLoader _mdx_model_loader;

    public:
        RaylibModelLoader(std::string zip_path) : _mdx_model_loader(zip_path) {
        }

        void load(Model& raylib_model, const std::string& model_name, const std::string& skin_name) {
            MdxModel mdx_model;
            _mdx_model_loader.load(mdx_model, model_name, skin_name);
        }
    };
}
