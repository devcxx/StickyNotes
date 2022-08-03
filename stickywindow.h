#ifndef STICKYWINDOW_H
#define STICKYWINDOW_H

#include <QFocusEvent>
#include <QHBoxLayout>
#include <QMainWindow>
#include <QStatusBar>
#include <QString>
#include <QTextEdit>
#include <QVBoxLayout>

#include "editbar.h"
#include "notedata.h"
#include "styleoptionbar.h"
#include "textedit.h"
#include "titlebar.h"

class StickyWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit StickyWindow(NoteData* noteData, QWidget* parent = 0);
    ~StickyWindow();
    void setNoteData(NoteData* noteData);
    void highlightSearch(const QString& searchString);

private:
    TitleBar* titlebar;
    EditBar* editbar;
    StyleOptionBar* styleOptionBar;
    TextEdit* textEdit;
    QStatusBar* statusBar;
    QVBoxLayout* content_layout;
    QWidget* parentW;
    QString style = "background: rgb(243,243,243);";
    QString lastImgPath_;
    NoteData* _noteData;

    void initTitlebar();
    void initEditbar();
    void setStyle(const QString& style);
    void mergeFormatOnWordOrSelection(const QTextCharFormat& format);
    void imageEmbed(const QString& path);
    QString selectImage();
    QString getFirstLine(const QString& str);

public slots:
    void show();

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
    void onListBtnClicked(bool checked);
    void onEmbedImageBtnClicked(bool checked);
    void onTextEditTextChanged();

signals:
    void saveNote(NoteData* note);
    void deleteNote(NoteData* note);
    void createNewNote();

protected:
    virtual void enterEvent(QEvent* event) override;
    virtual void leaveEvent(QEvent* event) override;
    virtual void moveEvent(QMoveEvent* event) override;
};

#endif // STICKYWINDOW_H
