#include <windows.h>
#include <stdlib.h>
#include <string.h>

#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.system.h>
#include <winrt/windows.ui.xaml.hosting.h>
#include <windows.ui.xaml.hosting.desktopwindowxamlsource.h>
#include <winrt/windows.ui.xaml.h>
#include <winrt/windows.ui.xaml.input.h>
#include <winrt/windows.ui.xaml.controls.h>
#include <winrt/windows.ui.xaml.controls.primitives.h>
#include <winrt/Windows.ui.xaml.media.h>

using namespace winrt;
using namespace Windows::UI;
using namespace Windows::UI::Composition;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Hosting;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::Foundation::Numerics;

namespace winrt {
	using namespace Windows::System;
	using namespace Windows::UI::Xaml;
	using namespace Windows::UI::Xaml::Controls;
}

LRESULT CALLBACK WindowProc(HWND, UINT, WPARAM, LPARAM);

HWND _hWnd;
HWND _childhWnd;
HINSTANCE _hInstance;

Popup GetPopup(XamlRoot xamlRoot) {
	auto popups =
		VisualTreeHelper::GetOpenPopupsForXamlRoot(xamlRoot);
	if (popups.Size() > 0)
		return popups.GetAt(0);
	return nullptr;
}

CommandBarOverflowPresenter FindPresenter(DependencyObject element) {
	if (element == nullptr) {
		return nullptr;
	}

	OutputDebugString(L"get_class_name: ");
	OutputDebugString(winrt::get_class_name(element).c_str());
	OutputDebugString(L"\n");


	if (auto presenter = element.try_as<CommandBarOverflowPresenter>()) {
		return presenter;
	}

	int childrenCount = VisualTreeHelper::GetChildrenCount(element);
	for (int i = 0; i < childrenCount; i++) {
		auto result = FindPresenter(VisualTreeHelper::GetChild(element, i));
		if (result != nullptr)
			return result;
	}

	return nullptr;
}

void HookKeyDownInOverflowPresenter(Popup popup, CommandBarFlyout cbf) {
	auto presenter = FindPresenter(popup.Child());
	if (!presenter) {
		return;
	}
	presenter.PreviewKeyDown([cbf](auto const&, KeyRoutedEventArgs const& args) {
		switch (args.Key())
		{

		case winrt::VirtualKey::Right:
		case winrt::VirtualKey::Left:
		case winrt::VirtualKey::Down:
		case winrt::VirtualKey::Up:
		{
			// TODO
			const bool isRightToLeft = false;// m_primaryItemsRoot&& m_primaryItemsRoot.get().FlowDirection() == winrt::FlowDirection::RightToLeft;
			const bool isLeft = (args.Key() == winrt::VirtualKey::Left && !isRightToLeft) || (args.Key() == winrt::VirtualKey::Right && isRightToLeft);
			const bool isRight = (args.Key() == winrt::VirtualKey::Right && !isRightToLeft) || (args.Key() == winrt::VirtualKey::Left && isRightToLeft);
			const bool isDown = args.Key() == winrt::VirtualKey::Down;
			const bool isUp = args.Key() == winrt::VirtualKey::Up;

			// To avoid code duplication, we'll use the key directionality to determine
			// both which control list to use and in which direction to iterate through
			// it to find the next control to focus.  Then we'll do that iteration
			// to focus the next control.
			// TODO: changed
			CommandBarFlyout tempCbf;
			auto const& accessibleControls{ isUp || isDown ? cbf.SecondaryCommands() : tempCbf.SecondaryCommands() };
			// TODO: was: auto const& accessibleControls{ isUp || isDown ? m_verticallyAccessibleControls : m_horizontallyAccessibleControls };
			int const startIndex = isLeft || isUp ? accessibleControls.Size() - 1 : 0;
			int const endIndex = isLeft || isUp ? -1 : accessibleControls.Size();
			int const deltaIndex = isLeft || isUp ? -1 : 1;
			bool const shouldLoop = isUp || isDown;
			winrt::Control focusedControl{ nullptr };
			int focusedControlIndex = -1;

			for (int i = startIndex;
				// We'll stop looping at the end index unless we're looping,
				// in which case we want to wrap back around to the start index.
				(i != endIndex || shouldLoop) ||
				// If we found a focused control but have looped all the way back around,
				// then there wasn't another control to focus, so we should quit.
				(focusedControlIndex > 0 && i == focusedControlIndex);
				i += deltaIndex)
			{
				// If we've reached the end index, that means we want to loop.
				// We'll wrap around to the start index.
				if (i == endIndex)
				{
					//MUX_ASSERT(shouldLoop);

					if (focusedControl)
					{
						i = startIndex;
					}
					else
					{
						// If no focused control was found after going through the entire list of controls,
						// then we have nowhere for focus to go.  Let's early-out in that case.
						break;
					}
				}

				auto const& control = accessibleControls.GetAt(i).as<Control>();

				// If we've yet to find the focused control, we'll keep looking for it.
				// Otherwise, we'll try to focus the next control after it.
				if (!focusedControl)
				{
					if (control.FocusState() != winrt::FocusState::Unfocused)
					{
						focusedControl = control;
						focusedControlIndex = i;
					}
				}
				else if (IsControlFocusable(control, false /*checkTabStop*/))
				{
					// If the control we're trying to focus is in the secondary command list,
					// then we'll make sure that that list is open before trying to focus the control.
					// TODO: need to get CommandBar to use IsOpen. Can get from CreatePresenter override.
					//if (auto const& controlAsCommandBarElement = control.try_as<winrt::ICommandBarElement>())
					//{
					//	uint32_t index = 0;
					//	if (cbf.SecondaryCommands().IndexOf(controlAsCommandBarElement, index) && !IsOpen())
					//	{
					//		IsOpen(true);
					//	}
					//}

					if (FocusControl(
						accessibleControls.GetAt(i).as<Control>() /*newFocus*/,
						focusedControl /*oldFocus*/,
						winrt::FocusState::Keyboard /*focusState*/,
						true /*updateTabStop*/))
					{
						args.Handled(true);
						break;
					}
				}
			}

			if (!args.Handled())
			{
				// Occurs for example with Right key while MoreButton has focus. Stay on that MoreButton.
				args.Handled(true);
			}
			break;
		}
		}

		args.Handled(true);
		});
}

