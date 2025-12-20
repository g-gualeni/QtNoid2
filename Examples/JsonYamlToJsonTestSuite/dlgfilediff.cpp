#include "dlgfilediff.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QFile>
#include <QTextStream>
#include <QDialogButtonBox>
#include <QFont>
#include <QScrollBar>
#include <QPushButton>
#include <QProcess>
#include <QDesktopServices>
#include <QUrl>
#include <QFileInfo>

DlgFileDiff::DlgFileDiff(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("File Comparison"));
    resize(1000, 600);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Splitter for two columns
    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);

    // Source side
    QWidget *sourceWidget = new QWidget();
    QVBoxLayout *sourceLayout = new QVBoxLayout(sourceWidget);
    sourceLayout->setContentsMargins(0, 0, 0, 0);

    // Source header with label and button
    QHBoxLayout *sourceHeaderLayout = new QHBoxLayout();
    QLabel *sourceLabel = new QLabel(tr("Source"));
    sourceLabel->setObjectName("sourceLabel");
    QPushButton *btnOpenSource = new QPushButton(tr("Edit"));
    btnOpenSource->setFixedWidth(100);
    connect(btnOpenSource, &QPushButton::clicked, this, &DlgFileDiff::onOpenSourceInEditor);
    sourceHeaderLayout->addWidget(sourceLabel, 1);
    sourceHeaderLayout->addWidget(btnOpenSource);

    m_sourceEdit = new QPlainTextEdit();
    m_sourceEdit->setReadOnly(true);
    m_sourceEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
    QFont monoFont("Consolas", 9);
    monoFont.setStyleHint(QFont::Monospace);
    m_sourceEdit->setFont(monoFont);
    sourceLayout->addLayout(sourceHeaderLayout);
    sourceLayout->addWidget(m_sourceEdit);

    // Destination side
    QWidget *destWidget = new QWidget();
    QVBoxLayout *destLayout = new QVBoxLayout(destWidget);
    destLayout->setContentsMargins(0, 0, 0, 0);

    // Destination header with label and button
    QHBoxLayout *destHeaderLayout = new QHBoxLayout();
    QLabel *destLabel = new QLabel(tr("Destination"));
    destLabel->setObjectName("destLabel");
    QPushButton *btnOpenDest = new QPushButton(tr("Edit"));
    btnOpenDest->setFixedWidth(100);
    connect(btnOpenDest, &QPushButton::clicked, this, &DlgFileDiff::onOpenDestInEditor);
    destHeaderLayout->addWidget(destLabel, 1);
    destHeaderLayout->addWidget(btnOpenDest);

    m_destEdit = new QPlainTextEdit();
    m_destEdit->setReadOnly(true);
    m_destEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
    m_destEdit->setFont(monoFont);
    destLayout->addLayout(destHeaderLayout);
    destLayout->addWidget(m_destEdit);

    splitter->addWidget(sourceWidget);
    splitter->addWidget(destWidget);
    splitter->setSizes({500, 500});

    mainLayout->addWidget(splitter);

    // Sync scrollbars
    connect(m_sourceEdit->verticalScrollBar(), &QScrollBar::valueChanged,
            m_destEdit->verticalScrollBar(), &QScrollBar::setValue);
    connect(m_destEdit->verticalScrollBar(), &QScrollBar::valueChanged,
            m_sourceEdit->verticalScrollBar(), &QScrollBar::setValue);
    connect(m_sourceEdit->horizontalScrollBar(), &QScrollBar::valueChanged,
            m_destEdit->horizontalScrollBar(), &QScrollBar::setValue);
    connect(m_destEdit->horizontalScrollBar(), &QScrollBar::valueChanged,
            m_sourceEdit->horizontalScrollBar(), &QScrollBar::setValue);

    // Button box with navigation and actions
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    QPushButton *btnRefresh = new QPushButton(tr("Refresh"));
    connect(btnRefresh, &QPushButton::clicked, this, &DlgFileDiff::onRefresh);
    buttonLayout->addWidget(btnRefresh);

    buttonLayout->addStretch();

    // Navigation buttons for prev/next file
    QPushButton *btnPrevious = new QPushButton(tr("< Previous"));
    connect(btnPrevious, &QPushButton::clicked, this, &DlgFileDiff::previousFileRequested);
    buttonLayout->addWidget(btnPrevious);

    m_navLabel = new QLabel();
    m_navLabel->setMinimumWidth(100);
    m_navLabel->setAlignment(Qt::AlignCenter);
    buttonLayout->addWidget(m_navLabel);

    QPushButton *btnNext = new QPushButton(tr("Next >"));
    connect(btnNext, &QPushButton::clicked, this, &DlgFileDiff::nextFileRequested);
    buttonLayout->addWidget(btnNext);

    buttonLayout->addStretch();

    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    buttonLayout->addWidget(buttonBox);
    mainLayout->addLayout(buttonLayout);
}

DlgFileDiff::~DlgFileDiff()
{
}

