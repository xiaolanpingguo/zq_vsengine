#pragma once


#include "math/plane.h"
#include "core/memorymanager.h"


namespace zq{


class MATH_API Polygon : public Plane , public MemObject
{
public:
	Polygon();
	~Polygon();
	Polygon(const Vector3 * pPoint,uint32 PointNum);


    inline void Set(const Vector3 *pPoint, uint32 PointNum)
    {
        if (!pPoint || !PointNum)
            return;

        m_PointNum = PointNum;
        m_IndexNum = 3 * (m_PointNum - 2);

        SAFE_DELETEA(m_pPoint);
        SAFE_DELETEA(m_pIndex);
        m_pPoint = MEM_NEW Vector3[m_PointNum];
        m_pIndex = MEM_NEW MATH_USHORT_INDEX[m_IndexNum];
        VSMemcpy(m_pPoint, pPoint, sizeof(Vector3) * PointNum);

        m_Flag = 1;

        uint32 I0, I1, I2;
        for (uint32 nLoop = 0; nLoop < (m_PointNum - 2); nLoop++)
        {
            if (nLoop == 0) { I0 = 0; I1 = 1; I2 = 2; }
            else { I1 = I2; I2++; }

            m_pIndex[(nLoop * 3)] = I0;
            m_pIndex[(nLoop * 3) + 1] = I1;
            m_pIndex[(nLoop * 3) + 2] = I2;
        }


        Vector3 vcEdge0 = m_pPoint[1] - m_pPoint[0];
        Vector3 vcEdge1;
        //��������ƽ��,ֱ���ҵ���ƽ�е������߶�Ϊֹ��
        for (uint32 i = 2; i < PointNum; i++)
        {
            vcEdge1 = m_pPoint[i] - m_pPoint[0];

            vcEdge0.Normalize();
            vcEdge1.Normalize();

            //�����Ƿ�ƽ��
            float dot = vcEdge0.Dot(vcEdge1);
            if (ABS(dot) < MATH_FLOAT_1)
                break;
        } // for

        Vector3 N;
        N.Cross(vcEdge0, vcEdge1);
        Plane::Set(N, m_pPoint[0]);
    }

    inline uint32 GetPointNum()const
    {
        return m_PointNum;
    }

    inline uint32 GetIndexNum()const
    {
        return m_IndexNum;
    }

    inline const Vector3* const GetpPoint()const
    {
        return m_pPoint;
    }

    inline const MATH_USHORT_INDEX* const GetpIndex()const
    {
        return m_pIndex;
    }

    inline uint32 GetFlag()const
    {
        return m_Flag;
    }

	//����
	bool CopyFrom(const Polygon & Polygon);
	//��ת��
	void SwapFaces();
	//��Ͷ���ξ���
	float SquaredDistance(const Vector3 & Point,int& IndexTriangle,float fTriangleParameter[3])const;
	//����κͶ���ξ���
	float SquaredDistance(const Triangle &Triangle,
							int& IndexTriangle,
							float fTriangle1Parameter[3],
							float fTriangle2Parameter[3])const;
	//����κ;��ξ���
	float SquaredDistance(const Rectangle &Rectangle,
							int& IndexTriangle,
							float fTriangleParameter[3],
							float fRectangleParameter[2])const;

	//ֱ�ߺͶ���ξ���
	float SquaredDistance(const Line &Line,int& IndexTriangle,
							float fTriangleParameter[3],float &fLineParameter)const;
	//���ߺͶ���ξ���
	float SquaredDistance(const Ray & Ray,int& IndexTriangle,
							float fTriangleParameter[3],float &fRayParameter)const;
	//�߶κͶ���ξ���
	float SquaredDistance(const Segment& Segment,int& IndexTriangle,
							float fTriangleParameter[3],float &fSegmentParameter)const;
	//����κ�OBB����
	float SquaredDistance(const OBB &OBB,int& IndexTriangle,
							float TriangleParameter[3], float OBBParameter[3])const;
	//����κ���ľ���
	float Distance(const Sphere &Sphere,int& IndexTriangle,
					float fTriangleParameter[3],Vector3 & SpherePoint)const;

	//ƽ��Ͷ���ξ���
	float Distance(const Plane &Plane,int& IndexTriangle,
					Vector3 &TrianglePoint,Vector3 &PlanePoint)const;
	//����κ�AABB����
	float SquaredDistance(const AABB &AABB ,int& IndexTriangle,
							float TriangleParameter[3]
							, float AABBParameter[3])const;

	//����κͶ���ξ���
	float SquaredDistance(const Polygon & Polygon ,int& Index1Triangle,
							float Triangle1Parameter[3],int& Index2Triangle,
							float Triangle2Parameter[3])const;
/********************************RelationWith******************************************/
	//����ֱ��������λ�ù�ϵ bCullΪ�Ƿ�Ϊ�������,�Ƿ��ǳ���,t�����ཻ����
	//MATH_NOINTERSECT VSNTERSECT
	int32 RelationWith(const Line &Line, bool bCull,int32 &iIndexTriangle,float fTriangleParameter[3],
					float &fLineParameter)const;
	//��������������λ�ù�ϵ
	//MATH_NOINTERSECT VSNTERSECT
	int32 RelationWith(const Ray &Ray, bool bCull,int32 &iIndexTriangle,float fTriangleParameter[3],
					float &fRayParameter)const;
	//�����߶�������λ�ù�ϵ
	//MATH_NOINTERSECT VSNTERSECT
	int32 RelationWith(const Segment &Segment, bool bCull,int32 &iIndexTriangle,float fTriangleParameter[3],
					float &fSegmentParameter)const;
	//ƽ��Ͷ����λ�ù�ϵ
	//MATH_VSON MATH_FRONT MATH_BACK MATH_INTERSECT
	int32 RelationWith(const Plane & Plane)const;
	int32 RelationWith(const Plane & Plane,Segment & Segment)const;
	//����κ�������λ�ù�ϵ
	//MATH_NOINTERSECT MATH_INTERSECT
	int32 RelationWith(const Triangle & Triangle)const;
	int32 RelationWith(const Triangle & Triangle,Segment & Segment)const;
	//����κ;���λ�ù�ϵ
	//MATH_NOINTERSECT MATH_INTERSECT
	int32 RelationWith(const Rectangle & Rectangle)const;
	int32 RelationWith(const Rectangle & Rectangle,Segment & Segment)const;
	//����κ�AABBλ�ù�ϵ
	//MATH_NOINTERSECT MATH_INTERSECT MATH_IN
	int32 RelationWith(const AABB &AABB)const;

	//����κ�OBBλ�ù�ϵ
	//MATH_NOINTERSECT MATH_INTERSECT MATH_IN
	int32 RelationWith(const OBB &OBB)const;

	//����κ�Բλ�ù�ϵ
	//MATH_NOINTERSECT MATH_INTERSECT MATH_IN
	int32 RelationWith(const Sphere &Sphere)const;
	/*************************************** Clip *********************************/
	//ƽ����ж����
	void	Clip(const Plane &Plane, 
						Polygon &pFront,
						Polygon &pBack)const;
	//��AABB���ж���������ʺ�AABB
	void	Clip(const AABB &ABBB);

private:

    unsigned int	m_PointNum;		//�����
    unsigned int	m_IndexNum;		//��������
    bool			m_Flag;			// ��־λ
    Vector3		*m_pPoint;		// ���б�
    MATH_USHORT_INDEX	*m_pIndex;		// �����б�
};

}