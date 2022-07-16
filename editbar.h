#ifndef EDITBAR_H
#define EDITBAR_H

#include <QHBoxLayout>
#include <QPushButton>
#include <QWidget>

class EditBar : public QWidget {
    Q_OBJECT
public:
    explicit EditBar(QWidget* parent = nullptr);
    void initControl();
    void initConnection();
    void initShortcut();

private:
    QHBoxLayout* edit_layout;
    QPushButton* boldButton;
    QPushButton* italicButton;
    QPushButton* underlineButton;
    QPushButton* strikeButton;
    QPushButton* embedImageButton;

signals:
    void boldBtnClickedSignal(bool checked);
    void italicBtnClickedSignal(bool checked);
    void underlineBtnClickedSignal(bool checked);
    void strikeBtnClickedSignal(bool checked);
    void embedImageBtnClickedSignal(bool checked);
};

#endif // EDITBAR_H