void OnFirstCommandLoaded(CommandBarFlyout flyout) {
	if (auto popup = GetPopup(flyout.Target().XamlRoot())) {
		HookKeyDownInOverflowPresenter(popup, flyout);
	}
}

struct CustomCbf : CommandBarFlyoutT<CustomCbf> {
	CommandBarFlyoutCommandBar m_commandBar;
	FrameworkElement::Loaded_revoker m_firstItemLoadedRevoker;

	Control CreatePresenter()
	{
		auto presenter = CommandBarFlyoutT::CreatePresenter().as<FlyoutPresenter>();
		m_commandBar = presenter.Content().as<CommandBarFlyoutCommandBar>();
		m_commandBar.Loaded([this](Windows::Foundation::IInspectable const& sender, auto const&) {
			auto cb = sender.as<CommandBarFlyoutCommandBar>();
			auto commands = cb.PrimaryCommands().Size() > 0 ? cb.PrimaryCommands() : (cb.SecondaryCommands().Size() > 0 ? cb.SecondaryCommands() : nullptr);

			if (commands) {
				const auto firstCommandAsFrameworkElement = commands.GetAt(0).try_as<winrt::FrameworkElement>();

				if (firstCommandAsFrameworkElement) {
					if (firstCommandAsFrameworkElement.IsLoaded()) {
						OnFirstCommandLoaded(*this);
					}
					else {
						m_firstItemLoadedRevoker = firstCommandAsFrameworkElement.Loaded(winrt::auto_revoke,
							{
									[this, firstCommandAsFrameworkElement](auto const&, auto const&)
									{
											OnFirstCommandLoaded(*this);
											m_firstItemLoadedRevoker.revoke();
									}
							});
					}
				}
			}
		});

		return presenter;
	}
};

bool IsControlFocusable(
	winrt::Control const& control,
	bool checkTabStop)
{
	return control &&
		control.Visibility() == winrt::Visibility::Visible &&
		(control.IsEnabled() || control.AllowFocusWhenDisabled()) &&
		(control.IsTabStop() || (!checkTabStop && !control.try_as<winrt::AppBarSeparator>())); // AppBarSeparator is not focusable if IsTabStop is false
}

bool FocusControl(
	winrt::Control const& newFocus,
	winrt::Control const& oldFocus,
	winrt::FocusState const& focusState,
	bool updateTabStop)
{
	//MUX_ASSERT(newFocus);

	if (updateTabStop)
	{
		newFocus.IsTabStop(true);
	}

	if (newFocus.Focus(focusState))
	{
		if (oldFocus && updateTabStop)
		{
			oldFocus.IsTabStop(false);
		}
		return true;
	}
	return false;
}

void addNButtonsToCbf(CommandBarFlyout cbf, int n) {
	for (int i = 0; i < n; i++) {
		AppBarButton btn;
		btn.Label(L"btn");
		cbf.SecondaryCommands().Append(btn);
	}
}

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
	// NOTE: must store this in a variable even though it's not used in order to increase reference count until this function returns.
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

	auto makeButtonWithFlyout = [](auto title, auto cbfConstructor) {
		Button btn;
		btn.Content(box_value(title));
		btn.Click([=](auto&&...) {
			CommandBarFlyout cbf = cbfConstructor();
			addNButtonsToCbf(cbf, 4);
			cbf.ShowAt(btn);
			});
		return btn;
	};

	xamlContainer.Children().Append(makeButtonWithFlyout(L"Open CommandBarFlyout", []() {
		return CommandBarFlyout();
		}));
	xamlContainer.Children().Append(makeButtonWithFlyout(L"Open Custom CommandBarFlyout", []() {
		return winrt::make<CustomCbf>();
		}));
	xamlContainer.UpdateLayout();
	desktopSource.Content(xamlContainer);

	// End XAML Island section.

	ShowWindow(_hWnd, nCmdShow);
	UpdateWindow(_hWnd);
	SetFocus(_hWnd);

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