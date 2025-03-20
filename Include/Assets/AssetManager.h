#pragma once
#include "ShaderManager.h"
#include "MeshManager.h"

class AssetManager
{
public:
	AssetManager();
	~AssetManager();

	ShaderManager& GetShaderManager() { return m_ShaderManager; }
	MeshManager& GetMeshManager() { return m_MeshManager; }
private:
	ShaderManager m_ShaderManager;
	MeshManager m_MeshManager;
};
