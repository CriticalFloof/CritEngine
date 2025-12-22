#pragma once

#include <fstream>
#include "ResourceLoader.h"

// Soon we need to define more OS-specific versions of this
#if defined(_MSC_VER)
#define BSWAP_16(x) _byteswap_ushort(x)
#define BSWAP_32(x) _byteswap_ulong(x)
#define BSWAP_64(x) _byteswap_uint64(x)

#elif defined(__linux__)
#include <byteswap.h> 
#define BSWAP_16(x) bswap_16(x)
#define BSWAP_32(x) bswap_32(x)
#define BSWAP_64(x) bswap_64(x)

#endif

namespace Engine {

	class FileAccessor  {

	public:

        FileAccessor();

        void OpenFile(const std::filesystem::path& filePath);

        void ReadAsBigEndian(const bool val);

        void Seek(const uint64_t offset);
        void SeekEnd();

        void ReadBuffer(uint8_t* dest, uint64_t size);

        uint8_t ReadUINT8();
        uint16_t ReadUINT16();
        uint32_t ReadUINT32();
        uint64_t ReadUINT64();

        int8_t ReadINT8();
        int16_t ReadINT16();
        int32_t ReadINT32();
        int64_t ReadINT64();

        std::string ReadLine(char delimiter = '\n');

        uint64_t GetPosition();
        uint64_t GetSize();
        bool IsAtEof() const;

        bool CheckReadBounds(uint64_t size) const;
        bool CheckOffsetBounds(uint64_t offset) const;

	private:
        bool IsSystemBigEndian() const;
        bool fileIsBigEndian = false;
        bool systemIsBigEndian = false;

        uint64_t position;
        std::ifstream file;
        std::vector<char> buffer;

    };
}

