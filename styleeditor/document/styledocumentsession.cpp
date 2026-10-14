#include "styledocumentsession.h"

#include "../utils/styledocumentcodec.h"

StyleDocumentSession::StyleDocumentSession(QObject* parent)
    : QObject(parent)
{
}

const StyleDocument& StyleDocumentSession::document() const {
    return m_document;
}

void StyleDocumentSession::setName(const QString& name) {
    if (m_document.name == name)
        return;

    m_document.name = name;
    markModified();
}

void StyleDocumentSession::updateContent(const StyleDocument& content) {
    StyleDocument updated = m_document;
    updated.items = content.items;
    if (StyleDocumentCodec::toJson(updated) == StyleDocumentCodec::toJson(m_document))
        return;

    m_document.items = updated.items;
    markModified();
}

void StyleDocumentSession::newDocument() {
    m_document = StyleDocument();
    emit metadataChanged();
}

bool StyleDocumentSession::open(const QString& filePath, QString& errorMessage) {
    StyleDocument loaded;
    if (!StyleDocumentCodec::readFile(filePath, loaded, errorMessage))
        return false;

    m_document = loaded;
    emit metadataChanged();
    return true;
}

bool StyleDocumentSession::save(const QString& filePath, QString& errorMessage) {
    StyleDocument saved = m_document;
    saved.name = saved.name.trimmed();
    if (saved.name.isEmpty())
        saved.name = QStringLiteral("Untitled");
    if (!StyleDocumentCodec::writeFile(filePath, saved, errorMessage))
        return false;

    m_document.name = saved.name;
    m_document.filePath = filePath;
    m_document.modified = false;
    emit metadataChanged();
    return true;
}

void StyleDocumentSession::markModified() {
    m_document.modified = true;
    emit metadataChanged();
}
