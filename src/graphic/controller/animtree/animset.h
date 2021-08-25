#pragma once
#include "graphic/core/object.h"
#include "math/vector3.h"
#include "math/quat.h"
#include "math/matrix3X3w.h"
#include "graphic/core/name.h"
#include "graphic/controller/controller.h"
#include "graphic/core/resource.h"
namespace zq
{
	class VSStream;
	class GRAPHIC_API VSKeyTimeInfo
	{
	public:
		VSKeyTimeInfo(){};
		~VSKeyTimeInfo(){};
		float m_dKeyTime;
	};
	class GRAPHIC_API VSKeyTimeVector : public VSKeyTimeInfo
	{
	public:
		VSKeyTimeVector(){};
		~VSKeyTimeVector(){};
		Vector3 m_Vector;
	};
	class GRAPHIC_API VSKeyTimeReal	:public VSKeyTimeInfo
	{
	public:
		VSKeyTimeReal(){}
		~VSKeyTimeReal(){}
		float	m_Real;
	};
	class GRAPHIC_API VSKeyTimeQuaternion : public VSKeyTimeInfo
	{
	public:
		VSKeyTimeQuaternion(){};
		~VSKeyTimeQuaternion(){};	
		Quat m_Quat;

	};

	class GRAPHIC_API VSKeyTimeVectorCompress : public VSKeyTimeInfo
	{
	public:
		VSKeyTimeVectorCompress(){};
		~VSKeyTimeVectorCompress(){};
		unsigned short m_X;
		unsigned short m_Y;
		unsigned short m_Z;
	};
	class GRAPHIC_API VSKeyTimeRealCompress	:public VSKeyTimeInfo
	{
	public:
		VSKeyTimeRealCompress(){}
		~VSKeyTimeRealCompress(){}
		unsigned short m_Real;
	};
	class GRAPHIC_API VSKeyTimeQuaternionCompress : public VSKeyTimeInfo
	{
	public:
		VSKeyTimeQuaternionCompress(){};
		~VSKeyTimeQuaternionCompress(){};	
		unsigned short m_X;
		unsigned short m_Y;
		unsigned short m_Z;
		unsigned short m_W;
	};



	DECLARE_Ptr(VSBoneKeyCompress);
	class GRAPHIC_API VSBoneKey : public VSObject
	{
		//RTTI
		//RTTI
		DECLARE_RTTI;

		DECLARE_INITIAL
		
	public:
		virtual ~VSBoneKey();

	public:
		VSBoneKey();
		VSArray<VSKeyTimeVector>			m_TranslationArray;
		VSArray<VSKeyTimeVector>			m_ScaleArray;
		VSArray<VSKeyTimeQuaternion>		m_RotatorArray;
		VSUsedName							m_cName;
		void CompressSameFrame();
		void Get(VSBoneKeyCompress * pBoneKeyCompress,
			const Vector3 & MaxTranslation , const Vector3 & MinTranslation ,
			const Vector3 MaxScale,const Vector3 MinScale);
	};
	DECLARE_Ptr(VSBoneKey);
	VSTYPE_MARCO(VSBoneKey);
	
	class GRAPHIC_API VSBoneKeyCompress : public VSObject
	{
		//RTTI
		//RTTI
		DECLARE_RTTI;

		DECLARE_INITIAL

	public:
		virtual ~VSBoneKeyCompress();

	public:
		VSBoneKeyCompress();
		VSArray<VSKeyTimeVectorCompress>			m_TranslationArray;
		VSArray<VSKeyTimeVectorCompress>			m_ScaleArray;
		VSArray<VSKeyTimeQuaternionCompress>		m_RotatorArray;
		VSUsedName									m_cName;
		void Get(VSBoneKey * pBoneKey,
			const Vector3 & MaxTranslation , const Vector3 & MinTranslation ,
			const Vector3 MaxScale,const Vector3 MinScale);
	};
	DECLARE_Ptr(VSBoneKeyCompress);
	VSTYPE_MARCO(VSBoneKeyCompress);

	DECLARE_Proxy(VSAnim);
	class GRAPHIC_API VSAnim : public VSObject,public VSResource
	{
		//RTTI
		DECLARE_RTTI;

		DECLARE_INITIAL
		
