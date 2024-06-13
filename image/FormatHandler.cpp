
#include "medialoader/image/FormatHandler.h"



namespace medialoader
{

FormatHandler::FormatHandler()
{
}

FormatHandler::~FormatHandler()
{
}

bool FormatHandler::canDecode(const char* data, size_t size)
{
	return false;
}

bool FormatHandler::canEncode(PixelFormat /*rawFormat*/, EncodedFormat /*encodedFormat*/)
{
	return false;
}

FormatHandler::DecodedImage FormatHandler::decode(const char* data, size_t size)
{
	return DecodedImage();
}

FormatHandler::EncodedImage FormatHandler::encode(const DecodedImage& /*img*/, EncodedFormat /*format*/)
{
	return EncodedImage();
}

bool FormatHandler::canParseCompressed(const char* data, size_t size)
{
	return false;
}

CompressedMemory* FormatHandler::parseCompressed(const char* data, size_t size, std::vector<CompressedSlice*>& /*images*/, PixelFormat& /*format*/, bool& /*sRGB*/)
{
	return nullptr;
}

void FormatHandler::freeRawPixels(unsigned char *mem)
{
	delete[] mem;
}

} // medialoader
