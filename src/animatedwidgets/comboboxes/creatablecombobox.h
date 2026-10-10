#pragma once

#include "animatedwidgets/comboboxes/searchcombobox.h"

namespace animatedwidgets {

class HoverButton;

class CreatableComboBox : public SearchComboBox {
    Q_OBJECT
    Q_PROPERTY(bool creationEnabled READ creationEnabled WRITE setCreationEnabled)
    Q_PROPERTY(QString createText READ createText WRITE setCreateText)
    Q_PROPERTY(Qt::CaseSensitivity creationCaseSensitivity READ creationCaseSensitivity WRITE setCreationCaseSensitivity)
    Q_PROPERTY(int minimumCreateLength READ minimumCreateLength WRITE setMinimumCreateLength)

public:
    explicit CreatableComboBox(QWidget* parent = nullptr);
    ~CreatableComboBox() override;

    bool creationEnabled() const;
    void setCreationEnabled(bool enabled);

    QString createText() const;
    void setCreateText(const QString& text);

    Qt::CaseSensitivity creationCaseSensitivity() const;
    void setCreationCaseSensitivity(Qt::CaseSensitivity sensitivity);

    int minimumCreateLength() const;
    void setMinimumCreateLength(int length);

    QString creationCandidate() const;
    // Available while the popup is open; the popup owns the button.
    HoverButton* createButton() const;

public slots:
    void requestCreation();

signals:
    void createRequested(const QString& text);

protected:
    bool event(QEvent* event) override;
    bool eventFilter(QObject* watched, QEvent* event) override;
    ComboListPopup* createPopup() override;
    bool canShowPopup() const override;
    void configurePopup(ComboListPopup* popup) override;
    void popupClosed() override;

private:
    void updateCreationEntry();
    void disconnectCreationModel();
    void selectCreationEntry(bool selected);
    bool navigateCreationEntry(int direction);

    QPointer<HoverButton> m_createButton;
    QList<QMetaObject::Connection> m_modelConnections;

    QString m_createText;
    Qt::CaseSensitivity m_creationCaseSensitivity = Qt::CaseInsensitive;
    int m_minimumCreateLength = 1;
    bool m_creationEnabled = true;
    bool m_creationSelected = false;
    bool m_preediting = false;
};

} // namespace animatedwidgets

#ifndef ANIMATEDWIDGETS_DESIGNER_PLUGIN
using CreatableComboBox = animatedwidgets::CreatableComboBox;
#endif
