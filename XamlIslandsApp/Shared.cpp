#include "pch.h"

#include "Shared.h"

void PopulateUI(StackPanel xamlContainer) {
  xamlContainer.HorizontalAlignment(HorizontalAlignment::Left);

  Button btn;
  btn.Content(box_value(L"Show CommandBarFlyout"));
  btn.Click([=](auto&&...) {
		mux::CommandBarFlyout cbf;

		AppBarButton abb;
		abb.Label(L"Button 1");
		cbf.SecondaryCommands().Append(abb);

		AppBarButton abb2;
		abb2.Label(L"Button 2");
		cbf.SecondaryCommands().Append(abb2);

		cbf.ShowAt(btn);
    });

  xamlContainer.Children().Append(btn);
}
