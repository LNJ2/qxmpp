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

#include "QXmppPubSubSubAuthorization.h"

#define FORM_TYPE_SUBSCRIBE_AUTHORIZATION QStringLiteral("http://jabber.org/protocol/pubsub#subscribe_authorization");

class QXmppPubSubSubAuthorizationPrivate : public QSharedData
{
public:
    std::optional<bool> allowSubscription;
    QString node;
    QString subscriberJid;
    QString subid;
};

std::optional<QXmppPubSubSubAuthorization> QXmppPubSubSubAuthorization::fromDataForm(const QXmppDataForm &form)
{
    auto parsed = QXmppPubSubSubAuthorization();
    if (QXmppDataFormBased::fromDataForm(form, parsed))
        return parsed;
    return std::nullopt;
}

QXmppPubSubSubAuthorization::QXmppPubSubSubAuthorization()
    : d(new QXmppPubSubSubAuthorizationPrivate)
{
}

QXmppPubSubSubAuthorization::QXmppPubSubSubAuthorization(const QXmppPubSubSubAuthorization &) = default;

QXmppPubSubSubAuthorization::~QXmppPubSubSubAuthorization() = default;

QXmppPubSubSubAuthorization &QXmppPubSubSubAuthorization::operator=(const QXmppPubSubSubAuthorization &) = default;

std::optional<bool> QXmppPubSubSubAuthorization::allowSubscription() const
{
    return d->allowSubscription;
}

void QXmppPubSubSubAuthorization::setAllowSubscription(std::optional<bool> allowSubscription)
{
    d->allowSubscription = allowSubscription;
}

QString QXmppPubSubSubAuthorization::node() const
{
    return d->node;
}

void QXmppPubSubSubAuthorization::setNode(const QString &node)
{
    d->node = node;
}

QString QXmppPubSubSubAuthorization::subscriberJid() const
{
    return d->subscriberJid;
}

void QXmppPubSubSubAuthorization::setSubscriberJid(const QString &subscriberJid)
{
    d->subscriberJid = subscriberJid;
}

QString QXmppPubSubSubAuthorization::subid() const
{
    return d->subid;
}

void QXmppPubSubSubAuthorization::setSubid(const QString &subid)
{
    d->subid = subid;
}

QList<QXmppDataFormBased::FieldDescriptor> QXmppPubSubSubAuthorization::fieldDescriptors()
{
    return {
        {
            QXmppDataForm::Field::BooleanField,
            QStringLiteral("pubsub#allow"),
            Optional,
            [=](const QXmppDataForm::Field &field) {
                d->allowSubscription = field.value().toBool();
            },
            [=](QXmppDataForm::Field &field) {
                if (d->allowSubscription) {
                    field.setValue(*d->allowSubscription);
                }
            }
        },
        {
            QXmppDataForm::Field::TextSingleField,
            QStringLiteral("pubsub#node"),
            Optional,
            [=](const QXmppDataForm::Field &field) {
                d->node = field.value().toString();
            },
            [=](QXmppDataForm::Field &field) {
                field.setValue(d->node);
            }
        },
        {
            QXmppDataForm::Field::TextSingleField,
            QStringLiteral("pubsub#subid"),
            Optional,
            [=](const QXmppDataForm::Field &field) {
                d->subid = field.value().toString();
            },
            [=](QXmppDataForm::Field &field) {
                field.setValue(d->subid);
            }
        },
        {
            QXmppDataForm::Field::JidSingleField,
            QStringLiteral("pubsub#subscriber_jid"),
            Optional,
            [=](const QXmppDataForm::Field &field) {
                d->subscriberJid = field.value().toString();
            },
            [=](QXmppDataForm::Field &field) {
                field.setValue(d->subscriberJid);
            }
        }
    };
}

QString QXmppPubSubSubAuthorization::formType() const
{
    return FORM_TYPE_SUBSCRIBE_AUTHORIZATION;
}
