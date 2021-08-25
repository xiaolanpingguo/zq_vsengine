#pragma once
#include "graphic/controller/animtree/animblendfunction.h"
namespace zq
{
	class VSStream;
	class GRAPHIC_API VSTwoParamAnimBlend : public VSAnimBlendFunction
	{
		//RTTI
		DECLARE_RTTI
		DECLARE_INITIAL
	public:
		virtual ~VSTwoParamAnimBlend();

		VSTwoParamAnimBlend(const VSUsedName & ShowName,VSAnimTree * pAnimTree);
		void CreateSlot(uint32 uiWidth, uint32 uiHeight);
		virtual bool SetObject(VSObject * pObject);
		static void LineBlendTwoAll(VSTwoParamAnimBlend * pOut, 
			VSAnimFunction * pAnimFunction1, VSAnimFunction * pAnimFunction2,
			VSAnimFunction * pAnimFunction3, VSAnimFunction * pAnimFunction4,
			float fWeight1, float fWeight2);
	protected:
		float m_fParam[2];
		float m_fParamMax[2];
		float m_fParamMin[2];
		VSArray<VSAnimAtom> m_BlendBoneMatrix[2];
		VSAnimAtom m_BlendRootMatrix[2];
		uint32 m_uiWidth;
		uint32 m_uiHeight;
		
		VSTwoParamAnimBlend();
		virtual bool ComputePara(double dAppTime);
		virtual bool ComputeOutBoneMatrix(double dAppTime);
		VSAnimFunction * GetAnimFunction(uint32 i, uint32 j);
	private:
		static void LineBlendTwo(VSArray<VSAnimAtom> *pOut,
			const VSArray<VSAnimAtom> * pIn1, const VSArray<VSAnimAtom> *pIn2, float fWeight);
		static void LineBlendTwo(VSArray<VSAnimAtom> *pOut,
			VSAnimFunction * pAnimFunction1, VSAnimFunction * pAnimFunction2, float fWeight);
		static bool LineBlendTwoRoot(VSAnimAtom &pOut,
			VSAnimFunction * pAnimFunction1, VSAnimFunction * pAnimFunction2, float fWeight);
	};
	DECLARE_Ptr(VSTwoParamAnimBlend);
	VSTYPE_MARCO(VSTwoParamAnimBlend);
}
