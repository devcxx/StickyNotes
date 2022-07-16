#include "mainwindow.h"

#include "filedialog.h"
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QList>
#include <QWidget>
#include <QXmlStreamWriter>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    this->parentW = parent;
    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    this->setCentralWidget(new QWidget());

    initTitlebar();
    initEditbar();
    styleOptionBar = new StyleOptionBar(this);
    content = new TextEdit(this);
    statusBar = new QStatusBar(this);

    this->setFocusProxy(this->content);

    connect(styleOptionBar, SIGNAL(styleBtnClickedSignal(QString)), this, SLOT(onStyleBtnClicked(QString)));
    styleOptionBar->setVisible(false);

    content->setFocusPolicy(Qt::StrongFocus);
    content->setStyleSheet("background-color:transparent;"
                           "font-size:14px;"
                           "border:none;"
                           "font-family:'Microsoft Yahei'");

    statusBar->setFixedHeight(6);
    statusBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    QVBoxLayout* main_layout = new QVBoxLayout(this);
    main_layout->addWidget(titlebar);
    main_layout->addWidget(styleOptionBar);

    content_layout = new QVBoxLayout(this);
    content_layout->addWidget(content);
    content_layout->setContentsMargins(12, 0, 12, 4);
    main_layout->addLayout(content_layout);
    main_layout->addWidget(editbar);
    main_layout->addWidget(statusBar);

    main_layout->setContentsMargins(0, 0, 0, 0);
    centralWidget()->setLayout(main_layout);
}

MainWindow::~MainWindow()
{
    delete content;
    delete styleOptionBar;
    delete titlebar;
}

QString MainWindow::getContentText()
{
    return content->toPlainText();
}

void MainWindow::setContentTest(QString text)
{
    this->content->append(text);
}

void MainWindow::initTitlebar()
{
    titlebar = new TitleBar(this);
    titlebar->setBtnsVisible(false);
    connect(titlebar, SIGNAL(newBtnClickedSignal()), this, SLOT(onNewBtnClicked()));
    connect(titlebar, SIGNAL(settingBtnClickedSignal()), this, SLOT(onSettingBtnClicked()));
    connect(titlebar, SIGNAL(deleteBtnClickedSignal()), this, SLOT(onDeleteBtnClicked()));
    connect(titlebar, SIGNAL(closeBtnClickedSignal()), this, SLOT(onCloseBtnClicked()));
}

void MainWindow::initEditbar()
{
    editbar = new EditBar(this);
    connect(editbar, SIGNAL(boldBtnClickedSignal(bool)), this, SLOT(onBoldBtnClicked(bool)));
    connect(editbar, SIGNAL(italicBtnClickedSignal(bool)), this, SLOT(onItalicBtnClicked(bool)));
    connect(editbar, SIGNAL(underlineBtnClickedSignal(bool)), this, SLOT(onUnderlineBtnClicked(bool)));
    connect(editbar, SIGNAL(strikeBtnClickedSignal(bool)), this, SLOT(onStrikeBtnClicked(bool)));
    connect(editbar, SIGNAL(embedImageBtnClickedSignal(bool)), this, SLOT(onEmbedImageBtnClicked(bool)));
}

void MainWindow::onNewBtnClicked()
{
    MainWindow* newWindow = new MainWindow(this->parentW);
    newWindow->setGeometry(this->pos().x() + 20,
        this->pos().y() + 20,
        this->size().width(),
        this->size().height());
    newWindow->setStyleSheet(this->style);
    newWindow->show();
    newWindow->setFocus();
}

void MainWindow::onSettingBtnClicked()
{
    this->styleOptionBar->setVisible(!styleOptionBar->isVisible());
    this->titlebar->setVisible(false);
    this->editbar->setVisible(false);
    this->styleOptionBar->setFocus();
    //this->content_layout->setContentsMargins(12,0,12,4);
}

void MainWindow::onDeleteBtnClicked()
{
    this->hide();
    bool isAllDeleted = true;
    QList<MainWindow*> windows = this->parentWidget()->findChildren<MainWindow*>();
    if (!windows.isEmpty()) {
        foreach (MainWindow* mainwindow, windows) {
            if (mainwindow->isVisible()) {
                isAllDeleted = false;
                break;
            }
        }
    }
    if (isAllDeleted)
        this->onCloseBtnClicked();
}

void MainWindow::onCloseBtnClicked()
{
    this->close();
}

void MainWindow::onStyleBtnClicked(QString btnName)
{
    this->style = btnName;
    this->setStyleSheet(btnName);
    this->styleOptionBar->setVisible(false);
    this->editbar->setVisible(true);
    this->titlebar->setVisible(true);
    this->content->setFocus();
}

void MainWindow::mergeFormatOnWordOrSelection(const QTextCharFormat& format)
{
    QTextCursor cur = content->textCursor();
    if (!cur.hasSelection()) { // apply to the word only if the cusor isn't at its end
        QTextCursor tmp = cur;
        tmp.movePosition(QTextCursor::EndOfWord);
        if (tmp.position() > cur.position())
            cur.select(QTextCursor::WordUnderCursor);
    }
    if (!cur.hasSelection())
        content->mergeCurrentCharFormat(format); // alows to type with the new format
    else
        cur.mergeCharFormat(format);
    /* correct the pressed states of the format buttons if necessary */
    //    formatChanged (content->currentCharFormat());
}

