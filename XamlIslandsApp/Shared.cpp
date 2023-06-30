#include "pch.h"

#include "Shared.h"

void PopulateUI(StackPanel xamlContainer) {
  xamlContainer.HorizontalAlignment(HorizontalAlignment::Stretch);
  xamlContainer.VerticalAlignment(VerticalAlignment::Stretch);

	ScrollViewer sv;
	sv.VerticalAlignment(VerticalAlignment::Stretch);
	sv.HorizontalAlignment(HorizontalAlignment::Stretch);
	mux::WebView2 wv;
	wv.Height(900);
	wv.HorizontalAlignment(HorizontalAlignment::Stretch);

  TextBox tb;
  tb.PlaceholderText(L"URL");
	tb.Text(L"https://microsoft.com");
  tb.Width(400);

  Button btn;
  btn.Content(winrt::box_value(L"Go"));
  btn.Click([=](auto&&...) {
		wv.Source(Windows::Foundation::Uri(tb.Text()));
		});

	sv.Content(wv);

  xamlContainer.Children().Append(tb);
  xamlContainer.Children().Append(btn);
	xamlContainer.Children().Append(sv);
}
