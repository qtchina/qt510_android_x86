/****************************************************************************
**
** Copyright (C) 2016 Klaralvdalens Datakonsult AB (KDAB).
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt3D module of the Qt Toolkit.
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

#ifndef QT3DRENDER_QTEXTUREIMAGEDATAGENERATOR_H
#define QT3DRENDER_QTEXTUREIMAGEDATAGENERATOR_H

#include <Qt3DRender/qabstractfunctor.h>
#include <QSharedPointer>

// TO DO TexImageDataPtr -> QImageDataPtr + d_ptr
// We might also get rid of the layer, face, mipmap level from
// TexImageDataPtr and store that in the functor directly
// or use the QTextureImage instead

QT_BEGIN_NAMESPACE

namespace Qt3DRender {

class QTextureImageData;
typedef QSharedPointer<QTextureImageData> QTextureImageDataPtr;

class QT3DRENDERSHARED_EXPORT QTextureImageDataGenerator : public QAbstractFunctor
{
public:
    virtual ~QTextureImageDataGenerator() {}
    virtual QTextureImageDataPtr operator()() = 0;
    virtual bool operator ==(const QTextureImageDataGenerator &other) const = 0;
};

typedef QSharedPointer<QTextureImageDataGenerator> QTextureImageDataGeneratorPtr;

} // namespace Qt3DRender

QT_END_NAMESPACE

Q_DECLARE_METATYPE(Qt3DRender::QTextureImageDataGeneratorPtr) // LCOV_EXCL_LINE

#endif // QT3DRENDER_QTEXTUREIMAGEDATAGENERATOR_H
