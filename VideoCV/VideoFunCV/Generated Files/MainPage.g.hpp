﻿

//------------------------------------------------------------------------------
//     This code was generated by a tool.
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
//------------------------------------------------------------------------------
#include "pch.h"
#include "MainPage.xaml.h"




void ::VideoFunCV::MainPage::InitializeComponent()
{
    if (_contentLoaded)
        return;

    _contentLoaded = true;

    // Call LoadComponent on ms-appx:///MainPage.xaml
    ::Windows::UI::Xaml::Application::LoadComponent(this, ref new ::Windows::Foundation::Uri(L"ms-appx:///MainPage.xaml"), ::Windows::UI::Xaml::Controls::Primitives::ComponentResourceLocation::Application);

    // Get the Image named 'imgCV'
    imgCV = safe_cast<::Windows::UI::Xaml::Controls::Image^>(static_cast<Windows::UI::Xaml::IFrameworkElement^>(this)->FindName(L"imgCV"));
}

void ::VideoFunCV::MainPage::Connect(int connectionId, Platform::Object^ target)
{
    switch (connectionId)
    {
    case 1:
        (safe_cast<::Windows::UI::Xaml::FrameworkElement^>(target))->Loaded +=
            ref new ::Windows::UI::Xaml::RoutedEventHandler(this, (void (::VideoFunCV::MainPage::*)(Platform::Object^, Windows::UI::Xaml::RoutedEventArgs^))&MainPage::Page_Loaded);
        break;
    case 2:
        (safe_cast<::Windows::UI::Xaml::UIElement^>(target))->Tapped +=
            ref new ::Windows::UI::Xaml::Input::TappedEventHandler(this, (void (::VideoFunCV::MainPage::*)(Platform::Object^, Windows::UI::Xaml::Input::TappedRoutedEventArgs^))&MainPage::imgCV_Tapped);
        break;
    }
    (void)connectionId; // Unused parameter
    (void)target; // Unused parameter
    _contentLoaded = true;
}

