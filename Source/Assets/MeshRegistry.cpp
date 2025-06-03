#include "Assets/MeshRegistry.h"
#include <numeric>
#include <spdlog/spdlog.h>
#include "Assets/AssetManager.h"

const MeshHandle* MeshRegistry::LoadMesh(const std::vector<tinyobj::shape_t>& shapes, const tinyobj::attrib_t& attrib, const std::string& meshName, const MaterialRange* materialRange)
{
	if (const MeshHandle* cached = GetAsset(meshName)) return cached;
	std::vector<TriangleWithAdjacency> triangles;
	VertexBufferLayout layout;
	TriangleWithAdjacency::AddVertexWithAdjacencyToLayout(layout);
	triangles.reserve(attrib.vertices.size()/3);

	for (size_t shapeIndex = 0; shapeIndex < shapes.size(); shapeIndex++)
	{
		const tinyobj::shape_t& shape = shapes[shapeIndex];
		const tinyobj::mesh_t& meshSource = shape.mesh;
		std::string groupName = shape.name.empty() ? AssetManager::GetDefaultAssetName<MeshHandle>(meshName, std::to_string(shapeIndex)) : shape.name;

		if (meshSource.indices.empty() || meshSource.num_face_vertices.empty())
		{
			spdlog::warn("Mesh '{}', group '{}' - has no indices or faces", meshName, groupName);
			continue;
		}

		BuildTriangles(attrib, meshSource, materialRange, triangles);
	}

	return AddAsset(meshName, CreateMeshHandle(triangles, layout));
}

void MeshRegistry::BuildTriangles
(
	const tinyobj::attrib_t& attrib,
	const tinyobj::mesh_t& meshSource,
	const MaterialRange* materialRange,
	std::vector<TriangleWithAdjacency>& triangles
)
{
	size_t indexOffset = 0;
	for (std::size_t face = 0; face < meshSource.num_face_vertices.size(); face++)
	{
		uint32_t faceVertexCount = meshSource.num_face_vertices[face];
		if (faceVertexCount != 3) continue;

		TriangleWithAdjacency triangle{};

		for (std::size_t v = 0; v < 3; v++)
		{
			const tinyobj::index_t& index = meshSource.indices[indexOffset + v];
			Vertex& vertex = triangle.vertices[v * 2];

			if (index.vertex_index >= 0)
			{
				vertex.Position = glm::vec3
				(
					attrib.vertices[3 * index.vertex_index + 0],
					attrib.vertices[3 * index.vertex_index + 1],
					attrib.vertices[3 * index.vertex_index + 2]
				);
			}

			if (index.normal_index >= 0)
			{
				vertex.Normal = glm::vec3
				(
					attrib.normals[3 * index.normal_index + 0],
					attrib.normals[3 * index.normal_index + 1],
					attrib.normals[3 * index.normal_index + 2]
				);
			}

			if (!attrib.colors.empty() && index.vertex_index >= 0)
			{
				vertex.Color = glm::vec3
				(
					attrib.colors[3 * index.vertex_index + 0],
					attrib.colors[3 * index.vertex_index + 1],
					attrib.colors[3 * index.vertex_index + 2]
				);
			}
			else
			{
				vertex.Color = glm::vec3(1.0f);
			}

			if (index.texcoord_index >= 0)
			{
				vertex.TextureCoordinates = glm::vec2
				(
					attrib.texcoords[2 * index.texcoord_index + 0],
					attrib.texcoords[2 * index.texcoord_index + 1]
				);
			}
			else
			{
				vertex.TextureCoordinates = glm::vec2(0.0f);
			}

			vertex.MaterialIndex = materialRange->Offset + meshSource.material_ids[face];

			triangle.vertices[v * 2 + 1] = Vertex{};
		}

		for (std::size_t v = 0; v < 3; ++v)
		{
			uint32_t idx = materialRange->Offset + meshSource.material_ids[face];

			triangle.vertices[v * 2].MaterialIndex = idx;
			triangle.vertices[v * 2 + 1].MaterialIndex = idx;
		}

		const glm::vec3& p0 = triangle.vertices[0].Position;
		const glm::vec3& p1 = triangle.vertices[2].Position;
		const glm::vec3& p2 = triangle.vertices[4].Position;

		const glm::vec2& uv0 = triangle.vertices[0].TextureCoordinates;
		const glm::vec2& uv1 = triangle.vertices[2].TextureCoordinates;
		const glm::vec2& uv2 = triangle.vertices[4].TextureCoordinates;

		glm::vec3 edge1 = p1 - p0;
		glm::vec3 edge2 = p2 - p0;
		glm::vec2 deltaUV1 = uv1 - uv0;
		glm::vec2 deltaUV2 = uv2 - uv0;

		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		glm::vec3 tangent = (edge1 * deltaUV2.y - edge2 * deltaUV1.y) * r;

		glm::vec3 bitangent = (edge2 * deltaUV1.x - edge1 * deltaUV2.x) * r;
		glm::vec3 normal = triangle.vertices[0].Normal;

		float handedness = (glm::dot(glm::cross(normal, tangent), bitangent) < 0.0f) ? -1.0f : 1.0f;
		glm::vec4 tangent4 = glm::vec4(glm::normalize(tangent), handedness);

		triangle.vertices[0].Tangent = tangent4;
		triangle.vertices[2].Tangent = tangent4;
		triangle.vertices[4].Tangent = tangent4;

		triangles.push_back(triangle);
		indexOffset += faceVertexCount;
	}
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
