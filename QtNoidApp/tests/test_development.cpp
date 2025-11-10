#include <QTest>
#include <QtGui/qclipboard.h>
#include <QtGui/qshortcut.h>
#include <QtNoidApp/QtNoidApp>
#include <QHBoxLayout>
#include <QLabel>
#include <QMainWindow>
#include <QMimeData>
#include <QDir>
#include <QFileInfo>

class TestQtNoidAppDevelopment : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();
    void testInitFullDialogGrabShortcut();
    void testInitFullDialogGrabShortcut_withFileName();

private:
    void initMainDialog(QMainWindow* frm, const QString &caption);

};

using namespace QtNoid::App;


void TestQtNoidAppDevelopment::initTestCase()
{}

void TestQtNoidAppDevelopment::cleanupTestCase()
{}

void TestQtNoidAppDevelopment::init()
{}

void TestQtNoidAppDevelopment::cleanup()
{}

void TestQtNoidAppDevelopment::initMainDialog(QMainWindow* frm, const QString& caption)
{
    frm->setWindowTitle(caption);
    frm->setCentralWidget(new QWidget());
    frm->centralWidget()->setLayout(new QHBoxLayout());
    auto myWidget = new QLabel(caption);
    frm->centralWidget()->layout()->addWidget(myWidget);
    frm->setFixedWidth(400);

    frm->show();
    Q_UNUSED(QTest::qWaitForWindowExposed(frm));
    QTest::qWait(500);
}

void TestQtNoidAppDevelopment::testInitFullDialogGrabShortcut()
{
    QMainWindow frm;
    initMainDialog(&frm, __func__);

    // Actitate shortcut
    QShortcut *shortcut = Development::initFullDialogGrabShortcut(&frm);

    // Check the shortcut creation
    QVERIFY(shortcut != nullptr);
    // QCOMPARE(shortcut->key(), QKeySequence(keySeq));
    QCOMPARE(shortcut->parent(), &frm);

    // fire the shortcut
    shortcut->activated();
    QTest::qWait(100);

    // Search the screenshot file knowing it'll start with __func__
    QStringList filters{{QString(__func__) + "-*.png"}};
    QDir dir;
    QStringList files = dir.entryList(filters, QDir::Files);
    // qDebug() << __func__ << files;
    QVERIFY(files.count());

    // Verify the screenshot
    QFileInfo fileInfo(files.first());
    QVERIFY2(fileInfo.exists(), "Screenshot file not found");
    QVERIFY(fileInfo.size() > 0);

    QPixmap pixmap(fileInfo.filePath());
    //qDebug() << __func__ << pixmap.size();

    // Verify the clipboard content
    QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();
    QVERIFY2(mimeData->hasImage(), "Clipboard should contain an image");
    auto actual = clipboard->image();
    // qDebug() << __func__ << actual.size();
    QCOMPARE(actual.size(), pixmap.size());
}


void TestQtNoidAppDevelopment::testInitFullDialogGrabShortcut_withFileName()
{
    QString sstFile = Settings::appExeOrAppBundleDirPath() + "/" + __func__ + ".png";
    // qDebug() << __func__ << sstFile;

    QMainWindow frm;
    initMainDialog(&frm, __func__);

    QString keySeq = "Ctrl+Shift+Q";

    // Actitate shortcut
    QShortcut *shortcut = Development::initFullDialogGrabShortcut(&frm,
                                                               keySeq,
                                                               sstFile,
                                                               false);

    // Check the shortcut creation
    QVERIFY(shortcut != nullptr);
    QCOMPARE(shortcut->key(), QKeySequence(keySeq));
    QCOMPARE(shortcut->parent(), &frm);

    // fire the shortcut
    shortcut->activated();
    QTest::qWait(100);

    // Verifica che il file PNG sia stato creato
    QFileInfo fileInfo(sstFile);
    QVERIFY2(fileInfo.exists(), "Screenshot file not found");
    QVERIFY(fileInfo.size() > 0);
}


QTEST_MAIN(TestQtNoidAppDevelopment)
#include "test_development.moc"
