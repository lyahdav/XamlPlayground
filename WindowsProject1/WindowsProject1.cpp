#include "pch.h"

using namespace winrt;
using namespace Windows::UI;
using namespace Windows::UI::Composition;
using namespace Windows::UI::Xaml::Hosting;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::Foundation::Numerics;

namespace mux {
	using namespace Microsoft::UI::Xaml::Controls;
}

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

HWND _hWnd;
HWND _childhWnd;
HINSTANCE _hInstance;

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	_hInstance = hInstance;

	const wchar_t szWindowClass[] = L"Win32DesktopApp";
	WNDCLASSEX windowClass = { };

	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.lpfnWndProc = WindowProc;
	windowClass.hInstance = hInstance;
	windowClass.lpszClassName = szWindowClass;
	windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

	windowClass.hIconSm = LoadIcon(windowClass.hInstance, IDI_APPLICATION);

	if (RegisterClassEx(&windowClass) == NULL)
	{
		MessageBox(NULL, L"Windows registration failed!", L"Error", NULL);
		return 0;
	}

	_hWnd = CreateWindow(
		szWindowClass,
		L"Windows c++ Win32 Desktop App",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	if (_hWnd == NULL)
	{
		MessageBox(NULL, L"Call to CreateWindow failed!", L"Error", NULL);
		return 0;
	}


	// Begin XAML Island section.

	// The call to winrt::init_apartment initializes COM; by default, in a multithreaded apartment.
	init_apartment(apartment_type::single_threaded);

	auto winuiIXMP = winrt::Microsoft::UI::Xaml::XamlTypeInfo::XamlControlsXamlMetaDataProvider();

	auto xapp = winrt::Microsoft::Toolkit::Win32::UI::XamlHost::XamlApplication({ winuiIXMP });

	// Initialize the XAML framework's core window for the current thread.
	WindowsXamlManager winxamlmanager = WindowsXamlManager::InitializeForCurrentThread();

	xapp.Resources().MergedDictionaries().Append(winrt::Microsoft::UI::Xaml::Controls::XamlControlsResources());

	// This DesktopWindowXamlSource is the object that enables a non-UWP desktop application 
	// to host WinRT XAML controls in any UI element that is associated with a window handle (HWND).
	DesktopWindowXamlSource desktopSource;

	// Get handle to the core window.
	auto interop = desktopSource.as<IDesktopWindowXamlSourceNative>();

	// Parent the DesktopWindowXamlSource object to the current window.
	check_hresult(interop->AttachToWindow(_hWnd));

	// This HWND will be the window handler for the XAML Island: A child window that contains XAML.  
	HWND hWndXamlIsland = nullptr;

	// Get the new child window's HWND. 
	interop->get_WindowHandle(&hWndXamlIsland);

	// Update the XAML Island window size because initially it is 0,0.
	SetWindowPos(hWndXamlIsland, 0, 0, 0, 1200, 300, SWP_SHOWWINDOW);

	// Create the XAML content.
	StackPanel xamlContainer;

	auto appendTextBlock = [=](auto text, auto includeWorkaround) {
		TextBlock tbl;
		mux::TextCommandBarFlyout cbf;
		if (includeWorkaround) {
			cbf.Closed([=](auto&&...) {
				mux::TextCommandBarFlyout newCbf;
				tbl.SelectionFlyout(newCbf);
				tbl.ContextFlyout(newCbf);
				});
		}
		tbl.SelectionFlyout(cbf);
		tbl.ContextFlyout(cbf);
		tbl.Text(text);
		tbl.IsTextSelectionEnabled(true);
		xamlContainer.Children().Append(tbl);
	};

	appendTextBlock(L"TextBlock with bug", false);
	appendTextBlock(L"TextBlock without bug", true);

	TextBox tb;
	tb.PlaceholderText(L"TextBox");
	mux::TextCommandBarFlyout cbf2;
	tb.SelectionFlyout(cbf2);
	tb.ContextFlyout(cbf2);
	xamlContainer.Children().Append(tb);

	xamlContainer.UpdateLayout();
	desktopSource.Content(xamlContainer);

	// End XAML Island section.

	ShowWindow(_hWnd, nCmdShow);
	UpdateWindow(_hWnd);

	//Message loop:
	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT messageCode, WPARAM wParam, LPARAM lParam)
{
	RECT rcClient;

	switch (messageCode)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

		// Create main window
	case WM_CREATE:
		_childhWnd = CreateWindowEx(0, L"ChildWClass", NULL, WS_CHILD | WS_BORDER, 0, 0, 0, 0, hWnd, NULL, _hInstance, NULL);
		return 0;

		// Main window changed size
	case WM_SIZE:
		// Get the dimensions of the main window's client
		// area, and enumerate the child windows. Pass the
		// dimensions to the child windows during enumeration.
		GetClientRect(hWnd, &rcClient);
		MoveWindow(_childhWnd, 200, 200, 400, 500, TRUE);
		ShowWindow(_childhWnd, SW_SHOW);

		return 0;

		// Process other messages.

	default:
		return DefWindowProc(hWnd, messageCode, wParam, lParam);
		break;
	}

	return 0;
}