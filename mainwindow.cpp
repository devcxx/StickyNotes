#include "mainwindow.h"
#include "noteview.h"
#include "notewidgetdelegate.h"
#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>
#include <QDir>
#include <QFileInfo>
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QPushButton>
#include <QShortcut>
#include <QSplitter>
#include <QTimer>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , m_noteModel(new NoteModel(this))
    , m_deletedNotesModel(new NoteModel(this))
    , m_proxyModel(new QSortFilterProxyModel(this))
    , m_noteCounter(0)
    , m_dbManager(Q_NULLPTR)
    , m_dbThread(Q_NULLPTR)
    , m_trayIcon(new QSystemTrayIcon(this))
    , m_trayIconMenu(new QMenu(this))
    , m_quitAction(new QAction(tr("&Quit"), this))
    , m_showAction(new QAction(tr("Sticky List"), this))
{
    initUI();
    setupDatabases();
    setupModelView();
    setupSearchEdit();
    setupSignalsSlots();
    setupShortcuts();
    setupTrayIcon();
    restoreStates();
    QTimer::singleShot(200, this, SLOT(InitData()));
}

void MainWindow::initUI()
{
    resize(320, 620);
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
    setSizePolicy(sizePolicy);
    setMinimumSize(QSize(320, 500));
    setStyleSheet("background:white;");
    setDocumentMode(false);
    setTabShape(QTabWidget::Rounded);
    QWidget* centralWidget = new QWidget(this);

    QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
    centralWidget->setSizePolicy(sizePolicy1);

    QVBoxLayout* verticalLayout_scrollArea = new QVBoxLayout();
    verticalLayout_scrollArea->setSpacing(0);
    verticalLayout_scrollArea->setObjectName(QString::fromUtf8("verticalLayout_scrollArea"));
    verticalLayout_scrollArea->setSizeConstraint(QLayout::SetNoConstraint);

    QSpacerItem* verticalSpacer_upSearchEdit = new QSpacerItem(20, 10, QSizePolicy::Minimum, QSizePolicy::Minimum);
    verticalLayout_scrollArea->addItem(verticalSpacer_upSearchEdit);

    QHBoxLayout* horizontalLayout_scrollArea_3 = new QHBoxLayout();
    horizontalLayout_scrollArea_3->setSpacing(0);
    horizontalLayout_scrollArea_3->setObjectName(QString::fromUtf8("horizontalLayout_scrollArea_3"));
    horizontalLayout_scrollArea_3->setContentsMargins(0, 0, -1, -1);
    QSpacerItem* horizontalSpacer_leftSearchEdit = new QSpacerItem(10, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);
    horizontalLayout_scrollArea_3->addItem(horizontalSpacer_leftSearchEdit);

    m_searchEdit = new QLineEdit(this);

    m_searchEdit->setObjectName(QString::fromUtf8("searchEdit"));
    QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Fixed);
    sizePolicy2.setHorizontalStretch(0);
    sizePolicy2.setVerticalStretch(0);
    sizePolicy2.setHeightForWidth(m_searchEdit->sizePolicy().hasHeightForWidth());
    m_searchEdit->setSizePolicy(sizePolicy2);
    m_searchEdit->setMinimumSize(QSize(0, 30));
    m_searchEdit->setMaximumSize(QSize(16777215, 22));
    m_searchEdit->setClearButtonEnabled(true);

    QFont font;
    font.setFamily(QString::fromUtf8("Arial"));
    font.setPointSize(10);
    font.setItalic(false);
    m_searchEdit->setFont(font);
    m_searchEdit->setFocusPolicy(Qt::StrongFocus);
    m_searchEdit->setAutoFillBackground(false);
    m_searchEdit->setStyleSheet(QString::fromUtf8(""));
    m_searchEdit->setFrame(true);
    m_searchEdit->setCursorMoveStyle(Qt::LogicalMoveStyle);
    m_searchEdit->setClearButtonEnabled(false);

    horizontalLayout_scrollArea_3->addWidget(m_searchEdit);

    m_newNoteButton = new QPushButton(this);
    m_newNoteButton->setFixedSize(24, 24);
    m_newNoteButton->setIcon(QIcon(":/Icons/add.png"));
    QString styleSheet = "QPushButton{border:none;background-color:transparent;padding-top:4px}";
    m_newNoteButton->setStyleSheet(styleSheet);
    m_newNoteButton->setIconSize(QSize(24, 24));
    m_newNoteButton->setToolTip(tr("Create New Note"));
    horizontalLayout_scrollArea_3->setSpacing(6);
    horizontalLayout_scrollArea_3->addWidget(m_newNoteButton);

    QSpacerItem* horizontalSpacer_rightSearchEdit = new QSpacerItem(10, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);
    horizontalLayout_scrollArea_3->addItem(horizontalSpacer_rightSearchEdit);
    verticalLayout_scrollArea->addLayout(horizontalLayout_scrollArea_3);
    QSpacerItem* verticalSpacer_upScrollArea = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Minimum);

    verticalLayout_scrollArea->addItem(verticalSpacer_upScrollArea);

    NoteView* listView = new NoteView(this);
    listView->setObjectName(QString::fromUtf8("listView"));
    QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Expanding);
    sizePolicy3.setHorizontalStretch(0);
    sizePolicy3.setVerticalStretch(0);
    sizePolicy3.setHeightForWidth(listView->sizePolicy().hasHeightForWidth());
    listView->setSizePolicy(sizePolicy3);
    listView->setMinimumSize(QSize(0, 0));
    listView->setBaseSize(QSize(20, 0));
    listView->setFrameShape(QFrame::NoFrame);
    listView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    listView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    listView->setProperty("showDropIndicator", QVariant(false));
    listView->setSelectionBehavior(QAbstractItemView::SelectRows);
    QScrollBar* scrollBar = new QScrollBar;
    scrollBar->setStyleSheet("QScrollBar:vertical {width: 6px;background: transparent;margin:0px 0px 0px 0px;}"
                             "QScrollBar::handle:vertical {width: 6px;min-height: 45px;background: #292929;margin-left: 0px;margin-right: 0px;}"
                             "QScrollBar::handle:vertical:hover {background: #3e3e3e;}"
                             "QScrollBar::handle:vertical:pressed {background: #272727;}"
                             "QScrollBar::sub-line:vertical {height: 6px;background: transparent;subcontrol-position: top;}"
                             "QScrollBar::add-line:vertical {height: 6px;background: transparent;subcontrol-position: bottom;}"
                             "QScrollBar::sub-line:vertical:hover {background: #292929;}"
                             "QScrollBar::add-line:vertical:hover {background: #292929;}"
                             "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {background: transparent;}");

    listView->setVerticalScrollBar(scrollBar);

    m_noteView = listView;

    verticalLayout_scrollArea->addWidget(listView);

    QHBoxLayout* horizontalLayout_version = new QHBoxLayout();
    QLabel* versionLabel = new QLabel(this);
    versionLabel->setText("v" + QCoreApplication::applicationVersion());
    versionLabel->setStyleSheet("color: gray;font-family: Microsoft Yahei;font-size: 12px;");
    horizontalLayout_version->addWidget(versionLabel);
    horizontalLayout_version->addStretch();
    verticalLayout_scrollArea->addLayout(horizontalLayout_version);

    centralWidget->setLayout(verticalLayout_scrollArea);
    setCentralWidget(centralWidget);
}

