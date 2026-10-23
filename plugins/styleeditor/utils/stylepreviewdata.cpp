#include "stylepreviewdata.h"

QStringList StylePreviewData::choices() {
    return {QStringLiteral("Aurora"), QStringLiteral("Orbit"), QStringLiteral("Horizon"), QStringLiteral("Nova")};
}

QStandardItemModel* StylePreviewData::tree(QObject* parent) {
    auto* model = new QStandardItemModel(parent);
    auto* studio = new QStandardItem(QStringLiteral("Studio"));
    auto* platform = new QStandardItem(QStringLiteral("Platform"));
    auto* artwork = new QStandardItem(QStringLiteral("Artwork"));
    artwork->appendRow(new QStandardItem(QStringLiteral("Illustration")));
    artwork->appendRow(new QStandardItem(QStringLiteral("Motion")));
    studio->appendRow(artwork);
    studio->appendRow(new QStandardItem(QStringLiteral("Audio")));
    platform->appendRow(new QStandardItem(QStringLiteral("Desktop")));
    platform->appendRow(new QStandardItem(QStringLiteral("Mobile")));
    model->appendRow(studio);
    model->appendRow(platform);
    return model;
}
