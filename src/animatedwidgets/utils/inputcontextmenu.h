#pragma once

#include <QObject>
#include <QPointer>
#include <QPoint>
#include <QString>

#include <functional>

class QAbstractSpinBox;
class QMenu;
class QWidget;

namespace animatedwidgets {

class InputContextMenu : public QObject {
    Q_OBJECT

public:
    static void install(QWidget* editor, QAbstractSpinBox* spinBox = nullptr);

private:
    explicit InputContextMenu(QWidget* editor, QAbstractSpinBox* spinBox);
    void connectMenuRequest(QWidget* widget, QWidget* coordinateWidget);
    void showMenu(const QPoint& globalPos);
    void appendStepActions(QMenu* menu) const;
    static QString displayText(const QString& text);
    static std::function<void()> captureSelection(QWidget* editor);

    QWidget* m_editor = nullptr;
    QPointer<QAbstractSpinBox> m_spinBox;
};

} // namespace animatedwidgets
