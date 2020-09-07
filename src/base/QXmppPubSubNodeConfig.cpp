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

#include "QXmppPubSubNodeConfig.h"

static const auto NODE_CONFIG_FORM_TYPE = QStringLiteral("http://jabber.org/protocol/pubsub#node_config");

class QXmppPubSubNodeConfigPrivate : public QSharedData
{
public:
    std::optional<QXmppPubSubNodeConfig::AccessModel> accessModel;
    QString bodyXslt;
    std::optional<QXmppPubSubNodeConfig::ChildAssociationPolicy> childAssociationPolicy;
    QStringList childAssociationWhitelist;
    QStringList childNodes;
    std::optional<quint32> childNodesMax;
    QStringList collections;
    QStringList contactJids;
    QString dataFormXslt;
    std::optional<bool> notificationsEnabled;
    std::optional<bool> includePayloads;
    QString description;
    std::optional<quint32> itemExpiry;
    std::optional<QXmppPubSubNodeConfig::ItemPublisher> notificationItemPublisher;
    QString language;
    std::optional<quint32> maxItems;
    std::optional<quint32> maxPayloadSize;
    std::optional<QXmppPubSubNodeConfig::NodeType> nodeType;
    std::optional<QXmppPubSubNodeConfig::NotificationType> notificationType;
    std::optional<bool> configNotificationsEnabled;
    std::optional<bool> nodeDeleteNotificationsEnabled;
    std::optional<bool> retractNotificationsEnabled;
    std::optional<bool> subNotificationsEnabled;
    std::optional<bool> persistItems;
    std::optional<bool> presenceBasedNotifications;
    std::optional<QXmppPubSubNodeConfig::PublishModel> publishModel;
    std::optional<bool> purgeWhenOffline;
    QStringList allowedRosterGroups;
    std::optional<QXmppPubSubNodeConfig::SendLastItemType> sendLastItem;
    std::optional<bool> temporarySubscriptions;
    std::optional<bool> allowSubscriptions;
    QString title;
    QString payloadType;
};

std::optional<QXmppPubSubNodeConfig::AccessModel> QXmppPubSubNodeConfig::accessModelFromString(const QString &string)
{
    if (string == QStringLiteral("open"))
        return Open;
    if (string == QStringLiteral("presence"))
        return Presence;
    if (string == QStringLiteral("roster"))
        return Roster;
    if (string == QStringLiteral("authorize"))
        return Authorize;
    if (string == QStringLiteral("whitelist"))
        return Whitelist;
    return std::nullopt;
}

QString QXmppPubSubNodeConfig::accessModelToString(AccessModel model)
{
    switch (model) {
    case Open:
        return QStringLiteral("open");
    case Presence:
        return QStringLiteral("presence");
    case Roster:
        return QStringLiteral("roster");
    case Authorize:
        return QStringLiteral("authorize");
    case Whitelist:
        return QStringLiteral("whitelist");
    }
    return {};
}

std::optional<QXmppPubSubNodeConfig::PublishModel> QXmppPubSubNodeConfig::publishModelFromString(const QString &string)
{
    if (string == QStringLiteral("publishers"))
        return Publishers;
    if (string == QStringLiteral("subscribers"))
        return Subscribers;
    if (string == QStringLiteral("open"))
        return Anyone;
    return std::nullopt;
}

QString QXmppPubSubNodeConfig::publishModelToString(QXmppPubSubNodeConfig::PublishModel model)
{
    switch (model) {
    case Publishers:
        return QStringLiteral("publishers");
    case Subscribers:
        return QStringLiteral("subscribers");
    case Anyone:
        return QStringLiteral("open");
    }
    return {};
}

std::optional<QXmppPubSubNodeConfig::ChildAssociationPolicy> QXmppPubSubNodeConfig::childAssociatationPolicyFromString(const QString &string)
{
    if (string == QStringLiteral("all"))
        return ChildAssociationPolicy::All;
    if (string == QStringLiteral("owners"))
        return ChildAssociationPolicy::Owners;
    if (string == QStringLiteral("whitelist"))
        return ChildAssociationPolicy::Whitelist;
    return std::nullopt;
}

