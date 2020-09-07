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

#include "QXmppPubSubMetadata.h"

#include <QDateTime>

class QXmppPubSubMetadataPrivate : public QSharedData
{
public:
    QStringList contactJids;
    QDateTime creationDate;
    QString creatorJid;
    QString description;
    QString language;
    std::optional<QXmppPubSubNodeConfig::AccessModel> accessModel;
    std::optional<QXmppPubSubNodeConfig::PublishModel> publishModel;
    qint64 numberOfSubscribers = -1;
    QStringList ownerJids;
    QStringList publisherJids;
    QString title;
    QString type;
    qint64 maxItems = -1;
};

QXmppPubSubMetadata::QXmppPubSubMetadata()
    : d(new QXmppPubSubMetadataPrivate)
{
}

QXmppPubSubMetadata::QXmppPubSubMetadata(const QXmppPubSubMetadata &) = default;

QXmppPubSubMetadata::~QXmppPubSubMetadata() = default;

QXmppPubSubMetadata &QXmppPubSubMetadata::operator=(const QXmppPubSubMetadata &) = default;

QStringList QXmppPubSubMetadata::contactJids() const
{
    return d->contactJids;
}

void QXmppPubSubMetadata::setContactJids(const QStringList &contactJids)
{
    d->contactJids = contactJids;
}

QDateTime QXmppPubSubMetadata::creationDate() const
{
    return d->creationDate;
}

void QXmppPubSubMetadata::setCreationDate(const QDateTime &creationDate)
{
    d->creationDate = creationDate;
}

QString QXmppPubSubMetadata::creatorJid() const
{
    return d->creatorJid;
}

void QXmppPubSubMetadata::setCreatorJid(const QString &creatorJid)
{
    d->creatorJid = creatorJid;
}

QString QXmppPubSubMetadata::description() const
{
    return d->description;
}

void QXmppPubSubMetadata::setDescription(const QString &description)
{
    d->description = description;
}

QString QXmppPubSubMetadata::language() const
{
    return d->language;
}

void QXmppPubSubMetadata::setLanguage(const QString &language)
{
    d->language = language;
}

std::optional<QXmppPubSubNodeConfig::AccessModel> QXmppPubSubMetadata::accessModel() const
{
    return d->accessModel;
}

void QXmppPubSubMetadata::setAccessModel(std::optional<QXmppPubSubNodeConfig::AccessModel> accessModel)
{
    d->accessModel = accessModel;
}

std::optional<QXmppPubSubNodeConfig::PublishModel> QXmppPubSubMetadata::publishModel() const
{
    return d->publishModel;
}

void QXmppPubSubMetadata::setPublishModel(std::optional<QXmppPubSubNodeConfig::PublishModel> publishModel)
{
    d->publishModel = publishModel;
}

qint64 QXmppPubSubMetadata::numberOfSubscribers() const
{
    return d->numberOfSubscribers;
}

void QXmppPubSubMetadata::setNumberOfSubscribers(qint64 numberOfSubscribers)
{
    d->numberOfSubscribers = numberOfSubscribers;
}

QStringList QXmppPubSubMetadata::ownerJids() const
{
    return d->ownerJids;
}

void QXmppPubSubMetadata::setOwnerJids(const QStringList &ownerJids)
{
    d->ownerJids = ownerJids;
}

QStringList QXmppPubSubMetadata::publisherJids() const
{
    return d->publisherJids;
}

void QXmppPubSubMetadata::setPublisherJids(const QStringList &publisherJids)
{
    d->publisherJids = publisherJids;
}

QString QXmppPubSubMetadata::title() const
{
    return d->title;
}

void QXmppPubSubMetadata::setTitle(const QString &title)
{
    d->title = title;
}

QString QXmppPubSubMetadata::type() const
{
    return d->type;
}

void QXmppPubSubMetadata::setType(const QString &type)
{
    d->type = type;
}

qint64 QXmppPubSubMetadata::maxItems() const
{
    return d->maxItems;
}

void QXmppPubSubMetadata::setMaxItems(qint64 maxItems)
{
    d->maxItems = maxItems;
}

