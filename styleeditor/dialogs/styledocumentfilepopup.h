#pragma once

#include "stylepopupdialog.h"

#include "ui_styledocumentfilepopup.h"

#include <QPointer>

class QFileSystemModel;
class QModelIndex;
class StyleMessagePopup;

class StyleDocumentFilePopup : public StylePopupDialog {
    Q_OBJECT

public:
    enum class Mode { Open, Save };
    Q_ENUM(Mode)

    explicit StyleDocumentFilePopup(Mode mode, const QString& initialPath, QWidget* parent = nullptr);

signals:
    void fileSelected(const QString& filePath);

private slots:
    void on_upButton_clicked();
    void on_homeButton_clicked();
    void on_pathEdit_returnPressed();
    void on_fileNameEdit_returnPressed();
    void on_fileNameEdit_textChanged();
    void on_fileView_activated(const QModelIndex& index);
    void on_acceptButton_clicked();
    void on_cancelButton_clicked();

private:
    void createLayout();
    void setDirectory(const QString& path);
    void selectFile(const QString& path);

    Ui::StyleDocumentFilePopup ui;

    QFileSystemModel* fileModel = nullptr;
    QPointer<StyleMessagePopup> overwritePopup;
    Mode m_mode;
    QString m_directory;
    QString m_selectedFile;
};
