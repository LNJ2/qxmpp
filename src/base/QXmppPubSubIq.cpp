/*
 * Copyright (C) 2008-2021 The QXmpp developers
 *
 * Author:
 *  Linus Jahn
 *  Jeremy Lainé
 *  Germán Márquez Mejía
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

#include "QXmppPubSubIq.h"

#include "QXmppConstants_p.h"
#include "QXmppPubSubAffiliation.h"
#include "QXmppPubSubSubscription.h"
#include "QXmppUtils.h"

#include <optional>

#include <QSharedData>

///
/// \class QXmppPubSubIqBase
///
/// \brief The QXmppPubSubIqBase class is an abstract class used for parsing of
/// generic PubSub IQs as defined by \xep{0060}: Publish-Subscribe.
///
/// This class does not handle queries working with items. For a full-featured
/// PubSub-IQ, please use QXmppPubSubIq<T> with your needed item class.
///
/// \since QXmpp 1.4
///

///
/// \brief The QXmppPubSubIq class represents an IQ used for the
/// publish-subscribe mechanisms defined by \xep{0060}: Publish-Subscribe.
///
/// \ingroup Stanzas
///
/// \since QXmpp 1.4
///

///
/// \fn QXmppPubSubIq<T>::items()
///
/// Returns the IQ's items.
///

///
/// \fn QXmppPubSubIq<T>::setItems()
///
/// Sets the IQ's items.
///
/// \param items
///

///
/// \fn QXmppPubSubIq<T>::isPubSubIq()
///
/// Returns true, if the element is a valid PubSub IQ stanza. The payload of the
/// &lt;item/&gt; is also checked.
///

static const QStringList PUBSUB_QUERIES = {
    QStringLiteral("affiliations"),
    QStringLiteral("affiliations"),
    QStringLiteral("configure"),
    QStringLiteral("create"),
    QStringLiteral("default"),
    QStringLiteral("default"),
    QStringLiteral("delete"),
    QStringLiteral("items"),
    QStringLiteral("options"),
    QStringLiteral("publish"),
    QStringLiteral("purge"),
    QStringLiteral("retract"),
    QStringLiteral("subscribe"),
    QStringLiteral("subscription"),
    QStringLiteral("subscriptions"),
    QStringLiteral("subscriptions"),
    QStringLiteral("unsubscribe"),
};

class QXmppPubSubIqPrivate : public QSharedData
{
public:
    QXmppPubSubIqPrivate();

    QXmppPubSubIqBase::QueryType queryType;
    QString queryJid;
    QString queryNode;
    QString subscriptionId;
    QList<QXmppPubSubSubscription> subscriptions;
    QList<QXmppPubSubAffiliation> affiliations;
    unsigned int maxItems;
};

QXmppPubSubIqPrivate::QXmppPubSubIqPrivate()
    : queryType(QXmppPubSubIqBase::Items),
      maxItems(0)
{
}

///
/// Constructs a PubSub IQ.
///
QXmppPubSubIqBase::QXmppPubSubIqBase()
    : d(new QXmppPubSubIqPrivate)
{
}

///
/// Constructs a copy of other.
///
/// \param other
///
QXmppPubSubIqBase::QXmppPubSubIqBase(const QXmppPubSubIqBase &other) = default;

///
/// Default destructor.
///
QXmppPubSubIqBase::~QXmppPubSubIqBase() = default;

///
/// Assigns \a other to this IQ.
///
/// \param other
///
QXmppPubSubIqBase &QXmppPubSubIqBase::operator=(const QXmppPubSubIqBase &other) = default;

///
/// Returns the PubSub query type for this IQ.
///
QXmppPubSubIqBase::QueryType QXmppPubSubIqBase::queryType() const
{
    return d->queryType;
}

///
/// Sets the PubSub query type for this IQ.
///
/// \param queryType
///
void QXmppPubSubIqBase::setQueryType(QXmppPubSubIqBase::QueryType queryType)
{
    d->queryType = queryType;
}

///
/// Returns the JID being queried.
///
QString QXmppPubSubIqBase::queryJid() const
{
    return d->queryJid;
}

///
/// Sets the JID being queried.
///
/// \param queryJid
///
void QXmppPubSubIqBase::setQueryJid(const QString &queryJid)
{
    d->queryJid = queryJid;
}

///
/// Returns the name of the node being queried.
///
QString QXmppPubSubIqBase::queryNode() const
{
    return d->queryNode;
}

///
/// Sets the name of the node being queried.
///
/// \param queryNodeName
///
void QXmppPubSubIqBase::setQueryNode(const QString &queryNodeName)
{
    d->queryNode = queryNodeName;
}

///
/// Returns the subscription ID for the request.
///
/// This does not work for SubscriptionQuery IQs, use subscription() instead.
///
QString QXmppPubSubIqBase::subscriptionId() const
{
    return d->subscriptionId;
}

///
/// Sets the subscription ID for the request.
///
/// This does not work for SubscriptionQuery IQs, use setSubscription() instead.
///
void QXmppPubSubIqBase::setSubscriptionId(const QString &subscriptionId)
{
    d->subscriptionId = subscriptionId;
}

QList<QXmppPubSubSubscription> QXmppPubSubIqBase::subscriptions() const
{
    return d->subscriptions;
}

void QXmppPubSubIqBase::setSubscriptions(const QList<QXmppPubSubSubscription> &subscriptions)
{
    d->subscriptions = subscriptions;
}

///
/// Returns the subscription.
///
/// This is a utility function for subscriptions(). It returns the first
/// subscription if existant. This can be used for both query types,
/// Subscription and Subscriptions.
///
std::optional<QXmppPubSubSubscription> QXmppPubSubIqBase::subscription() const
{
    if (d->subscriptions.isEmpty())
        return std::nullopt;
    return d->subscriptions.first();
}

///
/// Sets the subscription.
///
/// This is a utility function for setSubscriptions(). It can be used for both
/// query types, Subscription and Subscriptions.
///
void QXmppPubSubIqBase::setSubscription(const std::optional<QXmppPubSubSubscription> &subscription)
{
    if (subscription)
        d->subscriptions = { *subscription };
    else
        d->subscriptions.clear();
}

QList<QXmppPubSubAffiliation> QXmppPubSubIqBase::affiliations() const
{
    return d->affiliations;
}

void QXmppPubSubIqBase::setAffiliations(const QList<QXmppPubSubAffiliation> &affiliations)
{
    d->affiliations = affiliations;
}

///
/// Returns the maximum of items that are requested.
///
/// This is only used for queries with type ItemsQuery.
///
unsigned int QXmppPubSubIqBase::maxItems() const
{
    return d->maxItems;
}

///
/// Sets the maximum of items that are requested.
///
/// This is only used for queries with type ItemsQuery.
///
void QXmppPubSubIqBase::setMaxItems(unsigned int maxItems)
{
    d->maxItems = maxItems;
}

/// \cond
bool QXmppPubSubIqBase::isPubSubIq(const QDomElement &element)
{
    // no special requirements for the item / it's payload
    return QXmppPubSubIqBase::isPubSubIq(element, [](const QDomElement &) {
        return true;
    });
}

bool QXmppPubSubIqBase::isPubSubIq(const QDomElement &element, std::function<bool(const QDomElement &item)> isItemValid)
{
    // IQs must have only one direct child element.
    const QDomElement pubSubElement = element.firstChildElement();
    if (pubSubElement.tagName() != QStringLiteral("pubsub"))
        return false;

    // check for correct namespace
    bool isOwner = pubSubElement.namespaceURI() == ns_pubsub_owner;
    if (!isOwner && pubSubElement.namespaceURI() != ns_pubsub)
        return false;

    // check that the query type is valid
    auto queryElement = pubSubElement.firstChildElement();
    auto optionalType = queryTypeFromDomElement(queryElement);
    if (!optionalType)
        return false;
    auto queryType = *optionalType;

    // check for the "node" attribute
    switch (queryType) {
    case OwnerAffiliations:
    case Items:
    case Publish:
    case Retract:
    case Delete:
    case Purge:
        if (!queryElement.hasAttribute(QStringLiteral("node")))
            return false;
    default:
        break;
    }

    // check for the "jid" attribute
    switch (queryType) {
    case Options:
    case OwnerSubscriptions:
    case Subscribe:
    case Unsubscribe:
        if (!queryElement.hasAttribute(QStringLiteral("jid")))
            return false;
    default:
        break;
    }

    // check the individual content
    switch (queryType) {
    case Items:
    case Publish:
    case Retract: {
        // check the items using isItemValid()
        auto itemElement = queryElement.firstChildElement(QStringLiteral("item"));
        while (!itemElement.isNull()) {
            if (!isItemValid(itemElement))
                return false;

            itemElement = itemElement.nextSiblingElement(QStringLiteral("item"));
        }
        break;
    }
    case Subscription:
        if (!QXmppPubSubSubscription::isSubscription(queryElement)) {
            return false;
        }
    case Delete:
    case Purge:
    case Configure:
        if (!isOwner)
            return false;
        break;
    case Affiliations:
    case OwnerAffiliations:
    case Create:
    case Default:
    case OwnerDefault:
    case Options:
    case Subscribe:
    case Subscriptions:
    case OwnerSubscriptions:
    case Unsubscribe:
        break;
    }

    return true;
}

void QXmppPubSubIqBase::parseElementFromChild(const QDomElement &element)
{
    const QDomElement pubSubElement = element.firstChildElement(QStringLiteral("pubsub"));
    const QDomElement queryElement = pubSubElement.firstChildElement();

    // parse query type
    if (auto type = queryTypeFromDomElement(queryElement)) {
        d->queryType = *type;
    } else {
        return;
    }

    // SubscriptionQuery is special: The query element is directly handled by
    // QXmppPubSubSubscription.
    if (d->queryType == Subscription) {
        QXmppPubSubSubscription subscription;
        subscription.parse(queryElement);
        setSubscription(subscription);
        return;
    }

    d->queryJid = queryElement.attribute(QStringLiteral("jid"));
    d->queryNode = queryElement.attribute(QStringLiteral("node"));

    // parse subid
    switch (d->queryType) {
    case Items:
    case Unsubscribe:
    case Options:
        d->subscriptionId = queryElement.attribute(QStringLiteral("subid"));
    default:
        break;
    }

    // parse contents
    QDomElement childElement;

    switch (d->queryType) {
    case Affiliations:
    case OwnerAffiliations: {
        auto subElement = queryElement.firstChildElement();
        while (!subElement.isNull()) {
            if (QXmppPubSubAffiliation::isAffiliation(subElement)) {
                QXmppPubSubAffiliation affiliation;
                affiliation.parse(subElement);

                d->affiliations << affiliation;
            }
            subElement = subElement.nextSiblingElement();
        }
        break;
    }
    case Items:
        d->maxItems = queryElement.attribute(QStringLiteral("max_items")).toUInt();
        [[fallthrough]];
    case Publish:
    case Retract:
        parseItems(queryElement);
        break;
    case Subscriptions:
    case OwnerSubscriptions: {
        for (auto subElement = queryElement.firstChildElement();
             !subElement.isNull();
             subElement = subElement.nextSiblingElement()) {
            if (QXmppPubSubSubscription::isSubscription(subElement)) {
                QXmppPubSubSubscription subscription;
                subscription.parse(subElement);

                d->subscriptions << subscription;
            }
        }
        break;
    }
    case Configure:
    case Create:
    case Default:
    case OwnerDefault:
    case Delete:
    case Options:
    case Purge:
    case Subscribe:
    case Subscription:
    case Unsubscribe:
        break;
    }
}

void QXmppPubSubIqBase::toXmlElementFromChild(QXmlStreamWriter *writer) const
{
    writer->writeStartElement(QStringLiteral("pubsub"));
    writer->writeDefaultNamespace(queryTypeIsOwnerIq(d->queryType) ? ns_pubsub_owner : ns_pubsub);

    // The SubscriptionQuery is special here: The query element is directly
    // handled by a QXmppPubSubSubscription.
    if (d->queryType == Subscription) {
        subscription().value_or(QXmppPubSubSubscription()).toXml(writer);
    } else {
        // write query type
        writer->writeStartElement(PUBSUB_QUERIES.at(d->queryType));
        helperToXmlAddAttribute(writer, QStringLiteral("jid"), d->queryJid);
        helperToXmlAddAttribute(writer, QStringLiteral("node"), d->queryNode);

        // write subid
        switch (d->queryType) {
        case Items:
        case Unsubscribe:
        case Options:
            helperToXmlAddAttribute(writer, QStringLiteral("subid"), d->subscriptionId);
        default:
            break;
        }

        // write contents
        switch (d->queryType) {
        case Affiliations:
        case OwnerAffiliations:
            for (const auto &affiliation : qAsConst(d->affiliations)) {
                affiliation.toXml(writer);
            }
            break;
        case Items:
            if (d->maxItems > 0)
                writer->writeAttribute(QStringLiteral("max_items"), QString::number(d->maxItems));

            [[fallthrough]];
        case Publish:
        case Retract:
            serializeItems(writer);
            break;
        case Subscriptions:
        case OwnerSubscriptions:
            for (const auto &sub : qAsConst(d->subscriptions)) {
                sub.toXml(writer);
            }
            break;
        case Configure:
        case Create:
        case Default:
        case OwnerDefault:
        case Delete:
        case Options:
        case Purge:
        case Subscribe:
        case Subscription:
        case Unsubscribe:
            break;
        }

        writer->writeEndElement();  // query type
    }
    writer->writeEndElement();  // pubsub
}
/// \endcond

std::optional<QXmppPubSubIqBase::QueryType> QXmppPubSubIqBase::queryTypeFromDomElement(const QDomElement &element)
{
    auto index = PUBSUB_QUERIES.indexOf(element.tagName());
    if (index == -1) {
        return std::nullopt;
    }

    auto type = QueryType(index);

    // Some queries can have ns_pubsub_owner and normal ns_pubsub. To
    // distinguish those after parsing those with ns_pubsub_owner are replaced
    // by another query type.

    if (element.namespaceURI() != ns_pubsub_owner)
        return type;

    switch (type) {
    case Affiliations:
        return OwnerAffiliations;
    case Default:
        return OwnerDefault;
    case Subscriptions:
        return OwnerSubscriptions;
    default:
        return type;
    }
}

bool QXmppPubSubIqBase::queryTypeIsOwnerIq(QueryType type)
{
    switch (type) {
    case OwnerAffiliations:
    case OwnerSubscriptions:
    case OwnerDefault:
    case Configure:
    case Delete:
    case Purge:
        return true;
    case Affiliations:
    case Create:
    case Default:
    case Items:
    case Options:
    case Publish:
    case Retract:
    case Subscribe:
    case Subscription:
    case Subscriptions:
    case Unsubscribe:
        return false;
    }
    Q_UNREACHABLE();
}
