#include "notedata.h"
#include <QDataStream>

NoteData::NoteData(QObject* parent)
    : QObject(parent)
    , m_isModified(false)
    , m_isSelected(false)
    , m_scrollBarPosition(0)
    , m_visable(true)
{
}

int NoteData::id() const
{
    return m_id;
}

void NoteData::setId(const int& id)
{
    m_id = id;
}

QString NoteData::fullTitle() const
{
    return m_fullTitle;
}

void NoteData::setFullTitle(const QString& fullTitle)
{
    m_fullTitle = fullTitle;
}

QDateTime NoteData::lastModificationdateTime() const
{
    return m_lastModificationDateTime;
}

void NoteData::setLastModificationDateTime(const QDateTime& lastModificationdateTime)
{
    m_lastModificationDateTime = lastModificationdateTime;
}

QString NoteData::content() const
{
    return m_content;
}

void NoteData::setContent(const QString& content)
{
    m_content = content;
}

bool NoteData::isModified() const
{
    return m_isModified;
}

void NoteData::setModified(bool isModified)
{
    m_isModified = isModified;
}

bool NoteData::isSelected() const
{
    return m_isSelected;
}

void NoteData::setSelected(bool isSelected)
{
    m_isSelected = isSelected;
}

int NoteData::scrollBarPosition() const
{
    return m_scrollBarPosition;
}

void NoteData::setScrollBarPosition(int scrollBarPosition)
{
    m_scrollBarPosition = scrollBarPosition;
}

QDateTime NoteData::deletionDateTime() const
{
    return m_deletionDateTime;
}

void NoteData::setDeletionDateTime(const QDateTime& deletionDateTime)
{
    m_deletionDateTime = deletionDateTime;
}

QByteArray NoteData::geometry() const
{
    return m_geometry;
}

void NoteData::setGeometry(const QByteArray& geometry)
{
    m_geometry = geometry;
}

QString NoteData::color() const
{
    return m_color;
}

void NoteData::setColor(const QString& color)
{
    m_color = color;
}

bool NoteData::visable() const
{
    return m_visable;
}

void NoteData::setVisable(bool visable)
{
    m_visable = visable;
}

QDateTime NoteData::creationDateTime() const
{
    return m_creationDateTime;
}

void NoteData::setCreationDateTime(const QDateTime& creationDateTime)
{
    m_creationDateTime = creationDateTime;
}

QDataStream& operator<<(QDataStream& stream, const NoteData* noteData)
{
    return stream << noteData->id() << noteData->fullTitle() << noteData->creationDateTime()
                  << noteData->lastModificationdateTime() << noteData->content() << noteData->geometry()
                  << noteData->color() << (noteData->visable() ? 1 : 0);
}

QDataStream& operator>>(QDataStream& stream, NoteData*& noteData)
{
    noteData = new NoteData();
    int id;
    QString fullTitle;
    QDateTime lastModificationDateTime;
    QDateTime creationDateTime;
    QString content;
    QByteArray geometry;
    QString color;
    int visable;

    stream >> id >> fullTitle >> creationDateTime >> lastModificationDateTime >> content >> geometry >> color >> visable;
    noteData->setId(id);
    noteData->setFullTitle(fullTitle);
    noteData->setLastModificationDateTime(lastModificationDateTime);
    noteData->setCreationDateTime(creationDateTime);
    noteData->setContent(content);
    noteData->setGeometry(geometry);
    noteData->setColor(color);
    noteData->setVisable(visable == 1);
    return stream;
}
