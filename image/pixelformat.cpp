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

#include "medialoader/image/pixelformat.h"

namespace medialoader
{

bool isPixelFormatCompressed(PixelFormat format)
{
	int iformat = (int) format;
	return iformat >= (int) PIXELFORMAT_DXT1 && iformat < (int) PIXELFORMAT_MAX_ENUM;
}

bool isPixelFormatDepthStencil(PixelFormat format)
{
	int iformat = (int) format;
	return iformat >= (int) PIXELFORMAT_STENCIL8 && iformat <= (int) PIXELFORMAT_DEPTH32F_STENCIL8;
}

bool isPixelFormatDepth(PixelFormat format)
{
	int iformat = (int) format;
	return iformat >= (int) PIXELFORMAT_DEPTH16 && iformat <= (int) PIXELFORMAT_DEPTH32F_STENCIL8;
}

bool isPixelFormatStencil(PixelFormat format)
{
	return format == PIXELFORMAT_STENCIL8 || format == PIXELFORMAT_DEPTH24_STENCIL8 || format == PIXELFORMAT_DEPTH32F_STENCIL8;
}

int getPixelFormatSize(PixelFormat format)
{
	switch (format)
	{
	case PIXELFORMAT_R8:
	case PIXELFORMAT_STENCIL8:
		return 1;
	case PIXELFORMAT_RG8:
	case PIXELFORMAT_R16:
	case PIXELFORMAT_R16F:
	case PIXELFORMAT_LA8:
	case PIXELFORMAT_RGBA4:
	case PIXELFORMAT_RGB5A1:
	case PIXELFORMAT_RGB565:
	case PIXELFORMAT_DEPTH16:
		return 2;
	case PIXELFORMAT_RGBA8:
	case PIXELFORMAT_sRGBA8:
	case PIXELFORMAT_RG16:
	case PIXELFORMAT_RG16F:
	case PIXELFORMAT_R32F:
	case PIXELFORMAT_RGB10A2:
	case PIXELFORMAT_RG11B10F:
	case PIXELFORMAT_DEPTH24:
	case PIXELFORMAT_DEPTH32F:
	case PIXELFORMAT_DEPTH24_STENCIL8:
		return 4;
	case PIXELFORMAT_RGBA16:
	case PIXELFORMAT_RGBA16F:
	case PIXELFORMAT_RG32F:
	case PIXELFORMAT_DEPTH32F_STENCIL8:
		return 8;
	case PIXELFORMAT_RGBA32F:
		return 16;
	default:
		// TODO: compressed formats
		return 0;
	}
}

int getPixelFormatColorComponents(PixelFormat format)
{
	switch (format)
	{
	case PIXELFORMAT_R8:
	case PIXELFORMAT_R16:
	case PIXELFORMAT_R16F:
	case PIXELFORMAT_R32F:
		return 1;
	case PIXELFORMAT_RG8:
	case PIXELFORMAT_RG16:
	case PIXELFORMAT_RG16F:
	case PIXELFORMAT_RG32F:
	case PIXELFORMAT_LA8:
		return 2;
	case PIXELFORMAT_RGB565:
	case PIXELFORMAT_RG11B10F:
		return 3;
	case PIXELFORMAT_RGBA8:
	case PIXELFORMAT_sRGBA8:
	case PIXELFORMAT_RGBA16:
	case PIXELFORMAT_RGBA16F:
	case PIXELFORMAT_RGBA32F:
	case PIXELFORMAT_RGBA4:
	case PIXELFORMAT_RGB5A1:
	case PIXELFORMAT_RGB10A2:
		return 4;
	default:
		return 0;
	}
}

PixelFormat getPixelFormatFromName(std::string name)
{
	if (name == "R8") return PIXELFORMAT_R8;
	if (name == "RG8") return PIXELFORMAT_RG8;
	if (name == "RGBA8") return PIXELFORMAT_RGBA8;
	if (name == "sRGBA8") return PIXELFORMAT_sRGBA8;
	if (name == "R16") return PIXELFORMAT_R16;
	if (name == "RG16") return PIXELFORMAT_RG16;
	if (name == "RGBA16") return PIXELFORMAT_RGBA16;
	if (name == "R16F") return PIXELFORMAT_R16F;
	if (name == "RG16F") return PIXELFORMAT_RG16F;
	if (name == "RGBA16F") return PIXELFORMAT_RGBA16F;
	if (name == "R32F") return PIXELFORMAT_R32F;
	if (name == "RG32F") return PIXELFORMAT_RG32F;
	if (name == "RGBA32F") return PIXELFORMAT_RGBA32F;
	if (name == "LA8") return PIXELFORMAT_LA8;
	if (name == "RGBA4") return PIXELFORMAT_RGBA4;
	if (name == "RGB5A1") return PIXELFORMAT_RGB5A1;
	if (name == "RGB565") return PIXELFORMAT_RGB565;
	if (name == "RGB10A2") return PIXELFORMAT_RGB10A2;
	if (name == "RG11B10F") return PIXELFORMAT_RG11B10F;
	if (name == "STENCIL8") return PIXELFORMAT_STENCIL8;
	if (name == "DEPTH16") return PIXELFORMAT_DEPTH16;
	if (name == "DEPTH24") return PIXELFORMAT_DEPTH24;
	if (name == "DEPTH32F") return PIXELFORMAT_DEPTH32F;
	if (name == "DEPTH24_STENCIL8") return PIXELFORMAT_DEPTH24_STENCIL8;
	if (name == "DEPTH32F_STENCIL8") return PIXELFORMAT_DEPTH32F_STENCIL8;
	if (name == "DXT1") return PIXELFORMAT_DXT1;
	if (name == "DXT3") return PIXELFORMAT_DXT3;
	if (name == "DXT5") return PIXELFORMAT_DXT5;
	if (name == "BC4") return PIXELFORMAT_BC4;
	if (name == "BC4s") return PIXELFORMAT_BC4s;
	if (name == "BC5") return PIXELFORMAT_BC5;
	if (name == "BC5s") return PIXELFORMAT_BC5s;
	if (name == "BC6H") return PIXELFORMAT_BC6H;
	if (name == "BC6Hs") return PIXELFORMAT_BC6Hs;
	if (name == "BC7") return PIXELFORMAT_BC7;
	if (name == "PVR1_RGB2") return PIXELFORMAT_PVR1_RGB2;
	if (name == "PVR1_RGB4") return PIXELFORMAT_PVR1_RGB4;
	if (name == "PVR1_RGBA2") return PIXELFORMAT_PVR1_RGBA2;
	if (name == "PVR1_RGBA4") return PIXELFORMAT_PVR1_RGBA4;
	if (name == "ETC1") return PIXELFORMAT_ETC1;
	if (name == "ETC2_RGB") return PIXELFORMAT_ETC2_RGB;
	if (name == "ETC2_RGBA") return PIXELFORMAT_ETC2_RGBA;
	if (name == "ETC2_RGBA1") return PIXELFORMAT_ETC2_RGBA1;
	if (name == "EAC_R") return PIXELFORMAT_EAC_R;
	if (name == "EAC_Rs") return PIXELFORMAT_EAC_Rs;
	if (name == "EAC_RG") return PIXELFORMAT_EAC_RG;
	if (name == "EAC_RGs") return PIXELFORMAT_EAC_RGs;
	if (name == "ASTC_4x4") return PIXELFORMAT_ASTC_4x4;
	if (name == "ASTC_5x4") return PIXELFORMAT_ASTC_5x4;
	if (name == "ASTC_5x5") return PIXELFORMAT_ASTC_5x5;
	if (name == "ASTC_6x5") return PIXELFORMAT_ASTC_6x5;
	if (name == "ASTC_6x6") return PIXELFORMAT_ASTC_6x6;
	if (name == "ASTC_8x5") return PIXELFORMAT_ASTC_8x5;
	if (name == "ASTC_8x6") return PIXELFORMAT_ASTC_8x6;
	if (name == "ASTC_8x8") return PIXELFORMAT_ASTC_8x8;
	if (name == "ASTC_10x5") return PIXELFORMAT_ASTC_10x5;
	if (name == "ASTC_10x6") return PIXELFORMAT_ASTC_10x6;
	if (name == "ASTC_10x8") return PIXELFORMAT_ASTC_10x8;
	if (name == "ASTC_10x10") return PIXELFORMAT_ASTC_10x10;
	if (name == "ASTC_12x10") return PIXELFORMAT_ASTC_12x10;
	if (name == "ASTC_12x12") return PIXELFORMAT_ASTC_12x12;

	return PIXELFORMAT_UNKNOWN;
}



} // medialoader
