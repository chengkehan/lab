
// test1View.h : Ctest1View ��Ľӿ�
//

#pragma once


class Ctest1View : public CView
{
protected: // �������л�����
	Ctest1View();
	DECLARE_DYNCREATE(Ctest1View)

// ����
public:
	Ctest1Doc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~Ctest1View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
private:
	CButton m_jcButton;
	// m_jcbutton
	VOID m_jcbuttonHandler(void);
	BOOL m_jcbuttonState;
};

#ifndef _DEBUG  // test1View.cpp �еĵ��԰汾
inline Ctest1Doc* Ctest1View::GetDocument() const
   { return reinterpret_cast<Ctest1Doc*>(m_pDocument); }
#endif
