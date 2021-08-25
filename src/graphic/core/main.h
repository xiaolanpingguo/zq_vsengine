#pragma once


#include "core/system.h"
#include "graphic/core/priority.h"
#include "datastruct/VSArray.h"


namespace zq{



class VSRtti;
typedef bool(*Function)();
typedef bool(*FunctionProperty)(VSRtti*);
class GRAPHIC_API VSMain
{
public:

    static void AddInitialPropertyFunction(FunctionProperty Func);
    static void AddTerminalPropertyFunction(Function Func);
    static void AddInitialFunction(Function Func);
    static void AddInitialFunction(Function Func, VSPriority *pPriority);
    static bool Initialize();
    static void AddTerminalFunction(Function Func);
    static void AddTerminalFunction(Function Func, VSPriority *pPriority);
    static bool Terminate();
private:
    VSMain();
    ~VSMain();
    struct Element
    {
    public:
        Element()
        {
            Func = NULL;
            pPriority = NULL;
        }
        ~Element()
        {
            Func = NULL;
            pPriority = NULL;
        }
        Function		Func;
        VSPriority *	pPriority;

        bool operator > (const Element& e)const
        {
            static VSPriority Pr1;
            static VSPriority Pr2;

            VSPriority *p1 = NULL;
            VSPriority *p2 = NULL;
            if (pPriority)
                p1 = pPriority;
            else
                p1 = &Pr1;
            if (e.pPriority)
                p2 = e.pPriority;
            else
                p2 = &Pr2;
            return (*p1) > (*p2);
        }

        bool operator < (const Element& e)const
        {
            static VSPriority Pr1;
            static VSPriority Pr2;

            VSPriority *p1 = NULL;
            VSPriority *p2 = NULL;
            if (pPriority)
                p1 = pPriority;
            else
                p1 = &Pr1;
            if (e.pPriority)
                p2 = e.pPriority;
            else
                p2 = &Pr2;
            return (*p1) > (*p2);
        }

        bool operator == (const Element& e)const
        {
            static VSPriority Pr1;
            static VSPriority Pr2;

            VSPriority *p1 = NULL;
            VSPriority *p2 = NULL;
            if (pPriority)
                p1 = pPriority;
            else
                p1 = &Pr1;
            if (e.pPriority)
                p2 = e.pPriority;
            else
                p2 = &Pr2;
            return (*p1) == (*p2);
        }
    };

    static VSArray<Element>* ms_pInitialArray;
    static VSArray<FunctionProperty>* ms_pInitialPropertyArray;
    static VSArray<Function>* ms_pTerminalPropertyArray;
    static VSArray<Element>* ms_pTerminalArray;
    static uint32 ms_uiInitialObject;
    static uint32 ms_uiTerminalObject;

    class PriorityCompare
    {
    public:
        inline bool operator()(Element & e1, Element& e2)
        {
            static VSPriority Pr1;
            static VSPriority Pr2;

            VSPriority *p1 = NULL;
            VSPriority *p2 = NULL;
            if (e1.pPriority)
                p1 = e1.pPriority;
            else
                p1 = &Pr1;
            if (e2.pPriority)
                p2 = e2.pPriority;
            else
                p2 = &Pr2;
            return (*p1) <= (*p2);
        }
    };
};


}