void MainWindow::setupDatabases()
{
    m_settingsDatabase = new QSettings(QSettings::IniFormat, QSettings::UserScope,
        QStringLiteral("StickyNotes"), QStringLiteral("Settings"), this);
    m_settingsDatabase->setFallbacksEnabled(false);
    initializeSettingsDatabase();

    bool doCreate = false;
    QFileInfo fi(m_settingsDatabase->fileName());
    QDir dir(fi.absolutePath());
    bool folderCreated = dir.mkpath(QStringLiteral("."));
    if (!folderCreated)
        qFatal("ERROR: Can't create settings folder : %s", dir.absolutePath().toStdString().c_str());

    QString noteDBFilePath(dir.path() + QDir::separator() + QStringLiteral("notes.db"));

    if (!QFile::exists(noteDBFilePath)) {
        QFile noteDBFile(noteDBFilePath);
        if (!noteDBFile.open(QIODevice::WriteOnly))
            qFatal("ERROR : Can't create database file");

        noteDBFile.close();
        doCreate = true;
    }

    m_dbManager = new DBManager;
    m_dbThread = new QThread;
    m_dbThread->setObjectName(QStringLiteral("dbThread"));
    m_dbManager->moveToThread(m_dbThread);
    connect(m_dbThread, &QThread::started, [=]() { emit requestOpenDBManager(noteDBFilePath, doCreate); });
    connect(this, &MainWindow::requestOpenDBManager, m_dbManager, &DBManager::onOpenDBManagerRequested);
    connect(m_dbThread, &QThread::finished, m_dbManager, &QObject::deleteLater);
    m_dbThread->start();
}

