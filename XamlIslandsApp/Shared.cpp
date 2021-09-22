#include "pch.h"

#include "Shared.h"

Button buttonWithText(hstring text = L"Button") {
  Button btn;
  btn.Content(winrt::box_value(text));
  return btn;
}

StackPanel stackPanelWithBug(bool useWorkaround) {
  StackPanel sp;
  StackPanel svsp;
  if (useWorkaround) {
    svsp.TabFocusNavigation(xaml::Input::KeyboardNavigationMode::Once);
  }
  ScrollViewer sv;
  sp.TabFocusNavigation(xaml::Input::KeyboardNavigationMode::Once);
  TextBlock tb;
  tb.Text(useWorkaround ? L"with workaround" : L"without workaround");
  sp.Children().Append(tb);
  sp.Children().Append(sv);
  sv.Content(svsp);
  svsp.Children().Append(buttonWithText(L"Button inside TabFocusNavigation=Once StackPanel"));
  svsp.Children().Append(buttonWithText(L"Button inside TabFocusNavigation=Once StackPanel"));
  return sp;
}

void PopulateUI(StackPanel xamlContainer) {
  xamlContainer.Children().Append(buttonWithText(L"Button outside TabFocusNavigation=Once StackPanel"));

  xamlContainer.Children().Append(stackPanelWithBug(false));
  xamlContainer.Children().Append(stackPanelWithBug(true));

  xamlContainer.Children().Append(buttonWithText(L"Button outside TabFocusNavigation=Once StackPanel"));
}
