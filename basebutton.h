#ifndef BASEBUTTON_H
#define BASEBUTTON_H

#include <QPushButton>

class BaseButton : public QPushButton {
    Q_OBJECT
public:
    BaseButton(const QPixmap& pixmap, const QString& tooltip = QStringLiteral(), QWidget* parent = nullptr, bool checkable = true);
    BaseButton(const QString& pixmapUrl, const QString& tooltip = QStringLiteral(), QWidget* parent = nullptr, bool checkable = true);
    void enterEvent(QEvent*);
    void leaveEvent(QEvent*);
    void enableShadowEffect();

private:
    QWidget* __parent;
    QCursor __prevCursor;
    QColor _color;
};

#endif
