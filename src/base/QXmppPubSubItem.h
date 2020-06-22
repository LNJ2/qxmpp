/*
 * Copyright (C) 2008-2021 The QXmpp developers
 *
 * Author:
 *  Jeremy Lainé
 *  Germán Márquez Mejía
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

#ifndef QXMPPPUBSUBITEM_H
#define QXMPPPUBSUBITEM_H

#include "QXmppGlobal.h"

#include <functional>

#include <QDomElement>
#include <QMetaType>
#include <QSharedDataPointer>

class QXmlStreamWriter;
class QXmppPubSubItemPrivate;

///
/// \brief The QXmppPubSubItem class represents a publish-subscribe item
/// as defined by \xep{0060}: Publish-Subscribe.
///
/// To access the payload of an item, you need to create a derived class of this
/// and override QXmppPubSubItem::parsePayload() and
/// QXmppPubSubItem::serializePayload().
///
/// It is also required that you override QXmppPubSubItem::isItem() and also
/// check for the correct payload of the PubSub item. This can be easily done by
/// using the protected overload of isItem() with an function that checks the
/// tag name and namespace of the payload. The function is only called if a
/// payload exists.
///
/// In short, you need to reimplement these methods:
///  * QXmppPubSubItem::parsePayload()
///  * QXmppPubSubItem::serializePayload()
///  * QXmppPubSubItem::isItem()
///
class QXMPP_EXPORT QXmppPubSubItem
{
public:
    QXmppPubSubItem(const QString &id = {}, const QString &publisher = {});
    QXmppPubSubItem(const QXmppPubSubItem &);
    virtual ~QXmppPubSubItem();

    QXmppPubSubItem &operator=(const QXmppPubSubItem &);

    QString id() const;
    void setId(const QString &id);

    QString publisher() const;
    void setPublisher(const QString &publisher);

    /// \cond
    void parse(const QDomElement &element);
    void toXml(QXmlStreamWriter *writer) const;
    /// \endcond

    static bool isItem(const QDomElement &element);

protected:
    virtual void parsePayload(const QDomElement &payloadElement);
    virtual void serializePayload(QXmlStreamWriter *writer) const;

    static bool isItem(const QDomElement &element, std::function<bool(const QDomElement &payload)> isPayloadValid);

private:
    QSharedDataPointer<QXmppPubSubItemPrivate> d;
};

Q_DECLARE_METATYPE(QXmppPubSubItem)

#endif // QXMPPPUBSUBITEM_H
