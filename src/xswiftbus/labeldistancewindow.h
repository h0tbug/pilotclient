// SPDX-FileCopyrightText: Copyright (C) 2024 swift Project Community / Contributors
// SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-swift-pilot-client-1

#ifndef SWIFT_SIM_XSWIFTBUS_LABELDISTANCEWINDOW_H
#define SWIFT_SIM_XSWIFTBUS_LABELDISTANCEWINDOW_H

//! \file

#include <functional>
#include <string>

#include <Widgets/XPWidgets.h>
#include <Widgets/XPStandardWidgets.h>

namespace XSwiftBus
{
    /*!
     * Read label distance from file.
     * \return The distance value in nautical miles, or default value (5) if file doesn't exist or read fails.
     */
    double readLabelDistanceFromFile();

    /*!
     * Write label distance to file.
     * \param distance The distance value in nautical miles to save.
     * \return true if write succeeded, false otherwise (does not throw).
     */
    bool writeLabelDistanceToFile(double distance);

    /*!
     * Popup window for configuring aircraft label visibility distance.
     * Uses X-Plane widgets to display a simple dialog with a slider
     * and a save button.
     */
    class CLabelDistanceWindow
    {
    public:
        //! Callback type for when distance is saved
        using SaveCallback = std::function<void(double)>;

        //! Constructor
        //! \param initialValue Initial distance value in nautical miles
        //! \param onSave Callback function called when save button is clicked
        CLabelDistanceWindow(double initialValue, SaveCallback onSave);

        //! Destructor
        ~CLabelDistanceWindow();

        //! Show the window
        void show();

        //! Hide the window
        void hide();

        //! Is the window visible?
        bool isVisible() const;

        //! Update the displayed value
        void setValue(double nm);

        // Non-copyable
        CLabelDistanceWindow(const CLabelDistanceWindow &) = delete;
        CLabelDistanceWindow &operator=(const CLabelDistanceWindow &) = delete;

        //! Minimum label distance in nautical miles
        static constexpr int MinDistance = 5;
        //! Maximum label distance in nautical miles
        static constexpr int MaxDistance = 50;

    private:
        static int widgetCallback(XPWidgetMessage message, XPWidgetID widget, intptr_t param1, intptr_t param2);
        void createWidgets();
        void destroyWidgets();
        void handleSave();
        void updateValueLabel();

        int m_currentValue = 5;
        SaveCallback m_onSave;

        XPWidgetID m_windowWidget = nullptr;
        XPWidgetID m_sliderWidget = nullptr;
        XPWidgetID m_saveButtonWidget = nullptr;
        XPWidgetID m_labelWidget = nullptr;
        XPWidgetID m_valueLabelWidget = nullptr;
    };

} // namespace XSwiftBus

#endif // SWIFT_SIM_XSWIFTBUS_LABELDISTANCEWINDOW_H
