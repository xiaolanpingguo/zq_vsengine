#pragma once
#include "graphic/core/graphic.h"
#include "graphic/render/texture/texallstate.h"
#include "graphic/node/nodecomponent/meshcomponent.h"
#include "math/maths.h"
namespace zq
{
	struct GRAPHIC_API StreamElementType
	{
		VSArray<VSResourceProxyBase*> pResourceProxy;
		bool bFree = true;
		bool operator ==(const StreamElementType& SET)
		{
			return true;
		}
	};
	class GRAPHIC_API VSStreamingManager : public MemObject
	{
	public:
		VSStreamingManager();
		~VSStreamingManager();
		static VSStreamingManager* ms_pStreamingManager;
		void Update(double dAppTime);
		static void GetStreamInfo(const AABB& WorldAABB, StreamInformation_TYPE& StreamInformation);
		StreamElementType& GetFreeSlot();
		void ReleaseAllStream();
	protected:
		void GetStreamInfo();
		void DispathStream();
		VSList<StreamElementType> m_StreamObjectList;
	};
}
