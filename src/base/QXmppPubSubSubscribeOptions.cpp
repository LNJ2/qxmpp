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
    QDateTime expire;
    QXmppPubSubSubscribeOptions::PresenceStates notificationRules;
    QXmppPubSubSubscribeOptions::SubscriptionType subscriptionType;
    QXmppPubSubSubscribeOptions::SubscriptionDepth subscriptionDepth;
};

QXmppPubSubSubscribeOptionsPrivate::QXmppPubSubSubscribeOptionsPrivate()
    : notificationsEnabled(true),
      digestsEnabled(false),
      digestFrequencyMs(0),
      notificationRules(QXmppPubSubSubscribeOptions::Online),
      subscriptionType(QXmppPubSubSubscribeOptions::Items),
      subscriptionDepth(QXmppPubSubSubscribeOptions::)
{
}

QXmppPubSubSubscribeOptions::QXmppPubSubSubscribeOptions()
    : d(new QXmppPubSubSubscribeOptions())
{
}

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

void QXmppPubSubSubscribeOptions::setDigestFrequencyMs(const quint32 &digestFrequencyMs)
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

PresenceStates QXmppPubSubSubscribeOptions::notificationRules() const
{
    return d->notificationRules;
}

void QXmppPubSubSubscribeOptions::setNotificationRules(const PresenceStates &notificationRules)
{
    d->notificationRules = notificationRules;
}

SubscriptionType QXmppPubSubSubscribeOptions::subscriptionType() const
{
    return d->subscriptionType;
}

void QXmppPubSubSubscribeOptions::setSubscriptionType(const SubscriptionType &subscriptionType)
{
    d->subscriptionType = subscriptionType;
}

SubscriptionDepth QXmppPubSubSubscribeOptions::subscriptionDepth() const
{
    return d->subscriptionDepth;
}

void QXmppPubSubSubscribeOptions::setSubscriptionDepth(const SubscriptionDepth &subscriptionDepth)
{
    d->subscriptionDepth = subscriptionDepth;
}

void QXmppPubSubSubscribeOptions::toXml(QXmlStreamWriter *writer) const
{
    QXmppDataForm form;
}
