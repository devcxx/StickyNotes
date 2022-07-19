#include "stickywindow.h"

#include "filedialog.h"
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QList>
#include <QScreen>
#include <QWidget>

StickyWindow::StickyWindow(NoteData* noteData, QWidget* parent)
    : QMainWindow(parent)
    , _noteData(noteData)
{
    this->parentW = parent;
    //    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    this->setCentralWidget(new QWidget());

    setMinimumSize(306, 313);

    initTitlebar();
    initEditbar();
    styleOptionBar = new StyleOptionBar(this);
    textEdit = new TextEdit(this);
    statusBar = new QStatusBar(this);

    //    this->setFocusProxy(this->textEdit);

    connect(styleOptionBar, SIGNAL(styleBtnClickedSignal(QString)), this, SLOT(onStyleBtnClicked(QString)));
    styleOptionBar->setVisible(false);

    //    textEdit->setFocusPolicy(Qt::StrongFocus);
    textEdit->setStyleSheet("background-color:transparent;"
                            "font-size:14px;"
                            "border:none;"
                            "font-family:'Microsoft Yahei'");

    connect(textEdit, &TextEdit::textChanged, this, &StickyWindow::onTextEditTextChanged);

    statusBar->setFixedHeight(6);
    statusBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    QVBoxLayout* main_layout = new QVBoxLayout(this);
    main_layout->addWidget(titlebar);
    main_layout->addWidget(styleOptionBar);

    content_layout = new QVBoxLayout(this);
    content_layout->addWidget(textEdit);
    content_layout->setContentsMargins(12, 0, 12, 4);
    main_layout->addLayout(content_layout);
    main_layout->addWidget(editbar);
    main_layout->addWidget(statusBar);

    main_layout->setContentsMargins(0, 0, 0, 0);
    centralWidget()->setLayout(main_layout);

    setNoteData(noteData);
}

StickyWindow::~StickyWindow()
{
    delete textEdit;
    delete styleOptionBar;
    delete titlebar;
}

void StickyWindow::setNoteData(NoteData* noteData)
{
    _noteData = noteData;
    if (_noteData != Q_NULLPTR) {
        textEdit->setHtml(_noteData->content());
        if (!_noteData->geometry().isEmpty()) {
            qDebug() << _noteData->geometry();
            restoreGeometry(_noteData->geometry());
        }
        if (!_noteData->color().isEmpty()) {
            QString style = QString("background:%1").arg(_noteData->color());
            this->style = style;
            this->setStyleSheet(style);
        }
    }
}

void StickyWindow::initTitlebar()
{
    titlebar = new TitleBar(this);
    titlebar->setBtnsVisible(false);
    connect(titlebar, SIGNAL(newBtnClickedSignal()), this, SLOT(onNewBtnClicked()));
    connect(titlebar, SIGNAL(settingBtnClickedSignal()), this, SLOT(onSettingBtnClicked()));
    connect(titlebar, SIGNAL(deleteBtnClickedSignal()), this, SLOT(onDeleteBtnClicked()));
    connect(titlebar, SIGNAL(closeBtnClickedSignal()), this, SLOT(onCloseBtnClicked()));
}

void StickyWindow::initEditbar()
{
    editbar = new EditBar(this);
    connect(editbar, SIGNAL(boldBtnClickedSignal(bool)), this, SLOT(onBoldBtnClicked(bool)));
    connect(editbar, SIGNAL(italicBtnClickedSignal(bool)), this, SLOT(onItalicBtnClicked(bool)));
    connect(editbar, SIGNAL(underlineBtnClickedSignal(bool)), this, SLOT(onUnderlineBtnClicked(bool)));
    connect(editbar, SIGNAL(strikeBtnClickedSignal(bool)), this, SLOT(onStrikeBtnClicked(bool)));
    connect(editbar, SIGNAL(listBtnClickedSignal(bool)), this, SLOT(onListBtnClicked(bool)));
    connect(editbar, SIGNAL(embedImageBtnClickedSignal(bool)), this, SLOT(onEmbedImageBtnClicked(bool)));
}

void StickyWindow::onNewBtnClicked()
{
    //    StickyWindow* newWindow = new StickyWindow(Q_NULLPTR, this->parentW);
    //    newWindow->setGeometry(this->pos().x() + 20,
    //        this->pos().y() + 20,
    //        this->size().width(),
    //        this->size().height());
    //    newWindow->setStyleSheet(this->style);
    //    newWindow->show();
    //    newWindow->setFocus();
    emit createNewNote();
}

void StickyWindow::onSettingBtnClicked()
{
    this->styleOptionBar->setVisible(!styleOptionBar->isVisible());
    this->titlebar->setVisible(false);
    this->editbar->setVisible(false);
    this->styleOptionBar->setFocus();
    //this->content_layout->setContentsMargins(12,0,12,4);
}

