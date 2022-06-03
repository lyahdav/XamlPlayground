#include "pch.h"

#include "Shared.h"

void SetLeft(FrameworkElement fe, double value) {
  fe.SetValue(Canvas::LeftProperty(), winrt::box_value<double>(value));
}

void SetTop(FrameworkElement fe, double value) {
  fe.SetValue(Canvas::TopProperty(), winrt::box_value<double>(value));
}

void SetWidth(FrameworkElement fe, double value) {
  fe.Width(value);
}

void SetHeight(FrameworkElement fe, double value) {
  fe.Height(value);
}

StackPanel MakePanel() {
  StackPanel panel;
  panel.HorizontalAlignment(HorizontalAlignment::Left);
  return panel;
}

void PopulateUI(StackPanel xamlContainer) {
  xamlContainer.HorizontalAlignment(HorizontalAlignment::Left);
  StackPanel element15 = MakePanel();
  StackPanel element13 = MakePanel();
  StackPanel element9 = MakePanel();
  StackPanel element7 = MakePanel();
  element15.Background(SolidColorBrush(Colors::Blue()));
  element7.Background(SolidColorBrush(Colors::Green()));
  
  TextBlock element5;
  element5.FontSize(15);
  element5.TextTrimming(TextTrimming::CharacterEllipsis);
  element5.IsTextScaleFactorEnabled(true);

  xamlContainer.Children().Append(element15);
  element15.Children().Append(element13);
  element13.Children().Append(element9);
  element9.Children().Append(element7);
  element7.Children().Append(element5);

  element5.Text(L"lorem ipsum dolor lorem ipsum dolor lorem ipsum dolor lorem ipsum dolor");

  SetLeft(element5, 0.00000000);
  SetTop(element5, 0.00000000);
  SetWidth(element5, 500.666656);
  SetHeight(element5, 20.0000000);
  SetLeft(element7, 0.00000000);
  SetTop(element7, 0.00000000);
  SetWidth(element7, 500.666656);
  SetHeight(element7, 20.0000000);
  SetLeft(element9, 0.00000000);
  SetTop(element9, 0.00000000);
  SetWidth(element9, 834.666687);
  SetHeight(element9, 20.0000000);
  SetLeft(element13, 0.00000000);
  SetTop(element13, 0.00000000);
  SetWidth(element13, 834.666687);
  SetHeight(element13, 406.000000);
  SetLeft(element15, 0.00000000);
  SetTop(element15, 0.00000000);
  SetWidth(element15, 834.666687);
  SetHeight(element15, 406.000000);

}