QString QXmppPubSubNodeConfig::childAssociationPolicyToString(QXmppPubSubNodeConfig::ChildAssociationPolicy policy)
{
    switch (policy) {
    case ChildAssociationPolicy::All:
        return QStringLiteral("all");
    case ChildAssociationPolicy::Owners:
        return QStringLiteral("owners");
    case ChildAssociationPolicy::Whitelist:
        return QStringLiteral("whitelist");
    }
    return {};
}

std::optional<QXmppPubSubNodeConfig::ItemPublisher> QXmppPubSubNodeConfig::itemPublisherFromString(const QString &string)
{
    if (string == QStringLiteral("owner"))
        return NodeOwner;
    if (string == QStringLiteral("publisher"))
        return Publisher;
    return std::nullopt;
}

QString QXmppPubSubNodeConfig::itemPublisherToString(ItemPublisher publisher)
{
    switch (publisher) {
    case NodeOwner:
        return QStringLiteral("owner");
    case Publisher:
        return QStringLiteral("publisher");
    }
    return {};
}

std::optional<QXmppPubSubNodeConfig::NodeType> QXmppPubSubNodeConfig::nodeTypeFromString(const QString &string)
{
    if (string == QStringLiteral("leaf"))
        return Leaf;
    if (string == QStringLiteral("collection"))
        return Collection;
    return std::nullopt;
}

QString QXmppPubSubNodeConfig::nodeTypeToString(NodeType type)
{
    switch (type) {
    case Leaf:
        return QStringLiteral("leaf");
    case Collection:
        return QStringLiteral("collection");
    }
    return {};
}

std::optional<QXmppPubSubNodeConfig::NotificationType> QXmppPubSubNodeConfig::notificationTypeFromString(const QString &string)
{
    if (string == QStringLiteral("normal"))
        return Normal;
    if (string == QStringLiteral("headline"))
        return Headline;
    return std::nullopt;
}

QString QXmppPubSubNodeConfig::notificationTypeToString(NotificationType type)
{
    switch (type) {
    case Normal:
        return QStringLiteral("normal");
    case Headline:
        return QStringLiteral("headline");
    }
    return {};
}

std::optional<QXmppPubSubNodeConfig::SendLastItemType> QXmppPubSubNodeConfig::sendLastItemTypeFromString(const QString &string)
{
    if (string == QStringLiteral("never"))
        return Never;
    if (string == QStringLiteral("on_sub"))
        return OnSubscription;
    if (string == QStringLiteral("on_sub_and_presence"))
        return OnSubscriptionAndPresence;
    return std::nullopt;
}

QString QXmppPubSubNodeConfig::sendLastItemTypeToString(SendLastItemType type)
{
    switch (type) {
    case Never:
        return QStringLiteral("never");
    case OnSubscription:
        return QStringLiteral("on_sub");
    case OnSubscriptionAndPresence:
        return QStringLiteral("on_sub_and_presence");
    }
    return {};
}

std::optional<QXmppPubSubNodeConfig> QXmppPubSubNodeConfig::fromDataForm(const QXmppDataForm &form)
{
    if (form.formType() != NODE_CONFIG_FORM_TYPE) {
        return std::nullopt;
    }

    QXmppPubSubNodeConfig nodeConfig;
    if (QXmppDataFormBased::fromDataForm(form, nodeConfig)) {
        return nodeConfig;
    }
    return std::nullopt;
}

QXmppPubSubNodeConfig::QXmppPubSubNodeConfig()
    : d(new QXmppPubSubNodeConfigPrivate)
{
}

QXmppPubSubNodeConfig::QXmppPubSubNodeConfig(const QXmppPubSubNodeConfig &) = default;

QXmppPubSubNodeConfig::~QXmppPubSubNodeConfig() = default;

QXmppPubSubNodeConfig &QXmppPubSubNodeConfig::operator=(const QXmppPubSubNodeConfig &) = default;

std::optional<QXmppPubSubNodeConfig::AccessModel> QXmppPubSubNodeConfig::accessModel() const
{
    return d->accessModel;
}

void QXmppPubSubNodeConfig::setAccessModel(std::optional<AccessModel> accessModel)
{
    d->accessModel = accessModel;
}

QString QXmppPubSubNodeConfig::bodyXslt() const
{
    return d->bodyXslt;
}

void QXmppPubSubNodeConfig::setBodyXslt(const QString &bodyXslt)
{
    d->bodyXslt = bodyXslt;
}

