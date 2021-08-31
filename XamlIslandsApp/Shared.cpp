#include "pch.h"

#include "Shared.h"

void PopulateUI(StackPanel xamlContainer) {
	auto appendTextBlock = [=](auto text, auto includeWorkaround) {
		TextBlock tbl;
		mux::TextCommandBarFlyout cbf;
		if (includeWorkaround) {
			cbf.Closed([=](auto&&...) {
				mux::TextCommandBarFlyout newCbf;
				tbl.SelectionFlyout(newCbf);
				tbl.ContextFlyout(newCbf);
				});
		}
		tbl.SelectionFlyout(cbf);
		tbl.ContextFlyout(cbf);
		tbl.Text(text);
		tbl.IsTextSelectionEnabled(true);
		xamlContainer.Children().Append(tbl);
	};

	appendTextBlock(L"TextBlock with bug", false);
	appendTextBlock(L"TextBlock without bug", true);

	TextBox tb;
	tb.PlaceholderText(L"TextBox with WinUI 2 TextCommandBarFlyout");
	mux::TextCommandBarFlyout cbf2;
	tb.SelectionFlyout(cbf2);
	tb.ContextFlyout(cbf2);
	xamlContainer.Children().Append(tb);

	TextBox tb2;
	tb2.PlaceholderText(L"TextBox with default TextCommandBarFlyout");
	xamlContainer.Children().Append(tb2);
}
