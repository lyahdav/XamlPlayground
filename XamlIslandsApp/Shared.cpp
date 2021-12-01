#include "pch.h"

#include "Shared.h"

static int gCounter = 0;

namespace win {
  using namespace Xaml;
  using namespace Xaml::Media;
  using namespace Controls;
}

void PrintRect(winrt::Windows::Foundation::Rect rect, std::wstring rectName) noexcept {
  std::wostringstream wostringstream;
  wostringstream << rectName << L": X=" << rect.X << L", Y=" << rect.Y << L", W=" << rect.Width
    << L", H=" << rect.Height << std::endl;
  OutputDebugString(wostringstream.str().c_str());
}

Button CreateButton(bool logEffectiveViewportChanged = false) {
  std::string content = "Button ";
  content += std::to_string(++gCounter);

  Button btn;
  btn.Content(box_value(to_hstring(content)));
  btn.Height(45);

  if (logEffectiveViewportChanged) {
    btn.EffectiveViewportChanged([](FrameworkElement sender, EffectiveViewportChangedEventArgs args) {
      PrintRect(RectHelper::FromCoordinatesAndDimensions(args.BringIntoViewDistanceX(), args.BringIntoViewDistanceY(), 0, 0), L"BringIntoViewDistance");
      PrintRect(args.EffectiveViewport(), L"EffectiveViewport");
      PrintRect(args.MaxViewport(), L"MaxViewport");
      });
  }

  return btn;
}

void PopulateUI(StackPanel xamlContainer) {
  StackPanel sp;
  ScrollViewer sv;
  sv.Height(100);

  sv.Content(sp);

  sp.Children().Append(CreateButton());
  sp.Children().Append(CreateButton());
  sp.Children().Append(CreateButton());
  sp.Children().Append(CreateButton(true));
  sp.Children().Append(CreateButton());
  sp.Children().Append(CreateButton());
  sp.Children().Append(CreateButton());
  xamlContainer.Children().Append(sv);
}
