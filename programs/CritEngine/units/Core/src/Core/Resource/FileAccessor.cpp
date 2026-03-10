#include "FileAccessor.h"
#include "../Logging/Logger.h"
#include "../Profiler/Profiler.h"

namespace Engine
{
    FileAccessor::FileAccessor()
        : m_systemIsBigEndian(isSystemBigEndian())
    {
    }

    void FileAccessor::openFile(const std::filesystem::path& file_path)
    {
        this->m_file = std::ifstream(file_path, std::ios::binary);

        if (this->m_file.is_open())
        {
            this->m_file.seekg(0, std::ios::end);
            std::streamsize size = this->m_file.tellg();
            this->m_file.seekg(0, std::ios::beg);
            this->m_buffer.resize(size); // Pre-allocates memory for the entire file

            if (this->m_file.read(this->m_buffer.data(), size))
            {
                this->m_position = 0;
                // Successful read
            }
            this->m_file.close();
            this->readAsBigEndian(false);
        }
    }

    // If the value of the first byte is 1, we are in little endian since the least significant byte is stored first
    bool FileAccessor::isSystemBigEndian() const
    {
        int val = 1;
        return (!(*(char*)&val == 1));
    }

    void FileAccessor::seek(const uint64_t offset)
    {
        ASSERT(checkOffsetBounds(offset), "Position is out of bounds")

        this->m_position = offset;
    }

    void FileAccessor::seekEnd()
    {
        this->m_position = this->m_buffer.size();
    }

    uint64_t FileAccessor::getPosition()
    {
        return this->m_position;
    }

    uint64_t FileAccessor::getSize()
    {
        return this->m_buffer.size();
    }

    bool FileAccessor::isAtEof() const
    {
        return this->m_position >= m_buffer.size();
    }

    void FileAccessor::readAsBigEndian(const bool is_big_endian)
    {
        this->m_fileIsBigEndian = is_big_endian;
    }

    // This section reads binary data from a char* pointer and stores it for us in a specific interger type
    // After the read, we use pointer arithmetic to ensure the next read starts at the correct position

    // For a single byte type, we're simply using a static_cast to avoid unnecessary overhead from memcpy
    uint8_t FileAccessor::readUint8()
    {
        ASSERT(checkReadBounds(sizeof(uint8_t)), "Can not read out of buffer's bounds");

        uint8_t value = static_cast<uint8_t>(*(this->m_buffer.data() + this->m_position));
        this->m_position += sizeof(uint8_t);
        return value;
    }

    uint16_t FileAccessor::readUint16()
    {
        ASSERT(checkReadBounds(sizeof(uint16_t)), "Can not read out of buffer's bounds");

        uint16_t value;
        std::memcpy(&value, this->m_buffer.data() + this->m_position, sizeof(uint16_t));
        this->m_position += sizeof(uint16_t);

        if (m_fileIsBigEndian != m_systemIsBigEndian)
            value = BSWAP_16(value);
        return value;
    }

    uint32_t FileAccessor::readUint32()
    {
        ASSERT(checkReadBounds(sizeof(uint32_t)), "Can not read out of buffer's bounds");

        uint32_t value;
        std::memcpy(&value, this->m_buffer.data() + this->m_position, sizeof(uint32_t));
        this->m_position += sizeof(uint32_t);

        if (this->m_fileIsBigEndian != m_systemIsBigEndian)
            value = BSWAP_32(value);
        return value;
    }

    uint64_t FileAccessor::readUint64()
    {
        ASSERT(checkReadBounds(sizeof(uint64_t)), "Can not read out of buffer's bounds");

        uint64_t value;
        std::memcpy(&value, this->m_buffer.data() + this->m_position, sizeof(uint64_t));
        this->m_position += sizeof(uint64_t);

        if (this->m_fileIsBigEndian != m_systemIsBigEndian)
            value = BSWAP_64(value);
        return value;
    }

    int8_t FileAccessor::readInt8()
    {
        ASSERT(checkReadBounds(sizeof(int8_t)), "Can not read out of buffer's bounds");

        int8_t value = static_cast<int8_t>(*(this->m_buffer.data() + this->m_position));
        this->m_position += sizeof(int8_t);
        return value;
    }

    int16_t FileAccessor::readInt16()
    {
        ASSERT(checkReadBounds(sizeof(int16_t)), "Can not read out of buffer's bounds");

        int16_t value;
        std::memcpy(&value, this->m_buffer.data() + this->m_position, sizeof(int16_t));
        this->m_position += sizeof(int16_t);

        if (this->m_fileIsBigEndian != m_systemIsBigEndian)
            value = BSWAP_16(value);
        return value;
    }

    int32_t FileAccessor::readInt32()
    {
        ASSERT(checkReadBounds(sizeof(int32_t)), "Can not read out of buffer's bounds");

        int32_t value;
        std::memcpy(&value, this->m_buffer.data() + this->m_position, sizeof(int32_t));
        this->m_position += sizeof(int32_t);

        if (this->m_fileIsBigEndian != this->m_systemIsBigEndian)
            value = BSWAP_32(value);
        return value;
    }

    int64_t FileAccessor::readInt64()
    {
        ASSERT(checkReadBounds(sizeof(int64_t)), "Can not read out of buffer's bounds");

        int64_t value;
        std::memcpy(&value, this->m_buffer.data() + this->m_position, sizeof(int64_t));
        this->m_position += sizeof(int64_t);

        if (this->m_fileIsBigEndian != this->m_systemIsBigEndian)
            value = BSWAP_64(value);
        return value;
    }

    std::string FileAccessor::readLine(char delimiter)
    {
        std::string output = "";

        size_t iter = this->m_buffer.size() - this->m_position;
        for (size_t i = 0; i < iter; i++)
        {
            char value = *(this->m_buffer.data() + this->m_position);
            if (value == delimiter)
            {
                this->m_position += sizeof(char);
                break;
            }
            output += value;
            this->m_position += sizeof(char);
        }
        return output;
    }

    bool FileAccessor::checkReadBounds(uint64_t size) const
    {
        if (this->m_position < 0 || this->m_position + size > this->m_buffer.size())
        {
            return false;
        }
        return true;
    }

    bool FileAccessor::checkOffsetBounds(uint64_t offset) const
    {
        return offset <= this->m_buffer.size();
    }

    void FileAccessor::readBuffer(uint8_t* dest, uint64_t size)
    {
        //CE_PROFILE_FUNC(ReadBufferDataAssert);
        ASSERT(checkReadBounds(size), "Can not read out of buffer's bounds");
        std::memcpy(dest, this->m_buffer.data() + this->m_position, size);
        this->m_position += size;
    }
}
