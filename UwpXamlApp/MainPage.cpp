#include "pch.h"
#include "MainPage.h"
#include "MainPage.g.cpp"
#include "Shared.h"

using namespace winrt;
using namespace Windows::UI::Xaml;

namespace winrt::UwpXamlApp::implementation
{
    MainPage::MainPage()
    {
        InitializeComponent();

        StackPanel panel = Content().as<StackPanel>();
        PopulateUI(panel);
    }
}
