#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFocusEvent>
#include <QHBoxLayout>
#include <QMainWindow>
#include <QStatusBar>
#include <QString>
#include <QTextEdit>
#include <QVBoxLayout>

#include "editbar.h"
#include "styleoptionbar.h"
#include "textedit.h"
#include "titlebar.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

    QString getContentText();
    void setContentTest(QString);

private:
    TitleBar* titlebar;
    EditBar* editbar;
    StyleOptionBar* styleOptionBar;
    TextEdit* content;
    QStatusBar* statusBar;
    QVBoxLayout* content_layout;
    QWidget* parentW;
    QString style = "background: rgb(243,243,243);";
    QString lastImgPath_;

    void initTitlebar();
    void initEditbar();
    void setStyle(const QString& style);
    void mergeFormatOnWordOrSelection(const QTextCharFormat& format);
    void imageEmbed(const QString& path);
    QString selectImage();

private slots:
    void onNewBtnClicked();
    void onSettingBtnClicked();
    void onDeleteBtnClicked();
    void onCloseBtnClicked();
    void onStyleBtnClicked(QString btnName);
    void onBoldBtnClicked(bool checked);
    void onItalicBtnClicked(bool checked);
    void onUnderlineBtnClicked(bool checked);
    void onStrikeBtnClicked(bool checked);
    void onEmbedImageBtnClicked(bool checked);

protected:
    virtual void closeEvent(QCloseEvent* event) override;
    virtual void enterEvent(QEvent* event) override;
    virtual void leaveEvent(QEvent* event) override;
};

#endif // MAINWINDOW_H
