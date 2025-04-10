#include "Assets/MeshRegistry.h"

#include <numeric>
#include <spdlog/spdlog.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

const MeshHandle* MeshRegistry::LoadMesh(const aiMesh* mesh, const std::string& meshName)
{
	if (const MeshHandle* cached = GetAsset(meshName)) return cached;

	if (!mesh->HasFaces())
	{
		spdlog::warn("Mesh '{}' has no faces", meshName);
		return nullptr;
	}

	VertexBufferLayout layout;
	TriangleWithAdjacency::AddVertexWithAdjacencyToLayout(layout);

	std::vector<TriangleWithAdjacency> triangles = BuildTriangles(mesh);

	if (triangles.empty())
	{
		spdlog::error("Invalid primitives in mesh - requires triangles - '{}'", meshName);
		return nullptr;
	}

	auto handle = CreateMeshHandle(triangles, layout);
	return AddAsset(meshName, std::move(handle));
}

std::vector<MeshRegistry::TriangleWithAdjacency> MeshRegistry::BuildTriangles(const aiMesh* mesh)
{
	std::vector<TriangleWithAdjacency> triangles;

	for (uint32_t i = 0; i < mesh->mNumFaces; ++i)
	{
		const aiFace& face = mesh->mFaces[i];
		if (face.mNumIndices != 3) return {};

		TriangleWithAdjacency triangle{};

		for (int v = 0; v < 3; v++)
		{
			uint32_t index = face.mIndices[v];
			Vertex& vert = triangle.vertices[v * 2];

			vert.Position[0] = mesh->mVertices[index].x;
			vert.Position[1] = mesh->mVertices[index].y;
			vert.Position[2] = mesh->mVertices[index].z;

			if (mesh->HasNormals())
			{
				vert.Normal[0] = mesh->mNormals[index].x;
				vert.Normal[1] = mesh->mNormals[index].y;
				vert.Normal[2] = mesh->mNormals[index].z;
			}

			if (mesh->HasVertexColors(0))
			{
				vert.Color[0] = mesh->mColors[0][index].r;
				vert.Color[1] = mesh->mColors[0][index].g;
				vert.Color[2] = mesh->mColors[0][index].b;
			}
			else
			{
				vert.Color[0] = 1.0f;
				vert.Color[1] = 1.0f;
				vert.Color[2] = 1.0f;
			}

			if (mesh->HasTextureCoords(0))
			{
				vert.TextureCoordinates[0] = mesh->mTextureCoords[0][index].x;
				vert.TextureCoordinates[1] = mesh->mTextureCoords[0][index].y;
			}

			if (mesh->HasTangentsAndBitangents())
			{
				vert.Tangent[0] = mesh->mTangents[index].x;
				vert.Tangent[1] = mesh->mTangents[index].y;
				vert.Tangent[2] = mesh->mTangents[index].z;
			}

			triangle.vertices[v * 2 + 1] = Vertex{};
		}

		triangles.push_back(triangle);
	}

	return triangles;
}

std::unique_ptr<MeshHandle> MeshRegistry::CreateMeshHandle(const std::vector<TriangleWithAdjacency>& triangles, const VertexBufferLayout& layout)
{
	VertexBuffer vertexBuffer(triangles.data(), triangles.size() * sizeof(TriangleWithAdjacency));

	std::vector<uint32_t> indices(triangles.size() * 6);
	std::iota(indices.begin(), indices.end(), 0);

	IndexBuffer indexBuffer(indices.data(), indices.size());

	VertexArray vertexArray;
	vertexArray.Bind();
	vertexArray.AddBuffer(std::move(vertexBuffer), layout);
	indexBuffer.Bind();
	vertexArray.Unbind();

	return std::make_unique<MeshHandle>(std::move(vertexArray), std::move(indexBuffer));
}
