#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.system.h>
#include <winrt/windows.ui.xaml.hosting.h>
#include <windows.ui.xaml.hosting.desktopwindowxamlsource.h>
#include <winrt/windows.ui.xaml.input.h>
#include <winrt/windows.ui.xaml.controls.h>
#include <winrt/windows.ui.xaml.controls.primitives.h>
#include <winrt/Windows.ui.xaml.media.h>

using namespace std;
using namespace winrt;
using namespace Windows::UI;
using namespace Windows::UI::Composition;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Hosting;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::Foundation::Numerics;
using namespace Windows::Foundation;
using namespace Windows::System::Diagnostics;

namespace winrt {
	using namespace Windows::Foundation;
	using namespace Windows::UI;
	using namespace Windows::UI::Xaml;
	using namespace Windows::UI::Xaml::Input;
	using namespace Windows::UI::Xaml::Controls;
	using namespace Windows::UI::Xaml::Controls::Primitives;
	using namespace Windows::UI::Xaml::Media;
} // namespace winrt

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

HWND _hWnd;
HWND _childhWnd;
HINSTANCE _hInstance;

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	_hInstance = hInstance;

	// The main window class name.
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

	// Initialize the XAML framework's core window for the current thread.
	WindowsXamlManager winxamlmanager = WindowsXamlManager::InitializeForCurrentThread();

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
	SetWindowPos(hWndXamlIsland, 0, 200, 100, 800, 200, SWP_SHOWWINDOW);

	// Create the XAML content.
	Windows::UI::Xaml::Controls::StackPanel xamlContainer;

	bool useDark = true;
	if (useDark) {
		xamlContainer.RequestedTheme(Windows::UI::Xaml::ElementTheme::Dark);
		xamlContainer.Background(Windows::UI::Xaml::Media::SolidColorBrush{ Windows::UI::Colors::Black() });
	}

	Windows::UI::Xaml::Controls::TextBlock tb;
	tb.Text(L"Hello World from Xaml Islands!");
	tb.VerticalAlignment(Windows::UI::Xaml::VerticalAlignment::Center);
	tb.HorizontalAlignment(Windows::UI::Xaml::HorizontalAlignment::Center);
	tb.FontSize(48);

	//Windows::UI::Xaml::Controls::ToolTip t;

	Flyout f;
	Flyout f2;
	Button b;
	Button b2;
	Button ib1;
	Button ib2;
	MenuFlyout menu;
	MenuFlyoutItem menuItem;
	ContentDialog dialog;
	TextBox textBox;
	StackPanel panel;
	StackPanel innerPanel;
	panel.Margin({ 10.0f, 10.0f, 10.0f, 10.0f });

	ComboBox cb;
	ComboBoxItem cbi1;
	ComboBoxItem cbi2;

	//cb.RequestedTheme(Windows::UI::Xaml::ElementTheme::Dark);
	cbi1.Content(winrt::box_value(L"Item 1"));
	cbi2.Content(winrt::box_value(L"Item 2"));
	//cbi1.RequestedTheme(Windows::UI::Xaml::ElementTheme::Dark);
	//cbi2.RequestedTheme(Windows::UI::Xaml::ElementTheme::Dark);
	cb.Items().Append(cbi1);
	cb.Items().Append(cbi2);
	//cbi1.Parent().as<FrameworkElement>().RequestedTheme(ElementTheme::Dark);

	cbi1.Loaded([=](auto, auto) {
		auto popups = VisualTreeHelper::GetOpenPopupsForXamlRoot(cb.XamlRoot());
		for (auto const& popup : popups)
		{
			//popup.RequestedTheme(cb.ActualTheme());
			popup.Child().as<FrameworkElement>().RequestedTheme(cb.ActualTheme());
		}

		//FrameworkElement parent = cbi1;
		//while (parent = parent.Parent().as<FrameworkElement>()) {
		//	parent.RequestedTheme(ElementTheme::Dark);
		//}

	});
	cb.DropDownOpened([=](auto, auto) {
		auto popups = VisualTreeHelper::GetOpenPopups(Window::Current());
		auto size = popups.Size();

		cout << size;

		auto popups2 = VisualTreeHelper::GetOpenPopupsForXamlRoot(cb.XamlRoot());
		auto size2 = popups2.Size();
		cout << size2;

		auto hasParent = cbi1.Parent() != nullptr;
		cout << hasParent;
		});
	// doesn't help, makes it uglier
	//cbi1.Background(Windows::UI::Xaml::Media::SolidColorBrush(Windows::UI::Colors::Black()));
	//cbi2.Background(Windows::UI::Xaml::Media::SolidColorBrush(Windows::UI::Colors::Black()));

	dialog.Title(winrt::box_value(L"Title"));
	dialog.PrimaryButtonText(L"OK");

	menuItem.Text(L"Hi");
	auto icon = Windows::UI::Xaml::Controls::BitmapIcon();
	icon.Foreground(Windows::UI::Xaml::Media::SolidColorBrush(Windows::UI::Colors::White()));
	//Uri uri{ winrt::to_hstring(L"ms-appx:///settings.png") };
	Uri uri{ winrt::to_hstring(L"https://cdn1.iconfinder.com/data/icons/office-and-business-14/48/64-512.png") };
	//Uri uri{ winrt::to_hstring(L"http://localhost:8081/assets/src/assets/settings.png?platform=windows&hash=e48f2c19ce9e66f9853e7228ada0ceb8") };
	icon.UriSource(uri);
	menuItem.Icon(icon);
	menu.ShouldConstrainToRootBounds(true);
	menu.Items().Append(menuItem);

	//f.XamlRoot(xamlContainer.XamlRoot()); // This doesn't seem to make a difference
	f.ShouldConstrainToRootBounds(true);
	f2.ShouldConstrainToRootBounds(true); // This doesn't seem to make a difference
	b.Content(winrt::box_value(L"Hi"));
	b.Click([=](auto, auto) {
		//cbi1.XamlRoot(xamlContainer.XamlRoot());
		//cbi2.XamlRoot(xamlContainer.XamlRoot());

		//if (auto xamlRoot = b.XamlRoot()) {
		//	menu.XamlRoot(xamlRoot);
		//	dialog.XamlRoot(xamlRoot);
		//}
		//f.Placement(FlyoutPlacementMode::Full);
		f.ShowAt(b);


		//menu.ShowAt(b);

		//auto x = xamlContainer.XamlRoot();
		//t.XamlRoot(x);
		//t.RequestedTheme(b.ActualTheme());
		});
	//std::wstring s2 = winrt::to_hstring(s);
	//t.Content(winrt::to_hstring(L"hi"));
	//ToolTipService::SetToolTip(b, winrt::box_value(L"Tooltip"));
	//ToolTipService::SetToolTip(b, t);

	b2.Content(winrt::box_value(L"Try Focus"));
	ib1.Content(winrt::box_value(L"Button 1"));
	ib2.Content(winrt::box_value(L"Button 2"));
	b2.Click([=](auto, auto) {
		winrt::FindNextElementOptions findNextElementOptions = winrt::FindNextElementOptions();
		findNextElementOptions.SearchRoot(xamlContainer);

		winrt::Point anchorTopLeft = winrt::Point(0, 0);
		winrt::GeneralTransform transform = innerPanel.TransformToVisual(xamlContainer);
		winrt::Point anchorTopLeftConverted = transform.TransformPoint(anchorTopLeft);
		auto exclusionRect = winrt::Rect(anchorTopLeftConverted.X, anchorTopLeftConverted.Y, innerPanel.Width(), innerPanel.Height());
		findNextElementOptions.ExclusionRect(exclusionRect);

		auto nextElement = winrt::FocusManager::FindNextElement(winrt::FocusNavigationDirection::Up, findNextElementOptions);
		winrt::FocusManager::TryFocusAsync(nextElement, winrt::FocusState::Programmatic);

		//dialog.ShowAsync();
		//f2.Target(b2);// XamlRoot(b2.XamlRoot());
		//FlyoutShowOptions options;
		//options.ShowMode(FlyoutShowMode::Transient);
		//f2.ShowAt(b2/*, options*/);
		});
	f.Content(panel);
	//panel.Children().Append(textBox);
	panel.Children().Append(b2);
	panel.Children().Append(cb);
	panel.Children().Append(innerPanel);
	innerPanel.Children().Append(ib1);
	innerPanel.Children().Append(ib2);

	xamlContainer.Children().Append(tb);
	xamlContainer.Children().Append(b);
	//xamlContainer.Children().Append(cb);
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
	PAINTSTRUCT ps;
	HDC hdc;
	wchar_t greeting[] = L"Hello World in Win32!";
	RECT rcClient;

	switch (messageCode)
	{
	case WM_PAINT:
		if (hWnd == _hWnd)
		{
			hdc = BeginPaint(hWnd, &ps);
			TextOut(hdc, 300, 5, greeting, wcslen(greeting));
			EndPaint(hWnd, &ps);
		}
		break;
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