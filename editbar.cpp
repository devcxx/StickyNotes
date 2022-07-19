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
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_L), listButton, SLOT(click()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_S), strikeButton, SLOT(click()));
}

void EditBar::initControl()
{
    boldButton = new BaseButton(":/Icons/bold.png", tr("Bold"), this, true);
    italicButton = new BaseButton(":/Icons/italic.png", tr("Italic"), this, true);
    underlineButton = new BaseButton(":/Icons/underline.png", tr("Underline"), this, true);
    strikeButton = new BaseButton(":/Icons/strikethrough.png", tr("Strike through"), this, true);
    listButton = new BaseButton(":/Icons/list.png", tr("List"), this, true);
    embedImageButton = new BaseButton(":/Icons/image.png", tr("Embed image"), this, true);

    edit_layout = new QHBoxLayout(this);
    edit_layout->addWidget(boldButton);
    edit_layout->addWidget(italicButton);
    edit_layout->addWidget(underlineButton);
    edit_layout->addWidget(strikeButton);
    edit_layout->addWidget(listButton);
    edit_layout->addWidget(embedImageButton);

    edit_layout->addStretch();
    setLayout(edit_layout);
}

void EditBar::initConnection()
{
    connect(boldButton, SIGNAL(toggled(bool)), this, SIGNAL(boldBtnClickedSignal(bool)));
    connect(italicButton, SIGNAL(toggled(bool)), this, SIGNAL(italicBtnClickedSignal(bool)));
    connect(underlineButton, SIGNAL(toggled(bool)), this, SIGNAL(underlineBtnClickedSignal(bool)));
    connect(strikeButton, SIGNAL(toggled(bool)), this, SIGNAL(strikeBtnClickedSignal(bool)));
    connect(listButton, SIGNAL(toggled(bool)), this, SIGNAL(listBtnClickedSignal(bool)));
    connect(embedImageButton, SIGNAL(toggled(bool)), this, SIGNAL(embedImageBtnClickedSignal(bool)));
}