std::optional<QXmppPubSubNodeConfig::ChildAssociationPolicy> QXmppPubSubNodeConfig::childAssociationPolicy() const
{
    return d->childAssociationPolicy;
}

void QXmppPubSubNodeConfig::setChildAssociationPolicy(std::optional<ChildAssociationPolicy> childAssociationPolicy)
{
    d->childAssociationPolicy = childAssociationPolicy;
}

QStringList QXmppPubSubNodeConfig::childAssociationWhitelist() const
{
    return d->childAssociationWhitelist;
}

void QXmppPubSubNodeConfig::setChildAssociationWhitelist(const QStringList &childAssociationWhitelist)
{
    d->childAssociationWhitelist = childAssociationWhitelist;
}

QStringList QXmppPubSubNodeConfig::childNodes() const
{
    return d->childNodes;
}

void QXmppPubSubNodeConfig::setChildNodes(const QStringList &childNodes)
{
    d->childNodes = childNodes;
}

std::optional<quint32> QXmppPubSubNodeConfig::childNodesMax() const
{
    return d->childNodesMax;
}

void QXmppPubSubNodeConfig::setChildNodesMax(std::optional<quint32> childNodesMax)
{
    d->childNodesMax = childNodesMax;
}

QStringList QXmppPubSubNodeConfig::collections() const
{
    return d->collections;
}

void QXmppPubSubNodeConfig::setCollections(const QStringList &collections)
{
    d->collections = collections;
}

QStringList QXmppPubSubNodeConfig::contactJids() const
{
    return d->contactJids;
}

void QXmppPubSubNodeConfig::setContactJids(const QStringList &contactJids)
{
    d->contactJids = contactJids;
}

QString QXmppPubSubNodeConfig::dataFormXslt() const
{
    return d->dataFormXslt;
}

void QXmppPubSubNodeConfig::setDataFormXslt(const QString &dataFormXslt)
{
    d->dataFormXslt = dataFormXslt;
}

std::optional<bool> QXmppPubSubNodeConfig::notificationsEnabled() const
{
    return d->notificationsEnabled;
}

void QXmppPubSubNodeConfig::setNotificationsEnabled(std::optional<bool> notificationsEnabled)
{
    d->notificationsEnabled = notificationsEnabled;
}

std::optional<bool> QXmppPubSubNodeConfig::includePayloads() const
{
    return d->includePayloads;
}

void QXmppPubSubNodeConfig::setIncludePayloads(std::optional<bool> includePayloads)
{
    d->includePayloads = includePayloads;
}

QString QXmppPubSubNodeConfig::description() const
{
    return d->description;
}

void QXmppPubSubNodeConfig::setDescription(const QString &description)
{
    d->description = description;
}

std::optional<quint32> QXmppPubSubNodeConfig::itemExpiry() const
{
    return d->itemExpiry;
}

void QXmppPubSubNodeConfig::setItemExpiry(std::optional<quint32> itemExpiry)
{
    d->itemExpiry = itemExpiry;
}

std::optional<QXmppPubSubNodeConfig::ItemPublisher> QXmppPubSubNodeConfig::notificationItemPublisher() const
{
    return d->notificationItemPublisher;
}

void QXmppPubSubNodeConfig::setNotificationItemPublisher(std::optional<ItemPublisher> notificationItemPublisher)
{
    d->notificationItemPublisher = notificationItemPublisher;
}

QString QXmppPubSubNodeConfig::language() const
{
    return d->language;
}

void QXmppPubSubNodeConfig::setLanguage(const QString &language)
{
    d->language = language;
}

std::optional<quint32> QXmppPubSubNodeConfig::maxItems() const
{
    return d->maxItems;
}

void QXmppPubSubNodeConfig::setMaxItems(std::optional<quint32> maxItems)
{
    d->maxItems = maxItems;
}

std::optional<quint32> QXmppPubSubNodeConfig::maxPayloadSize() const
{
    return d->maxPayloadSize;
}

void QXmppPubSubNodeConfig::setMaxPayloadSize(std::optional<quint32> maxPayloadSize)
{
    d->maxPayloadSize = maxPayloadSize;
}

std::optional<QXmppPubSubNodeConfig::NodeType> QXmppPubSubNodeConfig::nodeType() const
{
    return d->nodeType;
}

void QXmppPubSubNodeConfig::setNodeType(std::optional<NodeType> nodeType)
{
    d->nodeType = nodeType;
}

