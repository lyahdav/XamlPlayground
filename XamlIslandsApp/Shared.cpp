#include "pch.h"

#include "Shared.h"

void PopulateUI(StackPanel xamlContainer) {
  xamlContainer.HorizontalAlignment(HorizontalAlignment::Left);

  TextBox tb;
  tb.PlaceholderText(L"Placeholder");
  tb.SizeChanged([tb](auto &&...) {
    time_t _tm = time(NULL);

    #pragma warning(suppress : 4996)
    struct tm* curtime = localtime(&_tm);
    #pragma warning(suppress : 4996)
    OutputDebugStringA(asctime(curtime));

    tb.Width(tb.Width() + 1);
    });
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
