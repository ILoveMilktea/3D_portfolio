
// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "Editor.h"

#include "MainFrm.h"
#include "EditorView.h"
#include "MenuView.h"



// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 생성/소멸

CMainFrame::CMainFrame()
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;	

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	cs.style &= ~WS_THICKFRAME; //창 크기 조절 기능 비활성화
	cs.style &= ~WS_MAXIMIZEBOX; //창 최대화버튼 비활성화

	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기



BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	CRect rect;
	CPoint pos;

	GetClientRect(&rect);
	pos.x = (LONG)(GetSystemMetrics(SM_CXSCREEN) / 2.0f - rect.Width() / 2.0f);
	pos.y = (LONG)(GetSystemMetrics(SM_CYSCREEN) / 2.0f - rect.Height() / 2.0f);

	SetWindowPos(NULL, 50, 50, 0, 0, SWP_NOSIZE);

	/*
	
	0, 0 | 0, 1
	1, 0 | 1, 1

	*/

	m_mainSplitter.CreateStatic(this, 1, 2);
	m_mainSplitter.CreateView(0, 0, RUNTIME_CLASS(CEditorView), CSize(VIEWCX, VIEWCY), pContext);
	m_mainSplitter.CreateView(0, 1, RUNTIME_CLASS(CMenuView), CSize(VIEWCX, VIEWCY), pContext);


	return TRUE;
}