std::optional<QXmppPubSubNodeConfig::NotificationType> QXmppPubSubNodeConfig::notificationType() const
{
    return d->notificationType;
}

void QXmppPubSubNodeConfig::setNotificationType(std::optional<QXmppPubSubNodeConfig::NotificationType> notificationType)
{
    d->notificationType = notificationType;
}

std::optional<bool> QXmppPubSubNodeConfig::configNotificationsEnabled() const
{
    return d->configNotificationsEnabled;
}

void QXmppPubSubNodeConfig::setConfigNotificationsEnabled(std::optional<bool> configNotificationsEnabled)
{
    d->configNotificationsEnabled = configNotificationsEnabled;
}

std::optional<bool> QXmppPubSubNodeConfig::nodeDeleteNotificationsEnabled() const
{
    return d->nodeDeleteNotificationsEnabled;
}

void QXmppPubSubNodeConfig::setNodeDeleteNotificationsEnabled(std::optional<bool> nodeDeleteNotificationsEnabled)
{
    d->nodeDeleteNotificationsEnabled = nodeDeleteNotificationsEnabled;
}

std::optional<bool> QXmppPubSubNodeConfig::retractNotificationsEnabled() const
{
    return d->retractNotificationsEnabled;
}

void QXmppPubSubNodeConfig::setRetractNotificationsEnabled(std::optional<bool> retractNotificationsEnabled)
{
    d->retractNotificationsEnabled = retractNotificationsEnabled;
}

std::optional<bool> QXmppPubSubNodeConfig::subNotificationsEnabled() const
{
    return d->subNotificationsEnabled;
}

void QXmppPubSubNodeConfig::setSubNotificationsEnabled(std::optional<bool> subNotificationsEnabled)
{
    d->subNotificationsEnabled = subNotificationsEnabled;
}

std::optional<bool> QXmppPubSubNodeConfig::persistItems() const
{
    return d->persistItems;
}

void QXmppPubSubNodeConfig::setPersistItems(std::optional<bool> persistItems)
{
    d->persistItems = persistItems;
}

std::optional<bool> QXmppPubSubNodeConfig::presenceBasedNotifications() const
{
    return d->presenceBasedNotifications;
}

void QXmppPubSubNodeConfig::setPresenceBasedNotifications(std::optional<bool> presenceBasedNotifications)
{
    d->presenceBasedNotifications = presenceBasedNotifications;
}

std::optional<QXmppPubSubNodeConfig::PublishModel> QXmppPubSubNodeConfig::publishModel() const
{
    return d->publishModel;
}

void QXmppPubSubNodeConfig::setPublishModel(std::optional<PublishModel> publishModel)
{
    d->publishModel = publishModel;
}

std::optional<bool> QXmppPubSubNodeConfig::purgeWhenOffline() const
{
    return d->purgeWhenOffline;
}

void QXmppPubSubNodeConfig::setPurgeWhenOffline(std::optional<bool> purgeWhenOffline)
{
    d->purgeWhenOffline = purgeWhenOffline;
}

QStringList QXmppPubSubNodeConfig::allowedRosterGroups() const
{
    return d->allowedRosterGroups;
}

void QXmppPubSubNodeConfig::setAllowedRosterGroups(const QStringList &allowedRosterGroups)
{
    d->allowedRosterGroups = allowedRosterGroups;
}

std::optional<QXmppPubSubNodeConfig::SendLastItemType> QXmppPubSubNodeConfig::sendLastItem() const
{
    return d->sendLastItem;
}

void QXmppPubSubNodeConfig::setSendLastItem(std::optional<QXmppPubSubNodeConfig::SendLastItemType> sendLastItem)
{
    d->sendLastItem = sendLastItem;
}

std::optional<bool> QXmppPubSubNodeConfig::temporarySubscriptions() const
{
    return d->temporarySubscriptions;
}

void QXmppPubSubNodeConfig::setTemporarySubscriptions(std::optional<bool> temporarySubscriptions)
{
    d->temporarySubscriptions = temporarySubscriptions;
}

std::optional<bool> QXmppPubSubNodeConfig::allowSubscriptions() const
{
    return d->allowSubscriptions;
}

void QXmppPubSubNodeConfig::setAllowSubscriptions(std::optional<bool> allowSubscriptions)
{
    d->allowSubscriptions = allowSubscriptions;
}

