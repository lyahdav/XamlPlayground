#include "pch.h"

#include "Shared.h"

void PopulateUI(StackPanel xamlContainer) {
	Grid spacer;
	spacer.Height(300);
	xamlContainer.Children().Append(spacer);

	StackPanel bottomPanel;
	bottomPanel.Orientation(Orientation::Horizontal);
	xamlContainer.Children().Append(bottomPanel);

	TextBox tb;
	tb.Width(300);
	tb.PlaceholderText(L"TextBox with WinUI 2 TextCommandBarFlyout");
	mux::TextCommandBarFlyout cbf2;
	tb.SelectionFlyout(cbf2);
	tb.ContextFlyout(cbf2);
	bottomPanel.Children().Append(tb);

	TextBox tb3;
	tb3.Width(400);
	tb3.PlaceholderText(L"TextBox with WinUI 2 TextCommandBarFlyout and Placement");
	mux::TextCommandBarFlyout cbf3;
	cbf3.Placement(FlyoutPlacementMode::BottomEdgeAlignedLeft);
	tb3.SelectionFlyout(cbf3);
	tb3.ContextFlyout(cbf3);
	bottomPanel.Children().Append(tb3);

	TextBox tb2;
	tb2.Width(300);
	tb2.PlaceholderText(L"TextBox with default TextCommandBarFlyout");
	bottomPanel.Children().Append(tb2);
}
