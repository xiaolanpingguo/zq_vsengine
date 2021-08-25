#pragma once
//#include "datastruct/VSArray.h"
#include "graphic/core/object.h"
#include "math/vector3.h"
#include "graphic/core/transform.h"
#include "math/vector3w.h"
#include "graphic/core/resource.h"
namespace zq
{
//#define  TYPE_ADD_TO_UI
	template<typename T> struct TIsVSIntType { enum { Value = false }; };
	template<> struct TIsVSIntType<int> { enum { Value = true }; };

	template<typename T> struct TIsVSUintType { enum { Value = false }; };
	template<> struct TIsVSUintType<unsigned int> { enum { Value = true }; };

	template<typename T> struct TIsVSUCharType { enum { Value = false }; };
	template<> struct TIsVSUCharType<unsigned char> { enum { Value = true }; };

	template<typename T> struct TIsVSTCharType { enum { Value = false }; };
	template<> struct TIsVSTCharType<char> { enum { Value = true }; };

	template<typename T> struct TIsVSRealType { enum { Value = false }; };
	template<> struct TIsVSRealType<float> { enum { Value = true }; };

	class GRAPHIC_API VSEditorElement
	{
	public:
		VSEditorElement()
		{
		}
		virtual ~VSEditorElement() = 0
		{

		}
		virtual bool IsCollection()
		{
			return false;
		}
	protected:
	};
	class VSEditorProperty;
	class GRAPHIC_API VSEditorSingle : public VSEditorElement
	{
	public:
		enum CUSTOM_UI_TYPE
		{
			CUT_CHECK,
			CUT_LABLE,
			CUT_TEXT,
			CUT_COMBO,
			CUT_SLIDER,
			CUT_COLOR,
			CUT_VIEW,
			CUT_COLLECTION
		};
		VSEditorSingle(VSString & Name)
		{
			m_Name = Name;
			m_pOwner = NULL;
		}
		virtual ~VSEditorSingle() = 0
		{

		}
		void SetOwner(VSEditorProperty * pOwner)
		{
			m_pOwner = pOwner;
		}	
		virtual void SetValue(void * pValue) = 0;
		VSString & GetName()
		{
			return m_Name;
		}
	protected:
		VSEditorProperty *m_pOwner;
		VSString  m_Name;
	};
	class GRAPHIC_API VSECheckBox : public VSEditorSingle
	{
	public:
		VSECheckBox(VSString & Name):VSEditorSingle(Name)
		{

		}
		virtual ~VSECheckBox() = 0
		{

		}
		virtual void CallBackValue(bool Value);
		
	};
// 	class QTCheckBox : public VSECheckBox
// 	{
// 	protected:
// 		QTCheckBox(VSString & Name) :VSECheckBox(Name)
// 		{
// 			m_pCheckBox = MEM_NEW QCheckBox(Name);
// 			connect(m_pCheckBox, SIGNAL(clicked()), this, SLOT(Slot1()));
// 		}
// 		virtual ~VSECheckBox() = 0
// 		{
// 			SAFE_DELETE(m_pCheckBox);
// 		}
// 		void Slot1()
// 		{
// 			CallBackValue(m_pCheckBox->GetChecked());
// 		}
// 		virtual void SetValue(void * pValue)
// 		{
// 			m_pCheckBox->SetChecked(*((bool *)pValue))
// 		}
// 		QCheckBox * m_pCheckBox;
// 	}

	class GRAPHIC_API VSELable : public VSEditorSingle
	{
	public:
		VSELable(VSString & Name):VSEditorSingle(Name)
		{

		}
		virtual ~VSELable() = 0
		{

		}
		virtual void CallBackValue(VSString & Str);
		
	};
	class GRAPHIC_API VSECombo : public VSEditorSingle 
	{
	public:
		VSECombo(VSString & Name):VSEditorSingle(Name)
		{

		}
		virtual ~VSECombo() = 0
		{

		}
		virtual void AddOption(VSString & VSString) = 0;

		virtual void CallBackValue(VSString & Str);
		virtual void AddOption(VSArray<VSString> & VS) = 0;
	};
	class GRAPHIC_API VSESlider : public VSEditorSingle // unsigned int
	{
	public:
		VSESlider(VSString & Name):VSEditorSingle(Name)
		{

		}
		virtual ~VSESlider() = 0
		{

		}
		virtual void SetRange(uint32 uiMin,uint32 uiMax,uint32 uiStep) = 0
		{
			m_uiMax = uiMax;
			m_uiMin = uiMin;
			m_uiStep = uiStep;
		}
		uint32 m_uiMax;
		uint32 m_uiMin;
		uint32 m_uiStep;
		virtual void CallBackValue(uint32 uiValue);
	};
	class GRAPHIC_API VSEViewWindow : public VSEditorSingle
	{
	
	};
	class GRAPHIC_API VSEColorTable : public VSEditorSingle
	{
	public:
		VSEColorTable(VSString & Name):VSEditorSingle(Name)
		{

		}
		virtual ~VSEColorTable()
		{

		}
		virtual void CallBackValue(VSColorRGBA& Value);
	};
	class GRAPHIC_API VSEText : public VSEditorSingle // float
	{
	public:
		VSEText(VSString & Name):VSEditorSingle(Name)
		{

		}
		virtual ~VSEText()
		{

		}
		virtual void CallBackValue(VSString & Str);
	};
	
	class GRAPHIC_API VSECollection : public VSEditorSingle
	{
	public:
		VSECollection(VSString & Name):VSEditorSingle(Name)
		{
		}
		~VSECollection()
		{

		}
		virtual void AddElement(VSEditorElement * pElement)
		{
			ChildElement.AddElement(pElement);
		}
		VSArray<VSEditorElement *> ChildElement;
		virtual bool IsCollection()
		{
			return true;
		}
	};

	typedef VSEditorSingle* (*CreateEditorUIProperty)(VSEditorSingle::CUSTOM_UI_TYPE type,VSString Name);
	class GRAPHIC_API VSEditorUIPropertyCreator
	{
	public:
		VSEditorUIPropertyCreator();
		~VSEditorUIPropertyCreator();
		static VSEditorUIPropertyCreator& GetInstance();

