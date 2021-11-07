#pragma once

#include <string>
#include <unordered_map>
#include <concepts>
#include <functional>

namespace Snow::media
{
/// @brief A resource should be default-constructible.
template<typename T>
concept Resource = std::constructible_from<T>;


/// @brief Storage of resources (audio, textures, etc...).
/// @details
/// Each resource is loaded with an "identifier" (usually the path like "assets/myfile.mp3")
/// If a resource is asked twice, it is only loaded once and cached for all further asks.
/// This allows you continue using simple identifier like paths but without to manage the lifetime of the resources.
template<Resource T>
class ResourceCache
{
public:

    /// @brief Construct the resource.
    /// @param resource
    ///     The default-constructed resource to build.
    /// @param id
    ///     To identify the resource to load. Is it up to the implementation to define what is it, but
    ///     a path is probably the most reasonnable one.
    virtual void loadResource(T& resource, const std::string& id) = 0;

    virtual ~ResourceCache() = default;

    /// @brief Load or a get a resource from its id.
    T& operator()(const std::string& id)
    {
        if (m_cache.contains(id))
        {
            // returns if already loaded

            return m_cache.at(id);
        }
        else
        {
            // not here, loads the resource

            T& ret = m_cache[id]; // operator[] insert by with default-constructor.
            loadResource(ret, id); // initialize the resource
            return ret;
        }
    }

    /// @brief Force to load a resource from an external source.
    void forceLoad(const std::string& id, std::function<void(T&)> loader)
    {
        loader(m_cache[id]);
    }

private:
    /// @brief All already loaded assets.
    std::unordered_map<std::string, T> m_cache;
};


}

