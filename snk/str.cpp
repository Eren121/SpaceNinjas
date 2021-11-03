#include "str.hpp"

namespace snk {
    str::operator std::string() const {
        return m_stream.str();
    }

    std::string str::operator<<(const end_t &) const {
        return m_stream.str();
    }
}