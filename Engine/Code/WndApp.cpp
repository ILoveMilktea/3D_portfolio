#include "EngineStdafx.h"
#include "WndApp.h"
#include "DataStore.h"
#include "CameraManager.h"

USING(Engine)
IMPLEMENT_SINGLETON(CWndApp)

void CWndApp::Awake(void)
{
	__super::Awake();
}

void CWndApp::Start(HINSTANCE hInstance, _int nCmdShow)
{
	m_className		= L"3D_Engine";
	m_windowName	= L"A HAT IN TIME";
	m_wndWidth		= 800;
	m_wndHeight		= 600;
	m_showCursor	= true;
	
	
	ShowCursor(m_showCursor);
	RegisterWndClass(hInstance);
	CreateWndHandle(hInstance, nCmdShow);


	POINT m_centerPt = { m_wndWidth >> 1, m_wndHeight >> 1 };
	ClientToScreen(m_hWnd, &m_centerPt);
	SetCursorPos(m_centerPt.x, m_centerPt.y);
}

void CWndApp::OnDestroy(void)
{
	DestroyWindow(m_hWnd);
}

void CWndApp::OnEnable(void)
{
}

void CWndApp::OnDisable(void)
{
}

void CWndApp::SetWndWidth(_int width)
{
	m_wndWidth = width;
}

void CWndApp::SetWndHeight(_int height)
{
	m_wndHeight = height;
}

ATOM CWndApp::RegisterWndClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLIENT));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL; /*MAKEINTRESOURCEW(IDC_CLIENT);*/
	wcex.lpszClassName = m_className.c_str();
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	

	return RegisterClassExW(&wcex);
}

bool CWndApp::CreateWndHandle(HINSTANCE hInstance, _int nCmdShow)
{
	RECT rc = { 0, 0, m_wndWidth, m_wndHeight };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	m_hWnd = CreateWindowW(m_className.c_str(), m_windowName.c_str(),
						   WS_OVERLAPPEDWINDOW,
						   CW_USEDEFAULT, 0, rc.right - rc.left, rc.bottom - rc.top,
						   nullptr, nullptr, hInstance, nullptr);

	SetWindowPos(m_hWnd, 0, 100, 150, rc.right - rc.left, rc.bottom - rc.top, 0);
	if (!m_hWnd)
		return false;


	
	ShowWindow(m_hWnd, nCmdShow);
	UpdateWindow(m_hWnd);

	return true;
}

LRESULT CWndApp::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
	}
	break;
	case WM_ACTIVATE:
		if (wParam == WA_ACTIVE || wParam == WA_CLICKACTIVE)
		{
			//돌고 있을때
		}
		else if (wParam == WA_INACTIVE)
		{
			//안돌고 있을때
		}
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
		break;
	}

	return 0;
}
