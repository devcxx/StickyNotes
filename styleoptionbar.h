#ifndef STYLEOPTIONS_H
#define STYLEOPTIONS_H

#include <QHBoxLayout>
#include <QPushButton>
#include <QString>
#include <QWidget>
class StyleOptionBar : public QWidget {
    Q_OBJECT
public:
    explicit StyleOptionBar(QWidget* parent = nullptr);
    ~StyleOptionBar();
    void init();

signals:
    void styleBtnClickedSignal(QString);

private:
    QPushButton* purpleStyle_btn;
    QPushButton* yellowStyle_btn;
    QPushButton* blueStyle_btn;
    QPushButton* greenStyle_btn;
    QPushButton* pinkStyle_btn;
    QPushButton* grayStyle_btn;
    QPushButton* charcoalStyle_btn;
    QHBoxLayout* styleOptions;
private slots:
    void onStyleBtnClicked();
};

#endif // STYLEOPTIONS_H
