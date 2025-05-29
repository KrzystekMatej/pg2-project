#pragma once
#include <string>
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <filesystem>
#include "AssetRegistry.h"
#include "Renderer/Buffers/Mesh/MeshHandle.h"
#include "Renderer/Material/MaterialAsset.h"
#include "Renderer/Buffers/ShaderStorageBuffer.h"

class AssetManager
{
public:
    void Initialize();

    template<typename TDerived, typename... Args>
    void AddRegistry(Args&&... args)
    {
        using Base = typename TDerived::BaseRegistryType;
        using AssetType = typename Base::AssetType;

        std::unique_ptr<AssetRegistryBase> registry = std::make_unique<TDerived>(std::forward<Args>(args)...);

        m_RegistryMap[typeid(AssetType)] = std::move(registry);
    }

    template<typename TDerived>
    TDerived* GetRegistry() const
    {
        using Base = typename TDerived::BaseRegistryType;
        using AssetType = typename Base::AssetType;

        auto it = m_RegistryMap.find(typeid(AssetType));
        if (it == m_RegistryMap.end()) return nullptr;

        return static_cast<TDerived*>(it->second.get());
    }

	const MeshHandle* LoadObjFile(const std::filesystem::path& filePath) const;

    template<typename T>
    static std::string GetDefaultAssetName(const std::string& assetName, const std::string& suffix)
    {
        return std::format("{}_{}_{}", assetName, typeid(T).name(), suffix);
    }

    template<typename T>
    static std::string GetDefaultAssetName(const std::filesystem::path& assetPath, const std::string& suffix)
    {
        return GetDefaultAssetName<T>(assetPath.stem().string(), suffix);
    }
private:
    std::unordered_map<std::type_index, std::unique_ptr<AssetRegistryBase>> m_RegistryMap;
};
