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

#ifndef QXMPPPUBSUBNODECONFIG_H
#define QXMPPPUBSUBNODECONFIG_H

#include "QXmppDataFormBased.h"

class QXmppPubSubNodeConfigPrivate;

class QXmppPubSubNodeConfig : public QXmppDataFormBased
{
public:
    enum AccessModel : quint8 {
        Open,
        Presence,
        Roster,
        Authorize,
        Whitelist
    };
    static std::optional<AccessModel> accessModelFromString(const QString &);
    static QString accessModelToString(AccessModel);

    enum PublishModel : quint8 {
        Publishers,
        Subscribers,
        Anyone
    };
    static std::optional<PublishModel> publishModelFromString(const QString &);
    static QString publishModelToString(PublishModel);

    enum class ChildAssociationPolicy : quint8 {
        All,
        Owners,
        Whitelist
    };
    static std::optional<ChildAssociationPolicy> childAssociatationPolicyFromString(const QString &);
    static QString childAssociationPolicyToString(ChildAssociationPolicy);

    enum ItemPublisher : quint8 {
        NodeOwner,
        Publisher
    };
    static std::optional<ItemPublisher> itemPublisherFromString(const QString &);
    static QString itemPublisherToString(ItemPublisher);

    enum NodeType : quint8 {
        Leaf,
        Collection
    };
    static std::optional<NodeType> nodeTypeFromString(const QString &);
    static QString nodeTypeToString(NodeType);

    enum NotificationType : quint8 {
        Normal,
        Headline
    };
    static std::optional<NotificationType> notificationTypeFromString(const QString &);
    static QString notificationTypeToString(NotificationType);

    enum SendLastItemType : quint8 {
        Never,
        OnSubscription,
        OnSubscriptionAndPresence
    };
    static std::optional<SendLastItemType> sendLastItemTypeFromString(const QString &);
    static QString sendLastItemTypeToString(SendLastItemType);

    static std::optional<QXmppPubSubNodeConfig> fromDataForm(const QXmppDataForm &form);

    QXmppPubSubNodeConfig();
    QXmppPubSubNodeConfig(const QXmppPubSubNodeConfig &);
    ~QXmppPubSubNodeConfig();

    QXmppPubSubNodeConfig &operator=(const QXmppPubSubNodeConfig &);

    std::optional<AccessModel> accessModel() const;
    void setAccessModel(std::optional<AccessModel> accessModel);

    QString bodyXslt() const;
    void setBodyXslt(const QString &bodyXslt);

    std::optional<ChildAssociationPolicy> childAssociationPolicy() const;
    void setChildAssociationPolicy(std::optional<ChildAssociationPolicy> childAssociationPolicy);

    QStringList childAssociationWhitelist() const;
    void setChildAssociationWhitelist(const QStringList &childAssociationWhitelist);

    QStringList childNodes() const;
    void setChildNodes(const QStringList &childNodes);

    std::optional<quint32> childNodesMax() const;
    void setChildNodesMax(std::optional<quint32> childNodesMax);

    QStringList collections() const;
    void setCollections(const QStringList &collections);

    QStringList contactJids() const;
    void setContactJids(const QStringList &contactJids);

    QString dataFormXslt() const;
    void setDataFormXslt(const QString &dataFormXslt);

    std::optional<bool> notificationsEnabled() const;
    void setNotificationsEnabled(std::optional<bool> notificationsEnabled);

    std::optional<bool> includePayloads() const;
    void setIncludePayloads(std::optional<bool> includePayloads);

    QString description() const;
    void setDescription(const QString &description);

    std::optional<quint32> itemExpiry() const;
    void setItemExpiry(std::optional<quint32> itemExpiry);

    std::optional<ItemPublisher> notificationItemPublisher() const;
    void setNotificationItemPublisher(std::optional<ItemPublisher> notificationItemPublisher);

    QString language() const;
    void setLanguage(const QString &language);

    std::optional<quint32> maxItems() const;
    void setMaxItems(std::optional<quint32> maxItems);

    std::optional<quint32> maxPayloadSize() const;
    void setMaxPayloadSize(std::optional<quint32> maxPayloadSize);

    std::optional<NodeType> nodeType() const;
    void setNodeType(std::optional<NodeType> nodeType);

    std::optional<QXmppPubSubNodeConfig::NotificationType> notificationType() const;
    void setNotificationType(std::optional<QXmppPubSubNodeConfig::NotificationType> notificationType);

    std::optional<bool> configNotificationsEnabled() const;
    void setConfigNotificationsEnabled(std::optional<bool> configNotificationsEnabled);

    std::optional<bool> nodeDeleteNotificationsEnabled() const;
    void setNodeDeleteNotificationsEnabled(std::optional<bool> nodeDeleteNotificationsEnabled);

    std::optional<bool> retractNotificationsEnabled() const;
    void setRetractNotificationsEnabled(std::optional<bool> retractNotificationsEnabled);

    std::optional<bool> subNotificationsEnabled() const;
    void setSubNotificationsEnabled(std::optional<bool> subNotificationsEnabled);

    std::optional<bool> persistItems() const;
    void setPersistItems(std::optional<bool> persistItems);

    std::optional<bool> presenceBasedNotifications() const;
    void setPresenceBasedNotifications(std::optional<bool> presenceBasedNotifications);

    std::optional<PublishModel> publishModel() const;
    void setPublishModel(std::optional<PublishModel> publishModel);

    std::optional<bool> purgeWhenOffline() const;
    void setPurgeWhenOffline(std::optional<bool> purgeWhenOffline);

    QStringList allowedRosterGroups() const;
    void setAllowedRosterGroups(const QStringList &allowedRosterGroups);

    std::optional<SendLastItemType> sendLastItem() const;
    void setSendLastItem(std::optional<SendLastItemType> sendLastItem);

    std::optional<bool> temporarySubscriptions() const;
    void setTemporarySubscriptions(std::optional<bool> temporarySubscriptions);

    std::optional<bool> allowSubscriptions() const;
    void setAllowSubscriptions(std::optional<bool> allowSubscriptions);

    QString title() const;
    void setTitle(const QString &title);

    QString payloadType() const;
    void setPayloadType(const QString &payloadType);

protected:
    QList<FieldDescriptor> fieldDescriptors() override;
    QString formType() const override;

private:
    QSharedDataPointer<QXmppPubSubNodeConfigPrivate> d;
};

class QXmppPubSubPublishOptions : public QXmppPubSubNodeConfig
{
public:
    static std::optional<QXmppPubSubPublishOptions> fromDataForm(const QXmppDataForm &form);

protected:
    QString formType() const;
};

#endif // QXMPPPUBSUBNODECONFIG_H
