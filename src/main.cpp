// This file is a part of "Candle" application.
// Copyright 2015-2016 Hayrullin Denis Ravilevich

#include <QApplication>
#include <QDebug>
#include <QGLWidget>
#include <QLocale>
#include <QTranslator>
#include <QFile>
#include <QStyleFactory>
#include <QFontDatabase>
#include <QMessageBox>

#include "parser/gcodepreprocessorutils.h"
#include "parser/gcodeparser.h"
#include "parser/gcodeviewparse.h"

#include "frmmain.h"

int main(int argc, char *argv[])
{
#ifdef UNIX
    bool styleOverrided = false;
    for (int i = 0; i < argc; i++) if (QString(argv[i]).toUpper() == "-STYLE") {
        styleOverrided = true;
        break;
    }
#endif

    QApplication a(argc, argv);

//    QFontDatabase::addApplicationFont(":/fonts/segoeui.ttf");
//    QFontDatabase::addApplicationFont(":/fonts/tahoma.ttf");

#ifdef GLES
    QFontDatabase::addApplicationFont(":/fonts/Ubuntu-R.ttf");
#endif

    QGLFormat glf = QGLFormat::defaultFormat();
    glf.setSampleBuffers(true);
    glf.setSamples(8);
    QGLFormat::setDefaultFormat(glf);

//    QLocale::setDefault(QLocale("es"));

    QString loc = QLocale().name().left(2);
    QString translationsFolder = qApp->applicationDirPath() + "/translations/";
    QString translationFileName = translationsFolder + qApp->applicationDisplayName() + "_" + loc + ".qm";

    qDebug() << "locale:" << loc << translationFileName;

    if(QFile::exists(translationFileName)) {
        QTranslator* translator = new QTranslator();
        if (translator->load(translationFileName)) a.installTranslator(translator); else delete translator;
    }

    QString baseTranslationFileName = translationsFolder + "qt_" + loc + ".qm";

    if(QFile::exists(translationFileName)) {
        QTranslator* baseTranslator = new QTranslator();
        if (baseTranslator->load(baseTranslationFileName)) a.installTranslator(baseTranslator); else delete baseTranslator;
    }

    a.setApplicationVersion(APP_VERSION);

    a.setAttribute(Qt::AA_EnableHighDpiScaling);

#ifdef UNIX
    if (!styleOverrided) foreach (QString str, QStyleFactory::keys()) {
        qDebug() << "style" << str;
        if (str.contains("GTK+")) {
            a.setStyle(QStyleFactory::create(str));
            break;
        }
    }
#endif

#ifdef GLES
    a.setStyle(QStyleFactory::create("Fusion"));
    QPalette palette;
    palette.setColor(QPalette::Highlight, QColor(204, 204, 254));
    palette.setColor(QPalette::HighlightedText, QColor(0, 0, 0));
    a.setPalette(palette);

    a.setStyleSheet("QWidget {font-family: \"Ubuntu\";}\
                    QMenuBar {background-color: #303030; padding-top: 2px; padding-bottom: 2px;}\
                    QMenuBar::item {spacing: 3px; padding: 2px 8px; background: transparent; color: white;}\
                    QMenuBar::item:pressed {border: 1px solid #505050; border-bottom: 1px; border-top-left-radius: 3px; border-top-right-radius: 3px; background: #404040; color: white;}\
                    QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white;}\
                    QDialog {border: 1px solid palette(mid);}");
#endif

    // Создаём палитру для тёмной темы оформления
    QPalette darkPalette;

    // Настраиваем палитру для цветовых ролей элементов интерфейса
    darkPalette.setColor(QPalette::WindowText, QColor(0xdadada));
    darkPalette.setColor(QPalette::Button, QColor(0x262626));
    darkPalette.setColor(QPalette::Dark, QColor(0x1f1f1f));
    darkPalette.setColor(QPalette::Mid, QColor(0xafafaf));
    darkPalette.setColor(QPalette::Text, QColor(0xdadada));
    darkPalette.setColor(QPalette::BrightText, QColor(0xff3333));
    darkPalette.setColor(QPalette::ButtonText, QColor(0xdadada));
    darkPalette.setColor(QPalette::ButtonText, QColor(0xdadada));
    darkPalette.setColor(QPalette::Base, QColor(0x262728));
    darkPalette.setColor(QPalette::Window, QColor(0x262626));
    darkPalette.setColor(QPalette::Highlight, QColor(0x1f75cc));
    darkPalette.setColor(QPalette::HighlightedText, QColor(0xffffff));
    darkPalette.setColor(QPalette::Link, QColor(0x007af4));
    darkPalette.setColor(QPalette::LinkVisited, QColor(0xa57aff));
    darkPalette.setColor(QPalette::AlternateBase, QColor(0x353637));
    darkPalette.setColor(QPalette::ToolTipBase, QColor(0x111111));
    darkPalette.setColor(QPalette::ToolTipText, QColor(0xdadada));
    darkPalette.setColor(QPalette::PlaceholderText , QColor(0xdadada));


    // Устанавливаем данную палитру
    a.setPalette(darkPalette);

    //  DarkGreen
    //  DarkMaroon
    QFile theme_file(QString("://styles/%1.qss").arg("Dark"));
    theme_file.open(QFile::ReadOnly);   //open theme file

    if(theme_file.isOpen())
    {
        a.setStyleSheet(theme_file.readAll());        //set the theme here!
        theme_file.close();
    }
    else
    {
        qDebug() << "File qss couldn't be opened!";
    }

    a.setStyleSheet(a.styleSheet() + "QWidget {font-size: 8pt}");

    frmMain w;
    w.show();

    return a.exec();
}
