/*
 * Copyright (C) 2008-2020 The QXmpp developers
 *
 * Author:
 *  Linus Jahn
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

#ifndef QXMPPPUBSUBEVENT_H
#define QXMPPPUBSUBEVENT_H

#include "QXmppMessage.h"
#include "QXmppPubSubSubscription.h"

#include <functional>

#include <QDomElement>
#include <QSharedData>

class QXmppDataForm;
class QXmppPubSubEventPrivate;
class QXmppPubSubItem;

///
/// \brief The QXmppAbstractPubSubEvent class is an abstract class used for
/// parsing of generic PubSub event notifications as defined by \xep{0060}:
/// Publish-Subscribe.
///
/// This class cannot be used directly. For a full-featured access to the event
/// notifications, please use the QXmppPubSubEvent class.
///
/// \since QXmpp 1.4
///
class QXMPP_EXPORT QXmppPubSubEventBase : public QXmppMessage
{
public:
    enum EventType {
        Configuration,
        Delete,
        Items,
        Purge,
        Subscription,
    };

    QXmppPubSubEventBase(EventType = Items, const QString &node = {});
    QXmppPubSubEventBase(const QXmppPubSubEventBase &other);
    virtual ~QXmppPubSubEventBase();

    QXmppPubSubEventBase &operator=(const QXmppPubSubEventBase &other);

    EventType eventType() const;
    void setEventType(EventType);

    QString node() const;
    void setNode(const QString &node);

    QStringList retractIds() const;
    void setRetractIds(const QStringList &);

    QString redirectUri() const;
    void setRedirectUri(const QString &);

    std::optional<QXmppPubSubSubscription> subscription() const;
    void setSubscription(const std::optional<QXmppPubSubSubscription> &subscription);

    std::optional<QXmppDataForm> configurationForm() const;
    void setConfigurationForm(const std::optional<QXmppDataForm> &configurationForm);

protected:
    /// \cond
    static bool isPubSubEvent(const QDomElement &element, std::function<bool(const QDomElement &)> isItemValid);

    bool parseExtension(const QDomElement &element) override;
    void serializeExtensions(QXmlStreamWriter *writer) const override;

    virtual void parseItems(const QDomElement &) = 0;
    virtual void serializeItems(QXmlStreamWriter *writer) const = 0;
    /// \endcond

private:
    QSharedDataPointer<QXmppPubSubEventPrivate> d;
};

template<class T = QXmppPubSubItem>
class QXMPP_EXPORT QXmppPubSubEvent : public QXmppPubSubEventBase
{
public:
    QList<T> items() const;
    void setItems(const QList<T> &items);

    static bool isPubSubEvent(const QDomElement &element);

protected:
    /// \cond
    void parseItems(const QDomElement &) override;
    void serializeItems(QXmlStreamWriter *writer) const override;
    /// \endcond

private:
    QList<T> m_items;
};

///
/// Returns the PubSub items of the event.
///
template<class T>
QList<T> QXmppPubSubEvent<T>::items() const
{
    return m_items;
}

///
/// Sets the PubSub items of the event.
///
template<class T>
void QXmppPubSubEvent<T>::setItems(const QList<T> &items)
{
    m_items = items;
}

///
/// Returns whether the element is a valid QXmppPubSubEvent and contains only
/// valid items of type T.
///
template<class T>
bool QXmppPubSubEvent<T>::isPubSubEvent(const QDomElement &element)
{
    return QXmppPubSubEventBase::isPubSubEvent(element, [](const QDomElement &element) {
        return T::isItem(element);
    });
}

/// \cond
template<class T>
void QXmppPubSubEvent<T>::parseItems(const QDomElement &parent)
{
    QDomElement child = parent.firstChildElement(QStringLiteral("item"));
    while (!child.isNull()) {
        T item;
        item.parse(child);
        m_items << item;

        child = child.nextSiblingElement(QStringLiteral("item"));
    }
}

template<class T>
void QXmppPubSubEvent<T>::serializeItems(QXmlStreamWriter *writer) const
{
    for (const auto &item : qAsConst(m_items)) {
        item.toXml(writer);
    }
}
/// \endcond

Q_DECLARE_METATYPE(QXmppPubSubEventBase::EventType)

#endif // QXMPPPUBSUBEVENT_H
