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

#ifndef QXMPPPUBSUBSUBSCRIBEOPTIONS_H
#define QXMPPPUBSUBSUBSCRIBEOPTIONS_H

#include "QXmppGlobal.h"

#include <QFlags>
#include <QSharedDataPointer>

class QDateTime;
class QDomElement;
class QXmlStreamWriter;
class QXmppDataForm;

class QXMPP_EXPORT QXmppPubSubSubscribeOptions
{
public:
    enum PresenceState : quint8 {
        Unset = 0x00,
        Online = 0x01,
        Away = 0x02,
        Chat = 0x04,
        DoNotDisturb = 0x08,
        ExtendedAway = 0x10
    };
    Q_DECLARE_FLAGS(PresenceStates, PresenceState)

    enum SubscriptionType : quint8 {
        Items,
        Nodes
    };

    enum SubscriptionDepth : quint8 {
        TopLevelOnly,
        Recursive
    };

    QXmppPubSubSubscribeOptions();
    QXmppPubSubSubscribeOptions(const QXmppPubSubSubscribeOptions &);
    ~QXmppPubSubSubscribeOptions();

    QXmppPubSubSubscribeOptions &operator=(const QXmppPubSubSubscribeOptions &);

    bool notificationsEnabled() const;
    void setNotifying(bool notifying);

    bool digestsEnabled() const;
    void setDigestsEnabled(bool digestsEnabled);

    quint32 digestFrequencyMs() const;
    void setDigestFrequencyMs(const quint32 &digestFrequencyMs);

    QDateTime expire() const;
    void setExpire(const QDateTime &expire);

    bool bodyIncluded() const;
    void setBodyIncluded(bool bodyIncluded);

    PresenceStates notificationRules() const;
    void setNotificationRules(const PresenceStates &notificationRules);

    SubscriptionType subscriptionType() const;
    void setSubscriptionType(const SubscriptionType &subscriptionType);

    SubscriptionDepth subscriptionDepth() const;
    void setSubscriptionDepth(const SubscriptionDepth &subscriptionDepth);

    static bool isSubscribeOptions(const QDomElement &);

    /// \cond
    void parse(const QDomElement &);
    void toXml(QXmlStreamWriter *) const;
    /// \endcond

private:
    QSharedDataPointer<QXmppPubSubSubscribeOptionsPrivate> d;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QXmppPubSubSubscribeOptions::PresenceState)

#endif // QXMPPPUBSUBSUBSCRIBEOPTIONS_H