void MainWindow::setupModelView()
{
    m_proxyModel->setSourceModel(m_noteModel);
    m_proxyModel->setFilterKeyColumn(0);
    m_proxyModel->setFilterRole(NoteModel::NoteContent);
    m_proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);

    m_noteView->setItemDelegate(new NoteWidgetDelegate(m_noteView));

    m_noteView->setModel(m_proxyModel);
    m_noteView->setContextMenuPolicy(Qt::CustomContextMenu);
    qDebug() << connect(m_noteView, SIGNAL(customContextMenuRequested(const QPoint&)),
        this, SLOT(showContextMenu(const QPoint&)));
}

void MainWindow::setupSignalsSlots()
{
    connect(m_newNoteButton, &QPushButton::clicked, this, &MainWindow::onNewNoteButtonClicked);
    connect(this, &MainWindow::requestNotesList,
        m_dbManager, &DBManager::onNotesListRequested, Qt::BlockingQueuedConnection);
    connect(this, &MainWindow::requestCreateUpdateNote,
        m_dbManager, &DBManager::onCreateUpdateRequested, Qt::BlockingQueuedConnection);
    connect(this, &MainWindow::requestDeleteNote,
        m_dbManager, &DBManager::onDeleteNoteRequested);
    connect(m_dbManager, &DBManager::notesReceived, this, &MainWindow::loadNotes);
    connect(m_noteView, &NoteView::pressed, this, &MainWindow::onNotePressed);
    // line edit text changed
    connect(m_searchEdit, &QLineEdit::textChanged, this, &MainWindow::onSearchEditTextChanged);
    // line edit enter key pressed
    connect(m_searchEdit, &QLineEdit::returnPressed, this, &MainWindow::onSearchEditReturnPressed);
    // clear button
    connect(m_clearButton, &QToolButton::clicked, this, &MainWindow::onClearButtonClicked);

    // note pressed
    connect(m_noteView, &NoteView::pressed, this, &MainWindow::onNotePressed);
    // noteView viewport pressed
    connect(m_noteView, &NoteView::viewportPressed, this, [this]() {
        if (m_proxyModel->rowCount() > 1) {
            QModelIndex indexInProxy = m_proxyModel->index(1, 0);
            selectNote(indexInProxy);
        }
    });
    // note model rows moved
    connect(m_noteModel, &NoteModel::rowsAboutToBeMoved, m_noteView, &NoteView::rowsAboutToBeMoved);
    connect(m_noteModel, &NoteModel::rowsMoved, m_noteView, &NoteView::rowsMoved);
}

void MainWindow::setupShortcuts()
{
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_N), this, SLOT(createNewNote()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_D), this, SLOT(deleteSelectedNote()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_F), m_searchEdit, SLOT(setFocus()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_E), m_searchEdit, SLOT(clear()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Down), this, SLOT(selectNoteDown()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Up), this, SLOT(selectNoteUp()));
}