void MainWindow::imageEmbed(const QString& path)
{
    if (path.isEmpty())
        return;

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        return;
    /* read the data serialized from the file */
    QDataStream in(&file);
    QByteArray rawarray;
    QDataStream datastream(&rawarray, QIODevice::WriteOnly);
    char a;
    /* copy between the two data streams */
    while (in.readRawData(&a, 1) != 0)
        datastream.writeRawData(&a, 1);
    file.close();
    QByteArray base64array = rawarray.toBase64();

    QImage img = QImage(path);
    QSize imgSize = img.size();
    int w, h;
    //    if (QObject::sender() == ui->embedImageButton) {
    //        w = imgSize.width() * imgScale_ / 100;
    //        h = imgSize.height() * imgScale_ / 100;
    //    } else {
    w = imgSize.width();
    h = imgSize.height();
    //    }
    //    TextEdit *textEdit = qobject_cast<TextEdit*>(ui->stackedWidget->currentWidget());
    //QString ("<img src=\"data:image/png;base64,%1\">")
    content->insertHtml(QString("<img src=\"data:image;base64,%1\" width=\"%2\" height=\"%3\" />")
                            .arg(QString(base64array))
                            .arg(w)
                            .arg(h));

    raise();
}

QString MainWindow::selectImage()
{
    QString path;
    if (!lastImgPath_.isEmpty()) {
        if (QFile::exists(lastImgPath_))
            path = lastImgPath_;
        else {
            QDir dir = QFileInfo(lastImgPath_).absoluteDir();
            if (!dir.exists())
                dir = QDir::home();
            path = dir.path();
        }
    } else
        path = QDir::home().path();

    QString imagePath;
    FileDialog dialog(this);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    dialog.setWindowTitle(tr("Open Image..."));
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setNameFilter(tr("Image Files (*.svg *.png *.jpg *.jpeg *.bmp *.gif);;All Files (*)"));
    if (QFileInfo(path).isDir())
        dialog.setDirectory(path);
    else {
        dialog.setDirectory(path.section("/", 0, -2)); // workaround for KDE
        dialog.selectFile(path);
        dialog.autoScroll();
    }
    if (dialog.exec()) {
        QStringList files = dialog.selectedFiles();
        if (files.count() > 0)
            imagePath = files.at(0);
    }

    return imagePath;

    //    if (!imagePath.isEmpty())
    //        ImagePathEntry_->setText(imagePath);
}

void MainWindow::onBoldBtnClicked(bool checked)
{
    QTextCharFormat fmt;
    fmt.setFontWeight(checked ? QFont::Bold : QFont::Normal);
    mergeFormatOnWordOrSelection(fmt);
}

void MainWindow::onItalicBtnClicked(bool checked)
{
    QTextCharFormat fmt;
    fmt.setFontItalic(checked);
    mergeFormatOnWordOrSelection(fmt);
}

void MainWindow::onUnderlineBtnClicked(bool checked)
{
    QTextCharFormat fmt;
    fmt.setFontUnderline(checked);
    mergeFormatOnWordOrSelection(fmt);
}

void MainWindow::onStrikeBtnClicked(bool checked)
{
    QTextCharFormat fmt;
    fmt.setFontStrikeOut(checked);
    mergeFormatOnWordOrSelection(fmt);
}

void MainWindow::onEmbedImageBtnClicked(bool checked)
{
    QString imagePath = selectImage();
    if (!imagePath.isEmpty()) {
        lastImgPath_ = imagePath;
        imageEmbed(imagePath);
    }
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    QList<MainWindow*> windows = this->parentWidget()->findChildren<MainWindow*>();
    if (!windows.isEmpty()) {
        QFile file("record.xml");
        file.open(QIODevice::WriteOnly);
        QXmlStreamWriter xmlWriter(&file);
        xmlWriter.setAutoFormatting(true);
        xmlWriter.writeStartDocument();
        xmlWriter.writeStartElement("windows");
        foreach (MainWindow* mainwindow, windows) {
            if (mainwindow->isVisible()) {
                xmlWriter.writeStartElement("window");
                xmlWriter.writeTextElement("x", QString::number(mainwindow->pos().x()));
                xmlWriter.writeTextElement("y", QString::number(mainwindow->pos().y()));
                xmlWriter.writeTextElement("width", QString::number(mainwindow->size().width()));
                xmlWriter.writeTextElement("height", QString::number(mainwindow->size().height()));
                xmlWriter.writeTextElement("style", mainwindow->style);
                xmlWriter.writeTextElement("content", mainwindow->getContentText());
                xmlWriter.writeEndElement();
            }
        }
        xmlWriter.writeEndElement();
        xmlWriter.writeEndDocument();
        file.close();
    }
    return QWidget::closeEvent(event);
}

void MainWindow::enterEvent(QEvent* event)
{
    this->titlebar->setBtnsVisible(true);
    this->editbar->setVisible(true);
    this->content->setFocus();
    return QWidget::enterEvent(event);
}

void MainWindow::leaveEvent(QEvent* event)
{
    this->titlebar->setBtnsVisible(false);
    this->editbar->setVisible(false);
    this->styleOptionBar->setVisible(false);
    this->content->clearFocus();
    return QWidget::leaveEvent(event);
}
