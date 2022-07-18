#include "editbar.h"
#include <QAction>
#include <QHBoxLayout>
#include <QShortcut>
#include <QSpacerItem>

EditBar::EditBar(QWidget* parent)
    : QWidget(parent)
{
    initControl();
    initConnection();
    initShortcut();
}

void EditBar::initShortcut()
{
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_B), boldButton, SLOT(click()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_I), italicButton, SLOT(click()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_U), underlineButton, SLOT(click()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_S), strikeButton, SLOT(click()));
}

void EditBar::initControl()
{
    boldButton = new QPushButton(this);
    boldButton->setCheckable(true);
    boldButton->setToolTip(tr("Bold"));
    boldButton->setText("B");
    boldButton->setFont(QFont(QStringLiteral("Microsoft Yahei"), 16, QFont::Bold));
    QString ss = QStringLiteral("QPushButton { "
                                "  width: 16; "
                                "  height: 16;"
                                "  border: none; "
                                "  padding: 0px; "
                                "  color: black;"
                                "}");
    boldButton->setStyleSheet(ss);

    italicButton = new QPushButton(this);
    italicButton->setCheckable(true);
    italicButton->setToolTip(tr("Italic"));
    italicButton->setFont(QFont(QStringLiteral("Microsoft Yahei"), 16, QFont::Bold, true));
    italicButton->setText("I");
    italicButton->setStyleSheet(ss);

    underlineButton = new QPushButton(this);
    underlineButton->setCheckable(true);
    underlineButton->setToolTip(tr("Underline"));
    QFont underlineFont(QStringLiteral("Microsoft Yahei"), 16, QFont::Bold);
    underlineFont.setUnderline(true);
    underlineButton->setText("U");
    underlineButton->setFont(underlineFont);
    underlineButton->setStyleSheet(ss);

    strikeButton = new QPushButton(this);
    strikeButton->setCheckable(true);
    strikeButton->setToolTip(tr("Strike through"));
    strikeButton->setText("S");
    strikeButton->setStyleSheet(ss);

    edit_layout = new QHBoxLayout(this);
    //    edit_layout->setContentsMargins(10,0,10,0);
    edit_layout->addWidget(boldButton);
    edit_layout->addWidget(italicButton);
    edit_layout->addWidget(underlineButton);
    edit_layout->addWidget(strikeButton);

    embedImageButton = new QPushButton(this);
    embedImageButton->setCheckable(true);
    embedImageButton->setToolTip(tr("Embed image"));
    embedImageButton->setText("EI");
    embedImageButton->setStyleSheet(ss);
    edit_layout->addWidget(embedImageButton);

    edit_layout->addStretch();
    setLayout(edit_layout);
}

void EditBar::initConnection()
{
    connect(boldButton, SIGNAL(clicked(bool)), this, SIGNAL(boldBtnClickedSignal(bool)));
    connect(italicButton, SIGNAL(clicked(bool)), this, SIGNAL(italicBtnClickedSignal(bool)));
    connect(underlineButton, SIGNAL(clicked(bool)), this, SIGNAL(underlineBtnClickedSignal(bool)));
    connect(strikeButton, SIGNAL(clicked(bool)), this, SIGNAL(strikeBtnClickedSignal(bool)));
    connect(embedImageButton, SIGNAL(clicked(bool)), this, SIGNAL(embedImageBtnClickedSignal(bool)));
}