		void Set(CreateEditorUIProperty pCreate);
		VSEditorSingle * CreateUIProperty(VSEditorSingle::CUSTOM_UI_TYPE type,VSString Name);
	protected:
		CreateEditorUIProperty m_pCreate;
	};
// 	VSEditorSingle* CreateEditorUIProperty(VSEditorSingle::CUSTOM_UI_TYPE type, VSString Name)
// 	{
// 		if (type == VSEditorSingle::CUSTOM_UI_TYPE::CUT_CHECK)
// 		{
// 			return MEM_NEW QTCheckBox(Name);
// 		}
// 		else if (type == VSEditorSingle::CUSTOM_UI_TYPE::CUT_TEXT)
// 		{
// 			return MEM_NEW QTText(Name);
// 		}
// 		else if (type == VSEditorSingle::CUSTOM_UI_TYPE::CUT_COLOR)
// 		{
// 			return MEM_NEW QTColor(Name);
// 		}
// 		.........................................................................
// 	}
	// you must set this macro
#define SETCreateEditorUIProperty(F) VSEditorUIPropertyCreator::GetInstance().Set(F);
#define  CREATE_UI_PROPERTY(type,Name) VSEditorUIPropertyCreator::GetInstance().CreateUIProperty(type,Name);

	class GRAPHIC_API VSEditorProperty : public VSEditorElement
	{
	public:
		VSEditorProperty(VSString & Name,VSObject * pOwner)
		{
			m_pName = (VSELable  *)CREATE_UI_PROPERTY(VSEditorSingle::CUT_LABLE,Name);
			m_pName->SetOwner(this);
			m_pName->SetValue((void*)&Name);
			m_pOwner = pOwner;
		}
		virtual ~VSEditorProperty()
		{
			SAFE_DELETE(m_pName);
			m_pOwner = NULL;
		}
		virtual void SetValue() = 0;
		virtual bool CallBackValue(VSEditorSingle * pElem, void * pValue);
	protected:
		VSELable * m_pName;
		VSObject * m_pOwner;
	};
	
	

	class GRAPHIC_API VSEBoolProperty : public VSEditorProperty
	{
	public:
		VSEBoolProperty(bool * b,VSString & Name,VSObject * pOwner)
			:VSEditorProperty(Name,pOwner)
		{
			m_pCheckBox = (VSECheckBox *)CREATE_UI_PROPERTY(VSEditorSingle::CUT_CHECK,Name);
			m_pb = b;
			m_pCheckBox->SetOwner(this);
			SetValue();
		}
		virtual void SetValue()
		{		
			m_pCheckBox->SetValue((void *)m_pb);
		}
		virtual ~VSEBoolProperty()
		{
			SAFE_DELETE(m_pCheckBox);
		}
		virtual bool CallBackValue(VSEditorSingle * pElem, void * pValue)
		{
			if (pElem == m_pCheckBox)
			{
				(*m_pb) = *((bool *)pValue);
				SetValue();
				VSEditorProperty::CallBackValue(pElem, pValue);
				return true;
			}
			
			return false;
		}
	protected:
		VSECheckBox * m_pCheckBox;
		bool *m_pb;
	};
	class GRAPHIC_API VSEResourceProperty : public VSEditorProperty
	{
	public: 
		VSEResourceProperty(VSResourceProxyBasePtr & pResource,VSString & Name,VSObject * pOwner)
			:VSEditorProperty(Name,pOwner),m_pResource(pResource)
		{
			m_pView = (VSEViewWindow  *)CREATE_UI_PROPERTY(VSEditorSingle::CUT_VIEW,Name);
			m_pView->SetOwner(this);
			SetValue();
		}
		virtual void SetValue()
		{
			m_pView->SetValue((void *)m_pResource);
		}
		virtual ~VSEResourceProperty()
		{
			SAFE_DELETE(m_pView);
		}
		virtual bool CallBackValue(VSEditorSingle * pElem, void * pValue)
		{
			if (pElem == m_pView)
			{
				m_pResource = *((VSResourceProxyBasePtr *)pValue);
				SetValue();
				VSEditorProperty::CallBackValue(pElem, pValue);
				return true;
			}	
			return false;
		}
	protected:
		VSEViewWindow * m_pView;
		VSResourceProxyBasePtr & m_pResource;
	};

	class GRAPHIC_API VSEColorProperty : public VSEditorProperty
	{
	public:
		VSEColorProperty(VSColorRGBA * pColor,VSString & Name,VSObject * pOwner)
			:VSEditorProperty(Name,pOwner)
		{
			m_pColorTable = (VSEColorTable *)CREATE_UI_PROPERTY(VSEditorSingle::CUT_COLOR,Name);
			m_pColorTable->SetOwner(this);
			m_pColor = pColor;
			SetValue();
		}
		virtual void SetValue()
		{
			m_pColorTable->SetValue((void *)m_pColor);
		}
		virtual ~VSEColorProperty()
		{
			SAFE_DELETE(m_pColorTable);
		}
		virtual bool CallBackValue(VSEditorSingle * pElem, void * pValue)
		{
			if (pElem == m_pColorTable)
			{
				*m_pColor = *((VSColorRGBA *)pValue);
				SetValue();
				VSEditorProperty::CallBackValue(pElem, pValue);
				return true;
			}		
			return false;
		}
	protected:
		VSEColorTable * m_pColorTable;
		VSColorRGBA * m_pColor;
	};

