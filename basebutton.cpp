#include "basebutton.h"
#include <QGraphicsDropShadowEffect>
//#include "settings.h"
#include "imagelib.h"

//extern Settings* settings;

BaseButton::BaseButton(const QPixmap& pixmap, const QString& tooltip, QWidget* parent, bool checkable)
    : QPushButton(parent)
{

    setCheckable(checkable);
    setFixedSize(24, 24);
    setIcon(pixmap);
    setIconSize(QSize(width(), height()));

    setToolTip(tooltip);

    QString styleSheet = "BaseButton{border:none;background-color:transparent}";
    setStyleSheet(styleSheet);
}

BaseButton::BaseButton(const QString& pixmapUrl, const QString& tooltip, QWidget* parent, bool checkable)
    : BaseButton(QPixmap(pixmapUrl), tooltip, parent, checkable)
{
}

void BaseButton::enableShadowEffect()
{
    auto dsEffect = new QGraphicsDropShadowEffect(this);
    dsEffect->setBlurRadius(5);
    dsEffect->setOffset(0);
    dsEffect->setColor(QColor(Qt::black));
    setGraphicsEffect(dsEffect);
}

void BaseButton::enterEvent(QEvent* e)
{
    Q_UNUSED(e);
    __prevCursor = cursor();
    setCursor(Qt::ArrowCursor);
}

void BaseButton::leaveEvent(QEvent* e)
{
    Q_UNUSED(e);
    setCursor(__prevCursor);
}
