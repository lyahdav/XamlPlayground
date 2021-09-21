#include "pch.h"

#include "Shared.h"

void PopulateUI(StackPanel xamlContainer) {
  auto buttonWithText = [=](hstring text = L"Button") {
    Button btn;
    btn.Width(50);
    btn.Content(winrt::box_value(text));
    return btn;
  };
  
  ScrollViewer sv;
  ScrollViewer svi;
  StackPanel svisp;
  svi.Content(svisp);
  svi.TabFocusNavigation(xaml::Input::KeyboardNavigationMode::Once);

  sv.Background(SolidColorBrush(Windows::UI::Colors::Blue()));
  sv.Height(100);
  sv.Width(100);
  StackPanel fz;
  fz.Orientation(Orientation::Horizontal);

  sv.SizeChanged([fz](auto sender, auto&&...) {
    auto senderAsScrollViewer = sender.as<ScrollViewer>();
    fz.Width(senderAsScrollViewer.ActualWidth() + 1);
    fz.Height(senderAsScrollViewer.ActualHeight() + 1);
    });

  sv.VerticalScrollMode(ScrollMode::Disabled);
  sv.VerticalScrollBarVisibility(ScrollBarVisibility::Hidden);
  sv.HorizontalScrollMode(ScrollMode::Disabled);
  sv.HorizontalScrollBarVisibility(ScrollBarVisibility::Hidden);
  xamlContainer.Children().Append(buttonWithText());

  fz.Background(SolidColorBrush(Windows::UI::Colors::Red()));
  fz.TabFocusNavigation(xaml::Input::KeyboardNavigationMode::Once);
  fz.XYFocusKeyboardNavigation(xaml::Input::XYFocusKeyboardNavigationMode::Enabled);

  fz.Children().Append(svi);

  svisp.Children().Append(buttonWithText(L"Btn"));
  svisp.Children().Append(buttonWithText(L"Btn"));

  sv.Content(fz);
  xamlContainer.Children().Append(sv);

  xamlContainer.Children().Append(buttonWithText());
}
