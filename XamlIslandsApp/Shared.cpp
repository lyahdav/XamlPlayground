#include "pch.h"

#include "Shared.h"

void PopulateUI(StackPanel xamlContainer) {
  Button btn;
  btn.Content(winrt::box_value(L"Button"));
  btn.Click([=](auto&&...) {
    ContentDialog dlg;
    dlg.Content(winrt::box_value(L"Content"));
    dlg.CloseButtonText(L"OK");
    dlg.XamlRoot(xamlContainer.XamlRoot());
    dlg.ShowAsync();
    });
  xamlContainer.Children().Append(btn);
}