	class GRAPHIC_API VSEEnumProperty : public VSEditorProperty
	{
	public:
		VSEEnumProperty(uint32 * pData,VSString & Name,VSObject * pOwner)
			:VSEditorProperty(Name,pOwner)
		{
			m_pData = pData;
			m_pCombo = (VSECombo *)CREATE_UI_PROPERTY(VSEditorSingle::CUT_COMBO,Name);
			m_pCombo->SetOwner(this);
			SetValue();
		}
		virtual void SetValue()
		{
			m_pCombo->SetValue((void *)m_pData);
		}
		virtual ~VSEEnumProperty()
		{
			SAFE_DELETE(m_pCombo);
		}
		virtual bool CallBackValue(VSEditorSingle * pElem, void * pValue)
		{
			if (pElem == m_pCombo)
			{
				*m_pData = *((uint32 *)pValue);
				SetValue();
				VSEditorProperty::CallBackValue(pElem, pValue);
				return true;
			}
			
			return false;
		}
		void AddEnumString(VSArray<VSString>& AS)
		{
			m_pCombo->AddOption(AS);
		}
	protected:
		VSECombo * m_pCombo;
		uint32 *m_pData;
	};
	template<typename T>
	class GRAPHIC_API VSEValueProperty : public VSEditorProperty
	{
	public:
		VSEValueProperty(T * pData,VSString & Name, VSObject * pOwner, bool bRange, T& Max, T& Min, T& Step)
			:VSEditorProperty(Name,pOwner)
		{
			VSMAC_ASSERT(pData != NULL);
			m_pText = (VSEText *)CREATE_UI_PROPERTY(VSEditorSingle::CUT_TEXT,Name);
			m_pText->SetOwner(this);
			m_fStep = Step;
			if (bRange)
			{
				if (Min > Max)
				{
					Swap(Max, Min);
				}
				VSMAC_ASSERT(Max - Min > Step);
				m_pSlider = (VSESlider *)CREATE_UI_PROPERTY(VSEditorSingle::CUT_SLIDER,Name);
				m_pSlider->SetOwner(this);
				m_pSlider->SetRange(0, int((Max - Min)/Step),1);
			}
			m_Min = Min;
			m_Max = Max;
			m_pData = pData;
#ifdef TYPE_ADD_TO_UI
			SetValue();
#endif
		}
		virtual void GetValueString(VSString& IntString)
		{
#ifdef TYPE_ADD_TO_UI
			if (TIsVSIntType<T>::Value)
			{
				IntString = IntToString(*m_pData);
			}
			else if (TIsVSUintType<T>::Value)
			{
				IntString = IntToString(*m_pData);
			}
			else if (TIsVSUCharType<T>::Value)
			{
				IntString = IntToString(*m_pData);
			}
			else if (TIsVSTCharType<T>::Value)
			{
				char tData[2];
				tData[0] = *m_pData;
				tData[1] = _T('\0');
				VSString Temp(tData);
				IntString = Temp;
			}
			else if (TIsVSRealType<T>::Value)
			{
				IntString = RealToString(*m_pData);
			}
#endif
		}
		virtual T GetStringValue(VSString& IntString)
		{
#ifdef TYPE_ADD_TO_UI
			if (TIsVSIntType<T>::Value)
			{
				return (T)StringToInt(IntString);
			}
			else if (TIsVSUintType<T>::Value)
			{
				return (T)StringToInt(IntString);
			}
			else if (TIsVSUCharType<T>::Value)
			{
				return (T)StringToInt(IntString);
			}
			else if (TIsVSTCharType<T>::Value)
			{
				return (T)*IntString.GetBuffer();
			}
			else if (TIsVSRealType<T>::Value)
			{
				return (T)StringToReal(IntString);
			}
#endif
			return T();
		}
		virtual void SetValue()
		{
			VSString IntString;
			GetValueString(IntString);
			m_pText->SetValue((void *)&IntString);
			if (m_pSlider)
			{
				T Value = (*m_pData - m_Min) / m_fStep;
				m_pSlider->SetValue((void *)&Value);
			}
		}
		virtual ~VSEValueProperty()
		{
			SAFE_DELETE(m_pSlider);
			SAFE_DELETE(m_pText);
		}
		virtual bool CallBackValue(VSEditorSingle * pElem, void * pValue)
		{
			if (pElem == m_pText)
			{
				VSString Data = *(VSString *)pValue;
				T Value = GetStringValue(Data);
				if (m_pSlider)
				{
					if (Value < m_Min)
					{
						Value = m_Min;
					}
					else if (Value > m_Max)
					{
						Value = m_Max;
					}
					*m_pData = Value;
				}
				else
				{
					*m_pData = Value;
				}
				SetValue();
				VSEValueProperty::CallBackValue(pElem, pValue);
				return true;
			}
			else if (m_pSlider && pElem == m_pSlider)
			{
				int32 Value = *(int32 *)pValue;
				*m_pData = ((T)(Value * m_fStep)) + m_Min;
				SetValue();
				VSEValueProperty::CallBackValue(pElem, pValue);
			}

			return false;
		}
public:
		VSESlider * m_pSlider;	
		VSEText	  * m_pText; 
		T m_fStep;
		T m_Max;
		T m_Min;
		T * m_pData;
	};
#ifndef TYPE_ADD_TO_UI
	class GRAPHIC_API VSEIntProperty : public VSEValueProperty<int>
	{
	public:
		VSEIntProperty(int32 * pData,VSString & Name,VSObject * pOwner,bool bRange,int32 Max,int32 Min,int32 Step)
			:VSEValueProperty(pData,Name, pOwner, bRange,Max,Min,Step)
		{
			SetValue();
		}
		virtual ~VSEIntProperty()
		{

		}
		virtual void GetValueString(VSString& IntString)
		{
			IntString = IntToString(*m_pData);
		}
		virtual int32 GetStringValue(VSString& IntString)
		{
			return StringToInt(IntString);
		}
	};
	class GRAPHIC_API VSEUnsignedIntProperty : public VSEValueProperty<unsigned int>
	{
	public:
		VSEUnsignedIntProperty(uint32 * pData, VSString & Name, VSObject * pOwner, bool bRange, uint32 Max, uint32 Min, uint32 Step)
			:VSEValueProperty(pData, Name, pOwner, bRange, Max, Min, Step)
		{
			SetValue();		
		}
		virtual ~VSEUnsignedIntProperty()
		{
		}
		virtual void GetValueString(VSString& IntString)
		{
			IntString = IntToString(*m_pData);
		}
		virtual uint32 GetStringValue(VSString& IntString)
		{
			return StringToInt(IntString);
		}
	};
	class GRAPHIC_API VSEUnsignedCharProperty : public VSEValueProperty<unsigned char>
	{
	public:
		VSEUnsignedCharProperty(unsigned char * pData, VSString & Name, VSObject * pOwner, bool bRange, unsigned char Max, unsigned char Min, unsigned char Step)
			:VSEValueProperty(pData, Name, pOwner, bRange, Max, Min, Step)
		{
			SetValue();
		}
		virtual ~VSEUnsignedCharProperty()
		{
		}
		virtual void GetValueString(VSString& IntString)
		{
			IntString = IntToString(*m_pData);
		}
		virtual unsigned char GetStringValue(VSString& IntString)
		{
			return StringToInt(IntString);
		}
	};
	class GRAPHIC_API VSECharProperty : public VSEValueProperty<char>
	{
	public:
		VSECharProperty(char * pData, VSString & Name, VSObject * pOwner, bool bRange, char Max, char Min, char Step)
			:VSEValueProperty(pData, Name, pOwner, bRange, Max, Min, Step)
		{
			SetValue();
		}
		virtual void GetValueString(VSString& IntString)
		{
			char tData[2];
			tData[0] = *m_pData;
			tData[1] = _T('\0');
			VSString Temp(tData);
			IntString = Temp;
		}
		virtual char GetStringValue(VSString& IntString)
		{
			return *IntString.GetBuffer();
		}
		virtual ~VSECharProperty()
		{

		}
	};
	class GRAPHIC_API VSERealProperty : public VSEValueProperty<float>
	{
	public:
		VSERealProperty(float * pData,VSString & Name,VSObject * pOwner,bool bRange,float Max,float Min,float Step)
			:VSEValueProperty(pData, Name, pOwner, bRange, Max, Min, Step)
		{
			SetValue();
		}
		virtual ~VSERealProperty()
		{

		}
		virtual void GetValueString(VSString& IntString)
		{
			IntString = RealToString(*m_pData);
		}
		virtual float GetStringValue(VSString& IntString)
		{
			return StringToReal(IntString);
		}
	};
#endif
	class GRAPHIC_API VSEVector3Property : public VSEditorProperty
	{
	public:
		VSEVector3Property(Vector3 * pVector3, VSString & Name, VSObject * pOwner, bool bRange, Vector3 Max, Vector3 Min, Vector3 Step)
			:VSEditorProperty(Name,pOwner)
		{
			VSMAC_ASSERT(pVector3 != NULL);

			if (bRange)
			{
				if (Min.x > Max.x)
				{
					Swap(Max.x, Min.x);
				}
				if (Min.y > Max.y)
				{
					Swap(Max.y, Min.y);
				}
				if (Min.z > Max.z)
				{
					Swap(Max.z, Min.z);
				}
				VSMAC_ASSERT(Max.x - Min.x > Step.x);
				VSMAC_ASSERT(Max.y - Min.y > Step.y);
				VSMAC_ASSERT(Max.z - Min.z > Step.z);
			}
			m_pVector3 = pVector3;
			m_fStep = Step;
			m_Min = Min;
			m_Max = Max;

			m_pNameX = (VSELable *)CREATE_UI_PROPERTY(VSEditorSingle::CUT_LABLE,"X");
			m_pNameY = (VSELable *)CREATE_UI_PROPERTY(VSEditorSingle::CUT_LABLE,"Y");
			m_pNameZ = (VSELable *)CREATE_UI_PROPERTY(VSEditorSingle::CUT_LABLE,"Z");

			m_pTextX = (VSEText *)CREATE_UI_PROPERTY(VSEditorSingle::CUT_TEXT,Name + "X");
			m_pTextX->SetOwner(this);

			m_pTextY = (VSEText *)CREATE_UI_PROPERTY(VSEditorSingle::CUT_TEXT,Name + "Y");
			m_pTextY->SetOwner(this);

			m_pTextZ = (VSEText *)CREATE_UI_PROPERTY(VSEditorSingle::CUT_TEXT,Name + "Z");
			m_pTextZ->SetOwner(this);

			if (bRange)
			{
				m_pSliderX = (VSESlider *)CREATE_UI_PROPERTY(VSEditorSingle::CUT_SLIDER,Name + "X");
				m_pSliderX->SetOwner(this);
				m_pSliderX->SetRange(0, int((Max.x - Min.x) / Step.x), 1);

				m_pSliderZ = (VSESlider *)CREATE_UI_PROPERTY(VSEditorSingle::CUT_SLIDER,Name + "Z");
				m_pSliderZ->SetOwner(this);
				m_pSliderZ->SetRange(0, int((Max.z - Min.z) / Step.z), 1);

				m_pSliderY = (VSESlider *)CREATE_UI_PROPERTY(VSEditorSingle::CUT_SLIDER,Name + "Y");
				m_pSliderY->SetOwner(this);
				m_pSliderY->SetRange(0, int((Max.y - Min.y) / Step.y), 1);
			}
			SetValue();
			
		}
		virtual void SetValue()
		{
			float fDataY = m_pVector3->y;
			VSString RealStringY = RealToString(fDataY);
			m_pTextY->SetValue((void *)&RealStringY);

			float fDataX = m_pVector3->x;
			VSString RealStringX = RealToString(fDataX);
			m_pTextX->SetValue((void *)&RealStringX);

			float fDataZ = m_pVector3->z;
			VSString RealStringZ = RealToString(fDataZ);
			m_pTextZ->SetValue((void *)&RealStringZ);
		}
		virtual ~VSEVector3Property()
		{
			SAFE_DELETE(m_pTextX);
			SAFE_DELETE(m_pTextY);
			SAFE_DELETE(m_pTextZ);

			SAFE_DELETE(m_pSliderX);
			SAFE_DELETE(m_pSliderY);
			SAFE_DELETE(m_pSliderZ);
			
			SAFE_DELETE(m_pNameX);
			SAFE_DELETE(m_pNameY);
			SAFE_DELETE(m_pNameZ);

		}	
		virtual bool CallBackValue(VSEditorSingle * pElem, void * pValue)
		{

			if (pElem == m_pTextX)
			{
				VSString Data = *(VSString *)pValue;
				float Value = StringToReal(Data);

				if (m_pSliderX)
				{
					if (Value < m_Min.x)
					{
						Value = m_Min.x;
					}
					else if (Value > m_Max.x)
					{
						Value = m_Max.x;
					}
					m_pVector3->x = Value;
				}
				else
				{
					m_pVector3->x= Value;
				}
				SetValue();
				VSEditorProperty::CallBackValue(pElem, pValue);
				return true;
			}
			else if (pElem == m_pTextY)
			{
				VSString Data = *(VSString *)pValue;
				float Value = StringToReal(Data);

				if (m_pSliderY)
				{
					if (Value < m_Min.y)
					{
						Value = m_Min.y;
					}
					else if (Value > m_Max.y)
					{
						Value = m_Max.y;
					}
					m_pVector3->y = Value;
				}
				else
				{
					m_pVector3->y= Value;
				}
				SetValue();
				VSEditorProperty::CallBackValue(pElem, pValue);
				return true;
			}
			else if (pElem == m_pTextZ)
			{
				VSString Data = *(VSString *)pValue;
				float Value = StringToReal(Data);

				if (m_pSliderZ)
				{
					if (Value < m_Min.z)
					{
						Value = m_Min.z;
					}
					else if (Value > m_Max.z)
					{
						Value = m_Max.z;
					}
					m_pVector3->z = Value;
				}
				else
				{
					m_pVector3->z= Value;
				}
				SetValue();
				VSEditorProperty::CallBackValue(pElem, pValue);
				return true;
			}
			else if (m_pSliderX && pElem == m_pSliderX)
			{
				int32 Value = *(int32 *)pValue;
				m_pVector3->x = ((float)(Value * m_fStep.x)) + m_Min.x;
				SetValue();
				VSEditorProperty::CallBackValue(pElem, pValue);
			}
			else if (m_pSliderY && pElem == m_pSliderY)
			{
				int32 Value = *(int32 *)pValue;
				m_pVector3->y = ((float)(Value * m_fStep.y)) + m_Min.y;
				SetValue();
				VSEditorProperty::CallBackValue(pElem, pValue);
			}
			else if (m_pSliderZ && pElem == m_pSliderZ)
			{
				int32 Value = *(int32 *)pValue;
				m_pVector3->z = ((float)(Value * m_fStep.z)) + m_Min.z;
				SetValue();
				VSEditorProperty::CallBackValue(pElem, pValue);
			}
			return false;
		}
	protected:

