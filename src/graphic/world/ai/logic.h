#pragma once
#include "core/memorymanager.h"
#include "graphic/core/name.h"
namespace zq
{
	struct VSLOGIC_INPUT_EVENT 
	{
		uint32 uiInputType;
		uint32 uiEvent;
		uint32 uiKey;
		int32 x;
		int32 y;
		int32 z;
	};
	struct VSMessage
	{
		VSMessage()
		{

		}
		~VSMessage()
		{

		}
		VSUsedName Sender;
		VSUsedName Receiver;
		uint32 MessageType;
		double DispatchTime;
		void * pData;
		uint32 uiDataSize;
	};
}

