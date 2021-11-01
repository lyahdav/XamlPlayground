#include "pch.h"

#include "Shared.h"

void PrintViewHierarchy(DependencyObject o, int depth = 0) {
  if (o == nullptr) {
    return;
  }

  for (int i = 0; i < depth; i++) {
    OutputDebugString(L" ");
  }
  auto t = get_class_name(o);
  OutputDebugString(t.c_str());
  OutputDebugString(L"\n");

  auto childCount = VisualTreeHelper::GetChildrenCount(o);
  for (int i = 0; i < childCount; i++) {
    auto child = VisualTreeHelper::GetChild(o, i);
    PrintViewHierarchy(child, depth + 2);
  }
}

template <typename T>
T GetFirstObjectInTreeWithType(DependencyObject root) {
  if (root == nullptr) {
    return nullptr;
  }

  if (auto result = root.try_as<T>()) {
    return result;
  }

  auto childCount = VisualTreeHelper::GetChildrenCount(root);
  for (int i = 0; i < childCount; i++) {
    auto child = VisualTreeHelper::GetChild(root, i);
    if (auto result = GetFirstObjectInTreeWithType<T>(child)) {
      return result;
    }
  }

  return nullptr;
}

CommandBarOverflowPresenter GetFirstCommandBarOverflowPresenterInOpenPopups(XamlRoot xamlRoot) {
  auto popups = VisualTreeHelper::GetOpenPopupsForXamlRoot(xamlRoot);
  for (auto const& popup : popups) {
    if (auto result = GetFirstObjectInTreeWithType<CommandBarOverflowPresenter>(popup.Child())) {
      return result;
    }
  }

  return nullptr;
}

void FixCbfKbNav(XamlRoot xamlRoot) {
  auto commandBarOverflowPresenter = GetFirstCommandBarOverflowPresenterInOpenPopups(xamlRoot);
  if (!commandBarOverflowPresenter) {
    return;
  }
  auto scrollViewer = GetFirstObjectInTreeWithType<ScrollViewer>(commandBarOverflowPresenter);
  if (!scrollViewer) {
    return;
  }
  auto needsFix = scrollViewer.ComputedVerticalScrollBarVisibility() == Visibility::Collapsed;
  if (!needsFix) {
    return;
  }

  scrollViewer.Height(scrollViewer.ActualHeight());

  scrollViewer.VerticalScrollMode(ScrollMode::Disabled);
  scrollViewer.VerticalScrollBarVisibility(ScrollBarVisibility::Hidden);
  scrollViewer.HorizontalScrollMode(ScrollMode::Disabled);
  scrollViewer.HorizontalScrollBarVisibility(ScrollBarVisibility::Hidden);
  scrollViewer.BringIntoViewOnFocusChange(false);

  auto content = scrollViewer.Content().as<FrameworkElement>();
  content.Height(scrollViewer.ActualHeight() + 1);
}

void PopulateUI(StackPanel xamlContainer) {
  auto abb = [=](bool includeSubmenu = false, hstring text = L"Btn") {
    AppBarButton abb;
    abb.Label(text);
    abb.Click([](auto &&...) {
      OutputDebugString(L"Click");
      });
    if (includeSubmenu) {
      MenuFlyout menuFlyout;
      MenuFlyoutItem mi;
      mi.Text(L"Menu item");
      menuFlyout.Items().Append(mi);
      abb.Flyout(menuFlyout);
    }
    return abb;
  };

  auto buttonWithText = [=](hstring text = L"Btn") {
    Button btn;
    btn.Width(50);
    btn.Height(50);
    btn.Content(winrt::box_value(text));
    return btn;
  };

  auto buttonWithTextAndClick = [=](hstring text = L"Btn", auto onClick) {
    Button btn;
    btn.Content(winrt::box_value(text));
    btn.Click([onClick, btn](auto &&...) {
      onClick(btn);
      });
    return btn;
  };
  
  ScrollViewer sv;
  sv.Background(SolidColorBrush(Windows::UI::Colors::Blue()));
  sv.Height(100);
  sv.Width(100);

  StackPanel fz;
  fz.Orientation(Orientation::Vertical);

  sv.SizeChanged([fz](auto sender, auto&&...) {
    auto senderAsScrollViewer = sender.as<ScrollViewer>();
    fz.Width(senderAsScrollViewer.ActualWidth() + 1);
    fz.Height(senderAsScrollViewer.ActualHeight() + 1);
    });

  sv.VerticalScrollMode(ScrollMode::Disabled);
  sv.VerticalScrollBarVisibility(ScrollBarVisibility::Hidden);
  sv.HorizontalScrollMode(ScrollMode::Disabled);
  sv.HorizontalScrollBarVisibility(ScrollBarVisibility::Hidden);
  sv.BringIntoViewOnFocusChange(false);

  xamlContainer.Children().Append(buttonWithTextAndClick(L"CBF without fix", [abb](auto btn) {
    CommandBarFlyout cbf;
    cbf.SecondaryCommands().Append(abb(true));
    cbf.SecondaryCommands().Append(abb());
    cbf.SecondaryCommands().Append(abb());
    cbf.SecondaryCommands().Append(abb());
    cbf.ShowAt(btn);
    }));

  xamlContainer.Children().Append(buttonWithTextAndClick(L"CBF without fix long", [abb](auto btn) {
    CommandBarFlyout cbf;
    cbf.SecondaryCommands().Append(abb(true));
    for (int i = 0; i < 20; i++) {
      cbf.SecondaryCommands().Append(abb());
    }
    cbf.ShowAt(btn);
    }));

  xamlContainer.Children().Append(buttonWithTextAndClick(L"CBF with fix", [abb](auto btn) {
    CommandBarFlyout cbf;
    auto btn1 = abb(true);
    btn1.Loaded([](auto &sender, auto &&...) {
      FixCbfKbNav(sender.as<AppBarButton>().XamlRoot());
    });
    cbf.SecondaryCommands().Append(btn1);
    cbf.SecondaryCommands().Append(abb());
    cbf.SecondaryCommands().Append(abb());
    cbf.SecondaryCommands().Append(abb());
    cbf.ShowAt(btn);
    }));

  xamlContainer.Children().Append(buttonWithTextAndClick(L"CBF with fix long", [abb](auto btn) {
    CommandBarFlyout cbf;
    auto btn1 = abb(true);
    btn1.Loaded([](auto& sender, auto &&...) {
      FixCbfKbNav(sender.as<AppBarButton>().XamlRoot());
    });
    cbf.SecondaryCommands().Append(btn1);
    for (int i = 0; i < 20; i++) {
      cbf.SecondaryCommands().Append(abb());
    }
    cbf.ShowAt(btn);
    }));
  fz.Background(SolidColorBrush(Windows::UI::Colors::Red()));
  fz.TabFocusNavigation(xaml::Input::KeyboardNavigationMode::Once);
  fz.XYFocusKeyboardNavigation(xaml::Input::XYFocusKeyboardNavigationMode::Enabled);

  fz.Children().Append(buttonWithText());
  fz.Children().Append(buttonWithText());

  sv.Content(fz);
  xamlContainer.Children().Append(sv);

  xamlContainer.Children().Append(buttonWithText());
}
