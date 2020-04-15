/*
 * Copyright (C) 2008-2020 The QXmpp developers
 *
 * Authors:
 *  Manjeet Dahiya
 *  Jeremy Lainé
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

#include "QXmppStream.h"

#include "QXmppConstants_p.h"
#include "QXmppLogger.h"
#include "QXmppSocket.h"
#include "QXmppStanza.h"
#include "QXmppStreamManagement_p.h"
#include "QXmppUtils.h"

#include <QBuffer>
#include <QDomDocument>
#include <QHostAddress>
#include <QMap>
#include <QRegExp>
#include <QStringList>
#include <QTime>
#include <QXmlStreamWriter>

#if QT_VERSION < QT_VERSION_CHECK(5, 10, 0)
static bool randomSeeded = false;
#endif
static const QByteArray streamRootElementEnd = QByteArrayLiteral("</stream:stream>");

class QXmppStreamPrivate
{
public:
    QXmppStreamPrivate();

    QString dataBuffer;
    QXmppSocket *socket;

    // incoming stream state
    QByteArray streamStart;

    bool streamManagementEnabled;
    QMap<unsigned int, QString> unacknowledgedStanzas;
    unsigned lastOutgoingSequenceNumber;
    unsigned lastIncomingSequenceNumber;
};

QXmppStreamPrivate::QXmppStreamPrivate()
    : socket(nullptr), streamManagementEnabled(false), lastOutgoingSequenceNumber(0), lastIncomingSequenceNumber(0)
{
}

///
/// Constructs a base XMPP stream.
///
/// \param parent
///
QXmppStream::QXmppStream(QObject *parent)
    : QXmppLoggable(parent),
      d(new QXmppStreamPrivate)
{
#if QT_VERSION < QT_VERSION_CHECK(5, 10, 0)
    // Make sure the random number generator is seeded
    if (!randomSeeded) {
        qsrand(QTime(0, 0, 0).msecsTo(QTime::currentTime()) ^ reinterpret_cast<quintptr>(this));
        randomSeeded = true;
    }
#endif
}

///
/// Destroys a base XMPP stream.
///
QXmppStream::~QXmppStream()
{
    delete d;
}

///
/// Disconnects from the remote host.
///
void QXmppStream::disconnectFromHost()
{
    d->streamManagementEnabled = false;
    if (d->socket) {
        if (d->socket->state() == QAbstractSocket::ConnectedState) {
            sendData(streamRootElementEnd);
            d->socket->flush();
        }
        // FIXME: according to RFC 6120 section 4.4, we should wait for
        // the incoming stream to end before closing the socket
        d->socket->disconnectFromHost();
    }
}

///
/// Handles a stream start event, which occurs when the underlying transport
/// becomes ready (socket connected, encryption started).
///
/// If you redefine handleStart(), make sure to call the base class's method.
///
void QXmppStream::handleStart()
{
    d->streamManagementEnabled = false;
    d->dataBuffer.clear();
    d->streamStart.clear();
}

///
/// Returns true if the stream is connected.
///
bool QXmppStream::isConnected() const
{
    return d->socket &&
        d->socket->state() == QAbstractSocket::ConnectedState;
}

///
/// Sends raw data to the peer.
///
/// \param data
///
bool QXmppStream::sendData(const QByteArray &data)
{
    return sendData(QString::fromUtf8(data));
}

bool QXmppStream::sendData(const QString &text)
{
    logSent(text);

    if (!d->socket || d->socket->state() != QAbstractSocket::ConnectedState)
        return false;

    return d->socket->sendTextMessage(text);
}

///
/// Sends an XMPP packet to the peer.
///
/// \param packet
///
bool QXmppStream::sendPacket(const QXmppStanza &packet)
{
    // prepare packet
    QString text;
    QXmlStreamWriter xmlStream(&text);
    packet.toXml(&xmlStream);

    bool isXmppStanza = packet.isXmppStanza();
    if (isXmppStanza && d->streamManagementEnabled)
        d->unacknowledgedStanzas[++d->lastOutgoingSequenceNumber] = text;

    // send packet
    bool success = sendData(text);
    if (isXmppStanza)
        sendAcknowledgementRequest();
    return success;
}

///
/// Returns the QSslSocket used for this stream.
///
QXmppSocket *QXmppStream::socket() const
{
    return d->socket;
}

///
/// Sets the QSslSocket used for this stream.
///
void QXmppStream::setSocket(QXmppSocket *socket)
{
    d->socket = socket;
    if (!d->socket)
        return;

    // socket events
    connect(socket, &QXmppSocket::connected, this, &QXmppStream::_q_socketConnected);
    connect(socket, &QXmppSocket::encryptionStarted, this, &QXmppStream::_q_socketEncrypted);
    connect(socket, &QXmppSocket::errorOccured, this, &QXmppStream::_q_socketError);
    connect(socket, &QXmppSocket::textMessageReceived, this, &QXmppStream::handleTextMessageReceived);
}

void QXmppStream::_q_socketConnected()
{
    info(QStringLiteral("Socket connected to %1 %2").arg(d->socket->peerAddress().toString(), QString::number(d->socket->peerPort())));
    handleStart();
}

void QXmppStream::_q_socketEncrypted()
{
    debug("Socket encrypted");
    handleStart();
}

void QXmppStream::_q_socketError(QAbstractSocket::SocketError socketError)
{
    Q_UNUSED(socketError);
    warning(QStringLiteral("Socket error: ") + socket()->errorString());
}

void QXmppStream::handleTextMessageReceived(const QString &text)
{
    d->dataBuffer.append(text);

    // handle whitespace pings
    if (!d->dataBuffer.isEmpty() && d->dataBuffer.trimmed().isEmpty()) {
        d->dataBuffer.clear();
        handleStanza(QDomElement());
        return;
    }

    // FIXME : maybe these QRegExps could be static?
    QRegExp startStreamRegex(R"(^(<\?xml.*\?>)?\s*<stream:stream.*>)");
    startStreamRegex.setMinimal(true);
    QRegExp endStreamRegex("</stream:stream>$");
    endStreamRegex.setMinimal(true);

    // check whether we need to add stream start / end elements
    //
    // NOTE: as we may only have partial XML content, do not alter the stream's
    // state until we have a valid XML document!
    QString completeXml = d->dataBuffer;
    bool streamStart = false;
    if (d->streamStart.isEmpty() && d->dataBuffer.contains(startStreamRegex))
        streamStart = true;
    else
        completeXml.prepend(d->streamStart);
    bool streamEnd = false;
    if (d->dataBuffer.contains(endStreamRegex))
        streamEnd = true;
    else
        completeXml.append(streamRootElementEnd);

    // check whether we have a valid XML document
    QDomDocument doc;
    if (!doc.setContent(completeXml, true))
        return;

    // remove data from buffer
    logReceived(text);
    d->dataBuffer.clear();

    // process stream start
    if (streamStart) {
        d->streamStart = startStreamRegex.cap(0).toUtf8();
        handleStream(doc.documentElement());
    }

    // process stanzas
    QDomElement nodeRecv = doc.documentElement().firstChildElement();
    while (!nodeRecv.isNull()) {
        if (QXmppStreamManagementAck::isStreamManagementAck(nodeRecv))
            handleAcknowledgement(nodeRecv);
        else if (QXmppStreamManagementReq::isStreamManagementReq(nodeRecv))
            sendAcknowledgement();
        else {
            handleStanza(nodeRecv);
            if (nodeRecv.tagName() == QLatin1String("message") ||
                nodeRecv.tagName() == QLatin1String("presence") ||
                nodeRecv.tagName() == QLatin1String("iq"))
                ++d->lastIncomingSequenceNumber;
        }
        nodeRecv = nodeRecv.nextSiblingElement();
    }

    // process stream end
    if (streamEnd)
        disconnectFromHost();
}

///
/// Enables Stream Management acks / reqs (\xep{0198}).
///
/// \param resetSequenceNumber Indicates if the sequence numbers should be
/// reset. This must be done if the stream is not resumed.
///
/// \since QXmpp 1.0
///
void QXmppStream::enableStreamManagement(bool resetSequenceNumber)
{
    d->streamManagementEnabled = true;

    if (resetSequenceNumber) {
        d->lastOutgoingSequenceNumber = 0;
        d->lastIncomingSequenceNumber = 0;

        // resend unacked stanzas
        if (!d->unacknowledgedStanzas.empty()) {
            QMap<unsigned, QString> oldUnackedStanzas = d->unacknowledgedStanzas;
            d->unacknowledgedStanzas.clear();
            for (QMap<unsigned int, QString>::iterator it = oldUnackedStanzas.begin(); it != oldUnackedStanzas.end(); ++it) {
                d->unacknowledgedStanzas[++d->lastOutgoingSequenceNumber] = it.value();
                sendData(it.value());
            }
            sendAcknowledgementRequest();
        }
    } else {
        // resend unacked stanzas
        if (!d->unacknowledgedStanzas.empty()) {
            for (QMap<unsigned int, QString>::iterator it = d->unacknowledgedStanzas.begin(); it != d->unacknowledgedStanzas.end(); ++it)
                sendData(it.value());
            sendAcknowledgementRequest();
        }
    }
}

///
/// Returns the sequence number of the last incoming stanza (\xep{0198}).
///
/// \since QXmpp 1.0
///
unsigned QXmppStream::lastIncomingSequenceNumber() const
{
    return d->lastIncomingSequenceNumber;
}

///
/// Sets the last acknowledged sequence number for outgoing stanzas
/// (\xep{0198}).
///
/// \since QXmpp 1.0
///
void QXmppStream::setAcknowledgedSequenceNumber(unsigned sequenceNumber)
{
    for (QMap<unsigned int, QString>::iterator it = d->unacknowledgedStanzas.begin(); it != d->unacknowledgedStanzas.end();) {
        if (it.key() <= sequenceNumber)
            it = d->unacknowledgedStanzas.erase(it);
        else
            ++it;
    }
}

///
/// Handles an incoming acknowledgement from \xep{0198}.
///
/// \param element
///
/// \since QXmpp 1.0
///
void QXmppStream::handleAcknowledgement(QDomElement &element)
{
    if (!d->streamManagementEnabled)
        return;

    QXmppStreamManagementAck ack;
    ack.parse(element);
    setAcknowledgedSequenceNumber(ack.seqNo());
}

///
/// Sends an acknowledgement as defined in \xep{0198}.
///
/// \since QXmpp 1.0
///
void QXmppStream::sendAcknowledgement()
{
    if (!d->streamManagementEnabled)
        return;

    // prepare packet
    QByteArray data;
    QXmlStreamWriter xmlStream(&data);
    QXmppStreamManagementAck ack(d->lastIncomingSequenceNumber);
    ack.toXml(&xmlStream);

    // send packet
    sendData(data);
}

///
/// Sends an acknowledgement request as defined in \xep{0198}.
///
/// \since QXmpp 1.0
///
void QXmppStream::sendAcknowledgementRequest()
{
    if (!d->streamManagementEnabled)
        return;

    // prepare packet
    QByteArray data;
    QXmlStreamWriter xmlStream(&data);
    QXmppStreamManagementReq::toXml(&xmlStream);

    // send packet
    sendData(data);
}
