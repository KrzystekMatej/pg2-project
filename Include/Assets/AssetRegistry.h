#pragma once
#include <string>
#include <memory>
#include <unordered_map>
#include <spdlog/spdlog.h>
#include "Project/Project.h"
#include "Utils/Text.h"

class AssetRegistryBase
{
public:
    virtual ~AssetRegistryBase() = default;
};

template<typename T>
class AssetRegistry : public AssetRegistryBase
{
public:
    using AssetType = T;

    const T* GetAsset(const std::string& name) const
    {
        std::string key = Utils::ToLower(name);
        auto it = m_AssetMap.find(key);
        if (it != m_AssetMap.end()) return it->second.get();
        return nullptr;
    }

    const T* AddAsset(const std::string& name, std::unique_ptr<T> asset)
    {
        std::string key = Utils::ToLower(name);
        auto [it, inserted] = m_AssetMap.emplace(key, std::move(asset));

        if (!inserted)
        {
            spdlog::warn("Asset '{}' already exists in registry as type '{}'.", name, typeid(T).name());
        }

        return it->second.get();
    }

    std::vector<const T*> GetValues()
    {
        std::vector<const T*> values;
        values.reserve(m_AssetMap.size());

        for (const auto& [key, asset] : m_AssetMap)
        {
            values.push_back(asset.get());
        }

        return values;
    }
protected:
    std::unordered_map<std::string, std::unique_ptr<T>> m_AssetMap;
};