		Vector3 m_fStep;
		VSELable * m_pNameX;
		VSESlider * m_pSliderX;	
		VSEText	  * m_pTextX;

		VSELable * m_pNameY;
		VSESlider * m_pSliderY;	
		VSEText	  * m_pTextY;

		VSELable * m_pNameZ;
		VSESlider * m_pSliderZ;	
		VSEText	  * m_pTextZ;

		Vector3 * m_pVector3;
		Vector3 m_Max;
		Vector3 m_Min;
	};
	class GRAPHIC_API VSETransformProperty : public VSEditorProperty // no range so no slider
	{
	public:
		VSETransformProperty(VSTransform * pTransform,VSString & Name,VSObject * pOwner)
			:VSEditorProperty(Name,pOwner)
		{
			m_pTransform = pTransform;

			m_pTranslateName = (VSELable *)CREATE_UI_PROPERTY(VSEditorSingle::CUT_LABLE,"Translate");
			m_pRotationName = (VSELable *)CREATE_UI_PROPERTY(VSEditorSingle::CUT_LABLE,"Rotation");
			m_pScaleName = (VSELable *)CREATE_UI_PROPERTY(VSEditorSingle::CUT_LABLE,"Scale");
			
			m_pTranslateNameX = (VSELable *)CREATE_UI_PROPERTY(VSEditorSingle::CUT_LABLE,"X");
			m_pTranslateNameY = (VSELable *)CREATE_UI_PROPERTY(VSEditorSingle::CUT_LABLE,"Y");
			m_pTranslateNameZ = (VSELable *)CREATE_UI_PROPERTY(VSEditorSingle::CUT_LABLE,"Z");

			m_pRotationNameX = (VSELable *)CREATE_UI_PROPERTY(VSEditorSingle::CUT_LABLE,"X");
			m_pRotationNameY = (VSELable *)CREATE_UI_PROPERTY(VSEditorSingle::CUT_LABLE,"Y");
			m_pRotationNameZ = (VSELable *)CREATE_UI_PROPERTY(VSEditorSingle::CUT_LABLE,"Z");

			m_pScaleNameX = (VSELable *)CREATE_UI_PROPERTY(VSEditorSingle::CUT_LABLE,"X");
			m_pScaleNameY = (VSELable *)CREATE_UI_PROPERTY(VSEditorSingle::CUT_LABLE,"Y");
			m_pScaleNameZ = (VSELable *)CREATE_UI_PROPERTY(VSEditorSingle::CUT_LABLE,"Z");

			m_pTranslateTextX = (VSEText *)CREATE_UI_PROPERTY(VSEditorSingle::CUT_TEXT,Name + "TranslateX");
			m_pTranslateTextX->SetOwner(this);

			m_pTranslateTextY = (VSEText *)CREATE_UI_PROPERTY(VSEditorSingle::CUT_TEXT,Name + "TranslateY");
			m_pTranslateTextY->SetOwner(this);

			m_pTranslateTextZ = (VSEText *)CREATE_UI_PROPERTY(VSEditorSingle::CUT_TEXT,Name + "TranslateZ");
			m_pTranslateTextZ->SetOwner(this);

			m_pRotationTextX = (VSEText *)CREATE_UI_PROPERTY(VSEditorSingle::CUT_TEXT,Name + "RotationX");
			m_pRotationTextX->SetOwner(this);

			m_pRotationTextZ = (VSEText *)CREATE_UI_PROPERTY(VSEditorSingle::CUT_TEXT,Name + "RotationY");
			m_pRotationTextZ->SetOwner(this);

			m_pRotationTextY = (VSEText *)CREATE_UI_PROPERTY(VSEditorSingle::CUT_TEXT,Name + "RotationZ");
			m_pRotationTextY->SetOwner(this);

			m_pScaleTextX = (VSEText *)CREATE_UI_PROPERTY(VSEditorSingle::CUT_TEXT,Name + "ScaleX");
			m_pScaleTextX->SetOwner(this);

			m_pScaleTextY = (VSEText *)CREATE_UI_PROPERTY(VSEditorSingle::CUT_TEXT,Name + "ScaleX");
			m_pScaleTextY->SetOwner(this);

			m_pScaleTextZ = (VSEText *)CREATE_UI_PROPERTY(VSEditorSingle::CUT_TEXT,Name + "ScaleX");
			m_pScaleTextZ->SetOwner(this);
			SetValue();
			
		}
		virtual void SetValue()
		{
			Vector3 Tran = m_pTransform->GetTranslate();
			float fTranDataX = Tran.x;
			VSString RealStringTranX = RealToString(fTranDataX);
			m_pTranslateTextX->SetValue((void *)&RealStringTranX);

			float fTranDataY = Tran.y;
			VSString RealStringTranY = RealToString(fTranDataY);
			m_pTranslateTextY->SetValue((void *)&RealStringTranY);

			float fTranDataZ = Tran.z;
			VSString RealStringTranZ = RealToString(fTranDataZ);
			m_pTranslateTextZ->SetValue((void *)&RealStringTranZ);

			Matrix3X3 Mat = m_pTransform->GetRotate();
			float X,Y,Z;
			Mat.GetEuler(Z, X, Y);;

			VSString RealStringRotateX = RealToString(X);
			m_pRotationTextX->SetValue((void *)&RealStringRotateX);


			VSString RealStringRotateY = RealToString(Y);
			m_pRotationTextY->SetValue((void *)&RealStringRotateY);

			VSString RealStringRotateZ = RealToString(Z);
			m_pRotationTextZ->SetValue((void *)&RealStringRotateZ);

			Vector3 Scale = m_pTransform->GetScale();

			float fScaleDataX = Scale.x;
			VSString RealStringScaleX = RealToString(fScaleDataX);
			m_pScaleTextX->SetValue((void *)&RealStringScaleX);

			float fScaleDataY = Scale.y;
			VSString RealStringScaleY = RealToString(fScaleDataY);
			m_pScaleTextY->SetValue((void *)&RealStringScaleY);

			float fScaleDataZ = Scale.z;
			VSString RealStringScaleZ = RealToString(fScaleDataZ);
			m_pScaleTextZ->SetValue((void *)&RealStringScaleZ);
		}
		virtual ~VSETransformProperty()
		{
			SAFE_DELETE(m_pTranslateTextX);
			SAFE_DELETE(m_pTranslateTextY);
			SAFE_DELETE(m_pTranslateTextZ);

			SAFE_DELETE(m_pRotationTextX);
			SAFE_DELETE(m_pRotationTextY);
			SAFE_DELETE(m_pRotationTextZ);

			SAFE_DELETE(m_pScaleTextX);
			SAFE_DELETE(m_pScaleTextY);
			SAFE_DELETE(m_pScaleTextZ);

			SAFE_DELETE(m_pTranslateName);
			SAFE_DELETE(m_pRotationName);
			SAFE_DELETE(m_pScaleName);

			SAFE_DELETE(m_pTranslateNameX);
			SAFE_DELETE(m_pTranslateNameY);
			SAFE_DELETE(m_pTranslateNameZ);

			SAFE_DELETE(m_pRotationNameX);
			SAFE_DELETE(m_pRotationNameY);
			SAFE_DELETE(m_pRotationNameZ);

			SAFE_DELETE(m_pScaleNameX);
			SAFE_DELETE(m_pScaleNameY);
			SAFE_DELETE(m_pScaleNameZ);
		}
		virtual bool CallBackValue(VSEditorSingle * pElem, void * pValue)
		{
			if (pElem == m_pTranslateTextX)
			{
				Vector3 Tran = m_pTransform->GetTranslate();
				VSString Data = *(VSString *)pValue;
				float fData = StringToReal(Data);
				Vector3 NewTran(fData,Tran.y,Tran.z);
				m_pTransform->SetTranslate(NewTran);
				SetValue();
				VSEditorProperty::CallBackValue(pElem, pValue);
				return true;
			}
			else if (pElem == m_pTranslateTextY)
			{
				Vector3 Tran = m_pTransform->GetTranslate();
				VSString Data = *(VSString *)pValue;
				float fData = StringToReal(Data);
				Vector3 NewTran(Tran.x,fData,Tran.z);
				m_pTransform->SetTranslate(NewTran);
				SetValue();
				VSEditorProperty::CallBackValue(pElem, pValue);
				return true;
			}
			else if (pElem == m_pTranslateTextZ)
			{
				Vector3 Tran = m_pTransform->GetTranslate();
				VSString Data = *(VSString *)pValue;
				float fData = StringToReal(Data);
				Vector3 NewTran(Tran.x,Tran.y,fData);
				m_pTransform->SetTranslate(NewTran);
				SetValue();
				VSEditorProperty::CallBackValue(pElem, pValue);
				return true;
			}
			else if (pElem == m_pRotationTextX)
			{
				Matrix3X3 Mat = m_pTransform->GetRotate();
				VSString Data = *(VSString *)pValue;
				float fData = StringToReal(Data);
				float X,Y,Z;
				Mat.GetEuler(Z, X, Y);
				Mat.CreateEuler(Z,fData, Y );
				m_pTransform->SetRotate(Mat);
				SetValue();
				VSEditorProperty::CallBackValue(pElem, pValue);
				return true;
			}
			else if (pElem == m_pRotationTextY)
			{
				Matrix3X3 Mat = m_pTransform->GetRotate();
				VSString Data = *(VSString *)pValue;
				float fData = StringToReal(Data);
				float X,Y,Z;
				Mat.GetEuler(Z, X, Y);
				Mat.CreateEuler( Z,X, fData);
				m_pTransform->SetRotate(Mat);
				SetValue();
				VSEditorProperty::CallBackValue(pElem, pValue);
				return true;
			}
			else if (pElem == m_pRotationTextZ)
			{
				Matrix3X3 Mat = m_pTransform->GetRotate();
				VSString Data = *(VSString *)pValue;
				float fData = StringToReal(Data);
				float X,Y,Z;
				Mat.GetEuler(Z,X,Y);
				Mat.CreateEuler(fData, X, Y);
				m_pTransform->SetRotate(Mat);
				SetValue();
				VSEditorProperty::CallBackValue(pElem, pValue);
				return true;
			}
			else if (pElem == m_pScaleTextX)
			{
				Vector3 Scale = m_pTransform->GetScale();
				VSString Data = *(VSString *)pValue;
				float fData = StringToReal(Data);
				Vector3 NewScale(fData,Scale.y,Scale.z);
				m_pTransform->SetScale(NewScale);
				SetValue();
				VSEditorProperty::CallBackValue(pElem, pValue);
				return true;
			}
			else if (pElem == m_pScaleTextY)
			{
				Vector3 Scale = m_pTransform->GetScale();
				VSString Data = *(VSString *)pValue;
				float fData = StringToReal(Data);
				Vector3 NewScale(Scale.x,fData,Scale.z);
				m_pTransform->SetScale(NewScale);
				SetValue();
				VSEditorProperty::CallBackValue(pElem, pValue);
				return true;
			}
			else if (pElem == m_pScaleTextZ)
			{
				Vector3 Scale = m_pTransform->GetScale();
				VSString Data = *(VSString *)pValue;
				float fData = StringToReal(Data);
				Vector3 NewScale(Scale.x,Scale.y,fData);
				m_pTransform->SetScale(NewScale);
				SetValue();
				VSEditorProperty::CallBackValue(pElem, pValue);
				return true;
			}
			
			return false;
		}
	protected:
		VSTransform * m_pTransform;
		VSELable * m_pTranslateName;

