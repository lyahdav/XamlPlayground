#include "pch.h"

#include "Shared.h"

void PopulateUI(StackPanel xamlContainer) {
  xamlContainer.HorizontalAlignment(HorizontalAlignment::Left);

  Button btnFlyout2;
  btnFlyout2.Content(winrt::box_value(L"Open Flyout"));
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

  xamlContainer.Children().Append(btnFlyout2);
}
