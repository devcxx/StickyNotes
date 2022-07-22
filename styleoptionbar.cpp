#include "styleoptionbar.h"

StyleOptionBar::StyleOptionBar(QWidget* parent)
    : QWidget(parent)
{
    this->setFixedHeight(28);
    purpleStyle_btn = new QPushButton(this);
    yellowStyle_btn = new QPushButton(this);
    blueStyle_btn = new QPushButton(this);
    greenStyle_btn = new QPushButton(this);
    pinkStyle_btn = new QPushButton(this);
    grayStyle_btn = new QPushButton(this);
    charcoalStyle_btn = new QPushButton(this);

    purpleStyle_btn->setObjectName("#b07fe0");
    purpleStyle_btn->setToolTip(tr("Purple"));
    yellowStyle_btn->setObjectName("#e5b804");
    yellowStyle_btn->setToolTip(tr("Yellow"));
    blueStyle_btn->setObjectName("#59c0e7");
    blueStyle_btn->setToolTip(tr("Blue"));
    greenStyle_btn->setObjectName("#65ba5a");
    greenStyle_btn->setToolTip(tr("Green"));
    pinkStyle_btn->setObjectName("#ea86c2");
    pinkStyle_btn->setToolTip(tr("Pink"));
    grayStyle_btn->setObjectName("#989898");
    grayStyle_btn->setToolTip(tr("Gray"));
    charcoalStyle_btn->setObjectName("#444444");
    charcoalStyle_btn->setToolTip(tr("Charcoal"));

    QString style = QString("QPushButton{background: %1;width:30px;height:22px;border:1px dashed black;}"
                            "QPushButton:hover:pressed:{none}");

    purpleStyle_btn->setStyleSheet(style.arg("#b07fe0"));
    yellowStyle_btn->setStyleSheet(style.arg("#e5b804"));
    blueStyle_btn->setStyleSheet(style.arg("#59c0e7"));
    greenStyle_btn->setStyleSheet(style.arg("#65ba5a"));
    pinkStyle_btn->setStyleSheet(style.arg("#ea86c2"));
    grayStyle_btn->setStyleSheet(style.arg("#989898"));
    charcoalStyle_btn->setStyleSheet(style.arg("#444444"));

    connect(purpleStyle_btn, SIGNAL(clicked(bool)), this, SLOT(onStyleBtnClicked()));
    connect(yellowStyle_btn, SIGNAL(clicked(bool)), this, SLOT(onStyleBtnClicked()));
    connect(blueStyle_btn, SIGNAL(clicked(bool)), this, SLOT(onStyleBtnClicked()));
    connect(greenStyle_btn, SIGNAL(clicked(bool)), this, SLOT(onStyleBtnClicked()));
    connect(pinkStyle_btn, SIGNAL(clicked(bool)), this, SLOT(onStyleBtnClicked()));
    connect(grayStyle_btn, SIGNAL(clicked(bool)), this, SLOT(onStyleBtnClicked()));
    connect(charcoalStyle_btn, SIGNAL(clicked(bool)), this, SLOT(onStyleBtnClicked()));

    styleOptions = new QHBoxLayout(this);
    styleOptions->setContentsMargins(0, 6, 0, 0);
    styleOptions->addStretch();
    styleOptions->addWidget(purpleStyle_btn);
    styleOptions->addWidget(yellowStyle_btn);
    styleOptions->addWidget(blueStyle_btn);
    styleOptions->addWidget(greenStyle_btn);
    styleOptions->addWidget(pinkStyle_btn);
    styleOptions->addWidget(grayStyle_btn);
    styleOptions->addWidget(charcoalStyle_btn);
    styleOptions->addStretch();
}

StyleOptionBar::~StyleOptionBar()
{
    delete purpleStyle_btn;
    delete yellowStyle_btn;
    delete blueStyle_btn;
    delete greenStyle_btn;
    delete pinkStyle_btn;
    delete grayStyle_btn;
    delete charcoalStyle_btn;
    delete styleOptions;
}

void StyleOptionBar::onStyleBtnClicked()
{
    emit styleBtnClickedSignal(QObject::sender()->objectName());
}
