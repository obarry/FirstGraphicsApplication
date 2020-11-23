#include <windows.h>
#include <tchar.h>

// Global variables

// The main window class name.
static TCHAR szWindowClass[] = _T("GraphicsApp");

// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T("Windows Desktop App with Graphics");

HINSTANCE hInst;

// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


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
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);

		// Here your application is laid out.
		// For this introduction, we just print out "Hello, Windows desktop!"
		// in the top left corner.
		TextOut(hdc,
			5, 5,
			greeting, _tcslen(greeting));
		// End application-specific layout section.

		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
	return 0;
}