void MainWindow::setupTrayIcon()
{
    // Quit Action
    connect(m_quitAction, &QAction::triggered, qApp, &QApplication::quit);
    connect(m_showAction, &QAction::triggered, this, &MainWindow::show);
    m_trayIconMenu->setStyleSheet("QMenu{padding:5px;background:white;border:1px solid gray;}"
                  "QMenu::item{padding:0px 40px 0px 30px;height:25px;}"
                  "QMenu::item:selected:!enabled{background:transparent;}"
                  "QMenu::item:selected:enabled{background:lightgray;color:white;}");
    m_trayIconMenu->addAction(m_showAction);
    m_trayIconMenu->addAction(m_quitAction);

    QIcon icon(QStringLiteral(":/Icons/StickyNote.png"));
    m_trayIcon->setToolTip(QCoreApplication::applicationName());
    m_trayIcon->setIcon(icon);
    m_trayIcon->setContextMenu(m_trayIconMenu);
    connect(m_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(onTrayActivated(QSystemTrayIcon::ActivationReason)));

    m_trayIcon->show();
}

void MainWindow::initializeSettingsDatabase()
{
    if (m_settingsDatabase->value(QStringLiteral("windowGeometry"), "NULL") == "NULL") {
        int initWidth = 870;
        int initHeight = 630;
        QPoint center = qApp->desktop()->geometry().center();
        QRect rect(center.x() - initWidth / 2, center.y() - initHeight / 2, initWidth, initHeight);
        setGeometry(rect);
        m_settingsDatabase->setValue(QStringLiteral("windowGeometry"), saveGeometry());
    }
}

void MainWindow::restoreStates()
{
    if (m_settingsDatabase->value(QStringLiteral("windowGeometry"), "NULL") != "NULL")
        this->restoreGeometry(m_settingsDatabase->value(QStringLiteral("windowGeometry")).toByteArray());
}

NoteData* MainWindow::generateNote(const int noteID)
{
    NoteData* newNote = new NoteData(this);
    newNote->setId(noteID);

    QDateTime noteDate = QDateTime::currentDateTime();
    newNote->setCreationDateTime(noteDate);
    newNote->setLastModificationDateTime(noteDate);
    newNote->setFullTitle(tr("New Note"));

    return newNote;
}

void MainWindow::showSticky(const QModelIndex& noteIndex)
{
    if (!noteIndex.isValid())
        return;

    int noteID = noteIndex.data(NoteModel::NoteID).toInt();
    if (m_stickys.contains(noteID)) {
        StickyWindow* stickyWnd = m_stickys[noteID];
        stickyWnd->show();
        stickyWnd->raise();
    } else {
        NoteData* note = m_noteModel->getNote(noteIndex);
        StickyWindow* stickyWnd = new StickyWindow(note);
        connect(stickyWnd, SIGNAL(saveNote(NoteData*)), this, SLOT(saveNoteToDB(NoteData*)));
        connect(stickyWnd, SIGNAL(deleteNote(NoteData*)), this, SLOT(deleteNote(NoteData*)));
        connect(stickyWnd, SIGNAL(createNewNote()), this, SLOT(createNewNote()));
        m_stickys[noteID]
            = stickyWnd;
        stickyWnd->show();
        stickyWnd->raise();
    }

    //    QString content = noteIndex.data(NoteModel::NoteContent).toString();
    //    QDateTime dateTime = noteIndex.data(NoteModel::NoteLastModificationDateTime).toDateTime();
    //    int scrollbarPos = noteIndex.data(NoteModel::NoteScrollbarPos).toInt();
}

