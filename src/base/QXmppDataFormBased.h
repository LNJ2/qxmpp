/*
 * Copyright (C) 2008-2020 The QXmpp developers
 *
 * Author:
 *  Linus Jahn
 *
 * Source:
 *  https://github.com/qxmpp-project/qxmpp
 *
 * This file is a part of QXmpp library.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 */

#ifndef QXMPPDATAFORMBASED_H
#define QXMPPDATAFORMBASED_H

#include "QXmppDataForm.h"

///
/// \brief QXmppDataFormBased is an abstract class types that can be serialized
/// to data forms.
///
/// QXmppDataFormBased based types can easily be converted to QXmppDataForms, it
/// is as simple as this:
/// \code
/// MyDataFormBased foo;
/// QXmppDataForm dataForm(foo);
/// \endcode
///
/// To make this work, you will need to at least implement the toDataForm()
/// method. For parsing your type you should also create a static creator
/// method, like this:
/// \code
/// static std::optional<MyType> fromDataForm(const QXmppDataForm &);
/// \endcode
///
/// \since QXmpp 1.4
///
class QXMPP_EXPORT QXmppDataFormBased
{
public:
    virtual ~QXmppDataFormBased() = default;

    virtual QXmppDataForm toDataForm() const;

protected:
    enum RequiredType : bool {
        Optional = false,
        Required = true,
    };

    class FieldDescriptorPrivate;

    class QXMPP_EXPORT FieldDescriptor
    {

    public:
        FieldDescriptor(QXmppDataForm::Field::Type type,
                        QString key,
                        RequiredType required,
                        const std::function<void (const QXmppDataForm::Field &)> &parse,
                        const std::function<void (QXmppDataForm::Field &)> &serialize);
        FieldDescriptor(const FieldDescriptor &);
        ~FieldDescriptor();

        FieldDescriptor &operator=(const FieldDescriptor &);

        QXmppDataForm::Field::Type type() const;
        QString key() const;
        RequiredType required() const;
        const std::function<void (const QXmppDataForm::Field &)> &parseFunction() const;
        const std::function<void (QXmppDataForm::Field &)> &serializeFunction() const;

    private:
        QSharedDataPointer<FieldDescriptorPrivate> d;
    };

    static bool fromDataForm(const QXmppDataForm &form, QXmppDataFormBased &parent);

    virtual QString formType() const;
    virtual QList<FieldDescriptor> fieldDescriptors();
    virtual QList<FieldDescriptor> fieldDescriptors() const;
    virtual void parseForm(const QXmppDataForm &);
    virtual void serializeForm(QXmppDataForm &) const;
};

#endif // QXMPPDATAFORMBASED_H
