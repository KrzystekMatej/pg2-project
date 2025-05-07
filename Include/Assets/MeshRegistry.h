#pragma once
#include <string>
#include <array>
#include <tiny_obj_loader.h>
#include "AssetRegistry.h"
#include "Renderer/MeshHandle.h"
#include "Assets/MaterialRegistry.h"

class MeshRegistry : public AssetRegistry<MeshHandle>
{
public:
	using BaseRegistryType = AssetRegistry;
	using AssetRegistry::AssetRegistry;

	struct Vertex
	{
		float Position[3];
		float Normal[3];
		float Color[3];
		float TextureCoordinates[2];
		float Tangent[3];
		uint32_t MaterialIndex;

		static void AddVertexToLayout(VertexBufferLayout& layout)
		{
			layout.Push<float>(3);
			layout.Push<float>(3);
			layout.Push<float>(3);
			layout.Push<float>(2);
			layout.Push<float>(3);
			layout.Push<uint32_t>(1);
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

	const MeshHandle* LoadMesh
	(
		const std::vector<tinyobj::shape_t>& shapes,
		const tinyobj::attrib_t& attrib,
		const std::string& meshName,
		const MaterialRange* materialRange
	);
private:

	static void BuildTriangles
	(
		const tinyobj::attrib_t& attrib,
		const tinyobj::mesh_t& meshSource,
		const MaterialRange* materialRange,
		std::vector<TriangleWithAdjacency>& triangles
	);

	static std::unique_ptr<MeshHandle> CreateMeshHandle(const std::vector<TriangleWithAdjacency>& triangles, const VertexBufferLayout& layout);
};