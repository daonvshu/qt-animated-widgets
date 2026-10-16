#include "styleeditorwindow.h"

#include "utils/objectfindtool.h"

#include <QApplication>
#include <Windows.h>

int main(int argc, char* argv[]) {
#ifdef QT_DEBUG
    ObjectFinderApplication app(argc, argv);
#else
    QApplication app(argc, argv);
#endif

    QApplication::setApplicationName(QStringLiteral("Animated Widgets Style Editor"));

#ifdef Q_OS_WIN
    NONCLIENTMETRICSW ncm;
    ncm.cbSize = sizeof(ncm);
    HRESULT hr;
    hr = SystemParametersInfoW(SPI_GETNONCLIENTMETRICS, ncm.cbSize, &ncm, 0);
    if (hr != 0) {
        auto font = QApplication::font();
        font.setFamily(QString::fromWCharArray(ncm.lfMenuFont.lfFaceName));
        QApplication::setFont(font);
    }
#endif

    StyleEditorWindow window;
    window.show();

    return app.exec();
}
