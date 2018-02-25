/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtXmlPatterns module of the Qt Toolkit.
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

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.

#ifndef Patternist_Untyped_H
#define Patternist_Untyped_H

#include <private/qanytype_p.h>

QT_BEGIN_NAMESPACE

namespace QPatternist
{
    class AtomicType;

    /**
     * @short Represents the complex W3C XML Schema type <tt>xs:untyped</tt>.
     *
     * @ingroup Patternist_types
     * @author Frans Englich <frans.englich@nokia.com>
     * @see <a href="http://www.w3.org/TR/xpath-datamodel/#types-predefined">XQuery 1.0 and XPath 2.0
     * Data Model, 2.6.2 Predefined Types</a>
     */
    class Untyped : public AnyType
    {
    public:

        typedef QExplicitlySharedDataPointer<Untyped> Ptr;

        /**
         * @returns always BuiltinTypes::xsAnyType.
         */
        virtual SchemaType::Ptr wxsSuperType() const;

        virtual QXmlName name(const NamePool::Ptr &np) const;

        /**
         * @returns always <tt>xs:untypedAtomic</tt>
         */
        virtual ItemType::Ptr atomizedType() const;

        /**
         * @returns always SchemaType::ComplexType
         */
        virtual TypeCategory category() const;

        /**
         * @returns always NoDerivation
         */
        virtual DerivationMethod derivationMethod() const;

    protected:
        friend class BuiltinTypes;

        Untyped();
    };
}

QT_END_NAMESPACE

#endif
