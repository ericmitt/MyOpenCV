﻿

//------------------------------------------------------------------------------
//     This code was generated by a tool.
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
//------------------------------------------------------------------------------
#include "pch.h"
#include "MainPage.xaml.h"




void ::VideoModern::MainPage::InitializeComponent()
{
    if (_contentLoaded)
        return;

    _contentLoaded = true;

    // Call LoadComponent on ms-appx:///MainPage.xaml
    ::Windows::UI::Xaml::Application::LoadComponent(this, ref new ::Windows::Foundation::Uri(L"ms-appx:///MainPage.xaml"), ::Windows::UI::Xaml::Controls::Primitives::ComponentResourceLocation::Application);

    // Get the Slider named 'slider1'
    slider1 = safe_cast<::Windows::UI::Xaml::Controls::Slider^>(static_cast<Windows::UI::Xaml::IFrameworkElement^>(this)->FindName(L"slider1"));
    // Get the Image named 'ImgCtl'
    ImgCtl = safe_cast<::Windows::UI::Xaml::Controls::Image^>(static_cast<Windows::UI::Xaml::IFrameworkElement^>(this)->FindName(L"ImgCtl"));
}

void ::VideoModern::MainPage::Connect(int connectionId, Platform::Object^ target)
{
    switch (connectionId)
    {
    case 1:
        (safe_cast<::Windows::UI::Xaml::FrameworkElement^>(target))->Loaded +=
            ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::VideoModern::MainPage::*)(Platform::Object^, Windows::UI::Xaml::RoutedEventArgs^))&MainPage::Page_Loaded);
        break;
    case 2:
        (safe_cast<::Windows::UI::Xaml::Controls::Primitives::RangeBase^>(target))->ValueChanged +=
            ref new ::Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventHandler(this, (void (::VideoModern::MainPage::*)(Platform::Object^, Windows::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs^))&MainPage::slider1_ValueChanged);
        break;
    case 3:
        (safe_cast<::Windows::UI::Xaml::UIElement^>(target))->Tapped +=
            ref new ::Windows::UI::Xaml::Input::TappedEventHandler(this, (void (::VideoModern::MainPage::*)(Platform::Object^, Windows::UI::Xaml::Input::TappedRoutedEventArgs^))&MainPage::ImgCtl_Tapped);
        break;
    }
    (void)connectionId; // Unused parameter
    (void)target; // Unused parameter
    _contentLoaded = true;
}

