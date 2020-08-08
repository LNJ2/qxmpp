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

#include "QXmppDataFormBased.h"

class QXmppDataFormBased::FieldDescriptorPrivate : public QSharedData
{
public:
    FieldDescriptorPrivate(QXmppDataForm::Field::Type type,
                           QString key,
                           QXmppDataFormBased::RequiredType required,
                           const std::function<void (const QXmppDataForm::Field &)> &parse,
                           const std::function<void (QXmppDataForm::Field &)> &serialize);

    QXmppDataForm::Field::Type type;
    QString key;
    RequiredType required;

    std::function<void (const QXmppDataForm::Field &)> parse;
    std::function<void (QXmppDataForm::Field &)> serialize;
};

QXmppDataFormBased::FieldDescriptorPrivate::FieldDescriptorPrivate(QXmppDataForm::Field::Type type,
        QString key,
        QXmppDataFormBased::RequiredType required,
        const std::function<void (const QXmppDataForm::Field &)> &parse,
        const std::function<void (QXmppDataForm::Field &)> &serialize)
    : type(type),
      key(key),
      required(required),
      parse(parse),
      serialize(serialize)
{
}

QXmppDataFormBased::FieldDescriptor::FieldDescriptor(QXmppDataForm::Field::Type type,
        QString key,
        QXmppDataFormBased::RequiredType required,
        const std::function<void (const QXmppDataForm::Field &)> &parse,
        const std::function<void (QXmppDataForm::Field &)> &serialize)
    : d(new FieldDescriptorPrivate(type, key, required, parse, serialize))
{
}

QXmppDataFormBased::FieldDescriptor::FieldDescriptor(const QXmppDataFormBased::FieldDescriptor &) = default;

QXmppDataFormBased::FieldDescriptor::~FieldDescriptor() = default;

QXmppDataFormBased::FieldDescriptor &QXmppDataFormBased::FieldDescriptor::operator=(const QXmppDataFormBased::FieldDescriptor &) = default;

QXmppDataForm::Field::Type QXmppDataFormBased::FieldDescriptor::type() const
{
    return d->type;
}

QString QXmppDataFormBased::FieldDescriptor::key() const
{
    return d->key;
}

const std::function<void (const QXmppDataForm::Field &)> &QXmppDataFormBased::FieldDescriptor::parseFunction() const
{
    return d->parse;
}

const std::function<void (QXmppDataForm::Field &)> &QXmppDataFormBased::FieldDescriptor::serializeFunction() const
{
    return d->serialize;
}

QXmppDataFormBased::RequiredType QXmppDataFormBased::FieldDescriptor::required() const
{
    return d->required;
}

QXmppDataForm QXmppDataFormBased::toDataForm() const
{
    QXmppDataForm form;

    // add FORM_TYPE
    if (!formType().isEmpty()) {
        form.fields() << QXmppDataForm::Field(QXmppDataForm::Field::HiddenField,
                                              QStringLiteral("FORM_TYPE"),
                                              formType());
    }

    // add fields from descriptors
    const auto fieldDescs = fieldDescriptors();
    for (const auto &fieldDescriptor : fieldDescs) {
        QXmppDataForm::Field field(fieldDescriptor.type(), fieldDescriptor.key());
        fieldDescriptor.serializeFunction()(field);

        if (!field.value().isNull())
            form.fields() << field;
    }

    // manual serialization parts
    serializeForm(form);

    return form;
}

bool QXmppDataFormBased::fromDataForm(const QXmppDataForm &form, QXmppDataFormBased &output)
{
    output.parseForm(form);

    const auto fields = form.fields();
    const auto fieldDescriptors = output.fieldDescriptors();

    for (const auto &fieldDescriptor : fieldDescriptors) {
        auto itr = std::find_if(fields.begin(), fields.end(), [&](const QXmppDataForm::Field &field) {
            return (field.type() == fieldDescriptor.type() ||
                    field.type() == QXmppDataForm::Field::HiddenField) &&
                field.key() == fieldDescriptor.key();
        });

        if (itr != fields.end()) {
            fieldDescriptor.parseFunction()(*itr);
        } else if (fieldDescriptor.required() == Required) {
            return false;
        }
    }

    return true;
}

QList<QXmppDataFormBased::FieldDescriptor> QXmppDataFormBased::fieldDescriptors()
{
    return {};
}

QList<QXmppDataFormBased::FieldDescriptor> QXmppDataFormBased::fieldDescriptors() const
{
    return const_cast<QXmppDataFormBased *>(this)->fieldDescriptors();
}

QString QXmppDataFormBased::formType() const
{
    return {};
}

void QXmppDataFormBased::parseForm(const QXmppDataForm &)
{
}

void QXmppDataFormBased::serializeForm(QXmppDataForm &form) const
{
    form.setType(QXmppDataForm::Form);
}
