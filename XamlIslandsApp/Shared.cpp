#include "pch.h"

#include "Shared.h"

void PopulateUI(StackPanel xamlContainer) {
  xamlContainer.HorizontalAlignment(HorizontalAlignment::Left);

	mux::CommandBarFlyout cbf;

	const auto addAppBarButton = [&]() {
		AppBarButton abb;
		abb.Label(L"Button");
		cbf.SecondaryCommands().Append(abb);
	};

	const auto addSeparator = [&]() {
		AppBarSeparator separator;
		cbf.SecondaryCommands().Append(separator);
	};
	
	addAppBarButton();
	addSeparator();
	addAppBarButton();
	addAppBarButton();
	addSeparator();
	addAppBarButton();
	addAppBarButton();
	addSeparator();
	addAppBarButton();
	addAppBarButton();

	Button btn;
	btn.Content(box_value(L"Show CommandBarFlyout"));
	btn.Click([=](auto&&...) {
		cbf.ShowAt(btn);
    });

  xamlContainer.Children().Append(btn);
}
