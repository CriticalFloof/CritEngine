#include <fstream>
#include <array>

#include "BitmapLoader.h"
#include "../../Logging/Logger.h"
#include "../../Profiler/Profiler.h"


namespace Engine {

	enum ColorSpaceEnum : uint8_t
	{
		LCS_CALIBRATED_RGB,
		LCS_GM_ABS_COLORIMETRIC,
		LCS_GM_BUSINESS,
		LCS_GM_GRAPHICS,
		LCS_GM_IMAGES,
		LCS_sRGB,
		LCS_WINDOWS_COLOR_SPACE,
		PROFILE_EMBEDDED,
		PROFILE_LINKED
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
		uint32_t ICCProfileData;
		uint32_t ICCProfileSize;
		uint32_t reserved3;
	};

	enum BitmapHeaderVersion
	{
		BITMAPCOREHEADER = 12,
		BITMAPINFOHEADER = 40,
		BITMAPV2INFOHEADER = 52,
		BITMAPV3INFOHEADER = 56,
		BITMAPV4HEADER = 108,
		BITMAPV5HEADER = 124
	};

	std::string BitmapLoader::GetResourceType()
	{
		return "Image";
	}

	std::shared_ptr<void> BitmapLoader::Load(std::filesystem::path filepath)
	{
		CE_PROFILE_FUNC(Load_Bitmap);
		FileAccessor file = FileAccessor();
		file.OpenFile(filepath);

		if (file.GetSize() <= 0)
		{
			LogError("BitmapLoader", "Failed to open file.");
			return nullptr;
		};

		BitmapFileHeader fileHeader;
		fileHeader.signature = file.ReadUINT16();
		fileHeader.fileSize = file.ReadUINT32();
		fileHeader.reserved1 = file.ReadUINT16();
		fileHeader.reserved2 = file.ReadUINT16();
		fileHeader.bitmapOffset = file.ReadUINT32();

		if (fileHeader.signature != 0x4d42)
		{
			LogError("BitmapLoader", "Bitmap file signature is invalid!");
			return nullptr;
		}

		// We incrementally load the header file, once we reach past the read header size, we'll initialize the rest of the data ourselves.

		BitmapInfoHeader bitmapHeader;
		bitmapHeader.headerSize = file.ReadUINT32();

		if (!(
			bitmapHeader.headerSize == BITMAPCOREHEADER ||
			bitmapHeader.headerSize == BITMAPINFOHEADER ||
			bitmapHeader.headerSize == BITMAPV2INFOHEADER ||
			bitmapHeader.headerSize == BITMAPV3INFOHEADER ||
			bitmapHeader.headerSize == BITMAPV4HEADER ||
			bitmapHeader.headerSize == BITMAPV5HEADER
			))
		{
			LogError("BitmapLoader", "Bitmap header size is invalid!");
			return nullptr;
		}

		if (bitmapHeader.headerSize == BITMAPCOREHEADER)
		{
			bitmapHeader.width = file.ReadINT16();
			bitmapHeader.height = file.ReadINT16();
			bitmapHeader.planes = file.ReadUINT16();
			bitmapHeader.bitCount = file.ReadUINT16();
		}

		if (bitmapHeader.headerSize >= BITMAPINFOHEADER)
		{
			bitmapHeader.width = file.ReadINT32();
			bitmapHeader.height = file.ReadINT32();
			bitmapHeader.planes = file.ReadUINT16();
			bitmapHeader.bitCount = file.ReadUINT16();
			bitmapHeader.compression = file.ReadUINT32();
			bitmapHeader.imageSize = file.ReadUINT32();
			bitmapHeader.pixelsPerMeterX = file.ReadUINT32();
			bitmapHeader.pixelsPerMeterY = file.ReadUINT32();
			bitmapHeader.colorsUsed = file.ReadUINT32();
			bitmapHeader.importantColors = file.ReadUINT32();
		}
		else
		{
			bitmapHeader.compression = 0;
			bitmapHeader.imageSize = 0;
			bitmapHeader.pixelsPerMeterX = 1;
			bitmapHeader.pixelsPerMeterY = 1;
			if (bitmapHeader.bitCount >= 16)
			{
				bitmapHeader.colorsUsed = 0;
			}
			else
			{
				bitmapHeader.colorsUsed = 1 << bitmapHeader.bitCount;
			}

			bitmapHeader.importantColors = 0;
		}

		if (!(
			bitmapHeader.bitCount == 1 ||
			bitmapHeader.bitCount == 4 ||
			bitmapHeader.bitCount == 8 ||
			bitmapHeader.bitCount == 16 ||
			bitmapHeader.bitCount == 24 ||
			bitmapHeader.bitCount == 32
			))
		{
			LogError("BitmapLoader", "Bits per pixel is invalid, must be 1, 4, 8, 16, 24, or 32.");
			return nullptr;
		}

		if (bitmapHeader.headerSize >= BITMAPV2INFOHEADER)
		{
			bitmapHeader.redBitMask = file.ReadUINT32();
			bitmapHeader.greenBitMask = file.ReadUINT32();
			bitmapHeader.blueBitMask = file.ReadUINT32();
		}
		else
		{
			if (bitmapHeader.bitCount == 16)
			{
				bitmapHeader.redBitMask = 0b11111000000000000000000000000000;
				bitmapHeader.greenBitMask = 0b00000111111000000000000000000000;
				bitmapHeader.blueBitMask = 0b00000000000111110000000000000000;
			}
			bitmapHeader.redBitMask = 0b00000000000000000000000000000000;
			bitmapHeader.greenBitMask = 0b00000000000000000000000000000000;
			bitmapHeader.blueBitMask = 0b00000000000000000000000000000000;
		}

		if (bitmapHeader.headerSize >= BITMAPV3INFOHEADER)
		{
			bitmapHeader.alphaBitMask = file.ReadUINT32();
		}
		else
		{
			bitmapHeader.alphaBitMask = 0b00000000000000000000000000000000;
		}

		if (bitmapHeader.headerSize >= BITMAPV4HEADER)
		{
			bitmapHeader.colorSpaceType = file.ReadUINT32();
			bitmapHeader.redX = file.ReadUINT32();
			bitmapHeader.redY = file.ReadUINT32();
			bitmapHeader.redZ = file.ReadUINT32();
			bitmapHeader.greenX = file.ReadUINT32();
			bitmapHeader.greenY = file.ReadUINT32();
			bitmapHeader.greenZ = file.ReadUINT32();
			bitmapHeader.blueX = file.ReadUINT32();
			bitmapHeader.blueY = file.ReadUINT32();
			bitmapHeader.blueZ = file.ReadUINT32();
			bitmapHeader.gammaRed = file.ReadUINT32();
			bitmapHeader.gammaGreen = file.ReadUINT32();
			bitmapHeader.gammaBlue = file.ReadUINT32();
		}
		else
		{
			bitmapHeader.colorSpaceType = 1;
			bitmapHeader.redX = 0;
			bitmapHeader.redY = 0;
			bitmapHeader.redZ = 0;
			bitmapHeader.greenX = 0;
			bitmapHeader.greenY = 0;
			bitmapHeader.greenZ = 0;
			bitmapHeader.blueX = 0;
			bitmapHeader.blueY = 0;
			bitmapHeader.blueZ = 0;
			bitmapHeader.gammaRed = 1;
			bitmapHeader.gammaGreen = 1;
			bitmapHeader.gammaBlue = 1;
		}

		if (bitmapHeader.headerSize == BITMAPV5HEADER)
		{
			bitmapHeader.intent = file.ReadUINT32();
			bitmapHeader.ICCProfileData = file.ReadUINT32();
			bitmapHeader.ICCProfileSize = file.ReadUINT32();
			bitmapHeader.reserved3 = file.ReadUINT32();
		}
		else
		{
			bitmapHeader.intent = LCS_CALIBRATED_RGB;
			bitmapHeader.ICCProfileData = 0;
			bitmapHeader.ICCProfileSize = 0;
			bitmapHeader.reserved3 = 0;
		}
		// Read color palette data if applicable.

		uint8_t colorPaletteSize = static_cast<uint8_t>((fileHeader.bitmapOffset - 14 - bitmapHeader.headerSize) / sizeof(bitmapHeader.headerSize == BITMAPCOREHEADER ? 3 : 4));

		std::array<ColorPalette, 256> colorPalette = std::array<ColorPalette, 256>();
		if (bitmapHeader.bitCount <= 8)
		{
			for (int i = 0; i < colorPaletteSize && i < (1 << bitmapHeader.bitCount); i++)
			{
				if (bitmapHeader.headerSize != BITMAPCOREHEADER)
				{
					//4 bytes per entry
					colorPalette[i].blue = file.ReadUINT8();
					colorPalette[i].green = file.ReadUINT8();
					colorPalette[i].red = file.ReadUINT8();
					colorPalette[i].alpha = file.ReadUINT8();
				}
				else
				{
					//3 bytes
					colorPalette[i].blue = file.ReadUINT8();
					colorPalette[i].green = file.ReadUINT8();
					colorPalette[i].red = file.ReadUINT8();
					colorPalette[i].alpha = 255;
				}
			}
		}


		// Prepare for processing color data.
		std::vector<char> image = std::vector<char>();
		image.resize(std::abs(bitmapHeader.width) * std::abs(bitmapHeader.height) * 4);
		std::array<uint8_t, 4> rawColorData = { 0, 0, 0, 0 };

		float uint32RowSize = ((float)bitmapHeader.width * ((float)bitmapHeader.bitCount / 8)) / sizeof(uint32_t);
		uint8_t rowPadding = (uint8_t)((std::ceil(uint32RowSize) - uint32RowSize) * sizeof(uint32_t));

		uint32_t blueBitMaskCopy = bitmapHeader.blueBitMask;
		uint8_t blueBitMaskOffset = 0;
		while (blueBitMaskCopy)
		{
			blueBitMaskOffset++;
			blueBitMaskCopy >>= 1;
		}
		blueBitMaskOffset -= 32 - bitmapHeader.bitCount;

		uint32_t greenBitMaskCopy = bitmapHeader.greenBitMask;
		uint8_t greenBitMaskOffset = 0;
		while (greenBitMaskCopy)
		{
			greenBitMaskOffset++;
			greenBitMaskCopy >>= 1;
		}
		greenBitMaskOffset -= 32 - bitmapHeader.bitCount;

		uint32_t redBitMaskCopy = bitmapHeader.redBitMask;
		uint8_t redBitMaskOffset = 0;
		while (redBitMaskCopy)
		{
			redBitMaskOffset++;
			redBitMaskCopy >>= 1;
		}
		redBitMaskOffset -= 32 - bitmapHeader.bitCount;

		uint32_t alphaBitMaskCopy = bitmapHeader.alphaBitMask;
		uint8_t alphaBitMaskOffset = 0;
		while (alphaBitMaskCopy)
		{
			alphaBitMaskOffset++;
			alphaBitMaskCopy >>= 1;
		}
		alphaBitMaskOffset -= 32 - bitmapHeader.bitCount;

		bool useDefaultBitMask = (bitmapHeader.blueBitMask | bitmapHeader.greenBitMask | bitmapHeader.redBitMask | bitmapHeader.alphaBitMask) == 0;

		// Process color Data.
		file.Seek(fileHeader.bitmapOffset);

		switch(bitmapHeader.bitCount)
		{
			case 1:
			{
				for (uint32_t y = 0; y < (uint32_t)abs(bitmapHeader.height); y++)
				{
					for (uint32_t x = 0; x < (uint32_t)abs(bitmapHeader.width); x++)
					{
						if (x % 8 == 0)
						{
							file.ReadBuffer(rawColorData.data(), 1);
						}

						uint8_t mask = 0b10000000;

						mask >>= (x % 8);

						image[(x + bitmapHeader.width * y) * 4] = colorPalette[rawColorData[0] & mask].red;
						image[(x + bitmapHeader.width * y) * 4 + 1] = colorPalette[rawColorData[0] & mask].green;
						image[(x + bitmapHeader.width * y) * 4 + 2] = colorPalette[rawColorData[0] & mask].blue;
						image[(x + bitmapHeader.width * y) * 4 + 3] = colorPalette[rawColorData[0] & mask].alpha;
					}
					file.Seek(file.GetPosition() + rowPadding);
				}
				break;
			}
			case 4:
			{
				for (uint32_t y = 0; y < (uint32_t)abs(bitmapHeader.height); y++)
				{
					for (uint32_t x = 0; x < (uint32_t)abs(bitmapHeader.width); x++)
					{
						if (x % 2 == 0)
						{
							file.ReadBuffer(rawColorData.data(), 1);
						}

						uint8_t mask = 0b11110000;

						mask >>= bitmapHeader.bitCount * (x % 2);

						uint8_t filteredIndex = (rawColorData[0] & mask) >> (bitmapHeader.bitCount * ((x + 1) % 2));

						image[(x + bitmapHeader.width * y) * 4] = colorPalette[filteredIndex].red;
						image[(x + bitmapHeader.width * y) * 4 + 1] = colorPalette[filteredIndex].green;
						image[(x + bitmapHeader.width * y) * 4 + 2] = colorPalette[filteredIndex].blue;
						image[(x + bitmapHeader.width * y) * 4 + 3] = colorPalette[filteredIndex].alpha;
					}
					file.Seek(file.GetPosition() + rowPadding);
				}
				break;
			}
			case 8:
			{
				for (uint32_t y = 0; y < (uint32_t)abs(bitmapHeader.height); y++)
				{
					for (uint32_t x = 0; x < (uint32_t)abs(bitmapHeader.width); x++)
					{
						file.ReadBuffer(rawColorData.data(), 1);

						image[(x + bitmapHeader.width * y) * 4] = colorPalette[rawColorData[0]].red;
						image[(x + bitmapHeader.width * y) * 4 + 1] = colorPalette[rawColorData[0]].green;
						image[(x + bitmapHeader.width * y) * 4 + 2] = colorPalette[rawColorData[0]].blue;
						image[(x + bitmapHeader.width * y) * 4 + 3] = colorPalette[rawColorData[0]].alpha;
					}
					file.Seek(file.GetPosition() + rowPadding);
				}
				break;
			}
			case 16:
			{
				for (uint32_t y = 0; y < (uint32_t)abs(bitmapHeader.height); y++)
				{
					for (uint32_t x = 0; x < (uint32_t)abs(bitmapHeader.width); x++)
					{
						file.ReadBuffer(rawColorData.data(), 2);

						uint16_t color =
							rawColorData[1] << 8 |
							rawColorData[0];

						// Currently Colors aren't normalized to RGBA8
						image[(x + bitmapHeader.width * y) * 4] = (((color >> greenBitMaskOffset) & bitmapHeader.redBitMask) >> (32 - bitmapHeader.bitCount));
						image[(x + bitmapHeader.width * y) * 4 + 1] = (((color >> blueBitMaskOffset) & bitmapHeader.greenBitMask) >> (32 - bitmapHeader.bitCount));
						image[(x + bitmapHeader.width * y) * 4 + 2] = (((color >> 0) & bitmapHeader.blueBitMask) >> (32 - bitmapHeader.bitCount));
						image[(x + bitmapHeader.width * y) * 4 + 3] = 0;
					}
					file.Seek(file.GetPosition() + rowPadding);
				}
				break;
			}
			case 24:
			{
				for (uint32_t y = 0; y < (uint32_t)abs(bitmapHeader.height); y++)
				{
					for (uint32_t x = 0; x < (uint32_t)abs(bitmapHeader.width); x++)
					{
						file.ReadBuffer(rawColorData.data(), 3);

						if (useDefaultBitMask)
						{
							image[(x + bitmapHeader.width * y) * 4] = rawColorData[2];
							image[(x + bitmapHeader.width * y) * 4 + 1] = rawColorData[1];
							image[(x + bitmapHeader.width * y) * 4 + 2] = rawColorData[0];
							image[(x + bitmapHeader.width * y) * 4 + 3] = (uint8_t)0xFF;
						}
						else
						{
							uint32_t color =
								rawColorData[0] << 16 |
								rawColorData[1] << 8 |
								rawColorData[2];

							image[(x + bitmapHeader.width * y) * 4] = (color & bitmapHeader.redBitMask) >> redBitMaskOffset;
							image[(x + bitmapHeader.width * y) * 4 + 1] = (color & bitmapHeader.greenBitMask) >> greenBitMaskOffset;
							image[(x + bitmapHeader.width * y) * 4 + 2] = (color & bitmapHeader.blueBitMask) >> blueBitMaskOffset;
							image[(x + bitmapHeader.width * y) * 4 + 3] = 0;
						}
					}
					file.Seek(file.GetPosition() + rowPadding);
				}
				break;
			}
			case 32:
			{
				for (uint32_t y = 0; y < (uint32_t)abs(bitmapHeader.height); y++)
				{
					for (uint32_t x = 0; x < (uint32_t)abs(bitmapHeader.width); x++)
					{
						file.ReadBuffer(rawColorData.data(), 4);

						if (useDefaultBitMask)
						{
							image[(x + bitmapHeader.width * y) * 4] = rawColorData[2];
							image[(x + bitmapHeader.width * y) * 4 + 1] = rawColorData[1];
							image[(x + bitmapHeader.width * y) * 4 + 2] = rawColorData[0];
							image[(x + bitmapHeader.width * y) * 4 + 3] = rawColorData[3];
						}
						else
						{
							uint32_t color =
								rawColorData[0] << 24 |
								rawColorData[1] << 16 |
								rawColorData[2] << 8 |
								rawColorData[3];

							image[(x + bitmapHeader.width * y) * 4] = (color & bitmapHeader.redBitMask) >> redBitMaskOffset;
							image[(x + bitmapHeader.width * y) * 4 + 1] = (color & bitmapHeader.greenBitMask) >> greenBitMaskOffset;
							image[(x + bitmapHeader.width * y) * 4 + 2] = (color & bitmapHeader.blueBitMask) >> blueBitMaskOffset;
							image[(x + bitmapHeader.width * y) * 4 + 3] = (color & bitmapHeader.alphaBitMask) >> alphaBitMaskOffset;
						}
					}
					file.Seek(file.GetPosition() + rowPadding);
				}
				break;
			}
		}

		LogInfo("BitmapLoader", "Texture stored");
		return Texture::Create(image.data(), abs(bitmapHeader.width), abs(bitmapHeader.height), 4);
	}
}