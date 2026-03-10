#include <array>
#include <unordered_set>
#include <algorithm>

#include "ObjLoader.h"
#include "../../Logging/Logger.h"
#include "../../Profiler/Profiler.h"

namespace Engine
{
    struct VertexIndex
    {
        VertexIndex(uint32_t position, uint32_t textureUV, uint32_t normal) : position(position), textureUV(textureUV),
                                                                              normal(normal)
        {
        };

        bool operator==(const VertexIndex& other)
        {
            return this->position == other.position && this->textureUV == other.textureUV && this->normal == other.
                normal;
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

    std::string ObjLoader::getResourceType()
    {
        return "Mesh";
    }

    std::shared_ptr<void> ObjLoader::load(std::filesystem::path filepath)
    {
        CE_PROFILE_FUNC(Load_Obj);
        auto file = FileAccessor();
        file.openFile(filepath);

        if (file.getSize() <= 0)
        {
            logError("BitmapLoader", "Failed to open file.");
            return nullptr;
        }

        std::vector<float> raw_vertex_positions;
        std::vector<float> raw_vertex_texture_uvs;
        std::vector<float> raw_vertex_normals;
        std::vector<VertexIndex> raw_vertex_indices;

        auto flags = static_cast<EnabledAttributes>(0);

        // Read Obj File

        while (!file.isAtEof())
        {
            std::string line = file.readLine();

            if (line.rfind("v ", 0) == 0)
            {
                std::string v_str = line.substr(2);
                size_t start = 0, end = 0;

                for (size_t i = 0; i < 3; i++)
                {
                    end = v_str.find(" ", start);
                    raw_vertex_positions.push_back(std::stof(v_str.substr(start, end - start)));

                    end == UINT64_MAX ? start = end : start = end + 1;
                }
            }
            else if (line.rfind("vt ", 0) == 0)
            {
                flags = flags | EnabledAttributes::TextureUV;
                std::string vt_str = line.substr(3);

                size_t start = 0, end = 0;

                for (size_t i = 0; i < 3; i++)
                {
                    end = vt_str.find(" ", start);
                    if (start != UINT64_MAX)
                    {
                        raw_vertex_texture_uvs.push_back(std::stof(vt_str.substr(start, end - start)));
                    }
                    else
                    {
                        raw_vertex_texture_uvs.push_back(0.f);
                    }

                    end == UINT64_MAX ? start = end : start = end + 1;
                }
            }
            else if (line.rfind("vn ", 0) == 0)
            {
                flags = flags | EnabledAttributes::Normal;
                std::string vn_str = line.substr(3);

                size_t start = 0, end = 0;

                for (size_t i = 0; i < 3; i++)
                {
                    end = vn_str.find(" ", start);
                    if (start != UINT64_MAX)
                    {
                        raw_vertex_normals.push_back(std::stof(vn_str.substr(start, end - start)));
                    }
                    else
                    {
                        raw_vertex_normals.push_back(0.f);
                    }

                    end == UINT64_MAX ? start = end : start = end + 1;
                }
            }
            else if (line.rfind("f ", 0) == 0)
            {
                std::string f_str = line.substr(2);
                size_t f_start = 0, f_end = 0;
                size_t i = 0;

                while (f_end <= f_str.size())
                {
                    f_end = f_str.find(' ', f_start);

                    std::string index_group_str = f_str.substr(f_start, f_end - f_start);
                    std::array<int32_t, 3> index_group = {INT32_MAX, INT32_MAX, INT32_MAX};
                    size_t ig_start = 0, ig_end = 0;

                    for (size_t j = 0; j < 3; j++)
                    {
                        ig_end = index_group_str.find('/', ig_start);
                        if (ig_start != UINT64_MAX)
                        {
                            index_group[j] = std::stoi(index_group_str.substr(ig_start, ig_end - ig_start));
                        }

                        ig_end == UINT64_MAX ? ig_start = ig_end : ig_start = ig_end + 1;
                    }

                    uint32_t position_index = index_group[0] > 0
                                                 ? index_group[0] - 1
                                                 : (static_cast<uint32_t>(raw_vertex_positions.size()) + index_group[0]);
                    uint32_t texture_uv_index = index_group[1] > 0
                                                  ? index_group[1] - 1
                                                  : (static_cast<uint32_t>(raw_vertex_texture_uvs.size()) + index_group[1]);
                    uint32_t normal_index = index_group[2] > 0
                                               ? index_group[2] - 1
                                               : (static_cast<uint32_t>(raw_vertex_normals.size()) + index_group[2]);

                    if (i >= 3)
                    {
                        raw_vertex_indices.push_back(raw_vertex_indices[raw_vertex_indices.size() - (3 * i - 6)]);
                        raw_vertex_indices.push_back(raw_vertex_indices[raw_vertex_indices.size() - 2]);
                    }
                    raw_vertex_indices.push_back(VertexIndex(position_index, texture_uv_index, normal_index));

                    f_end == UINT64_MAX ? f_start = f_end : f_start = f_end + 1;
                    i++;
                }
            }
        }

        // Normalize the multiple vertex index arrays into one index array

        std::vector<VertexIndex> intermediate;

        std::vector<float> vertices;
        std::vector<uint32_t> vertex_indices;

        for (size_t i = 0; i < raw_vertex_indices.size(); i++)
        {
            if (intermediate.capacity() < intermediate.size() + 1)
            {
                intermediate.capacity() == 0
                    ? intermediate.reserve(64)
                    : intermediate.reserve(static_cast<float>(intermediate.capacity()) * 1.5f);
            }

            auto result = std::find(intermediate.begin(), intermediate.end(), raw_vertex_indices[i]);
            if (result == intermediate.end())
            {
                vertex_indices.push_back(static_cast<uint32_t>(intermediate.size()));
                intermediate.push_back(raw_vertex_indices[i]);

                ASSERT(raw_vertex_positions.size() > raw_vertex_indices[i].position);
                vertices.push_back(raw_vertex_positions[raw_vertex_indices[i].position * 3]);
                vertices.push_back(raw_vertex_positions[raw_vertex_indices[i].position * 3 + 1]);
                vertices.push_back(raw_vertex_positions[raw_vertex_indices[i].position * 3 + 2]);

                if ((flags & EnabledAttributes::TextureUV) == EnabledAttributes::TextureUV)
                {
                    ASSERT(raw_vertex_texture_uvs.size() > raw_vertex_indices[i].textureUV);
                    vertices.push_back(raw_vertex_texture_uvs[raw_vertex_indices[i].textureUV * 3]);
                    vertices.push_back(raw_vertex_texture_uvs[raw_vertex_indices[i].textureUV * 3 + 1]);
                    //vertices.push_back(rawVertexTextureUVs[rawVertexIndices[i].textureUV * 3 + 2]); /* Attributes don't support 3D texture mapping yet. */
                }

                if ((flags & EnabledAttributes::Normal) == EnabledAttributes::Normal)
                {
                    ASSERT(raw_vertex_normals.size() > raw_vertex_indices[i].normal);
                    vertices.push_back(raw_vertex_normals[raw_vertex_indices[i].normal * 3]);
                    vertices.push_back(raw_vertex_normals[raw_vertex_indices[i].normal * 3 + 1]);
                    vertices.push_back(raw_vertex_normals[raw_vertex_indices[i].normal * 3 + 2]);
                }
            }
            else
            {
                vertex_indices.push_back(static_cast<uint32_t>(std::distance(intermediate.begin(), result)));
            }
        }

        // Convert Obj data into mesh data.

        std::shared_ptr<VertexBuffer> obj_vb = VertexBuffer::create(vertices.data(),
                                                                   static_cast<uint32_t>(vertices.size()) * sizeof(
                                                                       float));

        std::vector<BufferElement> attributes = {{ShaderDataType::Float3, "aPos"}};

        if ((flags & EnabledAttributes::TextureUV) == EnabledAttributes::TextureUV)
        {
            attributes.push_back({ShaderDataType::Float2, "aTexUV"});
        }
        if ((flags & EnabledAttributes::Normal) == EnabledAttributes::Normal)
        {
            attributes.push_back({ShaderDataType::Float3, "aNormal"});
        }

        obj_vb->setLayout(attributes);

        std::shared_ptr<IndexBuffer> obj_ib = IndexBuffer::create(vertex_indices.data(),
                                                                 static_cast<uint32_t>(vertex_indices.size()));

        std::shared_ptr<Mesh> obj_mesh = Mesh::create();
        obj_mesh->addVertexBuffer(obj_vb);
        obj_mesh->setIndexBuffer(obj_ib);

        // TODO: Add material support to OBJ reader.
        //objMesh->SetMaterial(this->material);

        return obj_mesh;
    }
}
