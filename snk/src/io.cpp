#include "io.hpp"
#include "str.hpp"
#include <fstream>

namespace snk {
    std::string read_all(const std::filesystem::path &path) {
        if (!std::filesystem::is_regular_file(path)) {
            throw file_not_found_exception(path);
        }

        std::ifstream ifs(path.string());
        std::string str((std::istreambuf_iterator<char>(ifs)),
                        std::istreambuf_iterator<char>());

        return str;
    }

    file_not_found_exception::file_not_found_exception(const std::filesystem::path &path,
                                                       const snk::source_location &location)
            : io_exception(str{} << "File not found: " << std::filesystem::absolute(path), location) {

    }
}