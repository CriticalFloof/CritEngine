#include <fstream>
#include <array>

#include "BitmapLoader.h"
#include "../../Logging/Logger.h"
#include "../../Profiler/Profiler.h"


namespace Engine
{
    enum class ColorSpaceEnum : uint32_t
    {
        Lcs_CalibratedRgb,
        Lcs_GmAbsColorimetric,
        Lcs_GmBusiness,
        Lcs_GmGraphics,
        Lcs_GmImages,
        Lcs_sRgb,
        Lcs_WindowsColorSpace,
        ProfileEmbedded,
        ProfileLinked
    };

    struct ColorPaletteCore
    {
        uint8_t blue;
        uint8_t green;
        uint8_t red;
    };

    struct ColorPalette
    {
        uint8_t blue;
        uint8_t green;
        uint8_t red;
        uint8_t alpha;
    };

    struct BitmapFileHeader
    {
        uint16_t signature;
        uint32_t fileSize;
        uint16_t reserved1;
        uint16_t reserved2;
        uint32_t bitmapOffset;
    };

    struct BitmapCoreHeader
    {
        uint32_t headerSize;
        int16_t width;
        int16_t height;
        uint16_t planes;
        uint16_t bitCount;
    };

    struct BitmapInfoHeader
    {
        uint32_t headerSize;
        int32_t width;
        int32_t height;
        uint16_t planes;
        uint16_t bitCount;
        uint32_t compression;
        uint32_t imageSize;
        uint32_t pixelsPerMeterX;
        uint32_t pixelsPerMeterY;
        uint32_t colorsUsed;
        uint32_t importantColors;
        uint32_t redBitMask;
        uint32_t greenBitMask;
        uint32_t blueBitMask;
        uint32_t alphaBitMask;
        uint32_t colorSpaceType;
        uint32_t redX;
        uint32_t redY;
        uint32_t redZ;
        uint32_t greenX;
        uint32_t greenY;
        uint32_t greenZ;
        uint32_t blueX;
        uint32_t blueY;
        uint32_t blueZ;
        uint32_t gammaRed;
        uint32_t gammaGreen;
        uint32_t gammaBlue;
        uint32_t intent;
        uint32_t iccProfileData;
        uint32_t iccProfileSize;
        uint32_t reserved3;
    };

    enum class BitmapHeaderVersion : uint32_t
    {
        Core = 12,
        Info = 40,
        V2Info = 52,
        V3Info = 56,
        V4 = 108,
        V5 = 124
    };

    std::string BitmapLoader::getResourceType()
    {
        return "Image";
    }