QString QXmppPubSubNodeConfig::title() const
{
    return d->title;
}

void QXmppPubSubNodeConfig::setTitle(const QString &title)
{
    d->title = title;
}

QString QXmppPubSubNodeConfig::payloadType() const
{
    return d->payloadType;
}

void QXmppPubSubNodeConfig::setPayloadType(const QString &payloadType)
{
    d->payloadType = payloadType;
}

QList<QXmppDataFormBased::FieldDescriptor> QXmppPubSubNodeConfig::fieldDescriptors()
{
    return {
        { QXmppDataForm::Field::ListSingleField,
          QStringLiteral("pubsub#access_model"),
          Optional,
          [&](const QXmppDataForm::Field &field) {
              d->accessModel = accessModelFromString(field.value().toString());
          },
          [&](QXmppDataForm::Field &field) {
              if (d->accessModel) {
                  field.setValue(accessModelToString(*d->accessModel));
              }
          } },
        { QXmppDataForm::Field::TextSingleField,
          QStringLiteral("pubsub#body_xslt"),
          Optional,
          std::bind(&QXmppDataFormBased::parseString, std::placeholders::_1, d->bodyXslt),
          std::bind(&QXmppDataFormBased::serializeString, std::placeholders::_1, d->bodyXslt) },
        { QXmppDataForm::Field::ListSingleField,
          QStringLiteral("pubsub#children_association_policy"),
          Optional,
          [&](const QXmppDataForm::Field &field) {
              d->childAssociationPolicy = childAssociatationPolicyFromString(field.value().toString());
          },
          [&](QXmppDataForm::Field &field) {
              if (d->childAssociationPolicy) {
                  field.setValue(childAssociationPolicyToString(*d->childAssociationPolicy));
              }
          } },
        { QXmppDataForm::Field::JidMultiField,
          QStringLiteral("pubsub#children_association_whitelist"),
          Optional,
          std::bind(&QXmppDataFormBased::parseStringList, std::placeholders::_1, d->childAssociationWhitelist),
          std::bind(&QXmppDataFormBased::serializeStringList, std::placeholders::_1, d->childAssociationWhitelist) },
        { QXmppDataForm::Field::TextMultiField,
          QStringLiteral("pubsub#children"),
          Optional,
          std::bind(&QXmppDataFormBased::parseStringList, std::placeholders::_1, d->childNodes),
          std::bind(&QXmppDataFormBased::serializeStringList, std::placeholders::_1, d->childNodes) },
        { QXmppDataForm::Field::TextSingleField,
          QStringLiteral("pubsub#children_max"),
          Optional,
          std::bind(&QXmppDataFormBased::parseOptionalUInt, std::placeholders::_1, d->childNodesMax),
          std::bind(&QXmppDataFormBased::serializeOptionalUInt, std::placeholders::_1, d->childNodesMax) },
        { QXmppDataForm::Field::TextMultiField,
          QStringLiteral("pubsub#collection"),
          Optional,
          std::bind(&QXmppDataFormBased::parseStringList, std::placeholders::_1, d->collections),
          std::bind(&QXmppDataFormBased::serializeStringList, std::placeholders::_1, d->collections) },
        { QXmppDataForm::Field::JidMultiField,
          QStringLiteral("pubsub#contact"),
          Optional,
          std::bind(&QXmppDataFormBased::parseStringList, std::placeholders::_1, d->contactJids),
          std::bind(&QXmppDataFormBased::serializeStringList, std::placeholders::_1, d->contactJids) },
        { QXmppDataForm::Field::TextSingleField,
          QStringLiteral("pubsub#dataform_xslt"),
          Optional,
          std::bind(&QXmppDataFormBased::parseString, std::placeholders::_1, d->dataFormXslt),
          std::bind(&QXmppDataFormBased::serializeString, std::placeholders::_1, d->dataFormXslt) },
        { QXmppDataForm::Field::BooleanField,
          QStringLiteral("pubsub#deliver_notifications"),
          Optional,
          std::bind(&QXmppDataFormBased::parseOptionalBool, std::placeholders::_1, d->notificationsEnabled),
          std::bind(&QXmppDataFormBased::serializeOptionalBool, std::placeholders::_1, d->notificationsEnabled) },
        { QXmppDataForm::Field::BooleanField,
          QStringLiteral("pubsub#deliver_payloads"),
          Optional,
          std::bind(&QXmppDataFormBased::parseOptionalBool, std::placeholders::_1, d->includePayloads),
          std::bind(&QXmppDataFormBased::serializeOptionalBool, std::placeholders::_1, d->includePayloads) },
        { QXmppDataForm::Field::TextSingleField,
          QStringLiteral("pubsub#description"),
          Optional,
          std::bind(&QXmppDataFormBased::parseString, std::placeholders::_1, d->description),
          std::bind(&QXmppDataFormBased::serializeString, std::placeholders::_1, d->description) },
        { QXmppDataForm::Field::TextSingleField,
          QStringLiteral("pubsub#item_expire"),
          Optional,
          std::bind(&QXmppDataFormBased::parseOptionalUInt, std::placeholders::_1, d->itemExpiry),
          std::bind(&QXmppDataFormBased::serializeOptionalUInt, std::placeholders::_1, d->itemExpiry) },
        { QXmppDataForm::Field::ListSingleField,
          QStringLiteral("pubsub#itemreply"),
          Optional,
          [&](const QXmppDataForm::Field &field) {
              d->notificationItemPublisher = itemPublisherFromString(field.value().toString());
          },
          [&](QXmppDataForm::Field &field) {
              if (d->notificationItemPublisher) {
                  field.setValue(itemPublisherToString(*d->notificationItemPublisher));
              }
          } },
        { QXmppDataForm::Field::TextSingleField,
          QStringLiteral("pubsub#language"),
          Optional,
          std::bind(&QXmppDataFormBased::parseString, std::placeholders::_1, d->language),
          std::bind(&QXmppDataFormBased::serializeString, std::placeholders::_1, d->language) },
        { QXmppDataForm::Field::TextSingleField,
          QStringLiteral("pubsub#max_items"),
          Optional,
          std::bind(&QXmppDataFormBased::parseOptionalUInt, std::placeholders::_1, d->maxItems),
          std::bind(&QXmppDataFormBased::serializeOptionalUInt, std::placeholders::_1, d->maxItems) },
        { QXmppDataForm::Field::TextSingleField,
          QStringLiteral("pubsub#max_payload_size"),
          Optional,
          std::bind(&QXmppDataFormBased::parseOptionalUInt, std::placeholders::_1, d->maxPayloadSize),
          std::bind(&QXmppDataFormBased::serializeOptionalUInt, std::placeholders::_1, d->maxPayloadSize) },
        { QXmppDataForm::Field::ListSingleField,
          QStringLiteral("pubsub#node_type"),
          Optional,
          [&](const QXmppDataForm::Field &field) {
              d->nodeType = nodeTypeFromString(field.value().toString());
          },
          [&](QXmppDataForm::Field &field) {
              if (d->nodeType) {
                  field.setValue(nodeTypeToString(*d->nodeType));
              }
          } },
        { QXmppDataForm::Field::ListSingleField,
          QStringLiteral("pubsub#notification_type"),
          Optional,
          [&](const QXmppDataForm::Field &field) {
              d->notificationType = notificationTypeFromString(field.value().toString());
          },
          [&](QXmppDataForm::Field &field) {
              if (d->notificationType) {
                  field.setValue(notificationTypeToString(*d->notificationType));
              }
          } },
        { QXmppDataForm::Field::BooleanField,
          QStringLiteral("pubsub#notify_config"),
          Optional,
          std::bind(&QXmppDataFormBased::parseOptionalBool, std::placeholders::_1, d->configNotificationsEnabled),
          std::bind(&QXmppDataFormBased::serializeOptionalBool, std::placeholders::_1, d->configNotificationsEnabled) },
        { QXmppDataForm::Field::BooleanField,
          QStringLiteral("pubsub#notify_delete"),
          Optional,
          std::bind(&QXmppDataFormBased::parseOptionalBool, std::placeholders::_1, d->nodeDeleteNotificationsEnabled),
          std::bind(&QXmppDataFormBased::serializeOptionalBool, std::placeholders::_1, d->nodeDeleteNotificationsEnabled) },
        { QXmppDataForm::Field::BooleanField,
          QStringLiteral("pubsub#notify_retract"),
          Optional,
          std::bind(&QXmppDataFormBased::parseOptionalBool, std::placeholders::_1, d->retractNotificationsEnabled),
          std::bind(&QXmppDataFormBased::serializeOptionalBool, std::placeholders::_1, d->retractNotificationsEnabled) },
        { QXmppDataForm::Field::BooleanField,
          QStringLiteral("pubsub#notify_sub"),
          Optional,
          std::bind(&QXmppDataFormBased::parseOptionalBool, std::placeholders::_1, d->subNotificationsEnabled),
          std::bind(&QXmppDataFormBased::serializeOptionalBool, std::placeholders::_1, d->subNotificationsEnabled) },
        { QXmppDataForm::Field::BooleanField,
          QStringLiteral("pubsub#pubsub#persist_items"),
          Optional,
          std::bind(&QXmppDataFormBased::parseOptionalBool, std::placeholders::_1, d->persistItems),
          std::bind(&QXmppDataFormBased::serializeOptionalBool, std::placeholders::_1, d->persistItems) },
        { QXmppDataForm::Field::BooleanField,
          QStringLiteral("pubsub#presence_based_delivery"),
          Optional,
          std::bind(&QXmppDataFormBased::parseOptionalBool, std::placeholders::_1, d->presenceBasedNotifications),
          std::bind(&QXmppDataFormBased::serializeOptionalBool, std::placeholders::_1, d->presenceBasedNotifications) },
        { QXmppDataForm::Field::ListSingleField,
          QStringLiteral("pubsub#publish_model"),
          Optional,
          [&](const QXmppDataForm::Field &field) {
              d->publishModel = publishModelFromString(field.value().toString());
          },
          [&](QXmppDataForm::Field &field) {
              if (d->publishModel) {
                  field.setValue(publishModelToString(*d->publishModel));
              }
          } },
        { QXmppDataForm::Field::BooleanField,
          QStringLiteral("pubsub#purge_offline"),
          Optional,
          std::bind(&QXmppDataFormBased::parseOptionalBool, std::placeholders::_1, d->purgeWhenOffline),
          std::bind(&QXmppDataFormBased::serializeOptionalBool, std::placeholders::_1, d->purgeWhenOffline) },
        { QXmppDataForm::Field::ListMultiField,
          QStringLiteral("pubsub#roster_groups_allowed"),
          Optional,
          std::bind(&QXmppDataFormBased::parseStringList, std::placeholders::_1, d->allowedRosterGroups),
          std::bind(&QXmppDataFormBased::serializeStringList, std::placeholders::_1, d->allowedRosterGroups) },
        { QXmppDataForm::Field::ListSingleField,
          QStringLiteral("pubsub#send_last_published_item"),
          Optional,
          [&](const QXmppDataForm::Field &field) {
              d->sendLastItem = sendLastItemTypeFromString(field.value().toString());
          },
          [&](QXmppDataForm::Field &field) {
              if (d->sendLastItem) {
                  field.setValue(sendLastItemTypeToString(*d->sendLastItem));
              }
          } },
        { QXmppDataForm::Field::BooleanField,
          QStringLiteral("pubsub#tempsub"),
          Optional,
          std::bind(&QXmppDataFormBased::parseOptionalBool, std::placeholders::_1, d->temporarySubscriptions),
          std::bind(&QXmppDataFormBased::serializeOptionalBool, std::placeholders::_1, d->temporarySubscriptions) },
        { QXmppDataForm::Field::BooleanField,
          QStringLiteral("pubsub#subscribe"),
          Optional,
          std::bind(&QXmppDataFormBased::parseOptionalBool, std::placeholders::_1, d->allowSubscriptions),
          std::bind(&QXmppDataFormBased::serializeOptionalBool, std::placeholders::_1, d->allowSubscriptions) },
        { QXmppDataForm::Field::TextSingleField,
          QStringLiteral("pubsub#title"),
          Optional,
          std::bind(&QXmppDataFormBased::parseString, std::placeholders::_1, d->title),
          std::bind(&QXmppDataFormBased::serializeString, std::placeholders::_1, d->title) },
        { QXmppDataForm::Field::TextSingleField,
          QStringLiteral("pubsub#type"),
          Optional,
          std::bind(&QXmppDataFormBased::parseString, std::placeholders::_1, d->payloadType),
          std::bind(&QXmppDataFormBased::serializeString, std::placeholders::_1, d->payloadType) }
    };
}

QString QXmppPubSubNodeConfig::formType() const
{
    return NODE_CONFIG_FORM_TYPE;
}