void MainWindow::setupSearchEdit()
{
    m_searchEdit->setAttribute(Qt::WA_MacShowFocusRect, 0);

    m_searchEdit->setStyleSheet("QLineEdit{ "
                                "  padding-left: 10px;"
                                "  padding-right: 20px;"
                                "  border: 1px solid rgb(205, 205, 205);"
                                "  border-radius: 3px;"
                                "  selection-background-color: rgb(61, 155, 218);"
                                "} "
                                "QLineEdit:focus { "
                                "  border: 2px solid rgb(61, 155, 218);"
                                "}"
                                "QToolButton { "
                                "  border: none; "
                                "  padding: 0px;"
                                "}");

    m_searchEdit->setPlaceholderText(tr("Search"));
    // clear button
    m_clearButton = new QToolButton(m_searchEdit);
    QPixmap pixmap(QStringLiteral(":Icons/closeButton.png"));
    m_clearButton->setIcon(QIcon(pixmap));
    QSize clearSize(15, 15);
    m_clearButton->setIconSize(clearSize);
    m_clearButton->setCursor(Qt::ArrowCursor);
    m_clearButton->hide();

    // search button
    //    QToolButton* searchButton = new QToolButton(m_searchEdit);
    //    QPixmap newPixmap(QStringLiteral(":Icons/magnifyingGlass.png"));
    //    searchButton->setIcon(QIcon(newPixmap));
    //    QSize searchSize(24, 25);
    //    searchButton->setIconSize(searchSize);
    //    searchButton->setCursor(Qt::ArrowCursor);

    // layout
    QBoxLayout* layout = new QBoxLayout(QBoxLayout::RightToLeft, m_searchEdit);
    layout->setContentsMargins(0, 0, 3, 0);
    layout->addWidget(m_clearButton);
    layout->addStretch();
    //    layout->addWidget(searchButton);
    m_searchEdit->setLayout(layout);

    m_searchEdit->installEventFilter(this);
}

void MainWindow::findNotesContain(const QString& keyword)
{
    m_proxyModel->setFilterFixedString(keyword);
    m_clearButton->show();
    if (m_proxyModel->rowCount() > 0) {
        selectFirstNote();
    } else {
        m_currentSelectedNoteProxy = QModelIndex();
    }
}

void MainWindow::selectFirstNote()
{
    if (m_proxyModel->rowCount() > 0) {
        QModelIndex index = m_proxyModel->index(0, 0);
        m_noteView->selectionModel()->select(index, QItemSelectionModel::ClearAndSelect);
        m_noteView->setCurrentIndex(index);

        m_currentSelectedNoteProxy = index;
        showSticky(index);
    }
}

void MainWindow::highlightSearch() const
{
    QString searchString = m_searchEdit->text();
    foreach (StickyWindow* stickyWnd, m_stickys) {
        stickyWnd->highlightSearch(searchString);
    }
}

void MainWindow::saveNoteToDB(const QModelIndex& noteIndex)
{
    if (noteIndex.isValid()) {
        QModelIndex indexInSrc = m_proxyModel->mapToSource(noteIndex);
        NoteData* note = m_noteModel->getNote(indexInSrc);
        if (note != Q_NULLPTR)
            emit requestCreateUpdateNote(note);
    }
}

void MainWindow::saveNoteToDB(NoteData* note)
{
    if (note != Q_NULLPTR)
        emit requestCreateUpdateNote(note);
}

void MainWindow::deleteNote(NoteData* noteData)
{
    QModelIndex indexToBeRemoved = m_noteModel->getNoteIndex(noteData);
    if (indexToBeRemoved.isValid()) {
        m_noteModel->removeNote(indexToBeRemoved);
        emit requestDeleteNote(noteData);
    }
}

void MainWindow::deleteNote(const QModelIndex& noteIndex, bool isFromUser)
{
    int noteID = noteIndex.data(NoteModel::NoteID).toInt();
    if (m_stickys.contains(noteID)) {
        StickyWindow* stickyWnd = m_stickys[noteID];
        stickyWnd->close();
        delete stickyWnd;
        m_stickys.remove(noteID);
    }

    if (noteIndex.isValid()) {
        // delete from model
        QModelIndex indexToBeRemoved = m_proxyModel->mapToSource(m_currentSelectedNoteProxy);
        NoteData* noteTobeRemoved = m_noteModel->removeNote(indexToBeRemoved);

        noteTobeRemoved->setDeletionDateTime(QDateTime::currentDateTime());
        emit requestDeleteNote(noteTobeRemoved);

        if (isFromUser) {

            if (m_noteModel->rowCount() > 0) {
                QModelIndex index = m_noteView->currentIndex();
                m_currentSelectedNoteProxy = index;
            } else {
                m_currentSelectedNoteProxy = QModelIndex();
            }
        }
    } else {
        qDebug() << "MainWindow::deleteNote noteIndex is not valid";
    }

    m_noteView->setFocus();
}

