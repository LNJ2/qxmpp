/*
 * Copyright (C) 2008-2020 The QXmpp developers
 *
 * Author:
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

#ifndef QXMPPPUBSUBMANAGER_H
#define QXMPPPUBSUBMANAGER_H

#include "QXmppClient.h"
#include "QXmppClientExtension.h"
#include "QXmppMessage.h"
#include "QXmppPubSubIq.h"

#include <QFuture>
#include <QFutureWatcher>

class QXmppPubSubManagerPrivate;
class QXmppPubSubPublishOptions;

class QXMPP_EXPORT QXmppPubSubManager : public QXmppClientExtension
{
    Q_OBJECT

public:
    /// Moin moin
    using Result = std::variant<std::monostate, QXmppStanza::Error, QXmpp::PacketState>;
    using InstantNodeResult = std::variant<QString, QXmppStanza::Error, QXmpp::PacketState>;
    using PublishItemResult = std::variant<QString, QXmppStanza::Error, QXmpp::PacketState>;
    using PublishItemsResult = std::variant<QStringList, QXmppStanza::Error, QXmpp::PacketState>;
    template<typename T>
    using ItemResult = std::variant<std::optional<T>, QXmppStanza::Error, QXmpp::PacketState>;
    template<typename T>
    using ItemsResult = std::variant<QList<T>, QXmppStanza::Error, QXmpp::PacketState>;

    QXmppPubSubManager();
    ~QXmppPubSubManager();

    // PEP-specific (the PubSub service is the current account)
    QFuture<Result> createPepNode(const QString &nodeName);
    QFuture<Result> deletePepNode(const QString &nodeName);
    template<typename T>
    QFuture<PublishItemResult> publishPepItem(const QString &nodeName, const QXmppPubSubItem &item);
    template<typename T>
    QFuture<PublishItemResult> publishPepItem(const QString &nodeName, const QXmppPubSubItem &item, const QXmppPubSubPublishOptions &publishOptions);
    template<typename T>
    QFuture<PublishItemsResult> publishPepItems(const QString &nodeName, const QList<QXmppPubSubItem> &items);
    template<typename T>
    QFuture<PublishItemsResult> publishPepItems(const QString &nodeName, const QList<QXmppPubSubItem> &items, const QXmppPubSubPublishOptions &publishOptions);
    QFuture<Result> retractPepItem(const QString &nodeName, const QString &itemId);
    QFuture<Result> purgePepItems(const QString &nodeName);

    // Generic PubSub (the PubSub service is the given entity)
    QFuture<Result> createNode(const QString &jid, const QString &nodeName);
    QFuture<InstantNodeResult> createInstantNode(const QString &jid);
    QFuture<Result> deleteNode(const QString &jid, const QString &nodeName);
    template<typename T>
    QFuture<ItemResult<T>> requestItem(const QString &jid, const QString &nodeName, const QString &itemId);
    template<typename T>
    QFuture<ItemsResult<T>> requestItems(const QString &jid, const QString &nodeName);
    template<typename T>
    QFuture<ItemsResult<T>> requestItems(const QString &jid, const QString &nodeName, const QStringList &itemIds);
    template<typename T>
    QFuture<PublishItemResult> publishItem(const QString &jid, const QString &nodeName, const T &items);
    template<typename T>
    QFuture<PublishItemResult> publishItem(const QString &jid, const QString &nodeName, const T &items, const QXmppPubSubPublishOptions &publishOptions);
    template<typename T>
    QFuture<PublishItemsResult> publishItems(const QString &jid, const QString &nodeName, const QList<T> &items);
    template<typename T>
    QFuture<PublishItemsResult> publishItems(const QString &jid, const QString &nodeName, const QList<T> &items, const QXmppPubSubPublishOptions &publishOptions);
    QFuture<Result> retractItem(const QString &jid, const QString &nodeName, const QString &itemId);
    QFuture<Result> purgeItems(const QString &jid, const QString &nodeName);

    /// \cond
    bool handleStanza(const QDomElement &element) override;
    /// \endcond

private:
    QFuture<PublishItemResult> publishItem(QXmppPubSubIqBase &&iq);
    QFuture<PublishItemsResult> publishItems(QXmppPubSubIqBase &&iq);
    static QXmppPubSubIq<> requestItemsIq(const QString &jid, const QString &nodeName, const QStringList &itemIds);

    QScopedPointer<QXmppPubSubManagerPrivate> d;
};

template<typename T>
auto QXmppPubSubManager::publishItem<T>(const QString &jid,
                                        const QString &nodeName,
                                        const T &item)
    -> QFuture<PublishItemResult>
{
    QXmppPubSubIq<T> request;
    request.setTo(jid);
    request.setItems({item});
    request.setQueryNode(nodeName);
    return publishItem(std::move(request));
}

template<typename T>
auto QXmppPubSubManager::publishItem<T>(const QString &jid,
                                        const QString &nodeName,
                                        const T &item,
                                        const QXmppPubSubPublishOptions &publishOptions)
    -> QFuture<PublishItemResult>
{
    QXmppPubSubIq<T> request;
    request.setTo(jid);
    request.setItems({item});
    request.setQueryNode(nodeName);
    request.setPublishOptions(publishOptions);
    return publishItem(std::move(request));
}

template<typename T>
auto QXmppPubSubManager::publishItems<T>(const QString &jid,
                                         const QString &nodeName,
                                         const QList<T> &items)
    -> QFuture<PublishItemsResult>
{
    QXmppPubSubIq<T> request;
    request.setTo(jid);
    request.setItems(items);
    request.setQueryNode(nodeName);
    return publishItems(std::move(request));
}

template<typename T>
auto QXmppPubSubManager::publishItems<T>(const QString &jid,
                                         const QString &nodeName,
                                         const QList<T> &items,
                                         const QXmppPubSubPublishOptions &publishOptions)
    -> QFuture<PublishItemsResult>
{
    QXmppPubSubIq<T> request;
    request.setTo(jid);
    request.setItems(items);
    request.setQueryNode(nodeName);
    request.setPublishOptions(publishOptions);
    return publishItems(std::move(request));
}

template<typename T>
auto QXmppPubSubManager::requestItem<T>(const QString &jid,
                                        const QString &nodeName,
                                        const QString &itemId)
    -> QFuture<ItemResult<T>>
{
    QFutureInterface<ItemResult<T>> resultInterface(QFutureInterfaceBase::Started);

    auto sendFuture = client()->sendIq(requestItemsIq(jid, nodeName, {itemId}));

    auto *watcher = new QFutureWatcher<QXmppClient::IqResult>(this);
    connect(watcher, &QFutureWatcherBase::finished, this, [=](){
        auto result = watcher->result();
        if (auto element = std::get_if<QDomElement>(&result)) {
            if (QXmppPubSubIq<T>::isPubSubIq(*element)) {
                // FIXME: could this still be an IQ of type error?
                QXmppPubSubIq<T> resultIq;
                resultIq.parse(*element);

                if (resultIq.items().isEmpty()) {
                    resultInterface.reportResult(std::nullopt);
                } else {
                    resultInterface.reportResult(resultIq.items().at(0));
                }
                resultInterface.reportFinished();
            } else {
                QXmppIq iq;
                iq.parse(*element);
                resultInterface.reportResult(iq.error());
            }
        } else if (const auto *error = std::get_if<QXmpp::PacketState>(&result)) {
            resultInterface.reportResult(*error);
            resultInterface.reportFinished();
        }
        watcher->deleteLater();
    });
    watcher->setFuture(sendFuture);

    return resultInterface.future();
}

template<typename T>
auto QXmppPubSubManager::requestItems<T>(const QString &jid,
                                         const QString &nodeName)
    -> QFuture<ItemsResult<T>>
{
    return requestItems<T>(jid, nodeName, {});
}

template<typename T>
auto QXmppPubSubManager::requestItems<T>(const QString &jid,
                                         const QString &nodeName,
                                         const QStringList &itemIds)
    -> QFuture<ItemsResult<T>>
{
    QFutureInterface<ItemsResult<T>> resultInterface(QFutureInterfaceBase::Started);

    auto sendFuture = client()->sendIq(requestItemsIq(jid, nodeName, itemIds));

    auto *watcher = new QFutureWatcher<QXmppClient::IqResult>(this);
    connect(watcher, &QFutureWatcherBase::finished, this, [=](){
        auto result = watcher->result();
        if (auto element = std::get_if<QDomElement>(&result)) {
            if (QXmppPubSubIq<T>::isPubSubIq(*element)) {
                // FIXME: could this still be an IQ of type error?
                QXmppPubSubIq<T> resultIq;
                resultIq.parse(*element);
                resultInterface.reportResult(resultIq.items());
                resultInterface.reportFinished();
            } else {
                QXmppIq iq;
                iq.parse(*element);

                // invalid server response
            }
        } else if (const auto *error = std::get_if<QXmpp::PacketState>(&result)) {
            resultInterface.reportResult(*error);
            resultInterface.reportFinished();
        }
        watcher->deleteLater();
    });
    watcher->setFuture(sendFuture);

    return resultInterface.future();
}

#endif // QXMPPPUBSUBMANAGER_H
