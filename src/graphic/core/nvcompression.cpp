#include "graphic/core/nvcompression.h"
using namespace zq;
void VSNVCompression::Setup( void* SourceData, uint32 PixelFormat, 
	uint32 SizeX, uint32 SizeY, uint32 SizeZ,
	uint32 ArraySize, uint32 MipLevel,
	bool bIsNormalMap, bool IsOnlyAlpha)
{
	nvtt::Format TextureFormat = nvtt::Format_RGBA;
	if (PixelFormat == NV_BC1)
	{
		TextureFormat = nvtt::Format_BC1;
	}
	else if (PixelFormat == NV_BC2)
	{
		TextureFormat = nvtt::Format_BC2;
	}
	else if (PixelFormat == NV_BC3)
	{
		TextureFormat = nvtt::Format_BC3;
	}
	else if (PixelFormat == NV_BC4)
	{
		TextureFormat = nvtt::Format_BC4;
	}
	else if (PixelFormat == NV_BC5)
	{
		TextureFormat = nvtt::Format_BC5;
	}
	InputOptions.setTextureLayout(nvtt::TextureType_2D, SizeX, SizeY, SizeZ);

	// Not generating mips with NVTT, we will pass each mip in and compress it individually
	InputOptions.setMipmapGeneration(true, MipLevel);
	InputOptions.setMipmapData(SourceData, SizeX, SizeY);
	InputOptions.setGamma(1.0f, 1.0f);
	// Only used for mip and normal map generation
	InputOptions.setWrapMode(nvtt::WrapMode_Mirror);
	InputOptions.setFormat(nvtt::InputFormat_BGRA_8UB);

	CompressionOptions.setFormat(TextureFormat);
	if (IsOnlyAlpha)
	{
		///////
	}
	// Highest quality is 2x slower with only a small visual difference
	// Might be worthwhile for normal maps though
	CompressionOptions.setQuality(nvtt::Quality_Production);

	if (bIsNormalMap)
	{
		// Use the weights originally used by nvDXT for normal maps
		//@todo - re-evaluate these
		CompressionOptions.setColorWeights(0.4f, 0.4f, 0.2f);
		InputOptions.setNormalMap(true);
	}
	else
	{
		CompressionOptions.setColorWeights(1, 1, 1);
	}

	Compressor.enableCudaAcceleration(false);
	uint32 uiSize = Compressor.estimateSize(InputOptions, CompressionOptions);
	OutputHandler.ReserveMemory(uiSize);

	// We're not outputting a dds file so disable the header
	OutputOptions.setOutputHeader( false);
	OutputOptions.setOutputHandler( &OutputHandler );
	OutputOptions.setErrorHandler( &ErrorHandler );
}

/** Performs the DXT compression. */
bool VSNVCompression::Process()
{
	const bool bSuccess = Compressor.process(InputOptions, CompressionOptions, OutputOptions);
	return bSuccess && ErrorHandler.bSuccess;
}