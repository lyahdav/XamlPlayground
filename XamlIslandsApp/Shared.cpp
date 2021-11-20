#include "pch.h"

#include "Shared.h"

static int gCounter = 0;

Button CreateButton(ScrollViewer sv) {
  std::string content = "Button ";
  content += std::to_string(++gCounter);

  Button btn;
  btn.Content(box_value(to_hstring(content)));
  btn.Height(45);

  btn.EffectiveViewportChanged([sv](FrameworkElement sender, EffectiveViewportChangedEventArgs args) {
    auto btnInner = sender.as<Button>();
    auto btnHeight = sender.ActualHeight();
    auto btnContent = unbox_value<hstring>(btnInner.Content());
    auto effectiveViewport = args.EffectiveViewport();
    std::wostringstream wostringstream;
    wostringstream << L"X=" << effectiveViewport.X << L", Y=" << effectiveViewport.Y << L", W=" << effectiveViewport.Width << L", H=" << effectiveViewport.Height;
    auto rectStr = wostringstream.str();
    auto BringIntoViewDistanceX = args.BringIntoViewDistanceX();
    auto BringIntoViewDistanceY = args.BringIntoViewDistanceY(); // In Islands this is abs value of how much to move element down to bring to top of ScrollViewer
    auto effectiveViewportY = effectiveViewport.Y; // In Islands this is how much to move element down to bring to top of ScrollViewer
    auto scrollViewerHeight = sv.ActualHeight();
    auto inViewportIslands = effectiveViewportY > 0 ? effectiveViewportY < btnHeight : -effectiveViewportY < scrollViewerHeight;
    auto inViewportUwp = args.BringIntoViewDistanceY() < btnHeight;
    OutputDebugString(L""); // {btnContent} EffectiveViewportChanged, inViewportIslands={inViewportIslands}, rectStr={rectStr}
    });

  return btn;
}

void PopulateUI(StackPanel xamlContainer) {
  StackPanel sp;
  ScrollViewer sv;
  sv.Height(100);

  sv.Content(sp);

  sp.Children().Append(CreateButton(sv));
  sp.Children().Append(CreateButton(sv));
  sp.Children().Append(CreateButton(sv));
  sp.Children().Append(CreateButton(sv));
  xamlContainer.Children().Append(sv);
}
