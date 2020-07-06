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

#include "QXmppPubSubSubscribeOptions.h"

#include "QXmppDataForm.h"

#include <QDateTime>

class QXmppPubSubSubscribeOptionsPrivate : public QSharedData
{
public:
    QXmppPubSubSubscribeOptionsPrivate();

    bool notificationsEnabled;
    bool digestsEnabled;
    quint32 digestFrequencyMs;
    bool bodyIncluded;
    QDateTime expire;
    QXmppPubSubSubscribeOptions::PresenceStates notificationRules;
    std::optional<QXmppPubSubSubscribeOptions::SubscriptionType> subscriptionType;
    std::optional<QXmppPubSubSubscribeOptions::SubscriptionDepth> subscriptionDepth;
};

QXmppPubSubSubscribeOptionsPrivate::QXmppPubSubSubscribeOptionsPrivate()
    : notificationsEnabled(true),
      digestsEnabled(false),
      digestFrequencyMs(0),
      notificationRules(QXmppPubSubSubscribeOptions::Online),
      subscriptionType(QXmppPubSubSubscribeOptions::Items),
      subscriptionDepth(QXmppPubSubSubscribeOptions::TopLevelOnly)
{
}

QXmppPubSubSubscribeOptions::PresenceStates QXmppPubSubSubscribeOptions::presenceStatesFromStringList(const QStringList &values)
{
    PresenceStates states;
    if (values.contains(QStringLiteral("away")))
        states |= Away;
    if (values.contains(QStringLiteral("chat")))
        states |= Chat;
    if (values.contains(QStringLiteral("dnd")))
        states |= DoNotDisturb;
    if (values.contains(QStringLiteral("online")))
        states |= Online;
    if (values.contains(QStringLiteral("xa")))
        states |= ExtendedAway;
    return states;
}

QStringList QXmppPubSubSubscribeOptions::presenceStatesToStringList(PresenceStates states)
{
    QStringList output;
    if (states & Away)
        output << QStringLiteral("away");
    if (states & Chat)
        output << QStringLiteral("chat");
    if (states & DoNotDisturb)
        output << QStringLiteral("dnd");
    if (states & Online)
        output << QStringLiteral("online");
    if (states & ExtendedAway)
        output << QStringLiteral("xa");
    return output;
}

QXmppPubSubSubscribeOptions::QXmppPubSubSubscribeOptions()
    : d(new QXmppPubSubSubscribeOptionsPrivate())
{
}

QXmppPubSubSubscribeOptions::QXmppPubSubSubscribeOptions(const QXmppPubSubSubscribeOptions &) = default;

QXmppPubSubSubscribeOptions::~QXmppPubSubSubscribeOptions() = default;

QXmppPubSubSubscribeOptions &QXmppPubSubSubscribeOptions::operator=(const QXmppPubSubSubscribeOptions &) = default;

bool QXmppPubSubSubscribeOptions::notificationsEnabled() const
{
    return d->notificationsEnabled;
}

void QXmppPubSubSubscribeOptions::setNotificationsEnabled(bool enabled)
{
    d->notificationsEnabled = enabled;
}

bool QXmppPubSubSubscribeOptions::digestsEnabled() const
{
    return d->digestsEnabled;
}

void QXmppPubSubSubscribeOptions::setDigestsEnabled(bool digestsEnabled)
{
    d->digestsEnabled = digestsEnabled;
}

quint32 QXmppPubSubSubscribeOptions::digestFrequencyMs() const
{
    return d->digestFrequencyMs;
}

void QXmppPubSubSubscribeOptions::setDigestFrequencyMs(quint32 digestFrequencyMs)
{
    d->digestFrequencyMs = digestFrequencyMs;
}

QDateTime QXmppPubSubSubscribeOptions::expire() const
{
    return d->expire;
}

void QXmppPubSubSubscribeOptions::setExpire(const QDateTime &expire)
{
    d->expire = expire;
}

bool QXmppPubSubSubscribeOptions::bodyIncluded() const
{
    return d->bodyIncluded;
}

void QXmppPubSubSubscribeOptions::setBodyIncluded(bool bodyIncluded)
{
    d->bodyIncluded = bodyIncluded;
}

QXmppPubSubSubscribeOptions::PresenceStates QXmppPubSubSubscribeOptions::notificationRules() const
{
    return d->notificationRules;
}

void QXmppPubSubSubscribeOptions::setNotificationRules(PresenceStates notificationRules)
{
    d->notificationRules = notificationRules;
}

