// SPDX-FileCopyrightText: Copyright (C) 2024 swift Project Community / Contributors
// SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-swift-pilot-client-1

#include "labeldistancewindow.h"

#include <algorithm>
#include <cstdio>

#include <XPLM/XPLMDisplay.h>

namespace XSwiftBus
{
    CLabelDistanceWindow::CLabelDistanceWindow(double initialValue, SaveCallback onSave)
        : m_currentValue((std::max)(MinDistance, (std::min)(MaxDistance, static_cast<int>(initialValue + 0.5)))),
          m_onSave(std::move(onSave))
    {
    }

    CLabelDistanceWindow::~CLabelDistanceWindow()
    {
        destroyWidgets();
    }

    void CLabelDistanceWindow::show()
    {
        if (!m_windowWidget)
        {
            createWidgets();
        }
        if (m_windowWidget)
        {
            // Update slider position
            if (m_sliderWidget)
            {
                XPSetWidgetProperty(m_sliderWidget, xpProperty_ScrollBarSliderPosition, m_currentValue);
                updateValueLabel();
            }

            XPShowWidget(m_windowWidget);
            XPBringRootWidgetToFront(m_windowWidget);
        }
    }

    void CLabelDistanceWindow::hide()
    {
        if (m_windowWidget)
        {
            XPHideWidget(m_windowWidget);
        }
    }

    bool CLabelDistanceWindow::isVisible() const
    {
        return m_windowWidget && XPIsWidgetVisible(m_windowWidget);
    }

    void CLabelDistanceWindow::setValue(double nm)
    {
        m_currentValue = (std::max)(MinDistance, (std::min)(MaxDistance, static_cast<int>(nm + 0.5)));
        if (m_sliderWidget)
        {
            XPSetWidgetProperty(m_sliderWidget, xpProperty_ScrollBarSliderPosition, m_currentValue);
            updateValueLabel();
        }
    }

    void CLabelDistanceWindow::updateValueLabel()
    {
        if (m_valueLabelWidget)
        {
            char buffer[32];
            std::snprintf(buffer, sizeof(buffer), "%d NM", m_currentValue);
            XPSetWidgetDescriptor(m_valueLabelWidget, buffer);
        }
    }

    void CLabelDistanceWindow::createWidgets()
    {
        // Get screen dimensions
        int screenWidth, screenHeight;
        XPLMGetScreenSize(&screenWidth, &screenHeight);

        // Window dimensions
        const int windowWidth = 320;
        const int windowHeight = 130;
        const int left = (screenWidth - windowWidth) / 2;
        const int top = (screenHeight + windowHeight) / 2;
        const int right = left + windowWidth;
        const int bottom = top - windowHeight;

        // Create main window widget
        m_windowWidget = XPCreateWidget(
            left, top, right, bottom,
            1, // Visible
            "Aircraft Labels Distance",
            1, // Root
            nullptr,
            xpWidgetClass_MainWindow);

        XPSetWidgetProperty(m_windowWidget, xpProperty_MainWindowHasCloseBoxes, 1);
        XPAddWidgetCallback(m_windowWidget, widgetCallback);

        // Store 'this' pointer for callback
        XPSetWidgetProperty(m_windowWidget, xpProperty_Refcon, reinterpret_cast<intptr_t>(this));

        // Create title label
        m_labelWidget = XPCreateWidget(
            left + 10, top - 30, left + 200, top - 50,
            1, // Visible
            "Distance (5-50 NM):",
            0, // Not root
            m_windowWidget,
            xpWidgetClass_Caption);

        // Create value label (shows current slider value)
        m_valueLabelWidget = XPCreateWidget(
            right - 70, top - 30, right - 10, top - 50,
            1, // Visible
            "",
            0, // Not root
            m_windowWidget,
            xpWidgetClass_Caption);

        // Create slider
        m_sliderWidget = XPCreateWidget(
            left + 10, top - 55, right - 10, top - 75,
            1, // Visible
            "",
            0, // Not root
            m_windowWidget,
            xpWidgetClass_ScrollBar);

        XPSetWidgetProperty(m_sliderWidget, xpProperty_ScrollBarType, xpScrollBarTypeSlider);
        XPSetWidgetProperty(m_sliderWidget, xpProperty_ScrollBarMin, MinDistance);
        XPSetWidgetProperty(m_sliderWidget, xpProperty_ScrollBarMax, MaxDistance);
        XPSetWidgetProperty(m_sliderWidget, xpProperty_ScrollBarSliderPosition, m_currentValue);

        // Update value label with initial value
        updateValueLabel();

        // Create save button
        m_saveButtonWidget = XPCreateWidget(
            left + windowWidth / 2 - 40, top - 90, left + windowWidth / 2 + 40, top - 115,
            1, // Visible
            "Save",
            0, // Not root
            m_windowWidget,
            xpWidgetClass_Button);

        XPSetWidgetProperty(m_saveButtonWidget, xpProperty_ButtonType, xpPushButton);
    }

    void CLabelDistanceWindow::destroyWidgets()
    {
        if (m_windowWidget)
        {
            XPDestroyWidget(m_windowWidget, 1); // Destroy children too
            m_windowWidget = nullptr;
            m_sliderWidget = nullptr;
            m_saveButtonWidget = nullptr;
            m_labelWidget = nullptr;
            m_valueLabelWidget = nullptr;
        }
    }

    void CLabelDistanceWindow::handleSave()
    {
        if (!m_sliderWidget) { return; }

        // Get current slider position
        m_currentValue = static_cast<int>(XPGetWidgetProperty(m_sliderWidget, xpProperty_ScrollBarSliderPosition, nullptr));

        // Call callback with the value
        if (m_onSave)
        {
            m_onSave(static_cast<double>(m_currentValue));
        }

        // Hide window after save
        hide();
    }

    int CLabelDistanceWindow::widgetCallback(XPWidgetMessage message, XPWidgetID widget, intptr_t param1, intptr_t param2)
    {
        (void)param2;

        auto *window = reinterpret_cast<CLabelDistanceWindow *>(XPGetWidgetProperty(widget, xpProperty_Refcon, nullptr));

        if (message == xpMessage_CloseButtonPushed)
        {
            XPHideWidget(widget);
            return 1;
        }

        if (message == xpMsg_PushButtonPressed)
        {
            if (window && reinterpret_cast<XPWidgetID>(param1) == window->m_saveButtonWidget)
            {
                window->handleSave();
                return 1;
            }
        }

        if (message == xpMsg_ScrollBarSliderPositionChanged)
        {
            if (window && reinterpret_cast<XPWidgetID>(param1) == window->m_sliderWidget)
            {
                // Update current value and label when slider moves
                window->m_currentValue = static_cast<int>(
                    XPGetWidgetProperty(window->m_sliderWidget, xpProperty_ScrollBarSliderPosition, nullptr));
                window->updateValueLabel();
                return 1;
            }
        }

        return 0;
    }

} // namespace XSwiftBus
