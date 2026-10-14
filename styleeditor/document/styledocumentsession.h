#pragma once

#include "styledocument.h"

#include <QObject>

class StyleDocumentSession : public QObject {
    Q_OBJECT

public:
    explicit StyleDocumentSession(QObject* parent = nullptr);

    const StyleDocument& document() const;
    void setName(const QString& name);
    void updateContent(const StyleDocument& content);

    void newDocument();
    bool open(const QString& filePath, QString& errorMessage);
    bool save(const QString& filePath, QString& errorMessage);

signals:
    void metadataChanged();

private:
    void markModified();

    StyleDocument m_document;
};
