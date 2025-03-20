#pragma once
#include "ShaderManager.h"

class AssetManager
{
public:
	AssetManager();
	~AssetManager();

	const ShaderManager& GetShaderManager() { return m_ShaderManager; }
private:
	ShaderManager m_ShaderManager;
};
