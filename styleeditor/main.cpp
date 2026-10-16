#include "styleeditorwindow.h"

#include "utils/objectfindtool.h"
#include "preview/stylepreviewworker.h"

#include <QApplication>
#ifdef Q_OS_WIN
#include <Windows.h>
#endif

#include <memory>

int main(int argc, char* argv[]) {
    const bool previewWorker = argc > 1 && QString::fromLocal8Bit(argv[1]) == QStringLiteral("--style-preview");
    if (previewWorker && argc != 4)
        return 2;

    std::unique_ptr<QApplication> app;
    if (previewWorker) {
        app = std::make_unique<QApplication>(argc, argv);
    } else {
#ifdef QT_DEBUG
        app = std::make_unique<ObjectFinderApplication>(argc, argv);
#else
        app = std::make_unique<QApplication>(argc, argv);
#endif
    }

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

    if (previewWorker) {
        QApplication::setQuitOnLastWindowClosed(false);
        StylePreviewWorker worker(QString::fromLocal8Bit(argv[2]), QString::fromLocal8Bit(argv[3]));
        return app->exec();
    }

    StyleEditorWindow window;
    window.show();

    return app->exec();
}
