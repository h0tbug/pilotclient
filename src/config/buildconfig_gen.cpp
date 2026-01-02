// SPDX-FileCopyrightText: Copyright (C) 2016 swift Project Community / Contributors
// SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-swift-pilot-client-1

#include "config/buildconfig.h"
#include <QString>
#include <QDateTime>

#ifdef SWIFT_VATSIM_SUPPORT
int swift::config::CBuildConfig::vatsimClientId()
{
    static const int id {  };
    return id;
}

const QString &swift::config::CBuildConfig::vatsimPrivateKey()
{
    static const auto pk = QString { "" };
    return pk;
}
#endif

const QString &swift::config::CBuildConfig::backtraceToken()
{
    static const auto bt = QString { "" };
    return bt;
}

const QString &swift::config::CBuildConfig::gitHeadSha1()
{
    static const QString gitHeadSha1("c1183565d");
    Q_ASSERT(!gitHeadSha1.isEmpty());
    return gitHeadSha1;
}

int swift::config::CBuildConfig::versionRevision() { return 170; }
