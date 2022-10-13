#include "pch.h"

#include "Shared.h"

#include "winrt/Windows.UI.Xaml.Documents.h"

using namespace Documents;

void PopulateUI(StackPanel xamlContainer) {
	Paragraph p;

  xamlContainer.HorizontalAlignment(HorizontalAlignment::Left);

  TextBox tb;
  tb.PlaceholderText(L"Placeholder");
  tb.Width(200);

  Button btn;
  btn.Content(winrt::box_value(L"Button"));
  btn.Click([=](auto&&...) {
    ContentDialog dlg;
    dlg.Content(winrt::box_value(L"Content"));
    dlg.CloseButtonText(L"OK");
    dlg.XamlRoot(xamlContainer.XamlRoot());
    dlg.ShowAsync();
    });

  xamlContainer.Children().Append(tb);
  xamlContainer.Children().Append(btn);
}