void DlgFileDiff::setFiles(const QString &sourceFilePath, const QString &destFilePath)
{
    m_sourceFilePath = sourceFilePath;
    m_destFilePath = destFilePath;
    loadAndCompare();
}

void DlgFileDiff::setTitles(const QString &sourceTitle, const QString &destTitle)
{
    QLabel *sourceLabel = findChild<QLabel*>("sourceLabel");
    QLabel *destLabel = findChild<QLabel*>("destLabel");
    if (sourceLabel) {
        QString html = sourceTitle;
        html.replace("Source:", "<b>Source:</b>");
        sourceLabel->setText(html);
    }
    if (destLabel) {
        QString html = destTitle;
        html.replace("Destination:", "<b>Destination:</b>");
        destLabel->setText(html);
    }
}

void DlgFileDiff::loadAndCompare()
{
    QStringList sourceLines;
    QStringList destLines;

    // Load source file
    QFile srcFile(m_sourceFilePath);
    if (srcFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&srcFile);
        while (!in.atEnd()) {
            sourceLines.append(in.readLine());
        }
        srcFile.close();
    }

    // Load destination file
    QFile dstFile(m_destFilePath);
    if (dstFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&dstFile);
        while (!in.atEnd()) {
            destLines.append(in.readLine());
        }
        dstFile.close();
    }

    highlightDifferences(sourceLines, destLines);
}

void DlgFileDiff::highlightDifferences(const QStringList &sourceLines, const QStringList &destLines)
{
    m_sourceEdit->clear();
    m_destEdit->clear();

    int maxLines = qMax(sourceLines.size(), destLines.size());

    QString sourceHtml;
    QString destHtml;

    for (int i = 0; i < maxLines; ++i) {
        QString srcLine = (i < sourceLines.size()) ? sourceLines[i] : QString();
        QString dstLine = (i < destLines.size()) ? destLines[i] : QString();

        // Escape HTML
        srcLine = srcLine.toHtmlEscaped();
        dstLine = dstLine.toHtmlEscaped();

        // Replace spaces with &nbsp; to preserve whitespace
        srcLine.replace(" ", "&nbsp;");
        dstLine.replace(" ", "&nbsp;");

        bool differs = (i >= sourceLines.size()) || (i >= destLines.size()) ||
                       (sourceLines[i] != destLines[i]);

        if (differs) {
            if (i >= sourceLines.size()) {
                // Line only in dest
                sourceHtml += QString("<div style='background-color: #ffcccc;'>&nbsp;</div>");
                destHtml += QString("<div style='background-color: #ccffcc;'>%1</div>").arg(dstLine);
            } else if (i >= destLines.size()) {
                // Line only in source
                sourceHtml += QString("<div style='background-color: #ccffcc;'>%1</div>").arg(srcLine);
                destHtml += QString("<div style='background-color: #ffcccc;'>&nbsp;</div>");
            } else {
                // Lines differ
                sourceHtml += QString("<div style='background-color: #ffffcc;'>%1</div>").arg(srcLine);
                destHtml += QString("<div style='background-color: #ffffcc;'>%1</div>").arg(dstLine);
            }
        } else {
            sourceHtml += QString("<div>%1</div>").arg(srcLine.isEmpty() ? "&nbsp;" : srcLine);
            destHtml += QString("<div>%1</div>").arg(dstLine.isEmpty() ? "&nbsp;" : dstLine);
        }
    }

    m_sourceEdit->appendHtml(sourceHtml);
    m_destEdit->appendHtml(destHtml);

    // Move cursor to beginning
    m_sourceEdit->moveCursor(QTextCursor::Start);
    m_destEdit->moveCursor(QTextCursor::Start);
}

void DlgFileDiff::setNavigationInfo(int currentIndex, int totalCount)
{
    m_navLabel->setText(tr("%1 of %2").arg(currentIndex + 1).arg(totalCount));
}

void DlgFileDiff::onOpenSourceInEditor()
{
    openFileInEditor(m_sourceFilePath);
}

void DlgFileDiff::onOpenDestInEditor()
{
    openFileInEditor(m_destFilePath);
}

void DlgFileDiff::onRefresh()
{
    loadAndCompare();
}

void DlgFileDiff::openFileInEditor(const QString &filePath)
{
    if (filePath.isEmpty() || !QFileInfo::exists(filePath))
        return;

    // Try Notepad++ first (common locations on Windows)
    QStringList notepadPlusPlusPaths = {
        "C:/Program Files/Notepad++/notepad++.exe",
        "C:/Program Files (x86)/Notepad++/notepad++.exe"
    };

    for (const QString &nppPath : notepadPlusPlusPaths) {
        if (QFileInfo::exists(nppPath)) {
            QProcess::startDetached(nppPath, {filePath});
            return;
        }
    }

    // Try notepad++ from PATH
    if (QProcess::startDetached("notepad++", {filePath}))
        return;

    // Fallback to system default
    QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
}
