#include "pch.h"

#include "Shared.h"

void PopulateUI(StackPanel xamlContainer) {
  auto buttonWithText = []() {
    Button btn;
    btn.Content(winrt::box_value(L"Button"));
    return btn;
  };
  
  xamlContainer.Children().Append(buttonWithText());

  StackPanel fz;
  fz.Background(SolidColorBrush(Windows::UI::Colors::Red()));
  fz.TabFocusNavigation(xaml::Input::KeyboardNavigationMode::Once);
  fz.XYFocusKeyboardNavigation(xaml::Input::XYFocusKeyboardNavigationMode::Enabled);

  fz.Children().Append(buttonWithText());
  fz.Children().Append(buttonWithText());
  fz.Children().Append(buttonWithText());

  xamlContainer.Children().Append(fz);

  xamlContainer.Children().Append(buttonWithText());

}
