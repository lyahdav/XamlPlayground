#include "pch.h"

#include "Shared.h"

void PopulateUI(StackPanel xamlContainer) {
  xamlContainer.HorizontalAlignment(HorizontalAlignment::Left);

  Button btn;
  btn.Content(winrt::box_value(L"Button"));
  btn.Click([=](auto&&...) {
    ContentDialog dlg;
    dlg.Content(winrt::box_value(L"Content"));
    dlg.CloseButtonText(L"OK");
    dlg.XamlRoot(xamlContainer.XamlRoot());
    dlg.ShowAsync();
    });

	Button btn2;
	btn2.Content(winrt::box_value(L"Button2"));
	Button btn3;
	btn3.Content(winrt::box_value(L"Button3"));
	
	StackPanel sp;
	sp.Children().Append(btn2);
	sp.Children().Append(btn3);

	ScrollViewer sv;
	sv.Content(sp);

  xamlContainer.Children().Append(btn);
	xamlContainer.Children().Append(sv);
}