void StickyWindow::onDeleteBtnClicked()
{
    this->close();
    emit deleteNote(_noteData);
}

void StickyWindow::onCloseBtnClicked()
{
    //    this->close();

    hide();
}

void StickyWindow::onStyleBtnClicked(QString btnName)
{
    QString style = QString("background:%1;").arg(btnName);
    this->style = style;
    this->setStyleSheet(style);
    this->styleOptionBar->setVisible(false);
    this->editbar->setVisible(true);
    this->titlebar->setVisible(true);
    this->textEdit->setFocus();
    if (_noteData) {
        _noteData->setColor(btnName);
        emit saveNote(_noteData);
    }
}

void StickyWindow::mergeFormatOnWordOrSelection(const QTextCharFormat& format)
{
    QTextCursor cur = textEdit->textCursor();
    if (!cur.hasSelection()) { // apply to the word only if the cusor isn't at its end
        QTextCursor tmp = cur;
        tmp.movePosition(QTextCursor::EndOfWord);
        if (tmp.position() > cur.position())
            cur.select(QTextCursor::WordUnderCursor);
    }
    if (!cur.hasSelection())
        textEdit->mergeCurrentCharFormat(format); // alows to type with the new format
    else
        cur.mergeCharFormat(format);
    /* correct the pressed states of the format buttons if necessary */
    //    formatChanged (content->currentCharFormat());
}

void StickyWindow::imageEmbed(const QString& path)
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
    textEdit->insertHtml(QString("<img src=\"data:image;base64,%1\" width=\"%2\" height=\"%3\" />")
                             .arg(QString(base64array))
                             .arg(w)
                             .arg(h));

    raise();
}

QString StickyWindow::selectImage()
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

void StickyWindow::onBoldBtnClicked(bool checked)
{
    QTextCharFormat fmt;
    fmt.setFontWeight(checked ? QFont::Bold : QFont::Normal);
    mergeFormatOnWordOrSelection(fmt);
}

void StickyWindow::onItalicBtnClicked(bool checked)
{
    QTextCharFormat fmt;
    fmt.setFontItalic(checked);
    mergeFormatOnWordOrSelection(fmt);
}

void StickyWindow::onUnderlineBtnClicked(bool checked)
{
    QTextCharFormat fmt;
    fmt.setFontUnderline(checked);
    mergeFormatOnWordOrSelection(fmt);
}

void StickyWindow::onStrikeBtnClicked(bool checked)
{
    QTextCharFormat fmt;
    fmt.setFontStrikeOut(checked);
    mergeFormatOnWordOrSelection(fmt);
}

void StickyWindow::onListBtnClicked(bool checked)
{
    QTextCursor cur = textEdit->textCursor();
    if (checked) {
        QTextListFormat listFormat;
        listFormat.setStyle(QTextListFormat::ListDisc);
        cur.createList(listFormat);
    } else {
        cur.setBlockFormat(QTextBlockFormat());
        //    cur.movePosition(QTextCursor::NextBlock);
        //      cur.insertBlock(QTextBlockFormat());
        textEdit->setTextCursor(cur);
    }
}

void StickyWindow::onEmbedImageBtnClicked(bool checked)
{
    QString imagePath = selectImage();
    if (!imagePath.isEmpty()) {
        lastImgPath_ = imagePath;
        imageEmbed(imagePath);
    }
}

#define FIRST_LINE_MAX 80

QString StickyWindow::getFirstLine(const QString& str)
{
    if (str.simplified().isEmpty())
        return "New Note";

    QString text = str.trimmed();
    QTextStream ts(&text);
    return ts.readLine(FIRST_LINE_MAX);
}

void StickyWindow::onTextEditTextChanged()
{
    QString content = _noteData->content();
    if (textEdit->toHtml() != content) {
        QString firstline = getFirstLine(textEdit->toPlainText());
        _noteData->setFullTitle(firstline);
        _noteData->setContent(textEdit->toHtml());
        QDateTime dateTime = QDateTime::currentDateTime();
        _noteData->setLastModificationDateTime(dateTime);
        emit saveNote(_noteData);
    }
}

void StickyWindow::enterEvent(QEvent* event)
{
    this->titlebar->setBtnsVisible(true);
    this->editbar->setVisible(true);
    this->textEdit->setFocus();
    return QWidget::enterEvent(event);
}

void StickyWindow::leaveEvent(QEvent* event)
{
    this->titlebar->setBtnsVisible(false);
    this->editbar->setVisible(false);
    this->styleOptionBar->setVisible(false);
    this->textEdit->clearFocus();
    return QWidget::leaveEvent(event);
}

void StickyWindow::moveEvent(QMoveEvent* event)
{
    QByteArray geometry = saveGeometry();
    if (_noteData) {
        _noteData->setGeometry(geometry);
        emit saveNote(_noteData);
    }
}
