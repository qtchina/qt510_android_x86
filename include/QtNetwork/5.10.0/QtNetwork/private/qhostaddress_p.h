/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtNetwork module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QHOSTADDRESSPRIVATE_H
#define QHOSTADDRESSPRIVATE_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of the QHostAddress and QNetworkInterface classes.  This header file may change from
// version to version without notice, or even be removed.
//
// We mean it.
//

#include <QtNetwork/private/qtnetworkglobal_p.h>
#include "qhostaddress.h"
#include "qabstractsocket.h"

QT_BEGIN_NAMESPACE

class QNetmask
{
    // stores 0-32 for IPv4, 0-128 for IPv6, or 255 for invalid
    quint8 length;
public:
    Q_DECL_CONSTEXPR QNetmask() : length(255) {}

    bool setAddress(const QHostAddress &address);
    QHostAddress address(QAbstractSocket::NetworkLayerProtocol protocol) const;

    int prefixLength() const { return length == 255 ? -1 : length; }
    void setPrefixLength(QAbstractSocket::NetworkLayerProtocol proto, int len)
    {
        int maxlen = -1;
        if (proto == QAbstractSocket::IPv4Protocol)
            maxlen = 32;
        else if (proto == QAbstractSocket::IPv6Protocol)
            maxlen = 128;
        if (len > maxlen || len < 0)
            length = 255U;
        else
            length = unsigned(len);
    }

    friend bool operator==(QNetmask n1, QNetmask n2)
    { return n1.length == n2.length; }
};

QT_END_NAMESPACE

#endif
