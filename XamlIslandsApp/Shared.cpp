#include "pch.h"

#include "Shared.h"

void PopulateUI(StackPanel xamlContainer) {
  auto buttonWithText = [=](hstring text = L"Button") {
    Button btn;
    btn.Content(winrt::box_value(text));
    return btn;
  };
  
  ScrollViewer sv;
  TextBlock tb;

  sv.Loaded([=](auto sender, auto &&...) {
    auto sv = sender.as<ScrollViewer>();
    sv.Height(sv.ActualHeight() - tb.ActualHeight());
    });
  sv.VerticalScrollMode(ScrollMode::Disabled);
  sv.VerticalScrollBarVisibility(ScrollBarVisibility::Hidden);
  xamlContainer.Children().Append(buttonWithText());

  StackPanel fz;
  fz.Background(SolidColorBrush(Windows::UI::Colors::Red()));
  fz.TabFocusNavigation(xaml::Input::KeyboardNavigationMode::Once);
  fz.XYFocusKeyboardNavigation(xaml::Input::XYFocusKeyboardNavigationMode::Enabled);

  fz.Children().Append(buttonWithText(L"Btn1"));
  fz.Children().Append(buttonWithText(L"Btn2"));
  fz.Children().Append(tb);

  sv.Content(fz);
  xamlContainer.Children().Append(sv);

  xamlContainer.Children().Append(buttonWithText());
}
