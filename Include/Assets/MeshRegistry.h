#pragma once
#include <string>
#include <assimp/scene.h>
#include <array>
#include "AssetRegistry.h"
#include "Renderer/MeshHandle.h"

class MeshRegistry : public AssetRegistry<MeshHandle>
{
public:
	using BaseRegistryType = AssetRegistry;

	struct Vertex
	{
		float Position[3];
		float Normal[3];
		float Color[3];
		float TextureCoordinates[2];
		float Tangent[3];

		static void AddVertexToLayout(VertexBufferLayout& layout)
		{
			layout.Push<float>(3);
			layout.Push<float>(3);
			layout.Push<float>(3);
			layout.Push<float>(2);
			layout.Push<float>(3);
		}
	};

	struct TriangleWithAdjacency
	{
		std::array<Vertex, 6> vertices;

		static void AddVertexWithAdjacencyToLayout(VertexBufferLayout& layout)
		{
			Vertex::AddVertexToLayout(layout);
			Vertex::AddVertexToLayout(layout);
		}
	};

    const MeshHandle* LoadMesh(const aiMesh* mesh, const std::string& meshName);
private:

	static std::vector<TriangleWithAdjacency> BuildTriangles(const aiMesh* mesh);
	static std::unique_ptr<MeshHandle> CreateMeshHandle(const std::vector<TriangleWithAdjacency>& triangles, const VertexBufferLayout& layout);
};