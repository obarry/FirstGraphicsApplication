#include <windows.h>
#include <d2d1.h>
#pragma comment(lib, "d2d1")


#include <tchar.h>

// Global variables

// The main window class name.
static TCHAR szWindowClass[] = _T("GraphicsApp");

// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T("Windows Desktop App with Graphics");

HINSTANCE hInst;

// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void DiscardGraphicsResources();
void OnPaint(HWND hWnd);
void Resize(HWND hWnd);
HRESULT CreateGraphicsResources(HWND hWnd);

ID2D1Factory* pFactory;
ID2D1HwndRenderTarget* pRenderTarget;
ID2D1SolidColorBrush* pBrush;
D2D1_ELLIPSE            ellipse;



// WinMain function
// Entry point of the Application
int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow
) {
	// Some constants (name and title of the Application)
	//static TCHAR szWindowClass[] = _T("DesktopApp");
	//static TCHAR szTitle[] = _T("Windows Desktop Guided Tour Application");

	// Windows structure containing information about the window (application icon, background color of the window, name to display in the title bar, etc.)
	// It also contains a function pointer to the window procedure
	WNDCLASSEX wcex;

	// Assign value to the WNDCLASSEX structure
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szWindowClass; // Name of the Application
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

	// Register the WNDCLASSEX with Windows
	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Windows Desktop Guided Tour"),
			NULL);

		return 1;
	}

	// Call CreateWindow function to create the window. See below the explanation of the opening parameters.
	// CreateWindow parameters:
	//		szWindowClass: the name of the application
	//		szTitle: the text that appears in the title bar
	//		WS_OVERLAPPEDWINDOW: the type of window to create
	//		CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
	//		500, 100: initial size (width, length)
	//		NULL: the parent of this window
	//		NULL: this application does not have a menu bar
	//		hInstance: the first parameter from WinMain
	//		NULL: not used in this application
	HWND hWnd = CreateWindow(
		szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		500, 100,
		NULL,
		NULL,
		hInstance,
		NULL
	); // Returns an HWND which is a handle to a Window

	// If unsuccessful
	if (!hWnd)
	{
		MessageBox(NULL,
			_T("Call to CreateWindow failed!"),
			_T("Windows Desktop Guided Tour"),
			NULL);

		return 1;
	}

	// Call ShowWindow to make the window visible
	// ShowWindow parameters:
	//		hWnd: the value returned from CreateWindow
	//		nCmdShow: the fourth parameter from WinMain
	ShowWindow(
		hWnd,
		nCmdShow
	);
	UpdateWindow(hWnd);

	// At last the Message Loop
	// When the application receives a message, this loop dispatches it to the WndProc function to be handled
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

// WndProc function receives callback messages
// The message loop itself is in WinMain function
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	// A greeting message
	TCHAR greeting[] = _T("Hello, Windows desktop!");

	// One important message to handle is the WM_PAINT message. The application receives the WM_PAINT message when part of its displayed window must be updated.
	// he event can occur when a user moves a window in front of the window, then moves it away again. Your application doesn't know when these events occur.
	// Only Windows knows, so it notifies your app with a WM_PAINT message. When the window is first displayed, all of it must be updated.
	switch (message)
	{
		// To handle a WM_PAINT message, first call BeginPaint, then handle all the logic to lay out the text, buttons, and other controls in the window,
		// and then call EndPaint. For the application, the logic between the beginning call and the ending call displays the string "Hello, Windows desktop!"
		// in the window. In the following code, the TextOut function is used to display the string.
	case WM_CREATE:
		if (FAILED(D2D1CreateFactory(
			D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory)))
		{
			return -1;  // Fail CreateWindowEx.
		}
		break;

	case WM_PAINT:
		OnPaint(hWnd);
		break;

	case WM_DESTROY:
		DiscardGraphicsResources();
		 //SafeRelease(&pFactory);
		PostQuitMessage(0);
		break;

	case WM_SIZE:
		Resize(hWnd);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
	return 0;
}


void CalculateLayout()
{
	if (pRenderTarget != NULL)
	{
		D2D1_SIZE_F size = pRenderTarget->GetSize();
		const float x = size.width / 2;
		const float y = size.height / 2;
		const float radius = min(x, y);
		ellipse = D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius);
	}

}

HRESULT CreateGraphicsResources(HWND hWnd)
{
	HRESULT hr = S_OK;
	if (pRenderTarget == NULL)
	{
		RECT rc;
		GetClientRect(hWnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

		hr = pFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(hWnd, size),
			&pRenderTarget);

		if (SUCCEEDED(hr))
		{
			const D2D1_COLOR_F color = D2D1::ColorF(1.0f, 1.0f, 0);
			hr = pRenderTarget->CreateSolidColorBrush(color, &pBrush);

			if (SUCCEEDED(hr))
			{
				CalculateLayout();
			}
		}
	}
	return hr;
}

void DiscardGraphicsResources()
{
	//SafeRelease(&pRenderTarget);
	//SafeRelease(&pBrush);
}

void OnPaint(HWND hWnd)
{
	HRESULT hr = CreateGraphicsResources(hWnd);
	if (SUCCEEDED(hr))
	{
		PAINTSTRUCT ps;
		BeginPaint(hWnd, &ps);

		pRenderTarget->BeginDraw();

		pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::SkyBlue));
		pRenderTarget->FillEllipse(ellipse, pBrush);

		hr = pRenderTarget->EndDraw();
		if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
		{
			DiscardGraphicsResources();
		}
		EndPaint(hWnd, &ps);
	}

}

void Resize(HWND hWnd)
{
	if (pRenderTarget != NULL)
	{
		RECT rc;
		GetClientRect(hWnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

		pRenderTarget->Resize(size);
		CalculateLayout();
		InvalidateRect(hWnd, NULL, FALSE);
	}
}

