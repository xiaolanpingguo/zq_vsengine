#pragma once


#include "thirdparty/NVCompress/Include/nvtt.h"
#include "core/system.h"
#include "core/memorymanager.h"
#include "graphic/core/graphic.h"
#include "datastruct/VSArray.h"


namespace zq
{
	//this code is copyed from unreal 


	// Structures required by nv Texture Tools 2 library.
	struct VSNVOutputHandler : public nvtt::OutputHandler
	{
	public:
		VSNVOutputHandler()
		{
		}
		~VSNVOutputHandler()
		{
		}
		void ReserveMemory(uint32 PreAllocateSize )
		{
			//m_CompressData.SetBufferNum(PreAllocateSize);
		}

		virtual void beginImage(int32 size, int32 width, int32 height, int32 depth, int32 face, int32 miplevel)
		{
			VSArray<unsigned char> Temp;
			m_DataBufferArray.AddElement(Temp);
			m_DataBufferArray[m_DataBufferArray.GetNum() - 1].AddBufferNum(size);
		}

		virtual bool writeData(const void * data, int32 size)
		{
			VSArray<unsigned char> Temp;
			Temp.SetBufferNum(size);
			VSMemcpy(Temp.GetBuffer(), data, size);
			m_DataBufferArray[m_DataBufferArray.GetNum() - 1].AddElement(Temp, 0, Temp.GetNum() - 1);
			return true;
		}
		virtual void endImage()
		{

		}
		VSArray<VSArray<unsigned char>> m_DataBufferArray;		
	};

	struct VSNVErrorHandler : public nvtt::ErrorHandler
	{
	public:
		VSNVErrorHandler() : 
		bSuccess(TRUE)
		{

		}

		virtual void error(nvtt::Error e)
		{

			VSOutputDebugString(nvtt::errorString(e));	
			bSuccess = FALSE;
		}

		bool bSuccess;
	};


	/** Helper struct that encapsulates everything needed for the NVidia DXT compression tool. */
	struct GRAPHIC_API VSNVCompression
	{
		enum //NV FORMAT
		{
			NV_ARGB,
			NV_BC1,
			NV_BC2,
			NV_BC3,
			NV_BC4,
			NV_BC5,
			NV_MAX
		};
		/**
		* Initialize the settings and pre-allocate memory for the compressed result.
		*
		* @param	SourceData				Source texture data to DXT compress, in BGRA 8bit per channel unsigned format.
		* @param	PixelFormat				Texture format
		* @param	SizeX					Number of texels along the X-axis
		* @param	SizeY					Number of texels along the Y-axis
		* @param	SRGB					Whether the texture is in SRGB space
		* @param	bIsNormalMap			Whether the texture is a normal map
		*/
		void	Setup( void* SourceData, 
			uint32 PixelFormat, 
			uint32 SizeX, 
			uint32 SizeY, 
			uint32 SizeZ,
			uint32 ArraySize,
			uint32 MipLevel,
			bool bIsNormalMap,bool IsOnlyAlpha = false);

		/** Performs the DXT compression. */
		bool	Process();

		/** Handles the output from the NVidia DXT-compression tool. Contains the resulting data buffer. */
		VSNVOutputHandler			OutputHandler;
		/** Handles any errors from the NVidia DXT-compression tool. */
		VSNVErrorHandler				ErrorHandler;
		/** NVidia object that contains options that describes the input data. */
		nvtt::InputOptions			InputOptions;
		/** NVidia object that contains compression options that describes the compression work. */
		nvtt::CompressionOptions	CompressionOptions;
		/** NVidia object that contains output options. */
		nvtt::OutputOptions			OutputOptions;
		/** NVidia object that performs that DXT compression. */
		nvtt::Compressor			Compressor;
	};

}