	public:
		virtual ~VSAnim();

	public:
		friend class VSResourceManager;
		VSAnim();
		inline bool IsAdditive()const
		{
			return m_pBlendAnim != NULL;
		}
		inline bool IsRootAnim()const
		{
			return m_bRootAnimPlay;
		}

		inline void SetRootMotion(bool bRootAnimPlay)
		{
			m_bRootAnimPlay = bRootAnimPlay;
		}

		VSAnimR * GetBlendAnim()const
		{
			return m_pBlendAnim;
		}
	protected:
		VSArray<VSBoneKeyPtr> m_pBoneKeyArray;
		VSArray<VSBoneKeyCompressPtr> m_pBoneKeyCompressArray;
		float m_fLength;
		bool m_bCompress;
		Vector3 m_MaxCompressTranslation;
		Vector3 m_MinCompressTranslation;
		Vector3 m_MaxCompressScale;
		Vector3 m_MinCompressScale;
		bool m_bRootAnimPlay;
		VSAnimRPtr m_pBlendAnim;
	public:
		// only use fbx
		void Compress();
		void ComputeAnimLength();
		inline float GetAnimLength()const
		{
			return m_fLength;
		}
		void AddBoneKey(VSBoneKey * pBoneKey);
		VSBoneKey * GetBoneKey(const VSUsedName & AnimName)const;
		VSBoneKey * GetBoneKey(uint32 uiIndex)const;
		inline uint32 GetBoneKeyNum()const
		{
			return m_pBoneKeyArray.GetNum();
		}
		virtual bool PostLoad(VSStream* pStream);
		VSUsedName m_cName;

		Vector3 GetTranslation(const VSUsedName & UseName,float fTime,uint32 uiRepeatType = VSController::RT_CLAMP)const;
		Vector3 GetScale(const VSUsedName & UseName,float fTime,uint32 uiRepeatType = VSController::RT_CLAMP)const;
		Quat	  GetQuat(const VSUsedName & UseName,float fTime,uint32 uiRepeatType = VSController::RT_CLAMP)const;
		Matrix3X3W GetMat(const VSUsedName & UseName,float fTime,uint32 uiRepeatType = VSController::RT_CLAMP)const;


		Vector3 GetTranslation(uint32 i,float fTime,uint32 uiRepeatType = VSController::RT_CLAMP)const;
		Vector3 GetScale(uint32 i,float fTime,uint32 uiRepeatType = VSController::RT_CLAMP)const;
		Quat	  GetQuat(uint32 i,float fTime,uint32 uiRepeatType = VSController::RT_CLAMP)const;
		Matrix3X3W GetMat(uint32 i,float fTime,uint32 uiRepeatType = VSController::RT_CLAMP)const;

		DECLARE_RESOURCE(VSAnim, RA_ASYN_LOAD | RA_ASYN_POSTLOAD | RA_ENABLE_GC,RT_ACTION,ACTION, Resource/Anim, VSCacheResource)

	};
	DECLARE_Ptr(VSAnim);
	VSTYPE_MARCO(VSAnim);
	DECLARE_Proxy(VSAnim);

	typedef VSDelegateEvent<void(void)> AddAnimEventType;

	
	class GRAPHIC_API VSAnimSet : public VSObject
	{
		//RTTI
		DECLARE_RTTI;

		DECLARE_INITIAL
	public:
		virtual ~VSAnimSet();
	public:
		VSAnimSet();
	protected:
		VSMap<VSUsedName,VSAnimRPtr> m_pAnimArray;

 	public:
		
		void AddAnim(VSUsedName AnimName,VSAnimR * pAnim);
		VSAnimR * GetAnim(const VSUsedName & AnimName)const;
		VSAnimR * GetAnim(uint32 i)const;
		const VSUsedName & GetAnimName(uint32 i)const;
		uint32 GetAnimIndex(VSAnimR * pAnim)const;

		inline uint32 GetAnimNum()const
		{
			return m_pAnimArray.GetNum();
		}



		AddAnimEventType m_AddAnimEvent;
		
		float GetMaxAnimLength()const;
		
		
	};
	DECLARE_Ptr(VSAnimSet);
	VSTYPE_MARCO(VSAnimSet);	
}
