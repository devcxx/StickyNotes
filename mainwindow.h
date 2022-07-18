#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "dbmanager.h"
#include "notemodel.h"
#include "noteview.h"
#include "stickywindow.h"
#include <QHash>
#include <QMainWindow>
#include <QPushButton>
#include <QSettings>
#include <QSortFilterProxyModel>
#include <QThread>

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = Q_NULLPTR);

private:
    void initUI();
    void setupDatabases();
    void setupModelView();
    void setupSignalsSlots();
    void initializeSettingsDatabase();
    void restoreStates();
    NoteData* generateNote(const int noteID);
    void showSticky(const QModelIndex& noteIndex);

private:
    QPushButton* m_createNewButton;
    NoteView* m_noteView;
    NoteModel* m_noteModel;
    NoteModel* m_deletedNotesModel;
    QSortFilterProxyModel* m_proxyModel;
    QModelIndex m_currentSelectedNoteProxy;
    int m_noteCounter;

    DBManager* m_dbManager;
    QThread* m_dbThread;
    QSettings* m_settingsDatabase;

    QHash<int, StickyWindow*> m_stickys;

private slots:
    void createNewNote();
    void InitData();
    void loadNotes(QList<NoteData*> noteList, int noteCounter);
    void onNotePressed(const QModelIndex& index);
    void selectNote(const QModelIndex& noteIndex);
    void saveNoteToDB(const QModelIndex& noteIndex);
    void saveNoteToDB(NoteData* noteData);
    void deleteNote(NoteData* noteData);

protected:
    void closeEvent(QCloseEvent* event) Q_DECL_OVERRIDE;

signals:
    void requestNotesList();
    void requestCreateUpdateNote(NoteData* note);
    void requestDeleteNote(NoteData* note);
    void requestOpenDBManager(QString path, bool doCreate);
};

#endif // MAINWINDOW_H
