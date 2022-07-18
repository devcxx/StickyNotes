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
    boldButton = new BaseButton(":/Icons/bold.png", tr("Bold"), this, true);

    italicButton = new BaseButton(":/Icons/italic.png", tr("Italic"), this, true);

    underlineButton = new BaseButton(":/Icons/underline.png", tr("Underline"), this, true);

    strikeButton = new BaseButton(":/Icons/strikethrough.png", tr("Strike through"), this, true);

    edit_layout = new QHBoxLayout(this);
    //    edit_layout->setContentsMargins(10,0,10,0);
    edit_layout->addWidget(boldButton);
    edit_layout->addWidget(italicButton);
    edit_layout->addWidget(underlineButton);
    edit_layout->addWidget(strikeButton);

    embedImageButton = new BaseButton(":/Icons/image.png", tr("Embed image"), this, true);

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
    connect(embedImageButton, SIGNAL(toggled(bool)), this, SIGNAL(embedImageBtnClickedSignal(bool)));
}
