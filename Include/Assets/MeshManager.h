#pragma once
#include <string>
#include <unordered_map>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Renderer/Mesh.h"

class MeshManager
{
public:
	explicit MeshManager(const std::string& meshDirectory) : m_MeshDirectory(meshDirectory) {}
	~MeshManager() = default;

	const Mesh* GetMesh(const std::string& meshName) const;
	const Mesh* LoadMesh(const std::string& meshName);
private:
	struct Vertex
	{
		float Position[3];
		float Normal[3];
		float Texture[2];
		float Tangent[3];
	};

	std::unordered_map<std::string, Mesh> m_MeshMap;
	static std::vector<Vertex> LoadVertices(const aiMesh* mesh, VertexBufferLayout& layout);
	static std::vector<uint32_t>  LoadIndices(aiMesh* mesh);
	std::string m_MeshDirectory;
};