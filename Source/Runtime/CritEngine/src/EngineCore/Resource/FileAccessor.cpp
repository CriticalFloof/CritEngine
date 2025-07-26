#include "FileAccessor.h"
#include "../Logging/Logger.h"
#include "../Profiler/Profiler.h"

namespace Engine {

    FileAccessor::FileAccessor() 
        : systemIsBigEndian(IsSystemBigEndian())
    { 
    }

    void FileAccessor::OpenFile(const std::filesystem::path& filePath)
    {
        this->file = std::ifstream(filePath, std::ios::binary);

        if (this->file.is_open()) {

            this->file.seekg(0, std::ios::end);
            std::streamsize size = this->file.tellg();
            this->file.seekg(0, std::ios::beg);
            this->buffer.resize(size); // Pre-allocates memory for the entire file

            if (this->file.read(this->buffer.data(), size)) {
                this->position = 0;
                // Successful read
            }
            this->file.close();
            this->ReadAsBigEndian(false);
        }
    }

    // If the value of the first byte is 1, we are in little endian since the least significant byte is stored first
    bool FileAccessor::IsSystemBigEndian() const {
        int val = 1;
        return (!(*(char*)&val == 1));
    }

    void FileAccessor::Seek(const uint64_t offset) {
        ASSERT(CheckOffsetBounds(offset), "Position is out of bounds")

        this->position = offset;
    }

    void FileAccessor::SeekEnd()
    {
        this->position = this->buffer.size();
    }

    uint64_t FileAccessor::GetPosition()
    {
        return this->position;
    }

    uint64_t FileAccessor::GetSize()
    {
        return this->buffer.size();
    }

    bool FileAccessor::IsAtEof() const
    {
        return this->position >= buffer.size();
    }

    void FileAccessor::ReadAsBigEndian(const bool isBigEndian) {
        this->fileIsBigEndian = isBigEndian;
    }

    // This section reads binary data from a char* pointer and stores it for us in a specific interger type
    // After the read, we use pointer arithmetic to ensure the next read starts at the correct position
 
    // For a single byte type, we're simply using a static_cast to avoid unnecessary overhead from memcpy
    uint8_t FileAccessor::ReadUINT8() {
        ASSERT(CheckReadBounds(sizeof(uint8_t)), "Can not read out of buffer's bounds");

        uint8_t value = static_cast<uint8_t>(*(this->buffer.data() + this->position));
        this->position += sizeof(uint8_t);
        return value;
    }

    uint16_t FileAccessor::ReadUINT16() {
        ASSERT(CheckReadBounds(sizeof(uint16_t)), "Can not read out of buffer's bounds");

        uint16_t value;
        std::memcpy(&value, this->buffer.data() + this->position, sizeof(uint16_t));
        this->position += sizeof(uint16_t);

        if (fileIsBigEndian != systemIsBigEndian)
            value = BSWAP_16(value);
        return value;
    }

    uint32_t FileAccessor::ReadUINT32() {
        ASSERT(CheckReadBounds(sizeof(uint32_t)), "Can not read out of buffer's bounds");

        uint32_t value;
        std::memcpy(&value, this->buffer.data() + this->position, sizeof(uint32_t));
        this->position += sizeof(uint32_t);

        if (this->fileIsBigEndian != systemIsBigEndian)
            value = BSWAP_32(value);      
        return value;
    }

    uint64_t FileAccessor::ReadUINT64() {
        ASSERT(CheckReadBounds(sizeof(uint64_t)), "Can not read out of buffer's bounds");

        uint64_t value;
        std::memcpy(&value, this->buffer.data() + this->position, sizeof(uint64_t));
        this->position += sizeof(uint64_t);

        if (this->fileIsBigEndian != systemIsBigEndian)
            value = BSWAP_64(value);      
        return value;
    }

    int8_t FileAccessor::ReadINT8() {
        ASSERT(CheckReadBounds(sizeof(int8_t)), "Can not read out of buffer's bounds");

        int8_t value = static_cast<int8_t>(*(this->buffer.data() + this->position));
        this->position += sizeof(int8_t);
        return value;
    }

    int16_t FileAccessor::ReadINT16() {
        ASSERT(CheckReadBounds(sizeof(int16_t)), "Can not read out of buffer's bounds");

        int16_t value;
        std::memcpy(&value, this->buffer.data() + this->position, sizeof(int16_t));
        this->position += sizeof(int16_t);

        if (this->fileIsBigEndian != systemIsBigEndian)
            value = BSWAP_16(value);      
        return value;
    }

    int32_t FileAccessor::ReadINT32() {
        ASSERT(CheckReadBounds(sizeof(int32_t)), "Can not read out of buffer's bounds");

        int32_t value;
        std::memcpy(&value, this->buffer.data() + this->position, sizeof(int32_t));
        this->position += sizeof(int32_t);

        if (this->fileIsBigEndian != this->systemIsBigEndian)
            value = BSWAP_32(value);     
        return value;
    }

    int64_t FileAccessor::ReadINT64() {
        ASSERT(CheckReadBounds(sizeof(int64_t)), "Can not read out of buffer's bounds");

        int64_t value;
        std::memcpy(&value, this->buffer.data() + this->position, sizeof(int64_t));
        this->position += sizeof(int64_t);

        if (this->fileIsBigEndian != this->systemIsBigEndian)
            value = BSWAP_64(value);   
        return value;
    }

    std::string FileAccessor::ReadLine(char delimiter)
    {
        std::string output = "";

        size_t iter = this->buffer.size() - this->position;
        for (size_t i = 0; i < iter; i++)
        {
            char value = static_cast<char>(*(this->buffer.data() + this->position));
            if (value == delimiter)
            {
                this->position += sizeof(char);
                break;
            }
            output += value;
            this->position += sizeof(char);
        }
        return output;
    }

    bool FileAccessor::CheckReadBounds(uint64_t size) const {
        if (this->position < 0 || this->position + size > this->buffer.size())
        {
            return false;
        }
        return true;
    }

    bool FileAccessor::CheckOffsetBounds(uint64_t offset) const
    {
        return offset <= this->buffer.size();
    }

    void FileAccessor::ReadBuffer(uint8_t* dest, uint64_t size)
    {
        //CE_PROFILE_FUNC(ReadBufferDataAssert);
        ASSERT(CheckReadBounds(size), "Can not read out of buffer's bounds");
        std::memcpy(dest, this->buffer.data() + this->position, size);
        this->position += size;
    }

} 