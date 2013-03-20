
// test1.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "test1.h"
#include "MainFrm.h"

#include "test1Doc.h"
#include "test1View.h"

#include "jcd3d.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Ctest1App

BEGIN_MESSAGE_MAP(Ctest1App, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &Ctest1App::OnAppAbout)
	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// ��׼��ӡ��������
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// Ctest1App ����

Ctest1App::Ctest1App()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// ���Ӧ�ó��������ù�����������ʱ֧��(/clr)�����ģ���:
	//     1) �����д˸������ã�������������������֧�ֲ�������������
	//     2) ��������Ŀ�У������밴������˳���� System.Windows.Forms ������á�
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: ������Ӧ�ó��� ID �ַ����滻ΪΨһ�� ID �ַ�����������ַ�����ʽ
	//Ϊ CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("test1.AppID.NoVersion"));

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// Ψһ��һ�� Ctest1App ����

Ctest1App theApp;

// thread
UINT myThreadID = 0;
BOOL myThreadRunning = TRUE;
UINT __stdcall myThreadProcress(VOID* p)
{
	while(myThreadRunning)
	{
		if(!theApp.myRenderThreadPause)
		{
			jcd3d::jcd3d_display(0);
		}
	}

	return 0;
}

// Ctest1App ��ʼ��
ID3DXMesh* lpMeshTeapot = NULL;
BOOL Ctest1App::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// ʹ�� RichEdit �ؼ���Ҫ  AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	LoadStdProfileSettings(4);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)


	// ע��Ӧ�ó�����ĵ�ģ�塣�ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(Ctest1Doc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ��ܴ���
		RUNTIME_CLASS(Ctest1View));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// ������׼ shell ���DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// ��������������ָ����������
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// �������к�׺ʱ�ŵ��� DragAcceptFiles
	//  �� SDI Ӧ�ó����У���Ӧ�� ProcessShellCommand ֮����

	if(!jcd3d::jcd3d_init(NULL, 300, 300, TRUE, D3DDEVTYPE_HAL, 1, m_pMainWnd->m_hWnd))
	{
		return FALSE;
	}
	//D3DVIEWPORT9 d3dViewPort = {0, 0, 300, 300, 0.0f, 1.0f};
	//jcd3d::jcd3d_lpd3dd->SetViewport(&d3dViewPort);
	jcd3d::jcd3d_setProjectionPerspectiveTransform(jcd3d::jcd3d_lpd3dd, 300, 300);
	jcd3d::jcd3d_initRenderState(jcd3d::jcd3d_lpd3dd, D3DCULL_CCW, FALSE, TRUE, D3DSHADE_GOURAUD, D3DFILL_WIREFRAME, FALSE);
	D3DXCreateTeapot(jcd3d::jcd3d_lpd3dd, &lpMeshTeapot, NULL);

	_beginthreadex(NULL, 0, myThreadProcress, NULL, 0, &myThreadID);
	myRenderThreadPause = FALSE;

	return TRUE;
}

int Ctest1App::ExitInstance()
{
	//TODO: �����������ӵĸ�����Դ
	AfxOleTerm(FALSE);
	jcd3d::jcd3d_release();
	myThreadRunning = FALSE;

	return CWinApp::ExitInstance();
}

// Ctest1App ��Ϣ�������


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// �������жԻ����Ӧ�ó�������
void Ctest1App::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// Ctest1App ��Ϣ�������


// jcd3d
VOID jcd3d::jcd3d_release()
{
	jcd3d::jcd3d_releaseHandle(&lpMeshTeapot);
}

BOOL jcd3d::jcd3d_setup()
{
	return TRUE;
}

VOID jcd3d::jcd3d_display(DWORD timeDelta)
{
	static FLOAT eyeRadius = 2.5f;
	static FLOAT eyeAngle = 0.0f;
	static FLOAT eyeSpeed = 0.05f;
	jcd3d::jcd3d_setViewTransform(jcd3d::jcd3d_lpd3dd, cosf(eyeAngle) * eyeRadius, 1.0f, sinf(eyeAngle) * eyeRadius, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	eyeAngle += eyeSpeed;

	RECT srchRect = {0, 0, 300, 300};
	RECT refreshRect = {200, 200, 500, 500};
	jcd3d::jcd3d_lpd3dd->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
	jcd3d::jcd3d_lpd3dd->BeginScene();
	lpMeshTeapot->DrawSubset(0);

	jcd3d::jcd3d_lpd3dd->EndScene();
	jcd3d::jcd3d_lpd3dd->Present(&srchRect, &refreshRect, NULL, NULL);
}
