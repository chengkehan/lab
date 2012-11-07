
// test1View.cpp : Ctest1View 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "test1.h"
#endif

#include "test1Doc.h"
#include "test1View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// Ctest1View

IMPLEMENT_DYNCREATE(Ctest1View, CView)

BEGIN_MESSAGE_MAP(Ctest1View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_BN_CLICKED(M_JCBUTTON_ID, &Ctest1View::m_jcbuttonHandler)
END_MESSAGE_MAP()

// Ctest1View 构造/析构

Ctest1View::Ctest1View()
{
	// TODO: 在此处添加构造代码

}

Ctest1View::~Ctest1View()
{
}

BOOL Ctest1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// Ctest1View 绘制

void Ctest1View::OnDraw(CDC* /*pDC*/)
{
	Ctest1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// Ctest1View 打印

BOOL Ctest1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void Ctest1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void Ctest1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// Ctest1View 诊断

#ifdef _DEBUG
void Ctest1View::AssertValid() const
{
	CView::AssertValid();
}

void Ctest1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Ctest1Doc* Ctest1View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Ctest1Doc)));
	return (Ctest1Doc*)m_pDocument;
}
#endif //_DEBUG


// Ctest1View 消息处理程序


int Ctest1View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	m_jcButton.Create(_T("STOP"), BS_PUSHBUTTON | WS_CHILD | WS_VISIBLE, CRect(0, 0, 100, 100), this, M_JCBUTTON_ID);
	m_jcbuttonState = TRUE;

	return 0;
}


// m_jcbutton
VOID Ctest1View::m_jcbuttonHandler(void)
{
	if(m_jcbuttonState)
	{
		m_jcButton.SetWindowTextW(_T("PLAY"));
		theApp.myRenderThreadPause = TRUE;
	}
	else
	{
		m_jcButton.SetWindowTextW(_T("STOP"));
		theApp.myRenderThreadPause = FALSE;
	}
	m_jcbuttonState = !m_jcbuttonState;
}
