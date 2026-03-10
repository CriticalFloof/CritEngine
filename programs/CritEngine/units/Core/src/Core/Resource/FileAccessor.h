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

namespace Engine
{
    class FileAccessor
    {
    public:
        FileAccessor();

        void openFile(const std::filesystem::path& file_path);

        void readAsBigEndian(bool is_big_endian);

        void seek(uint64_t offset);
        void seekEnd();

        void readBuffer(uint8_t* dest, uint64_t size);

        uint8_t readUint8();
        uint16_t readUint16();
        uint32_t readUint32();
        uint64_t readUint64();

        int8_t readInt8();
        int16_t readInt16();
        int32_t readInt32();
        int64_t readInt64();

        std::string readLine(char delimiter = '\n');

        uint64_t getPosition();
        uint64_t getSize();
        bool isAtEof() const;

        bool checkReadBounds(uint64_t size) const;
        bool checkOffsetBounds(uint64_t offset) const;

    private:
        bool isSystemBigEndian() const;
        bool m_fileIsBigEndian = false;
        bool m_systemIsBigEndian = false;

        uint64_t m_position;
        std::ifstream m_file;
        std::vector<char> m_buffer;
    };
}
