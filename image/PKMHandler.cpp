/**
 * Copyright (c) 2006-2021 LOVE Development Team
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 **/

// LOVE
#include "medialoader/image/PKMHandler.h"
#include "medialoader/Exception.h"
#include <cstdint>
#include "medialoader/intswap.h"


namespace medialoader
{

namespace
{

// Big endian to host (and vice versa.)
inline uint16_t swap16big(uint16_t x)
{
#ifdef LOVE_BIG_ENDIAN
	return x;
#else
	return swapuint16(x);
#endif
}

static const uint8_t pkmIdentifier[] = {'P','K','M',' '};

struct PKMHeader
{
	uint8_t identifier[4];
	uint8_t version[2];
	uint16_t textureFormatBig;
	uint16_t extendedWidthBig;
	uint16_t extendedHeightBig;
	uint16_t widthBig;
	uint16_t heightBig;
};

enum PKMTextureFormat
{
	ETC1_RGB_NO_MIPMAPS = 0,
	ETC2PACKAGE_RGB_NO_MIPMAPS,
	ETC2PACKAGE_RGBA_NO_MIPMAPS_OLD,
	ETC2PACKAGE_RGBA_NO_MIPMAPS,
	ETC2PACKAGE_RGBA1_NO_MIPMAPS,
	ETC2PACKAGE_R_NO_MIPMAPS,
	ETC2PACKAGE_RG_NO_MIPMAPS,
	ETC2PACKAGE_R_SIGNED_NO_MIPMAPS,
	ETC2PACKAGE_RG_SIGNED_NO_MIPMAPS
};

static PixelFormat convertFormat(uint16_t texformat)
{
	switch (texformat)
	{
	case ETC1_RGB_NO_MIPMAPS:
		return PIXELFORMAT_ETC1;
	case ETC2PACKAGE_RGB_NO_MIPMAPS:
		return PIXELFORMAT_ETC2_RGB;
	case ETC2PACKAGE_RGBA_NO_MIPMAPS_OLD:
	case ETC2PACKAGE_RGBA_NO_MIPMAPS:
		return PIXELFORMAT_ETC2_RGBA;
	case ETC2PACKAGE_RGBA1_NO_MIPMAPS:
		return PIXELFORMAT_ETC2_RGBA1;
	case ETC2PACKAGE_R_NO_MIPMAPS:
		return PIXELFORMAT_EAC_R;
	case ETC2PACKAGE_RG_NO_MIPMAPS:
		return PIXELFORMAT_EAC_RG;
	case ETC2PACKAGE_R_SIGNED_NO_MIPMAPS:
		return PIXELFORMAT_EAC_Rs;
	case ETC2PACKAGE_RG_SIGNED_NO_MIPMAPS:
		return PIXELFORMAT_EAC_RGs;
	default:
		return PIXELFORMAT_UNKNOWN;
	}
}

} // Anonymous namespace.

bool PKMHandler::canParseCompressed(const char* data, size_t size)
{
	if (size <= sizeof(PKMHeader))
		return false;

	const PKMHeader *header = (const PKMHeader *) data;

	if (memcmp(header->identifier, pkmIdentifier, 4) != 0)
		return false;

	// At the time of this writing, only v1.0 and v2.0 exist.
	if ((header->version[0] != '2' && header->version[0] != '1') || header->version[1] != '0')
		return false;

	return true;
}

CompressedMemory* PKMHandler::parseCompressed(const char* data, size_t size, std::vector<CompressedSlice*> &images, PixelFormat &format, bool &sRGB)
{
	if (!canParseCompressed(data, size))
		throw Exception("Could not decode compressed data (not a PKM file?)");

	PKMHeader header = *(const PKMHeader *) data;

	header.textureFormatBig = swap16big(header.textureFormatBig);
	header.extendedWidthBig = swap16big(header.extendedWidthBig);
	header.extendedHeightBig = swap16big(header.extendedHeightBig);
	header.widthBig = swap16big(header.widthBig);
	header.heightBig = swap16big(header.heightBig);

	PixelFormat cformat = convertFormat(header.textureFormatBig);

	if (cformat == PIXELFORMAT_UNKNOWN)
		throw Exception("Could not parse PKM file: unsupported texture format.");

	// The rest of the file after the header is all texture data.
	size_t totalsize = size - sizeof(PKMHeader);

	CompressedMemory* memory = new CompressedMemory(totalsize);

	// PKM files only store a single mipmap level.
	memcpy(memory->data, (uint8_t *) data + sizeof(PKMHeader), totalsize);

	// TODO: verify whether glCompressedTexImage works properly with the unpadded
	// width and height values (extended == padded.)
	int width = header.widthBig;
	int height = header.heightBig;

	images.push_back(new CompressedSlice(cformat, width, height, memory, 0, totalsize));

	format = cformat;
	sRGB = false;

	return memory;
}

} // medialoader


