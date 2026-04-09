
#pragma once

#include <zip.h>
#include <string>
#include <format>
#include <stdexcept>
#include <vector>
#include <memory>

namespace nyx
{
    class ZipException : public std::runtime_error
    {
    private:

        static std::string get_zip_strerror(int err)
        {
            zip_error_t zerr;
            zip_error_init_with_code(&zerr, err);
            std::string zip_msg(zip_error_strerror(&zerr));

            zip_error_fini(&zerr);
            return zip_msg;
        }


        static std::string get_zip_strerror(const std::string& msg, int err)
        {
            return msg + get_zip_strerror(err);
        }


        static std::string get_zip_strerror(const std::string& msg, zip& zip)
        {
            int err = zip_error_code_zip(zip_get_error(&zip));
            return msg + get_zip_strerror(err);
        }

    public:
        ZipException(const std::string& msg, int err)
        : std::runtime_error(get_zip_strerror(msg, err))
        {}

        ZipException(const std::string& msg, zip& zip)
        : std::runtime_error(get_zip_strerror(msg, zip))
        {}
    };


    class ZipReader
    {
    private:
        zip* _zip = NULL;

    public:

        ZipReader(const std::string& zip_path)
        {
            int err;
            _zip = zip_open(zip_path.c_str(), 0, &err);

            if (!_zip)
                throw ZipException(std::format("Failed to open: {}.", zip_path), err);
        }

        ~ZipReader()
        {
            if (_zip != NULL)
                zip_close(_zip);
        }

        int get_num_entries()
        {
            return (int)zip_get_num_entries(_zip,  ZIP_FL_UNCHANGED);
        }

        bool zip_name_located(const std::string& path, int* idx)
        {
            int res = zip_name_locate(_zip, path.c_str(), ZIP_FL_ENC_GUESS);

            if (res < 0 )
                return false;

            *idx = res;
            return true;
        }

        std::string get_file_name(int i)
        {
            auto name = zip_get_name(_zip, i, ZIP_FL_ENC_GUESS);

            if (name == NULL)
                throw ZipException(std::format("zip_get_name failed with index: {}.", i).c_str(), *_zip);

            return name;
        }

        std::unique_ptr<std::vector<std::string>> get_file_names()
        {
            auto paths = std::make_unique<std::vector<std::string>>();
            int n = get_num_entries();

            for (int i = 0; i < n; ++i)
                paths->push_back(get_file_name(i));

            return paths;
        }

        std::vector<uint8_t> fread(const std::string& path)
        {
            zip_stat_t stat;
            zip_stat_init(&stat);  // always initialize first!

            if (zip_stat(_zip, path.c_str(), ZIP_FL_ENC_GUESS, &stat) != 0)
                throw ZipException(std::format("zip_stat failed: {}.", path), *_zip);

            // MUST check valid flags before trusting stat fields
            if (!(stat.valid & ZIP_STAT_SIZE))
                throw std::runtime_error(std::format("zip_stat size not valid: {}.", path));

            zip_file_t* file = zip_fopen(_zip, path.c_str(), ZIP_FL_ENC_GUESS);

            if (!file)
                throw ZipException(std::format("zip_fopen failed: {}.", path), *_zip);

            std::vector<uint8_t> buffer(stat.size);
            zip_int64_t bytes_read = zip_fread(file, buffer.data(), stat.size);
            zip_fclose(file);

            if (bytes_read != stat.size)
                throw std::runtime_error(std::format("zip_fread incomplete: {}.", path));

            return buffer;
        }
    };
}