QList<QXmppDataFormBased::FieldDescriptor> QXmppPubSubMetadata::fieldDescriptors()
{
    return {
        { QXmppDataForm::Field::JidMultiField,
          QStringLiteral("pubsub#contact"),
          Optional,
          [&](const QXmppDataForm::Field &field) {
              d->contactJids = field.value().toStringList();
          },
          [&](QXmppDataForm::Field &field) {
              if (!d->contactJids.isEmpty()) {
                  field.setValue(d->contactJids);
              }
          } },
        { QXmppDataForm::Field::TextSingleField,
          QStringLiteral("pubsub#creation_date"),
          Optional,
          [&](const QXmppDataForm::Field &field) {
              d->creationDate = QDateTime::fromString(field.value().toString(), Qt::ISODate).toUTC();
          },
          [&](QXmppDataForm::Field &field) {
              if (!d->creationDate.isNull() && d->creationDate.isValid()) {
                  field.setValue(d->creationDate.toUTC().toString(Qt::ISODate));
              }
          } },
        { QXmppDataForm::Field::JidSingleField,
          QStringLiteral("pubsub#creator"),
          Optional,
          [&](const QXmppDataForm::Field &field) {
              d->creatorJid = field.value().toString();
          },
          [&](QXmppDataForm::Field &field) {
              if (!d->creatorJid.isNull()) {
                  field.setValue(d->creatorJid);
              }
          } },
        { QXmppDataForm::Field::TextSingleField,
          QStringLiteral("pubsub#description"),
          Optional,
          [&](const QXmppDataForm::Field &field) {
              d->description = field.value().toString();
          },
          [&](QXmppDataForm::Field &field) {
              if (!d->description.isNull()) {
                  field.setValue(d->description);
              }
          } },
        { QXmppDataForm::Field::JidSingleField,
          QStringLiteral("pubsub#language"),
          Optional,
          [&](const QXmppDataForm::Field &field) {
              d->language = field.value().toString();
          },
          [&](QXmppDataForm::Field &field) {
              if (!d->language.isNull()) {
                  field.setValue(d->language);
              }
          } },
        { QXmppDataForm::Field::ListSingleField,
          QStringLiteral("pubsub#access_model"),
          Optional,
          [&](const QXmppDataForm::Field &field) {
              d->accessModel = QXmppPubSubNodeConfig::accessModelFromString(field.value().toString());
          },
          [&](QXmppDataForm::Field &field) {
              if (d->accessModel) {
                  field.setValue(QXmppPubSubNodeConfig::accessModelToString(*d->accessModel));
              }
          } },
        { QXmppDataForm::Field::ListSingleField,
          QStringLiteral("pubsub#publish_model"),
          Optional,
          [&](const QXmppDataForm::Field &field) {
              d->publishModel = QXmppPubSubNodeConfig::publishModelFromString(field.value().toString());
          },
          [&](QXmppDataForm::Field &field) {
              if (d->publishModel) {
                  field.setValue(QXmppPubSubNodeConfig::publishModelToString(*d->publishModel));
              }
          } },
        { QXmppDataForm::Field::TextSingleField,
          QStringLiteral("pubsub#num_subscribers"),
          Optional,
          [&](const QXmppDataForm::Field &field) {
              bool ok;
              d->numberOfSubscribers = field.value().toString().toLongLong(&ok);
              if (!ok) {
                  d->numberOfSubscribers = -1;
              }
          },
          [&](QXmppDataForm::Field &field) {
              if (d->numberOfSubscribers >= 0) {
                  field.setValue(QString::number(d->numberOfSubscribers));
              }
          } },
        { QXmppDataForm::Field::JidMultiField,
          QStringLiteral("pubsub#owner"),
          Optional,
          [&](const QXmppDataForm::Field &field) {
              d->ownerJids = field.value().toStringList();
          },
          [&](QXmppDataForm::Field &field) {
              if (!d->ownerJids.isEmpty()) {
                  field.setValue(d->ownerJids);
              }
          } },
        { QXmppDataForm::Field::JidMultiField,
          QStringLiteral("pubsub#publisher"),
          Optional,
          [&](const QXmppDataForm::Field &field) {
              d->publisherJids = field.value().toStringList();
          },
          [&](QXmppDataForm::Field &field) {
              if (!d->publisherJids.isEmpty()) {
                  field.setValue(d->publisherJids);
              }
          } },
        { QXmppDataForm::Field::TextSingleField,
          QStringLiteral("pubsub#title"),
          Optional,
          [&](const QXmppDataForm::Field &field) {
              d->title = field.value().toString();
          },
          [&](QXmppDataForm::Field &field) {
              if (!d->title.isNull()) {
                  field.setValue(d->title);
              }
          } },
        { QXmppDataForm::Field::TextSingleField,
          QStringLiteral("pubsub#type"),
          Optional,
          [&](const QXmppDataForm::Field &field) {
              d->type = field.value().toString();
          },
          [&](QXmppDataForm::Field &field) {
              if (!d->type.isNull()) {
                  field.setValue(d->type);
              }
          } },
        { QXmppDataForm::Field::TextSingleField,
          QStringLiteral("pubsub#max_items"),
          Optional,
          [&](const QXmppDataForm::Field &field) {
              bool ok;
              d->maxItems = field.value().toString().toLongLong(&ok);
              if (!ok) {
                  d->maxItems = -1;
              }
          },
          [&](QXmppDataForm::Field &field) {
              if (d->maxItems >= 0) {
                  field.setValue(QString::number(d->maxItems));
              } else {
                  field.setValue(QStringLiteral("max"));
              }
          } }
    };
}