    std::shared_ptr<void> BitmapLoader::load(std::filesystem::path filepath)
    {
        CE_PROFILE_FUNC(Load_Bitmap);
        auto file = FileAccessor();
        file.openFile(filepath);

        if (file.getSize() <= 0)
        {
            logError("BitmapLoader", "Failed to open file.");
            return nullptr;
        }

        BitmapFileHeader file_header;
        file_header.signature = file.readUint16();
        file_header.fileSize = file.readUint32();
        file_header.reserved1 = file.readUint16();
        file_header.reserved2 = file.readUint16();
        file_header.bitmapOffset = file.readUint32();

        if (file_header.signature != 0x4d42)
        {
            logError("BitmapLoader", "Bitmap file signature is invalid!");
            return nullptr;
        }

        // We incrementally load the header file, once we reach past the read header size, we'll initialize the rest of the data ourselves.

        BitmapInfoHeader bitmap_header;
        bitmap_header.headerSize = file.readUint32();

        if (!(
            bitmap_header.headerSize == static_cast<uint32_t>(BitmapHeaderVersion::Core) ||
            bitmap_header.headerSize == static_cast<uint32_t>(BitmapHeaderVersion::Info) ||
            bitmap_header.headerSize == static_cast<uint32_t>(BitmapHeaderVersion::V2Info) ||
            bitmap_header.headerSize == static_cast<uint32_t>(BitmapHeaderVersion::V3Info) ||
            bitmap_header.headerSize == static_cast<uint32_t>(BitmapHeaderVersion::V4) ||
            bitmap_header.headerSize == static_cast<uint32_t>(BitmapHeaderVersion::V5)
        ))
        {
            logError("BitmapLoader", "Bitmap header size is invalid!");
            return nullptr;
        }

        if (bitmap_header.headerSize == static_cast<uint32_t>(BitmapHeaderVersion::Core))
        {
            bitmap_header.width = file.readInt16();
            bitmap_header.height = file.readInt16();
            bitmap_header.planes = file.readUint16();
            bitmap_header.bitCount = file.readUint16();
        }

        if (bitmap_header.headerSize >= static_cast<uint32_t>(BitmapHeaderVersion::Info))
        {
            bitmap_header.width = file.readInt32();
            bitmap_header.height = file.readInt32();
            bitmap_header.planes = file.readUint16();
            bitmap_header.bitCount = file.readUint16();
            bitmap_header.compression = file.readUint32();
            bitmap_header.imageSize = file.readUint32();
            bitmap_header.pixelsPerMeterX = file.readUint32();
            bitmap_header.pixelsPerMeterY = file.readUint32();
            bitmap_header.colorsUsed = file.readUint32();
            bitmap_header.importantColors = file.readUint32();
        }
        else
        {
            bitmap_header.compression = 0;
            bitmap_header.imageSize = 0;
            bitmap_header.pixelsPerMeterX = 1;
            bitmap_header.pixelsPerMeterY = 1;
            if (bitmap_header.bitCount >= 16)
            {
                bitmap_header.colorsUsed = 0;
            }
            else
            {
                bitmap_header.colorsUsed = 1 << bitmap_header.bitCount;
            }

            bitmap_header.importantColors = 0;
        }

        if (!(
            bitmap_header.bitCount == 1 ||
            bitmap_header.bitCount == 4 ||
            bitmap_header.bitCount == 8 ||
            bitmap_header.bitCount == 16 ||
            bitmap_header.bitCount == 24 ||
            bitmap_header.bitCount == 32
        ))
        {
            logError("BitmapLoader", "Bits per pixel is invalid, must be 1, 4, 8, 16, 24, or 32.");
            return nullptr;
        }

        if (bitmap_header.headerSize >= static_cast<uint32_t>(BitmapHeaderVersion::V2Info))
        {
            bitmap_header.redBitMask = file.readUint32();
            bitmap_header.greenBitMask = file.readUint32();
            bitmap_header.blueBitMask = file.readUint32();
        }
        else
        {
            if (bitmap_header.bitCount == 16)
            {
                bitmap_header.redBitMask = 0b11111000000000000000000000000000;
                bitmap_header.greenBitMask = 0b00000111111000000000000000000000;
                bitmap_header.blueBitMask = 0b00000000000111110000000000000000;
            }
            bitmap_header.redBitMask = 0b00000000000000000000000000000000;
            bitmap_header.greenBitMask = 0b00000000000000000000000000000000;
            bitmap_header.blueBitMask = 0b00000000000000000000000000000000;
        }

        if (bitmap_header.headerSize >= static_cast<uint32_t>(BitmapHeaderVersion::V3Info))
        {
            bitmap_header.alphaBitMask = file.readUint32();
        }
        else
        {
            bitmap_header.alphaBitMask = 0b00000000000000000000000000000000;
        }

        if (bitmap_header.headerSize >= static_cast<uint32_t>(BitmapHeaderVersion::V4))
        {
            bitmap_header.colorSpaceType = file.readUint32();
            bitmap_header.redX = file.readUint32();
            bitmap_header.redY = file.readUint32();
            bitmap_header.redZ = file.readUint32();
            bitmap_header.greenX = file.readUint32();
            bitmap_header.greenY = file.readUint32();
            bitmap_header.greenZ = file.readUint32();
            bitmap_header.blueX = file.readUint32();
            bitmap_header.blueY = file.readUint32();
            bitmap_header.blueZ = file.readUint32();
            bitmap_header.gammaRed = file.readUint32();
            bitmap_header.gammaGreen = file.readUint32();
            bitmap_header.gammaBlue = file.readUint32();
        }
        else
        {
            bitmap_header.colorSpaceType = 1;
            bitmap_header.redX = 0;
            bitmap_header.redY = 0;
            bitmap_header.redZ = 0;
            bitmap_header.greenX = 0;
            bitmap_header.greenY = 0;
            bitmap_header.greenZ = 0;
            bitmap_header.blueX = 0;
            bitmap_header.blueY = 0;
            bitmap_header.blueZ = 0;
            bitmap_header.gammaRed = 1;
            bitmap_header.gammaGreen = 1;
            bitmap_header.gammaBlue = 1;
        }

        if (bitmap_header.headerSize == static_cast<uint32_t>(BitmapHeaderVersion::V5))
        {
            bitmap_header.intent = file.readUint32();
            bitmap_header.iccProfileData = file.readUint32();
            bitmap_header.iccProfileSize = file.readUint32();
            bitmap_header.reserved3 = file.readUint32();
        }
        else
        {
            bitmap_header.intent = static_cast<uint32_t>(ColorSpaceEnum::Lcs_CalibratedRgb);
            bitmap_header.iccProfileData = 0;
            bitmap_header.iccProfileSize = 0;
            bitmap_header.reserved3 = 0;
        }
        // Read color palette data if applicable.

        uint8_t color_palette_size = static_cast<uint8_t>((file_header.bitmapOffset - 14 - bitmap_header.headerSize) /
            sizeof(bitmap_header.headerSize == static_cast<uint32_t>(BitmapHeaderVersion::Core) ? 3 : 4));

        auto color_palettes = std::array<ColorPalette, 256>();
        if (bitmap_header.bitCount <= 8)
        {
            for (int i = 0; i < color_palette_size && i < (1 << bitmap_header.bitCount); i++)
            {
                if (bitmap_header.headerSize != static_cast<uint32_t>(BitmapHeaderVersion::Core))
                {
                    //4 bytes per entry
                    color_palettes[i].blue = file.readUint8();
                    color_palettes[i].green = file.readUint8();
                    color_palettes[i].red = file.readUint8();
                    color_palettes[i].alpha = file.readUint8();
                }
                else
                {
                    //3 bytes
                    color_palettes[i].blue = file.readUint8();
                    color_palettes[i].green = file.readUint8();
                    color_palettes[i].red = file.readUint8();
                    color_palettes[i].alpha = 255;
                }
            }
        }


        // Prepare for processing color data.
        auto image = std::vector<uint8_t>();
        image.resize(std::abs(bitmap_header.width) * std::abs(bitmap_header.height) * 4);
        std::array<uint8_t, 4> raw_color_data = {0, 0, 0, 0};

        float uint32_row_size = (static_cast<float>(bitmap_header.width) * (static_cast<float>(bitmap_header.bitCount) / 8))
            / sizeof(uint32_t);
        uint8_t row_padding = static_cast<uint8_t>((std::ceil(uint32_row_size) - uint32_row_size) * sizeof(uint32_t));

        uint32_t blue_bit_mask_copy = bitmap_header.blueBitMask;
        uint8_t blue_bit_mask_offset = 0;
        while (blue_bit_mask_copy)
        {
            blue_bit_mask_offset++;
            blue_bit_mask_copy >>= 1;
        }
        blue_bit_mask_offset -= 32 - bitmap_header.bitCount;

        uint32_t green_bit_mask_copy = bitmap_header.greenBitMask;
        uint8_t green_bit_mask_offset = 0;
        while (green_bit_mask_copy)
        {
            green_bit_mask_offset++;
            green_bit_mask_copy >>= 1;
        }
        green_bit_mask_offset -= 32 - bitmap_header.bitCount;

        uint32_t red_bit_mask_copy = bitmap_header.redBitMask;
        uint8_t red_bit_mask_offset = 0;
        while (red_bit_mask_copy)
        {
            red_bit_mask_offset++;
            red_bit_mask_copy >>= 1;
        }
        red_bit_mask_offset -= 32 - bitmap_header.bitCount;

        uint32_t alpha_bit_mask_copy = bitmap_header.alphaBitMask;
        uint8_t alpha_bit_mask_offset = 0;
        while (alpha_bit_mask_copy)
        {
            alpha_bit_mask_offset++;
            alpha_bit_mask_copy >>= 1;
        }
        alpha_bit_mask_offset -= 32 - bitmap_header.bitCount;

        bool use_default_bit_mask = (bitmap_header.blueBitMask | bitmap_header.greenBitMask | bitmap_header.redBitMask |
            bitmap_header.alphaBitMask) == 0;

        // Process color Data.
        file.seek(file_header.bitmapOffset);

        switch (bitmap_header.bitCount)
        {
        case 1:
            {
                for (uint32_t y = 0; y < static_cast<uint32_t>(abs(bitmap_header.height)); y++)
                {
                    for (uint32_t x = 0; x < static_cast<uint32_t>(abs(bitmap_header.width)); x++)
                    {
                        if (x % 8 == 0)
                        {
                            file.readBuffer(raw_color_data.data(), 1);
                        }

                        uint8_t mask = 0b10000000;

                        mask >>= (x % 8);

                        image[(x + bitmap_header.width * y) * 4] = color_palettes[raw_color_data[0] & mask].red;
                        image[(x + bitmap_header.width * y) * 4 + 1] = color_palettes[raw_color_data[0] & mask].green;
                        image[(x + bitmap_header.width * y) * 4 + 2] = color_palettes[raw_color_data[0] & mask].blue;
                        image[(x + bitmap_header.width * y) * 4 + 3] = color_palettes[raw_color_data[0] & mask].alpha;
                    }
                    file.seek(file.getPosition() + row_padding);
                }
                break;
            }
        case 4:
            {
                for (uint32_t y = 0; y < static_cast<uint32_t>(abs(bitmap_header.height)); y++)
                {
                    for (uint32_t x = 0; x < static_cast<uint32_t>(abs(bitmap_header.width)); x++)
                    {
                        if (x % 2 == 0)
                        {
                            file.readBuffer(raw_color_data.data(), 1);
                        }

                        uint8_t mask = 0b11110000;

                        mask >>= bitmap_header.bitCount * (x % 2);

                        uint8_t filtered_index = (raw_color_data[0] & mask) >> (bitmap_header.bitCount * ((x + 1) % 2));

                        image[(x + bitmap_header.width * y) * 4] = color_palettes[filtered_index].red;
                        image[(x + bitmap_header.width * y) * 4 + 1] = color_palettes[filtered_index].green;
                        image[(x + bitmap_header.width * y) * 4 + 2] = color_palettes[filtered_index].blue;
                        image[(x + bitmap_header.width * y) * 4 + 3] = color_palettes[filtered_index].alpha;
                    }
                    file.seek(file.getPosition() + row_padding);
                }
                break;
            }
        case 8:
            {
                for (uint32_t y = 0; y < static_cast<uint32_t>(abs(bitmap_header.height)); y++)
                {
                    for (uint32_t x = 0; x < static_cast<uint32_t>(abs(bitmap_header.width)); x++)
                    {
                        file.readBuffer(raw_color_data.data(), 1);

                        image[(x + bitmap_header.width * y) * 4] = color_palettes[raw_color_data[0]].red;
                        image[(x + bitmap_header.width * y) * 4 + 1] = color_palettes[raw_color_data[0]].green;
                        image[(x + bitmap_header.width * y) * 4 + 2] = color_palettes[raw_color_data[0]].blue;
                        image[(x + bitmap_header.width * y) * 4 + 3] = color_palettes[raw_color_data[0]].alpha;
                    }
                    file.seek(file.getPosition() + row_padding);
                }
                break;
            }
        case 16:
            {
                for (uint32_t y = 0; y < static_cast<uint32_t>(abs(bitmap_header.height)); y++)
                {
                    for (uint32_t x = 0; x < static_cast<uint32_t>(abs(bitmap_header.width)); x++)
                    {
                        file.readBuffer(raw_color_data.data(), 2);

                        uint16_t color =
                            raw_color_data[1] << 8 |
                            raw_color_data[0];

                        // Currently Colors aren't normalized to RGBA8
                        image[(x + bitmap_header.width * y) * 4] = (((color >> green_bit_mask_offset) & bitmap_header.
                            redBitMask) >> (32 - bitmap_header.bitCount));
                        image[(x + bitmap_header.width * y) * 4 + 1] = (((color >> blue_bit_mask_offset) & bitmap_header.
                            greenBitMask) >> (32 - bitmap_header.bitCount));
                        image[(x + bitmap_header.width * y) * 4 + 2] = (((color >> 0) & bitmap_header.blueBitMask) >> (32
                            - bitmap_header.bitCount));
                        image[(x + bitmap_header.width * y) * 4 + 3] = 0;
                    }
                    file.seek(file.getPosition() + row_padding);
                }
                break;
            }
        case 24:
            {
                for (uint32_t y = 0; y < static_cast<uint32_t>(abs(bitmap_header.height)); y++)
                {
                    for (uint32_t x = 0; x < static_cast<uint32_t>(abs(bitmap_header.width)); x++)
                    {
                        file.readBuffer(raw_color_data.data(), 3);

                        if (use_default_bit_mask)
                        {
                            image[(x + bitmap_header.width * y) * 4] = raw_color_data[2];
                            image[(x + bitmap_header.width * y) * 4 + 1] = raw_color_data[1];
                            image[(x + bitmap_header.width * y) * 4 + 2] = raw_color_data[0];
                            image[(x + bitmap_header.width * y) * 4 + 3] = static_cast<uint8_t>(0xFF);
                        }
                        else
                        {
                            uint32_t color =
                                raw_color_data[0] << 16 |
                                raw_color_data[1] << 8 |
                                raw_color_data[2];

                            image[(x + bitmap_header.width * y) * 4] = (color & bitmap_header.redBitMask) >>
                                red_bit_mask_offset;
                            image[(x + bitmap_header.width * y) * 4 + 1] = (color & bitmap_header.greenBitMask) >>
                                green_bit_mask_offset;
                            image[(x + bitmap_header.width * y) * 4 + 2] = (color & bitmap_header.blueBitMask) >>
                                blue_bit_mask_offset;
                            image[(x + bitmap_header.width * y) * 4 + 3] = 0;
                        }
                    }
                    file.seek(file.getPosition() + row_padding);
                }
                break;
            }
        case 32:
            {
                for (uint32_t y = 0; y < static_cast<uint32_t>(abs(bitmap_header.height)); y++)
                {
                    for (uint32_t x = 0; x < static_cast<uint32_t>(abs(bitmap_header.width)); x++)
                    {
                        file.readBuffer(raw_color_data.data(), 4);

                        if (use_default_bit_mask)
                        {
                            image[(x + bitmap_header.width * y) * 4] = raw_color_data[2];
                            image[(x + bitmap_header.width * y) * 4 + 1] = raw_color_data[1];
                            image[(x + bitmap_header.width * y) * 4 + 2] = raw_color_data[0];
                            image[(x + bitmap_header.width * y) * 4 + 3] = raw_color_data[3];
                        }
                        else
                        {
                            uint32_t color =
                                raw_color_data[0] << 24 |
                                raw_color_data[1] << 16 |
                                raw_color_data[2] << 8 |
                                raw_color_data[3];

                            image[(x + bitmap_header.width * y) * 4] = (color & bitmap_header.redBitMask) >>
                                red_bit_mask_offset;
                            image[(x + bitmap_header.width * y) * 4 + 1] = (color & bitmap_header.greenBitMask) >>
                                green_bit_mask_offset;
                            image[(x + bitmap_header.width * y) * 4 + 2] = (color & bitmap_header.blueBitMask) >>
                                blue_bit_mask_offset;
                            image[(x + bitmap_header.width * y) * 4 + 3] = (color & bitmap_header.alphaBitMask) >>
                                alpha_bit_mask_offset;
                        }
                    }
                    file.seek(file.getPosition() + row_padding);
                }
                break;
            }
        }

        logInfo("BitmapLoader", "Texture stored");
        return Texture::create(reinterpret_cast<char*>(image.data()), abs(bitmap_header.width), abs(bitmap_header.height), 4);
    }
}
