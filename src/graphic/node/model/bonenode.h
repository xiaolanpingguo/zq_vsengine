#pragma once


#include "graphic/node/node.h"


namespace zq{


class VSStream;
class GRAPHIC_API VSBoneNode : public VSNode
{
	//PRIORITY
		
	//RTTI
	DECLARE_RTTI;
	DECLARE_INITIAL
public:
	VSBoneNode();
	virtual ~VSBoneNode();
public:
		
	
	virtual void ComputeNodeVisibleSet(VSCuller & Culler,bool bNoCull,double dAppTime);

	//Use for IK
	bool m_bIsEffector;
	Vector3 m_TargetPosInWorld;

	VSArray<bool> m_bAllowTranslation;
	VSArray<float> m_fMinTranslation;
	VSArray<float> m_fMaxTranslation;

	VSArray<bool> m_bAllowRotation;
	VSArray<float> m_fMinRotation;
	VSArray<float> m_fMaxRotation;
		
	float m_fIKWeight;

	VSUsedName m_cName;

	void GetIKMoveAxis(Vector3 Axis[3])const;
	void ComputeIKLocalRotDelta(const Matrix3X3 & WorldRot,Matrix3X3 & LocalRotDelta)const;

	Matrix3X3W m_OffsetMatrix;


	virtual void SetLocalMat(const Matrix3X3W VSMat);
		
public:

	inline const Matrix3X3W & GetBoneOffsetMatrix()const
	{
		return m_OffsetMatrix;
	}


	VSBoneNode * GetBoneNodeFromLevel(const VSUsedName & BoneName);
	uint32 GetAllBoneNum()const;
	void GetAllBoneArray(VSArray<VSBoneNode *> & AllNodeArray);
};
DECLARE_Ptr(VSBoneNode);
VSTYPE_MARCO(VSBoneNode);


}