std::optional<QXmppPubSubSubscribeOptions::SubscriptionType> QXmppPubSubSubscribeOptions::subscriptionType() const
{
    return d->subscriptionType;
}

void QXmppPubSubSubscribeOptions::setSubscriptionType(std::optional<SubscriptionType> subscriptionType)
{
    d->subscriptionType = subscriptionType;
}

std::optional<QXmppPubSubSubscribeOptions::SubscriptionDepth> QXmppPubSubSubscribeOptions::subscriptionDepth() const
{
    return d->subscriptionDepth;
}

void QXmppPubSubSubscribeOptions::setSubscriptionDepth(std::optional<SubscriptionDepth> subscriptionDepth)
{
    d->subscriptionDepth = subscriptionDepth;
}

QList<QXmppDataFormBased::FieldDescriptor> QXmppPubSubSubscribeOptions::fieldDescriptors()
{
    return {
        { QXmppDataForm::Field::BooleanField,
          QStringLiteral("pubsub#deliver"),
          Optional,
          [=](const QXmppDataForm::Field &field) {
              d->notificationsEnabled = field.value().toBool();
          },
          [=](QXmppDataForm::Field &field) {
              field.setValue(d->notificationsEnabled);
          } },
        { QXmppDataForm::Field::BooleanField,
          QStringLiteral("pubsub#digest"),
          Optional,
          [=](const QXmppDataForm::Field &field) {
              d->digestsEnabled = field.value().toBool();
          },
          [=](QXmppDataForm::Field &field) {
              field.setValue(d->digestsEnabled);
          } },
        { QXmppDataForm::Field::TextSingleField,
          QStringLiteral("pubsub#digest_frequency"),
          Optional,
          [=](const QXmppDataForm::Field &field) {
              d->digestFrequencyMs = field.value().toUInt();
          },
          [=](QXmppDataForm::Field &field) {
              field.setValue(QString::number(d->digestFrequencyMs));
          } },
        { QXmppDataForm::Field::TextSingleField,
          QStringLiteral("pubsub#expire"),
          Optional,
          [=](const QXmppDataForm::Field &field) {
              d->expire = QDateTime::fromString(field.value().toString(), Qt::ISODate);
          },
          [=](QXmppDataForm::Field &field) {
              field.setValue(d->expire.toString(Qt::ISODate));
          } },
        { QXmppDataForm::Field::TextSingleField,
          QStringLiteral("pubsub#include_body"),
          Optional,
          [=](const QXmppDataForm::Field &field) {
              d->bodyIncluded = field.value().toBool();
          },
          [=](QXmppDataForm::Field &field) {
              field.setValue(d->bodyIncluded);
          } },
        { QXmppDataForm::Field::ListMultiField,
          QStringLiteral("pubsub#show-values"),
          Optional,
          [=](const QXmppDataForm::Field &field) {
              d->notificationRules = presenceStatesFromStringList(field.value().toStringList());
          },
          [=](QXmppDataForm::Field &field) {
              field.setValue(presenceStatesToStringList(d->notificationRules));
          } },
        { QXmppDataForm::Field::ListSingleField,
          QStringLiteral("pubsub#subscription_type"),
          Optional,
          [=](const QXmppDataForm::Field &field) {
              const auto value = field.value().toString();
              if (value == QStringLiteral("items"))
                  d->subscriptionType = Items;
              else if (value == QStringLiteral("nodes"))
                  d->subscriptionType = Nodes;
          },
          [=](QXmppDataForm::Field &field) {
              if (!d->subscriptionType)
                  return;

              switch (*d->subscriptionType) {
              case Items:
                  field.setValue(QStringLiteral("items"));
                  return;
              case Nodes:
                  field.setValue(QStringLiteral("nodes"));
                  return;
              }
          } },
        { QXmppDataForm::Field::ListSingleField,
          QStringLiteral("pubsub#subscription_depth"),
          Optional,
          [=](const QXmppDataForm::Field &field) {
              const auto value = field.value().toString();

              if (value == QChar(u'1')) {
                  d->subscriptionDepth = TopLevelOnly;
              } else if (value == QStringLiteral("all")) {
                  d->subscriptionDepth = Recursive;
              }
          },
          [=](QXmppDataForm::Field &field) {
              if (!d->subscriptionDepth)
                  return;

              switch (*d->subscriptionDepth) {
              case TopLevelOnly:
                  field.setValue(QStringLiteral("1"));
                  return;
              case Recursive:
                  field.setValue(QStringLiteral("all"));
                  return;
              }
          } },
    };
}
