#include "pch.h"

#include "Shared.h"

void PopulateUI(StackPanel xamlContainer) {
  xamlContainer.HorizontalAlignment(HorizontalAlignment::Left);

  Button btn;
  btn.Content(winrt::box_value(L"Open ContentDialog"));
  btn.Click([=](auto&&...) {
    ContentDialog dlg;
    dlg.Content(winrt::box_value(L"Content"));
    dlg.CloseButtonText(L"OK");
    dlg.XamlRoot(xamlContainer.XamlRoot());
    dlg.ShowAsync();
    });


  Button btn2;
  btn2.Content(winrt::box_value(L"Button2"));

  StackPanel p;
  StackPanel p2;
  p2.Width(100);
  p2.Height(100);
  p2.Background(SolidColorBrush(Colors::Red()));
  p.Children().Append(btn);
  p.Children().Append(p2);
  p.Children().Append(btn2);

  Button btnFlyout;
  btnFlyout.Content(winrt::box_value(L"Open Flyout: ShouldConstrainToRootBounds=false"));
  btnFlyout.Click([=](auto&&...) {
		Flyout flyout;
		flyout.ShouldConstrainToRootBounds(false);
		flyout.Content(p);

		flyout.ShowAt(btnFlyout);
    });

  Button btnFlyout2;
  btnFlyout2.Content(winrt::box_value(L"Open Flyout: ShouldConstrainToRootBounds=true"));
  btnFlyout2.Click([=](auto&&...) {
		StackPanel p3;

		Button btn3;
		btn3.Content(winrt::box_value(L"Button3"));
		Button btn4;
		btn4.Content(winrt::box_value(L"Button4"));

		p3.Children().Append(btn3);
		p3.Children().Append(btn4);

		Flyout flyout2;
		flyout2.ShouldConstrainToRootBounds(true);
		flyout2.Content(p3);
		
		flyout2.ShowAt(btnFlyout2);
    });

  xamlContainer.Children().Append(btnFlyout);
  xamlContainer.Children().Append(btnFlyout2);
}
