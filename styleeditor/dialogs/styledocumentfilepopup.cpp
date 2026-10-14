#include "styledocumentfilepopup.h"

#include "stylemessagepopup.h"

#include "animatedwidgets/utils/smoothscrolling.h"

#include <QDir>
#include <QFileInfo>
#include <QFileSystemModel>
#include <QHeaderView>
#include <QItemSelectionModel>
#include <QStandardPaths>

StyleDocumentFilePopup::StyleDocumentFilePopup(Mode mode, const QString& initialPath, QWidget* parent)
    : StylePopupDialog(parent)
    , m_mode(mode)
{
    createLayout();
    const QString title = mode == Mode::Open ? QStringLiteral("Open style document") : QStringLiteral("Save style document");
    setWindowTitle(title);
    ui.titleLabel->setText(title);
    ui.acceptButton->setText(mode == Mode::Open ? QStringLiteral("Open") : QStringLiteral("Save"));
    setFocusProxy(mode == Mode::Open ? static_cast<QWidget*>(ui.fileView) : ui.fileNameEdit);

    QString directory = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    if (!initialPath.isEmpty()) {
        const QFileInfo initial(initialPath);
        directory = initial.isDir() ? initial.absoluteFilePath() : initial.absolutePath();
        if (!initial.isDir())
            ui.fileNameEdit->setText(initial.fileName());
    }
    if (!QFileInfo(directory).isDir())
        directory = QDir::currentPath();
    setDirectory(directory);

    connect(this, &StylePopupDialog::finished, this, [this](int result) {
        if (result == 1)
            emit fileSelected(m_selectedFile);
    });
}

void StyleDocumentFilePopup::createLayout() {
    ui.setupUi(this);
    ui.errorLabel->hide();

    fileModel = new QFileSystemModel(this);
    fileModel->setReadOnly(true);
    fileModel->setFilter(QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot);
    fileModel->setNameFilters({QStringLiteral("*.json")});
    fileModel->setNameFilterDisables(false);

    ui.fileView->setModel(fileModel);
    ui.fileView->setColumnHidden(2, true);
    ui.fileView->header()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui.fileView->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui.fileView->header()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    ui.fileView->sortByColumn(0, Qt::AscendingOrder);
    animatedwidgets::SmoothScrolling::install(ui.fileView);

    connect(ui.fileView->selectionModel(), &QItemSelectionModel::currentChanged, this, [this](const QModelIndex& index) {
        if (index.isValid() && !fileModel->isDir(index))
            ui.fileNameEdit->setText(fileModel->fileName(index));
    });
}

void StyleDocumentFilePopup::setDirectory(const QString& path) {
    const QFileInfo directory(path);
    if (!directory.isDir() || !directory.isReadable()) {
        ui.errorLabel->setText(QStringLiteral("This directory is not accessible."));
        ui.errorLabel->show();
        return;
    }

    m_directory = directory.absoluteFilePath();
    ui.pathEdit->setText(QDir::toNativeSeparators(m_directory));
    ui.fileView->setRootIndex(fileModel->setRootPath(m_directory));
    ui.upButton->setEnabled(!QDir(m_directory).isRoot());
    ui.errorLabel->hide();
}

void StyleDocumentFilePopup::on_upButton_clicked() {
    QDir directory(m_directory);
    if (directory.cdUp())
        setDirectory(directory.absolutePath());
}

void StyleDocumentFilePopup::on_homeButton_clicked() {
    setDirectory(QDir::homePath());
}

void StyleDocumentFilePopup::on_pathEdit_returnPressed() {
    setDirectory(QDir(m_directory).absoluteFilePath(ui.pathEdit->text().trimmed()));
}

void StyleDocumentFilePopup::on_fileNameEdit_returnPressed() {
    on_acceptButton_clicked();
}

void StyleDocumentFilePopup::on_fileNameEdit_textChanged() {
    ui.errorLabel->hide();
}

void StyleDocumentFilePopup::on_fileView_activated(const QModelIndex& index) {
    if (!index.isValid())
        return;

    if (fileModel->isDir(index)) {
        setDirectory(fileModel->filePath(index));
        return;
    }
    ui.fileNameEdit->setText(fileModel->fileName(index));
    on_acceptButton_clicked();
}

void StyleDocumentFilePopup::on_acceptButton_clicked() {
    if (overwritePopup)
        return;

    const QString name = ui.fileNameEdit->text().trimmed();
    if (name.isEmpty()) {
        ui.errorLabel->setText(QStringLiteral("Enter a file name."));
        ui.errorLabel->show();
        ui.fileNameEdit->setFocus();
        return;
    }

    QString path = QDir(m_directory).absoluteFilePath(name);
    if (QFileInfo(path).isDir()) {
        setDirectory(path);
        return;
    }
    if (m_mode == Mode::Save && QFileInfo(path).suffix().isEmpty())
        path += QStringLiteral(".json");

    const QFileInfo file(path);
    if (file.suffix().compare(QStringLiteral("json"), Qt::CaseInsensitive) != 0) {
        ui.errorLabel->setText(QStringLiteral("Select a JSON file (*.json)."));
        ui.errorLabel->show();
        return;
    }
    if (m_mode == Mode::Open && (!file.isFile() || !file.isReadable())) {
        ui.errorLabel->setText(QStringLiteral("This file does not exist or cannot be read."));
        ui.errorLabel->show();
        return;
    }
    if (m_mode == Mode::Save && !QFileInfo(file.absolutePath()).isDir()) {
        ui.errorLabel->setText(QStringLiteral("The destination directory does not exist."));
        ui.errorLabel->show();
        return;
    }

    if (m_mode == Mode::Save && file.exists()) {
        auto* popup = new StyleMessagePopup(QStringLiteral("Replace file?"),
            QStringLiteral("%1 already exists. Replace it with the current document?").arg(file.fileName()), this);
        overwritePopup = popup;
        popup->setPrimaryAction(StyleMessagePopup::Action::Accept, QStringLiteral("Replace"));
        popup->setCancelVisible(true);
        connect(popup, &StyleMessagePopup::actionSelected, this, [this, path](StyleMessagePopup::Action action) {
            overwritePopup = nullptr;
            if (action == StyleMessagePopup::Action::Accept)
                selectFile(path);
        });
        popup->showDialog();
        return;
    }
    selectFile(path);
}

void StyleDocumentFilePopup::selectFile(const QString& path) {
    m_selectedFile = path;
    finish(1);
}

void StyleDocumentFilePopup::on_cancelButton_clicked() {
    finish(0);
}
