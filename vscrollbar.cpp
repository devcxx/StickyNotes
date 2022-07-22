/*
 * Copyright (C) Pedram Pourang (aka Tsu Jan) 2016-2021 <tsujan2000@gmail.com>
 *
 * FeatherNotes is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * FeatherNotes is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "vscrollbar.h"

VScrollBar::VScrollBar(QWidget* parent)
    : QScrollBar(parent)
{
    setStyleSheet("QScrollBar:vertical {width: 6px;background: transparent;margin:0px 0px 0px 0px;}"
                  "QScrollBar::handle:vertical {width: 6px;min-height: 45px;background: #292929;margin-left: 0px;margin-right: 0px;}"
                  "QScrollBar::handle:vertical:hover {background: #3e3e3e;}"
                  "QScrollBar::handle:vertical:pressed {background: #272727;}"
                  "QScrollBar::sub-line:vertical {height: 6px;background: transparent;subcontrol-position: top;}"
                  "QScrollBar::add-line:vertical {height: 6px;background: transparent;subcontrol-position: bottom;}"
                  "QScrollBar::sub-line:vertical:hover {background: #292929;}"
                  "QScrollBar::add-line:vertical:hover {background: #292929;}"
                  "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {background: transparent;}");
}
/*************************/
void VScrollBar::wheelEvent(QWheelEvent* event)
{
    if (!underMouse()
        || !event->spontaneous()
        || event->source() != Qt::MouseEventNotSynthesized
        /* Apparently, Qt's hover bug is never going to be fixed! */
        || !rect().contains(mapFromGlobal(QCursor::pos()))) {
        QScrollBar::wheelEvent(event);
        return;
    }
    QPoint anglePoint = event->angleDelta();
    int delta = qAbs(anglePoint.x()) > qAbs(anglePoint.y()) ? anglePoint.x()
                                                            : anglePoint.y();

    /* wait until the angle delta reaches that of an ordinary mouse wheel */
    static int _effectiveDelta = 0;
    _effectiveDelta += delta;
    if (qAbs(_effectiveDelta) < 120)
        return;

    int step = (_effectiveDelta < 0 ? 1 : -1) * qMax(pageStep() / ((event->modifiers() & Qt::ShiftModifier) ? 2 : 1), 1);
    _effectiveDelta = 0;
    setSliderPosition(sliderPosition() + step);
}