		VSELable * m_pTranslateNameX;
		VSEText	  * m_pTranslateTextX;

		VSELable * m_pTranslateNameY;
		VSEText	  * m_pTranslateTextY;

		VSELable * m_pTranslateNameZ;	
		VSEText	  * m_pTranslateTextZ;

		VSELable * m_pRotationName;

		VSELable * m_pRotationNameX;
		VSEText	  * m_pRotationTextX;

		VSELable * m_pRotationNameY;
		VSEText	  * m_pRotationTextY;

		VSELable * m_pRotationNameZ;	
		VSEText	  * m_pRotationTextZ;

		VSELable * m_pScaleName;

		VSELable * m_pScaleNameX;
		VSEText	  * m_pScaleTextX;

		VSELable * m_pScaleNameY;
		VSEText	  * m_pScaleTextY;

		VSELable * m_pScaleNameZ;	
		VSEText	  * m_pScaleTextZ;
	};
	class GRAPHIC_API VSENoDefineProperty : public VSEditorProperty
	{
	public:
		VSENoDefineProperty()
			:VSEditorProperty(VSString::ms_StringNULL,NULL)
		{

		}
		virtual ~VSENoDefineProperty()
		{

		}
	};
	template<typename T>
	VSEditorElement *  CreateEElement(T& Value,VSString & Name,VSObject * pOwner,bool bRange,T & Max,T & Min,T& fStep)
	{
		VSMAC_ASSERT(0);
		return NULL;
	}
	template<> inline VSEditorElement * CreateEElement<Vector3>(Vector3& Value, VSString & Name, VSObject * pOwner, bool bRange, Vector3 & Max, Vector3 & Min, Vector3& fStep)
	{
		return MEM_NEW VSEVector3Property(&Value, Name, pOwner, bRange, Max, Min, fStep);
	}
	template<> inline VSEditorElement * CreateEElement<bool>(bool& Value, VSString & Name, VSObject * pOwner, bool bRange, bool & Max, bool & Min, bool& fStep)
	{
		return MEM_NEW VSEBoolProperty(&Value,Name,pOwner);
	}
	template<> inline VSEditorElement * CreateEElement<VSTransform>(VSTransform& Value, VSString & Name, VSObject * pOwner, bool bRange, VSTransform & Max, VSTransform & Min, VSTransform& fStep)
	{
		return MEM_NEW VSETransformProperty(&Value,Name,pOwner);
	}
	template<> inline VSEditorElement * CreateEElement<VSColorRGBA>(VSColorRGBA& Value, VSString & Name, VSObject * pOwner, bool bRange, VSColorRGBA & Max, VSColorRGBA & Min, VSColorRGBA& fStep)
	{
		return MEM_NEW VSEColorProperty(&Value,Name,pOwner);
	}
#ifdef TYPE_ADD_TO_UI
	template<> inline VSEditorElement * CreateEElement<unsigned int>(unsigned int& Value, VSString & Name, VSObject * pOwner, bool bRange, uint32 & Max, uint32 & Min, unsigned int& fStep)
	{
		return MEM_NEW VSValueProperty<unsigned int>(&Value, Name, pOwner, bRange, Max, Min, fStep);
	}
	template<> inline VSEditorElement * CreateEElement<int>(int& Value, VSString & Name, VSObject * pOwner, bool bRange, int32 & Max, int32 & Min, int& fStep)
	{
		return MEM_NEW VSValueProperty<int>(&Value, Name, pOwner, bRange, Max, Min, fStep);
	}
	template<> inline VSEditorElement * CreateEElement<unsigned char>(unsigned char& Value, VSString & Name, VSObject * pOwner, bool bRange, unsigned char & Max, unsigned char & Min, unsigned char& fStep)
	{
		return MEM_NEW VSValueProperty<unsigned char>(&Value, Name, pOwner, bRange, Max, Min, fStep);
	}
	template<> inline VSEditorElement * CreateEElement<char>(char& Value, VSString & Name, VSObject * pOwner, bool bRange, char & Max, char & Min, char & fStep)
	{
		return MEM_NEW VSValueProperty<char>(&Value, Name, pOwner, bRange, Max, Min, fStep);
	}
	template<> inline VSEditorElement * CreateEElement<float>(float& Value, VSString & Name, VSObject * pOwner, bool bRange, float & Max, float & Min, float & fStep)
	{
		return MEM_NEW VSValueProperty<float>(&Value, Name, pOwner, bRange, Max, Min, fStep);
	}
#else
	template<> inline VSEditorElement * CreateEElement<unsigned int>(unsigned int& Value, VSString & Name, VSObject * pOwner, bool bRange, uint32 & Max, uint32 & Min, unsigned int& fStep)
	{
		return MEM_NEW VSEUnsignedIntProperty(&Value, Name, pOwner, bRange, Max, Min, fStep);
	}
	template<> inline VSEditorElement * CreateEElement<int>(int& Value, VSString & Name, VSObject * pOwner, bool bRange, int32 & Max, int32 & Min, int& fStep)
	{
		return MEM_NEW VSEIntProperty(&Value, Name, pOwner, bRange, Max, Min, fStep);
	}
	template<> inline VSEditorElement * CreateEElement<unsigned char>(unsigned char& Value, VSString & Name, VSObject * pOwner, bool bRange, unsigned char & Max, unsigned char & Min, unsigned char& fStep)
	{
		return MEM_NEW VSEUnsignedCharProperty(&Value, Name, pOwner, bRange, Max, Min, fStep);
	}
	template<> inline VSEditorElement * CreateEElement<char>(char& Value, VSString & Name, VSObject * pOwner, bool bRange, char & Max, char & Min, char & fStep)
	{
		return MEM_NEW VSECharProperty(&Value, Name, pOwner, bRange, Max, Min, fStep);
	}
	template<> inline VSEditorElement * CreateEElement<float>(float& Value,VSString & Name,VSObject * pOwner,bool bRange,float & Max,float & Min,float & fStep)
	{
		return MEM_NEW VSERealProperty(&Value, Name, pOwner, bRange, Max, Min, fStep);
	}
#endif
	/*to else
	---------------------------------------------------
	*/
	GRAPHIC_API VSEditorElement * CreateObjectEditorElement(VSObject * pObject, VSString & Name);
	GRAPHIC_API void  ReleaseObjectEditorElement(VSECollection * Root);
	template<typename T>
	void CreateEditorElement(T & Value, VSObject * pOwner, VSECollection * pParent, VSString& Name, bool Range = false, T  Max = T(), T  Min = T(), T fStep = T())
	{
		if (TIsVSResourceProxyPointerType<T>::Value)
		{
			VSResourceProxyBasePtr & Temp = *(VSResourceProxyBasePtr*)(void *)&Value;
			VSEResourceProperty * pEp = MEM_NEW VSEResourceProperty(Temp, Name, pOwner);
			pParent->AddElement(pEp);
		}
		else if(TIsVSPointerType<T>::Value)
		{
			VSObject* & TempSrc = *(VSObject**)(void *)&Value;	
			VSEditorElement * pEp = CreateObjectEditorElement(TempSrc,Name);
			pParent->AddElement(pEp);

		}
		else if (TIsVSType<T>::Value)
		{
			VSObject* TempSrc = (VSObject *)&Value;	
			VSEditorElement * pEp = CreateObjectEditorElement(TempSrc,Name);
			pParent->AddElement(pEp);
		}
		else if(TIsVSSmartPointerType<T>::Value)
		{
			VSObjectPtr & TempSrc = *(VSObjectPtr*)(void *)&Value;
			VSEditorElement * pEp = CreateObjectEditorElement(TempSrc,Name);
			pParent->AddElement(pEp);
		}
		else if (TIsCustomType<T>::Value)
		{
			VSCustomArchiveObject * TempSrc = (VSCustomArchiveObject*)(void *)&Value;
			VSEditorElement * pEp = TempSrc->CreateEElement(Name, pOwner);
			pParent->AddElement(pEp);
		}
		else if (TIsVSEnumType<T>::Value)
		{
			
		}
		else 
		{
			VSEditorElement * pEp = CreateEElement(Value, Name, pOwner,Range, Max, Min, fStep);
			pParent->AddElement(pEp);
		}
	}
	template<typename T,class VSMemManagerClass>
	void CreateEditorElement(VSArray<T, VSMemManagerClass> & Value, VSObject * pOwner, VSECollection * pParent, VSString& Name, bool Range = false, T  Max = T(), T  Min = T(), T fStep = T())
	{
		VSECollection * pEc = NULL;
		if (Value.GetNum() > 0)
		{
			pEc = (VSECollection *)CREATE_UI_PROPERTY(VSEditorSingle::CUT_COLLECTION,Name);
			pParent->AddElement(pEc);
		}
		for (uint32 i = 0 ; i < Value.GetNum() ;i++)
		{
			VSString NewName = Name + IntToString(i);
			CreateEditorElement(Value[i], pOwner, pEc, NewName, Range, Max, Min, fStep);
		}
	}

	template<class Key,class T,class VSMemManagerClass>
	void CreateEditorElement(VSMap<Key, T, VSMemManagerClass> & Value, VSObject * pOwner, VSECollection * pParent, VSString& Name, bool Range = false, T  Max = T(), T  Min = T(), T fStep = T())
	{
		VSECollection * pEc = NULL;
		if (Value.GetNum() > 0)
		{
			pEc = (VSECollection *)CREATE_UI_PROPERTY(VSEditorSingle::CUT_COLLECTION,Name);
			pParent->AddElement(pEc);
		}
		for (uint32 i = 0 ; i < Value.GetNum() ;i++)
		{
			VSString NewNameKey = Name + _T(" Key");
			VSString NewNameValue = Name + _T(" Value");
			MapElement<Key,T> &ElementDest = Value[i];
			CreateEditorElement(ElementDest.Key,pOwner,pEc,NewNameKey);
			CreateEditorElement(ElementDest.Value, pOwner, pEc, NewNameValue, Range, Max, Min, fStep);
		}
	}

}
