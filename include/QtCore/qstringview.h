/****************************************************************************
**
** Copyright (C) 2017 Klarälvdalens Datakonsult AB, a KDAB Group company, info@kdab.com, author Marc Mutz <marc.mutz@kdab.com>
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtCore module of the Qt Toolkit.
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
#ifndef QSTRINGVIEW_H
#define QSTRINGVIEW_H

#ifndef QT_STRINGVIEW_LEVEL
#  define QT_STRINGVIEW_LEVEL 1
#endif

#include <QtCore/qchar.h>
#include <QtCore/qbytearray.h>
#include <QtCore/qstringliteral.h>
#include <QtCore/qstringalgorithms.h>

#include <string>

QT_BEGIN_NAMESPACE

class QString;
class QStringRef;

namespace QtPrivate {
template <typename Char>
struct IsCompatibleCharTypeHelper
    : std::integral_constant<bool,
                             std::is_same<Char, QChar>::value ||
                             std::is_same<Char, ushort>::value ||
#if defined(Q_COMPILER_UNICODE_STRINGS)
                             std::is_same<Char, char16_t>::value ||
#endif
                             (std::is_same<Char, wchar_t>::value && sizeof(wchar_t) == sizeof(QChar))> {};
template <typename Char>
struct IsCompatibleCharType
    : IsCompatibleCharTypeHelper<typename std::remove_cv<typename std::remove_reference<Char>::type>::type> {};

template <typename Array>
struct IsCompatibleArrayHelper : std::false_type {};
template <typename Char, size_t N>
struct IsCompatibleArrayHelper<Char[N]>
    : IsCompatibleCharType<Char> {};
template <typename Array>
struct IsCompatibleArray
    : IsCompatibleArrayHelper<typename std::remove_cv<typename std::remove_reference<Array>::type>::type> {};

template <typename Pointer>
struct IsCompatiblePointerHelper : std::false_type {};
template <typename Char>
struct IsCompatiblePointerHelper<Char*>
    : IsCompatibleCharType<Char> {};
template <typename Pointer>
struct IsCompatiblePointer
    : IsCompatiblePointerHelper<typename std::remove_cv<typename std::remove_reference<Pointer>::type>::type> {};

template <typename T>
struct IsCompatibleStdBasicStringHelper : std::false_type {};
template <typename Char, typename...Args>
struct IsCompatibleStdBasicStringHelper<std::basic_string<Char, Args...> >
    : IsCompatibleCharType<Char> {};

template <typename T>
struct IsCompatibleStdBasicString
    : IsCompatibleStdBasicStringHelper<
        typename std::remove_cv<typename std::remove_reference<T>::type>::type
      > {};

} // namespace QtPrivate

class QStringView
{
public:
#if defined(Q_OS_WIN) && !defined(Q_COMPILER_UNICODE_STRINGS)
    typedef wchar_t storage_type;
#else
    typedef char16_t storage_type;
#endif
    typedef const QChar value_type;
    typedef std::ptrdiff_t difference_type;
    typedef qsizetype size_type;
    typedef value_type &reference;
    typedef value_type &const_reference;
    typedef value_type *pointer;
    typedef value_type *const_pointer;

    typedef pointer iterator;
    typedef const_pointer const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

private:
    template <typename Char>
    using if_compatible_char = typename std::enable_if<QtPrivate::IsCompatibleCharType<Char>::value, bool>::type;

    template <typename Array>
    using if_compatible_array = typename std::enable_if<QtPrivate::IsCompatibleArray<Array>::value, bool>::type;

    template <typename Pointer>
    using if_compatible_pointer = typename std::enable_if<QtPrivate::IsCompatiblePointer<Pointer>::value, bool>::type;

    template <typename T>
    using if_compatible_string = typename std::enable_if<QtPrivate::IsCompatibleStdBasicString<T>::value, bool>::type;

    template <typename T>
    using if_compatible_qstring_like = typename std::enable_if<std::is_same<T, QString>::value || std::is_same<T, QStringRef>::value, bool>::type;

    template <typename Char, size_t N>
    static Q_DECL_CONSTEXPR qsizetype lengthHelperArray(const Char (&)[N]) Q_DECL_NOTHROW
    {
        return qsizetype(N - 1);
    }

    template <typename Char>
    static qsizetype lengthHelperPointer(const Char *str) Q_DECL_NOTHROW
    {
#if defined(Q_CC_GNU) && !defined(Q_CC_CLANG) && !defined(Q_CC_INTEL)
        if (__builtin_constant_p(*str)) {
            qsizetype result = 0;
            while (*str++)
                ++result;
        }
#endif
        return QtPrivate::qustrlen(reinterpret_cast<const ushort *>(str));
    }
    static qsizetype lengthHelperPointer(const QChar *str) Q_DECL_NOTHROW
    {
        return QtPrivate::qustrlen(reinterpret_cast<const ushort *>(str));
    }

    template <typename Char>
    static const storage_type *castHelper(const Char *str) Q_DECL_NOTHROW
    { return reinterpret_cast<const storage_type*>(str); }
    static Q_DECL_CONSTEXPR const storage_type *castHelper(const storage_type *str) Q_DECL_NOTHROW
    { return str; }

public:
    Q_DECL_CONSTEXPR QStringView() Q_DECL_NOTHROW
        : m_size(0), m_data(nullptr) {}
    Q_DECL_CONSTEXPR QStringView(std::nullptr_t) Q_DECL_NOTHROW
        : QStringView() {}

    template <typename Char, if_compatible_char<Char> = true>
    Q_DECL_CONSTEXPR QStringView(const Char *str, qsizetype len)
        : m_size((Q_ASSERT(len >= 0), Q_ASSERT(str || !len), len)),
          m_data(castHelper(str)) {}

    template <typename Char, if_compatible_char<Char> = true>
    Q_DECL_CONSTEXPR QStringView(const Char *f, const Char *l)
        : QStringView(f, l - f) {}

#ifdef Q_QDOC
    template <typename Char, size_t N>
    Q_DECL_CONSTEXPR QStringView(const Char (&array)[N]) Q_DECL_NOTHROW;

    template <typename Char>
    Q_DECL_CONSTEXPR QStringView(const Char *str) Q_DECL_NOTHROW;
#else
    template <typename Array, if_compatible_array<Array> = true>
    Q_DECL_CONSTEXPR QStringView(const Array &str) Q_DECL_NOTHROW
        : QStringView(str, lengthHelperArray(str)) {}

    template <typename Pointer, if_compatible_pointer<Pointer> = true>
    Q_DECL_CONSTEXPR QStringView(const Pointer &str) Q_DECL_NOTHROW
        : QStringView(str, str ? lengthHelperPointer(str) : 0) {}
#endif

#ifdef Q_QDOC
    QStringView(const QString &str) Q_DECL_NOTHROW;
    QStringView(const QStringRef &str) Q_DECL_NOTHROW;
#else
    template <typename String, if_compatible_qstring_like<String> = true>
    QStringView(const String &str) Q_DECL_NOTHROW
        : QStringView(str.isNull() ? nullptr : str.data(), qsizetype(str.size())) {}
#endif

    template <typename StdBasicString, if_compatible_string<StdBasicString> = true>
    QStringView(const StdBasicString &str) Q_DECL_NOTHROW
        : QStringView(str.data(), qsizetype(str.size())) {}

    Q_REQUIRED_RESULT inline QString toString() const; // defined in qstring.h

    Q_REQUIRED_RESULT Q_DECL_CONSTEXPR qsizetype size() const Q_DECL_NOTHROW { return m_size; }
    Q_REQUIRED_RESULT const_pointer data() const Q_DECL_NOTHROW { return reinterpret_cast<const_pointer>(m_data); }
    Q_REQUIRED_RESULT Q_DECL_CONSTEXPR const storage_type *utf16() const Q_DECL_NOTHROW { return m_data; }

    Q_REQUIRED_RESULT Q_DECL_CONSTEXPR QChar operator[](qsizetype n) const
    { return Q_ASSERT(n >= 0), Q_ASSERT(n < size()), QChar(m_data[n]); }

    //
    // QString API
    //

    Q_REQUIRED_RESULT QByteArray toLatin1() const { return QtPrivate::convertToLatin1(*this); }
    Q_REQUIRED_RESULT QByteArray toUtf8() const { return QtPrivate::convertToUtf8(*this); }
    Q_REQUIRED_RESULT QByteArray toLocal8Bit() const { return QtPrivate::convertToLocal8Bit(*this); }
    Q_REQUIRED_RESULT inline QVector<uint> toUcs4() const; // defined in qvector.h

    Q_REQUIRED_RESULT Q_DECL_CONSTEXPR QChar at(qsizetype n) const { return (*this)[n]; }

    Q_REQUIRED_RESULT Q_DECL_CONSTEXPR QStringView mid(qsizetype pos) const
    { return Q_ASSERT(pos >= 0), Q_ASSERT(pos <= size()), QStringView(m_data + pos, m_size - pos); }
    Q_REQUIRED_RESULT Q_DECL_CONSTEXPR QStringView mid(qsizetype pos, qsizetype n) const
    { return Q_ASSERT(pos >= 0), Q_ASSERT(n >= 0), Q_ASSERT(pos + n <= size()), QStringView(m_data + pos, n); }
    Q_REQUIRED_RESULT Q_DECL_CONSTEXPR QStringView left(qsizetype n) const
    { return Q_ASSERT(n >= 0), Q_ASSERT(n <= size()), QStringView(m_data, n); }
    Q_REQUIRED_RESULT Q_DECL_CONSTEXPR QStringView right(qsizetype n) const
    { return Q_ASSERT(n >= 0), Q_ASSERT(n <= size()), QStringView(m_data + m_size - n, n); }
    Q_REQUIRED_RESULT Q_DECL_CONSTEXPR QStringView chopped(qsizetype n) const
    { return Q_ASSERT(n >= 0), Q_ASSERT(n <= size()), QStringView(m_data, m_size - n); }

    Q_DECL_RELAXED_CONSTEXPR void truncate(qsizetype n)
    { Q_ASSERT(n >= 0); Q_ASSERT(n <= size()); m_size = n; }
    Q_DECL_RELAXED_CONSTEXPR void chop(qsizetype n)
    { Q_ASSERT(n >= 0); Q_ASSERT(n <= size()); m_size -= n; }

    Q_REQUIRED_RESULT QStringView trimmed() const Q_DECL_NOTHROW { return QtPrivate::trimmed(*this); }

    Q_REQUIRED_RESULT bool startsWith(QStringView s, Qt::CaseSensitivity cs = Qt::CaseSensitive) const Q_DECL_NOTHROW
    { return QtPrivate::startsWith(*this, s, cs); }
    Q_REQUIRED_RESULT inline bool startsWith(QLatin1String s, Qt::CaseSensitivity cs = Qt::CaseSensitive) const Q_DECL_NOTHROW;
    Q_REQUIRED_RESULT bool startsWith(QChar c) const Q_DECL_NOTHROW
    { return !empty() && front() == c; }
    Q_REQUIRED_RESULT bool startsWith(QChar c, Qt::CaseSensitivity cs) const Q_DECL_NOTHROW
    { return QtPrivate::startsWith(*this, QStringView(&c, 1), cs); }

    Q_REQUIRED_RESULT bool endsWith(QStringView s, Qt::CaseSensitivity cs = Qt::CaseSensitive) const Q_DECL_NOTHROW
    { return QtPrivate::endsWith(*this, s, cs); }
    Q_REQUIRED_RESULT inline bool endsWith(QLatin1String s, Qt::CaseSensitivity cs = Qt::CaseSensitive) const Q_DECL_NOTHROW;
    Q_REQUIRED_RESULT bool endsWith(QChar c) const Q_DECL_NOTHROW
    { return !empty() && back() == c; }
    Q_REQUIRED_RESULT bool endsWith(QChar c, Qt::CaseSensitivity cs) const Q_DECL_NOTHROW
    { return QtPrivate::endsWith(*this, QStringView(&c, 1), cs); }

    //
    // STL compatibility API:
    //
    Q_REQUIRED_RESULT const_iterator begin()   const Q_DECL_NOTHROW { return data(); }
    Q_REQUIRED_RESULT const_iterator end()     const Q_DECL_NOTHROW { return data() + size(); }
    Q_REQUIRED_RESULT const_iterator cbegin()  const Q_DECL_NOTHROW { return begin(); }
    Q_REQUIRED_RESULT const_iterator cend()    const Q_DECL_NOTHROW { return end(); }
    Q_REQUIRED_RESULT const_reverse_iterator rbegin()  const Q_DECL_NOTHROW { return const_reverse_iterator(end()); }
    Q_REQUIRED_RESULT const_reverse_iterator rend()    const Q_DECL_NOTHROW { return const_reverse_iterator(begin()); }
    Q_REQUIRED_RESULT const_reverse_iterator crbegin() const Q_DECL_NOTHROW { return rbegin(); }
    Q_REQUIRED_RESULT const_reverse_iterator crend()   const Q_DECL_NOTHROW { return rend(); }

    Q_REQUIRED_RESULT Q_DECL_CONSTEXPR bool empty() const Q_DECL_NOTHROW { return size() == 0; }
    Q_REQUIRED_RESULT Q_DECL_CONSTEXPR QChar front() const { return Q_ASSERT(!empty()), QChar(m_data[0]); }
    Q_REQUIRED_RESULT Q_DECL_CONSTEXPR QChar back()  const { return Q_ASSERT(!empty()), QChar(m_data[m_size - 1]); }

    //
    // Qt compatibility API:
    //
    Q_REQUIRED_RESULT Q_DECL_CONSTEXPR bool isNull() const Q_DECL_NOTHROW { return !m_data; }
    Q_REQUIRED_RESULT Q_DECL_CONSTEXPR bool isEmpty() const Q_DECL_NOTHROW { return empty(); }
    Q_REQUIRED_RESULT Q_DECL_CONSTEXPR int length() const /* not nothrow! */
    { return Q_ASSERT(int(size()) == size()), int(size()); }
    Q_REQUIRED_RESULT Q_DECL_CONSTEXPR QChar first() const { return front(); }
    Q_REQUIRED_RESULT Q_DECL_CONSTEXPR QChar last()  const { return back(); }
private:
    qsizetype m_size;
    const storage_type *m_data;
};
Q_DECLARE_TYPEINFO(QStringView, Q_PRIMITIVE_TYPE);

template <typename QStringLike, typename std::enable_if<
    std::is_same<QStringLike, QString>::value || std::is_same<QStringLike, QStringRef>::value,
    bool>::type = true>
inline QStringView qToStringViewIgnoringNull(const QStringLike &s) Q_DECL_NOTHROW
{ return QStringView(s.data(), s.size()); }

QT_END_NAMESPACE

#endif /* QSTRINGVIEW_H */
