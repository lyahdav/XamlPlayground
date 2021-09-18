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
  StackPanel fz;
  StackPanel fzc;
  fzc.SizeChanged([=](auto&&...) {
    sv.Height(fzc.ActualHeight());
    });

  sv.VerticalScrollMode(ScrollMode::Disabled);
  sv.VerticalScrollBarVisibility(ScrollBarVisibility::Hidden);
  xamlContainer.Children().Append(buttonWithText());

  fz.Background(SolidColorBrush(Windows::UI::Colors::Red()));
  fz.TabFocusNavigation(xaml::Input::KeyboardNavigationMode::Once);
  fz.XYFocusKeyboardNavigation(xaml::Input::XYFocusKeyboardNavigationMode::Enabled);

  fzc.Children().Append(buttonWithText(L"Btn1"));
  fzc.Children().Append(buttonWithText(L"Btn2"));
  fz.Children().Append(fzc);
  fz.Children().Append(tb);

  sv.Content(fz);
  xamlContainer.Children().Append(sv);

  auto btn = buttonWithText(L"Add btn to fz");
  btn.Click([=](auto&&...) {
    fzc.Children().Append(buttonWithText());
    });
  xamlContainer.Children().Append(btn);
}
