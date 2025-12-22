#include <array>
#include <unordered_set>
#include <algorithm>

#include "ObjLoader.h"
#include "../../Logging/Logger.h"
#include "../../Profiler/Profiler.h"

namespace Engine {

	struct VertexIndex
	{

		VertexIndex(uint32_t position, uint32_t textureUV, uint32_t normal) : position(position), textureUV(textureUV), normal(normal) {};

		inline bool operator==(const VertexIndex& other)
		{
			return this->position == other.position && this->textureUV == other.textureUV && this->normal == other.normal;
		}

		uint32_t position;
		uint32_t textureUV;
		uint32_t normal;
	};

	enum class EnabledAttributes : uint8_t
	{
		TextureUV = 1 << 0,
		Normal = 1 << 1,
	};

	inline EnabledAttributes operator|(EnabledAttributes a, EnabledAttributes b)
	{
		return static_cast<EnabledAttributes>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
	}

	inline EnabledAttributes operator&(EnabledAttributes a, EnabledAttributes b)
	{
		return static_cast<EnabledAttributes>(static_cast<uint8_t>(a) & static_cast<uint8_t>(b));
	}

	std::string ObjLoader::GetResourceType()
	{
		return "Mesh";
	}

	std::shared_ptr<void> ObjLoader::Load(std::filesystem::path filepath)
	{
		CE_PROFILE_FUNC(Load_Obj);
		FileAccessor file = FileAccessor();
		file.OpenFile(filepath);

		if (file.GetSize() <= 0)
		{
			LogError("BitmapLoader", "Failed to open file.");
			return nullptr;
		};

		std::vector<float> rawVertexPositions;
		std::vector<float> rawVertexTextureUVs;
		std::vector<float> rawVertexNormals;
		std::vector<VertexIndex> rawVertexIndices;

		EnabledAttributes flags = static_cast<EnabledAttributes>(0);

		// Read Obj File

		while (!file.IsAtEof())
		{
			std::string line = file.ReadLine();

			if (line.rfind("v ", 0) == 0)
			{
				std::string vStr = line.substr(2);
				size_t start = 0, end = 0;

				for (size_t i = 0; i < 3; i++)
				{
					end = vStr.find(" ", start);
					rawVertexPositions.push_back(std::stof(vStr.substr(start, end - start)));

					end == UINT64_MAX ? start = end : start = end + 1;
				}
			}
			else if (line.rfind("vt ", 0) == 0)
			{
				flags = flags | EnabledAttributes::TextureUV;
				std::string vtStr = line.substr(3);

				size_t start = 0, end = 0;

				for (size_t i = 0; i < 3; i++)
				{
					end = vtStr.find(" ", start);
					if (start != UINT64_MAX)
					{
						rawVertexTextureUVs.push_back(std::stof(vtStr.substr(start, end - start)));
					}
					else
					{
						rawVertexTextureUVs.push_back(0.f);
					}
					
					end == UINT64_MAX ? start = end : start = end + 1;
				}
			}
			else if (line.rfind("vn ", 0) == 0)
			{
				flags = flags | EnabledAttributes::Normal;
				std::string vnStr = line.substr(3);

				size_t start = 0, end = 0;

				for (size_t i = 0; i < 3; i++)
				{
					end = vnStr.find(" ", start);
					if (start != UINT64_MAX)
					{
						rawVertexNormals.push_back(std::stof(vnStr.substr(start, end - start)));
					}
					else
					{
						rawVertexNormals.push_back(0.f);
					}

					end == UINT64_MAX ? start = end : start = end + 1;
				}
			}
			else if (line.rfind("f ", 0) == 0)
			{
				std::string fStr = line.substr(2);
				size_t fStart = 0, fEnd = 0;
				size_t i = 0;

				while (fEnd <= fStr.size())
				{
					fEnd = fStr.find(' ', fStart);

					std::string indexGroupStr = fStr.substr(fStart, fEnd - fStart);
					std::array<int32_t, 3> indexGroup = { INT32_MAX, INT32_MAX, INT32_MAX };
					size_t igStart = 0, igEnd = 0;

					for (size_t j = 0; j < 3; j++)
					{
						igEnd = indexGroupStr.find('/', igStart);
						if (igStart != UINT64_MAX)
						{
							indexGroup[j] = std::stoi(indexGroupStr.substr(igStart, igEnd - igStart));
						}

						igEnd == UINT64_MAX ? igStart = igEnd : igStart = igEnd + 1;
					}

					uint32_t positionIndex = indexGroup[0] > 0 ? indexGroup[0] - 1 : (static_cast<uint32_t>(rawVertexPositions.size()) + indexGroup[0]);
					uint32_t textureUVIndex = indexGroup[1] > 0 ? indexGroup[1] - 1 : (static_cast<uint32_t>(rawVertexTextureUVs.size()) + indexGroup[1]);
					uint32_t normalIndex = indexGroup[2] > 0 ? indexGroup[2] - 1 : (static_cast<uint32_t>(rawVertexNormals.size()) + indexGroup[2]);

					if (i >= 3)
					{
						rawVertexIndices.push_back(rawVertexIndices[rawVertexIndices.size() - (3 * i - 6)]);
						rawVertexIndices.push_back(rawVertexIndices[rawVertexIndices.size() - 2]);
					}
					rawVertexIndices.push_back(VertexIndex(positionIndex, textureUVIndex, normalIndex));

					fEnd == UINT64_MAX ? fStart = fEnd : fStart = fEnd + 1;
					i++;
				}
			}
		}

		// Normalize the multiple vertex index arrays into one index array

		std::vector<VertexIndex> intermediate;

		std::vector<float> vertices;
		std::vector<uint32_t> vertexIndices;

		for (size_t i = 0; i < rawVertexIndices.size(); i++)
		{
			if (intermediate.capacity() < intermediate.size() + 1)
			{
				intermediate.capacity() == 0 ? intermediate.reserve(64) : intermediate.reserve(static_cast<size_t>(static_cast<float>(intermediate.capacity()) * 1.5f));
			}

			std::vector<VertexIndex>::iterator result = std::find(intermediate.begin(), intermediate.end(), rawVertexIndices[i]);
			if (result == intermediate.end())
			{
				vertexIndices.push_back(static_cast<uint32_t>(intermediate.size()));
				intermediate.push_back(rawVertexIndices[i]);

				ASSERT(rawVertexPositions.size() > rawVertexIndices[i].position);
				vertices.push_back(rawVertexPositions[rawVertexIndices[i].position * 3]);
				vertices.push_back(rawVertexPositions[rawVertexIndices[i].position * 3 + 1]);
				vertices.push_back(rawVertexPositions[rawVertexIndices[i].position * 3 + 2]);

				if ((flags & EnabledAttributes::TextureUV) == EnabledAttributes::TextureUV)
				{
					ASSERT(rawVertexTextureUVs.size() > rawVertexIndices[i].textureUV);
					vertices.push_back(rawVertexTextureUVs[rawVertexIndices[i].textureUV * 3]);
					vertices.push_back(rawVertexTextureUVs[rawVertexIndices[i].textureUV * 3 + 1]);
					//vertices.push_back(rawVertexTextureUVs[rawVertexIndices[i].textureUV * 3 + 2]); /* Attributes don't support 3D texture mapping yet. */
				}

				if ((flags & EnabledAttributes::Normal) == EnabledAttributes::Normal)
				{
					ASSERT(rawVertexNormals.size() > rawVertexIndices[i].normal);
					vertices.push_back(rawVertexNormals[rawVertexIndices[i].normal * 3]);
					vertices.push_back(rawVertexNormals[rawVertexIndices[i].normal * 3 + 1]);
					vertices.push_back(rawVertexNormals[rawVertexIndices[i].normal * 3 + 2]);
				}
			}
			else
			{
				vertexIndices.push_back(static_cast<uint32_t>(std::distance(intermediate.begin(), result)));
			}

		}

		// Convert Obj data into mesh data.

		std::shared_ptr<Engine::VertexBuffer> objVB = Engine::VertexBuffer::Create(vertices.data(), static_cast<uint32_t>(vertices.size()) * sizeof(float));

		std::vector<BufferElement> Attributes = {{Engine::ShaderDataType::Float3, "aPos"}};

		if ((flags & EnabledAttributes::TextureUV) == EnabledAttributes::TextureUV)
		{
			Attributes.push_back({Engine::ShaderDataType::Float2, "aTexUV"});
		}
		if ((flags & EnabledAttributes::Normal) == EnabledAttributes::Normal)
		{
			Attributes.push_back({ Engine::ShaderDataType::Float3, "aNormal" });
		}

		objVB->SetLayout(Attributes);

		std::shared_ptr<Engine::IndexBuffer> objIB = Engine::IndexBuffer::Create(vertexIndices.data(), static_cast<uint32_t>(vertexIndices.size()));

		std::shared_ptr<Mesh> objMesh = Mesh::Create();
		objMesh->AddVertexBuffer(objVB);
		objMesh->SetIndexBuffer(objIB);

		// TODO: Add material support to OBJ reader.
		//objMesh->SetMaterial(this->material);

		return objMesh;
	}

}