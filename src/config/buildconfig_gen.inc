// SPDX-FileCopyrightText: Copyright (C) 2018 swift Project Community / Contributors
// SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-swift-pilot-client-1

#ifndef IN_BUILDCONFIG_H
#error This file is only to be included by buildconfig.inc
#endif

#define SWIFTCONFIG_ON true
#define SWIFTCONFIG_OFF false

constexpr bool swift::config::CBuildConfig::isCompiledWithFs9Support()
{
    return SWIFTCONFIG_OFF;
}

constexpr bool swift::config::CBuildConfig::isCompiledWithP3DSupport()
{
    return SWIFTCONFIG_OFF;
}

constexpr bool swift::config::CBuildConfig::isCompiledWithFsxSupport()
{
    return SWIFTCONFIG_OFF;
}

constexpr bool swift::config::CBuildConfig::isCompiledWithFsuipcSupport()
{
    return isCompiledWithFsxSupport() || isCompiledWithP3DSupport() || isCompiledWithFs9Support() || isCompiledWithMSFSSupport() || isCompiledWithMSFS2024Support();
}

constexpr bool swift::config::CBuildConfig::isCompiledWithXPlaneSupport()
{
    return SWIFTCONFIG_ON;
}

constexpr bool swift::config::CBuildConfig::isCompiledWithFGSupport()
{
    return SWIFTCONFIG_OFF;
}

constexpr bool swift::config::CBuildConfig::isCompiledWithMSFSSupport()
{
    return SWIFTCONFIG_OFF;
}

constexpr bool swift::config::CBuildConfig::isCompiledWithMSFS2024Support()
{
    return SWIFTCONFIG_OFF;
}

constexpr int swift::config::CBuildConfig::versionMajor() { return 0; }
constexpr int swift::config::CBuildConfig::versionMinor() { return 15; }

#undef SWIFTCONFIG_ON
#undef SWIFTCONFIG_OFF
