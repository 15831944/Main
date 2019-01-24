// COMDemo.h : CCOMDemo ������

#pragma once
#include "resource.h"       // ������



#include "COMServer_i.h"
#include "ICompletedEvents_CP.H"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif

using namespace ATL;


// CCOMDemo

class ATL_NO_VTABLE CCOMDemo :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CCOMDemo, &CLSID_COMDemo>,
	public IDispatchImpl<IWelcome, &IID_IWelcome, &LIBID_COMServerLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IDispatchImpl<IMath, &IID_IMath, &LIBID_COMServerLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IConnectionPointContainerImpl<CCOMDemo>,
	public CProxyICompletedEvents<CCOMDemo>
{
public:
	CCOMDemo()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_COMDEMO)


BEGIN_COM_MAP(CCOMDemo)
	COM_INTERFACE_ENTRY(IWelcome)
	COM_INTERFACE_ENTRY(IMath)
	COM_INTERFACE_ENTRY2(IDispatch,IWelcome)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:



	STDMETHOD(Greeting)(BSTR name, BSTR* message);
	STDMETHOD(Add)(long val1, long val2, long* result);
	STDMETHOD(Sub)(long val1, long val2, long* result);
	BEGIN_CONNECTION_POINT_MAP(CCOMDemo)
		CONNECTION_POINT_ENTRY(__uuidof(ICompletedEvents))
	END_CONNECTION_POINT_MAP()
};

OBJECT_ENTRY_AUTO(__uuidof(COMDemo), CCOMDemo)
