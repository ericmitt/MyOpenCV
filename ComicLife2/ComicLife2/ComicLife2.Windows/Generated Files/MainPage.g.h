﻿

#pragma once
//------------------------------------------------------------------------------
//     This code was generated by a tool.
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
//------------------------------------------------------------------------------

namespace Windows {
    namespace UI {
        namespace Xaml {
            namespace Controls {
                ref class Button;
                ref class Image;
                ref class CaptureElement;
                ref class TextBox;
            }
        }
    }
}

namespace ComicLife2
{
    partial ref class MainPage : public ::Windows::UI::Xaml::Controls::Page, 
        public ::Windows::UI::Xaml::Markup::IComponentConnector
    {
    public:
        void InitializeComponent();
        virtual void Connect(int connectionId, ::Platform::Object^ target);
    
    private:
        bool _contentLoaded;
    
        private: ::Windows::UI::Xaml::Controls::Button^ btn1;
        private: ::Windows::UI::Xaml::Controls::Image^ img;
        private: ::Windows::UI::Xaml::Controls::Button^ btn2;
        private: ::Windows::UI::Xaml::Controls::Button^ btn3;
        private: ::Windows::UI::Xaml::Controls::Button^ btn4;
        private: ::Windows::UI::Xaml::Controls::CaptureElement^ preview1;
        private: ::Windows::UI::Xaml::Controls::TextBox^ txtMsg;
    };
}