void MainWindow::onSearchEditTextChanged(const QString& keyword)
{
    m_searchQueue.enqueue(keyword);
    // disable animation while searching
    m_noteView->setAnimationEnabled(false);
    while (!m_searchQueue.isEmpty()) {
        qApp->processEvents();
        QString str = m_searchQueue.dequeue();
        if (str.isEmpty()) {
            clearSearch();
            QModelIndex indexInProxy = m_proxyModel->mapFromSource(m_selectedNoteBeforeSearchingInSource);
            selectNote(indexInProxy);
            m_selectedNoteBeforeSearchingInSource = QModelIndex();
        } else {
            m_noteView->setFocusPolicy(Qt::NoFocus);
            findNotesContain(str);
        }
    }
    highlightSearch();
}

void MainWindow::onSearchEditReturnPressed()
{
    if (m_searchEdit->text().isEmpty())
        return;
}

void MainWindow::onClearButtonClicked()
{
    clearSearch();

    if (m_noteModel->rowCount() > 0) {
        QModelIndex indexInProxy = m_proxyModel->mapFromSource(m_selectedNoteBeforeSearchingInSource);
        int row = m_selectedNoteBeforeSearchingInSource.row();
        if (row == m_noteModel->rowCount())
            indexInProxy = m_proxyModel->index(m_proxyModel->rowCount() - 1, 0);

        selectNote(indexInProxy);
    } else {
        m_currentSelectedNoteProxy = QModelIndex();
    }

    m_selectedNoteBeforeSearchingInSource = QModelIndex();
}

void MainWindow::clearSearch()
{
    m_searchEdit->blockSignals(true);
    m_searchEdit->clear();
    m_searchEdit->blockSignals(false);
    m_proxyModel->setFilterFixedString(QString());
    highlightSearch();
    m_clearButton->hide();
    m_searchEdit->setFocus();
}

void MainWindow::selectNoteDown()
{
    if (m_currentSelectedNoteProxy.isValid()) {
        int currentRow = m_noteView->currentIndex().row();
        QModelIndex belowIndex = m_noteView->model()->index(currentRow + 1, 0);
        if (belowIndex.isValid()) {
            m_noteView->setCurrentIndex(belowIndex);
            m_noteView->setCurrentRowActive(false);
            m_currentSelectedNoteProxy = belowIndex;
            showSticky(m_currentSelectedNoteProxy);
        }
        //if the searchEdit is not empty, set the focus to it
        if (!m_searchEdit->text().isEmpty()) {
            m_searchEdit->setFocus();
        } else {
            m_noteView->setFocus();
        }
    }
}

void MainWindow::selectNoteUp()
{
    if (m_currentSelectedNoteProxy.isValid()) {
        int currentRow = m_noteView->currentIndex().row();
        QModelIndex aboveIndex = m_noteView->model()->index(currentRow - 1, 0);
        if (aboveIndex.isValid()) {
            m_noteView->setCurrentIndex(aboveIndex);
            m_noteView->setCurrentRowActive(false);
            m_currentSelectedNoteProxy = aboveIndex;
            showSticky(m_currentSelectedNoteProxy);
        }
        if (!m_searchEdit->text().isEmpty()) {
            m_searchEdit->setFocus();
        } else {
            m_noteView->setFocus();
        }
    }
}

void MainWindow::deleteSelectedNote()
{
    if (m_currentSelectedNoteProxy.isValid()) {

        // update the index of the selected note before searching
        if (!m_searchEdit->text().isEmpty()) {
            QModelIndex currentIndexInSource = m_proxyModel->mapToSource(m_currentSelectedNoteProxy);
            int beforeSearchSelectedRow = m_selectedNoteBeforeSearchingInSource.row();
            if (currentIndexInSource.row() < beforeSearchSelectedRow) {
                m_selectedNoteBeforeSearchingInSource = m_noteModel->index(beforeSearchSelectedRow - 1);
            }
        }

        deleteNote(m_currentSelectedNoteProxy, true);
        showSticky(m_currentSelectedNoteProxy);
    }
}

void MainWindow::onNewNoteButtonClicked()
{
    if (!m_searchEdit->text().isEmpty()) {
        clearSearch();
        m_selectedNoteBeforeSearchingInSource = QModelIndex();
    }

    this->createNewNote();
}

