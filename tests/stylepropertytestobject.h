#pragma once

#include <QColor>
#include <QIcon>
#include <QKeySequence>
#include <QObject>
#include <QSize>

class StylePropertyTestObject : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString text MEMBER text)
    Q_PROPERTY(int cornerRadius MEMBER cornerRadius)
    Q_PROPERTY(double borderWidth MEMBER borderWidth)
    Q_PROPERTY(bool animateContentColor MEMBER animateContentColor)
    Q_PROPERTY(QColor backgroundHover MEMBER backgroundHover)
    Q_PROPERTY(QIcon icon READ icon WRITE setIcon)
    Q_PROPERTY(bool iconHint MEMBER iconHint)
    Q_PROPERTY(QSize iconSize MEMBER iconSize)
    Q_PROPERTY(QKeySequence shortcut MEMBER shortcut)
    Q_PROPERTY(Placement placement MEMBER placement)
    Q_PROPERTY(Qt::Alignment contentAlignment MEMBER contentAlignment)
    Q_PROPERTY(int readOnly READ readOnly)

public:
    enum class Placement { BeforeText, AfterText };
    Q_ENUM(Placement)

    int readOnly() const {
        return 1;
    }

    QIcon icon() const {
        return m_icon;
    }

    void setIcon(const QIcon& icon) {
        m_icon = icon;
    }

    QString text;
    QSize iconSize = QSize(16, 16);
    QKeySequence shortcut;
    int cornerRadius = 6;
    double borderWidth = 1.0;
    bool animateContentColor = false;
    bool iconHint = false;
    QColor backgroundHover;
    Placement placement = Placement::BeforeText;
    Qt::Alignment contentAlignment = Qt::AlignCenter;

private:
    QIcon m_icon;
};
