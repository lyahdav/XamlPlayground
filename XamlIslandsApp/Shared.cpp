#include "pch.h"

#include "Shared.h"

void AddComboBoxItem(ComboBox cmb, hstring label) {
  auto comboBoxItem = ComboBoxItem();
  comboBoxItem.Content(box_value(label));
  cmb.Items().Append(comboBoxItem);
}

void PopulateUI(StackPanel xamlContainer) {
  xamlContainer.HorizontalAlignment(HorizontalAlignment::Left);
  xamlContainer.VerticalAlignment(VerticalAlignment::Bottom);

  ComboBox cmb;

  for (int i = 1; i <= 10; i++)
  {
    AddComboBoxItem(cmb, L"Item " + to_hstring(i));
  }
  AddComboBoxItem(cmb, L"Last Item");
  cmb.SelectedIndex(0);

  xamlContainer.Children().Append(cmb);
}