void MainWindow::showContextMenu(const QPoint&)
{
    QMenu* cmenu = new QMenu(m_noteView);

    QAction* showAction = cmenu->addAction(tr("Show Note"));
    QAction* deleteAction = cmenu->addAction(tr("Delete Note"));
    connect(showAction, SIGNAL(triggered(bool)), this, SLOT(showSelectedSticky()));
    connect(deleteAction, SIGNAL(triggered(bool)), this, SLOT(deleteSelectedSticky()));
    cmenu->exec(QCursor::pos());
}

void MainWindow::showSelectedSticky()
{
    QModelIndexList selectedIndexes = m_noteView->selectionModel()->selectedIndexes();
    QModelIndex index;
    foreach (index, selectedIndexes) {
        showSticky(index);
    }
}

void MainWindow::deleteSelectedSticky()
{
    QModelIndexList selectedIndexes = m_noteView->selectionModel()->selectedIndexes();
    QModelIndex index;
    foreach (index, selectedIndexes) {
        deleteNote(index);
    }
}

void MainWindow::createNewNote()
{
    ++m_noteCounter;
    NoteData* tmpNote = generateNote(m_noteCounter);
    // insert the new note to NoteModel
    QModelIndex indexSrc = m_noteModel->insertNote(tmpNote, 0);
    // update the current selected index
    m_currentSelectedNoteProxy = m_proxyModel->mapFromSource(indexSrc);
    m_noteView->setCurrentIndex(m_currentSelectedNoteProxy);
}

void MainWindow::InitData()
{
    emit requestNotesList();
    /// Check if it is running with an argument (ex. hide)
    if (qApp->arguments().contains(QStringLiteral("--autostart"))) {
        //        setMainWindowVisibility(false);
    }
}

void MainWindow::loadNotes(QList<NoteData*> noteList, int noteCounter)
{
    if (!noteList.isEmpty()) {
        m_noteModel->addListNote(noteList);
        m_noteModel->sort(0, Qt::AscendingOrder);
    }
    m_noteCounter = noteCounter;

    foreach (NoteData* note, noteList) {
        if (note->visable()) {
            int noteID = note->id();
            if (m_stickys.contains(noteID)) {
                StickyWindow* stickyWnd = m_stickys[noteID];
                stickyWnd->show();
                stickyWnd->raise();
            } else {
                StickyWindow* stickyWnd = new StickyWindow(note);
                connect(stickyWnd, SIGNAL(saveNote(NoteData*)), this, SLOT(saveNoteToDB(NoteData*)));
                connect(stickyWnd, SIGNAL(deleteNote(NoteData*)), this, SLOT(deleteNote(NoteData*)));
                connect(stickyWnd, SIGNAL(createNewNote()), this, SLOT(createNewNote()));
                m_stickys[noteID]
                    = stickyWnd;
                stickyWnd->show();
                stickyWnd->raise();
            }
        }
    }
}

void MainWindow::onNotePressed(const QModelIndex& index)
{
    if (sender() != Q_NULLPTR) {
        QModelIndex indexInProxy = m_proxyModel->index(index.row(), 0);
        selectNote(indexInProxy);
        m_noteView->setCurrentRowActive(false);
    }
}

void MainWindow::selectNote(const QModelIndex& noteIndex)
{
    // show the content of the pressed note in the text editor
    showSticky(noteIndex);
    m_currentSelectedNoteProxy = noteIndex;
    m_noteView->selectionModel()->select(m_currentSelectedNoteProxy, QItemSelectionModel::ClearAndSelect);
    m_noteView->setCurrentIndex(m_currentSelectedNoteProxy);
    m_noteView->scrollTo(m_currentSelectedNoteProxy);
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    if (windowState() != Qt::WindowFullScreen) {
        m_settingsDatabase->setValue(QStringLiteral("windowGeometry"), saveGeometry());
    }

    m_settingsDatabase->sync();
    QMainWindow::closeEvent(event);
}

void MainWindow::onTrayActivated(QSystemTrayIcon::ActivationReason reason)
{

    switch (reason) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        show();
        raise();
        break;
    default:
        break;
    }
}
