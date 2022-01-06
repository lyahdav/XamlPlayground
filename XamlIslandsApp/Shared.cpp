#include "pch.h"

#include "Shared.h"

void PopulateUI(StackPanel xamlContainer) {
  Button btn;
  btn.Content(winrt::box_value(L"Button"));
  btn.Click([=](auto&&...) {
    ContentDialog dlg{};
    TextBlock titleTextBlock;
    TextBlock messageTextBlock;
    titleTextBlock.Text(L"Title");
    titleTextBlock.IsTextSelectionEnabled(true);
    titleTextBlock.MaxLines(2);
    titleTextBlock.TextWrapping(Xaml::TextWrapping::Wrap);
    messageTextBlock.Text(L"Message Text");
    messageTextBlock.IsTextSelectionEnabled(true);
    messageTextBlock.TextWrapping(Xaml::TextWrapping::Wrap);
    dlg.Title(titleTextBlock);
    dlg.Content(messageTextBlock);
    dlg.PrimaryButtonText(L"Primary Button");
    dlg.SecondaryButtonText(L"Secondary Button");
    dlg.CloseButtonText(L"OK");

    //ContentDialog dlg;
    //dlg.Title(winrt::box_value(L"Title"));
    //dlg.Content(winrt::box_value(L"Content"));
    //dlg.CloseButtonText(L"OK");

    dlg.XamlRoot(xamlContainer.XamlRoot());
    dlg.ShowAsync();
    });
  xamlContainer.Children().Append(btn);
}
