/*
 * Copyright (C) 2008-2021 The QXmpp developers
 *
 * Author:
 *  Manjeet Dahiya
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

#include "QXmppEntityTimeManager.h"

#include "QXmppClient.h"
#include "QXmppConstants_p.h"
#include "QXmppEntityTimeIq.h"
#include "QXmppUtils.h"

#include <QDateTime>
#include <QDomElement>
#include <QFutureWatcher>

///
/// Request the time from an XMPP entity.
///
/// \param jid
///
QString QXmppEntityTimeManager::requestTime(const QString &jid)
{
    QXmppEntityTimeIq request;
    request.setType(QXmppIq::Get);
    request.setTo(jid);
    if (client()->sendPacket(request))
        return request.id();
    else
        return QString();
}

QFuture<QXmppEntityTimeManager::EntityTimeResult> QXmppEntityTimeManager::requestEntityTime(const QString &jid)
{
    QXmppEntityTimeIq iq;
    iq.setType(QXmppIq::Get);
    iq.setTo(jid);
    auto iqFuture = client()->sendIq(iq);

    auto futureInterface = QSharedPointer<QFutureInterface<QXmppEntityTimeManager::EntityTimeResult>>(new QFutureInterface<QXmppEntityTimeManager::EntityTimeResult>());
    futureInterface->reportStarted();

    auto *watcher = new QFutureWatcher<QXmppClient::IqResult>();
    connect(watcher, &QFutureWatcher<QXmppClient::IqResult>::finished, [=]() {
        const auto result = iqFuture.resultAt(iqFuture.resultCount() - 1);
        if (const auto *element = std::get_if<QDomElement>(&result)) {
            QXmppEntityTimeIq resultIq;
            resultIq.parse(*element);

            futureInterface->reportResult(resultIq);
        } else if (const auto *packetError = std::get_if<QXmpp::PacketState>(&result)) {
            futureInterface->reportResult(*packetError);
        }

        futureInterface->reportFinished();
        watcher->deleteLater();
    });
    watcher->setFuture(iqFuture);

    return futureInterface->future();
}

/// \cond
QStringList QXmppEntityTimeManager::discoveryFeatures() const
{
    return QStringList() << ns_entity_time;
}

bool QXmppEntityTimeManager::handleStanza(const QDomElement &element)
{
    if (element.tagName() == "iq" && QXmppEntityTimeIq::isEntityTimeIq(element)) {
        QXmppEntityTimeIq entityTime;
        entityTime.parse(element);

        if (entityTime.type() == QXmppIq::Get) {
            // respond to query
            QXmppEntityTimeIq responseIq;
            responseIq.setType(QXmppIq::Result);
            responseIq.setId(entityTime.id());
            responseIq.setTo(entityTime.from());

            QDateTime currentTime = QDateTime::currentDateTime();
            QDateTime utc = currentTime.toUTC();
            responseIq.setUtc(utc);

            currentTime.setTimeSpec(Qt::UTC);
            responseIq.setTzo(utc.secsTo(currentTime));

            client()->sendPacket(responseIq);
        }

        emit timeReceived(entityTime);
        return true;
    }

    return false;
}
/// \endcond
