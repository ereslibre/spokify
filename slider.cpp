/*
 * This file is part of Spokify.
 * Copyright (C) 2010 Rafael Fernández López <ereslibre@kde.org>
 *
 * Spokify is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Spokify is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Spokify.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "slider.h"

#include <QtGui/QBrush>
#include <QtGui/QPainter>
#include <QtGui/QPaintEvent>

#include <KIconEffect>
#include <KStandardDirs>

Slider::Slider(QWidget *parent)
    : QWidget(parent)
    , m_leftBackground(KStandardDirs::locate("appdata", "images/slider_left.png"))
    , m_leftForeground(KStandardDirs::locate("appdata", "images/slider_body_left.png"))
    , m_rightBackground(KStandardDirs::locate("appdata", "images/slider_right.png"))
    , m_rightForeground(KStandardDirs::locate("appdata", "images/slider_body_right.png"))
    , m_bodyBackground(KStandardDirs::locate("appdata", "images/slider_body_background.png"))
    , m_bodyForeground(KStandardDirs::locate("appdata", "images/slider_body.png"))
    , m_slider(KStandardDirs::locate("appdata", "images/slider.png"))
    , m_minimum(0)
    , m_maximum(0)
    , m_value(0)
    , m_cacheValue(0)
{
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
    setMinimumSize(100, 24);

    m_leftBackground = m_leftBackground.scaledToHeight(24, Qt::SmoothTransformation);
    m_leftForeground = m_leftForeground.scaledToHeight(12, Qt::SmoothTransformation);
    m_rightBackground = m_rightBackground.scaledToHeight(24, Qt::SmoothTransformation);
    m_rightForeground = m_rightForeground.scaledToHeight(12, Qt::SmoothTransformation);
    m_bodyBackground = m_bodyBackground.scaledToHeight(24, Qt::SmoothTransformation);
    m_bodyForeground = m_bodyForeground.scaledToHeight(12, Qt::SmoothTransformation);

    m_slider = m_slider.scaledToHeight(20, Qt::SmoothTransformation);

    m_disabledSlider = m_slider;
    KIconEffect::semiTransparent(m_disabledSlider);
}

Slider::~Slider()
{
}

QSize Slider::sizeHint() const
{
    return QSize(1, 24);
}

void Slider::setRange(int minimum, int maximum)
{
    const double pos = ((double) m_value - (double) m_minimum) / ((double) m_maximum - (double) m_minimum + 1.0);
    const double pos2 = ((double) m_cacheValue - (double) m_minimum) / ((double) m_maximum - (double) m_minimum + 1.0);
    m_minimum = minimum;
    m_maximum = maximum;
    m_value = ((double) pos * ((double) m_maximum - (double) m_minimum)) + (double) m_minimum;
    m_cacheValue = ((double) pos2 * ((double) m_maximum - (double) m_minimum)) + (double) m_minimum;
    update();
}

int Slider::maximum() const
{
    return m_maximum;
}

int Slider::minimum() const
{
    return m_minimum;
}

void Slider::setValue(int value)
{
    if (value < m_minimum || value > m_maximum) {
        return;
    }
    m_value = value;
    update();
}

int Slider::value() const
{
    return m_value;
}

void Slider::setCacheValue(int value)
{
    if (value < m_minimum || value > m_maximum) {
        return;
    }
    m_cacheValue = value;
    update();
}

int Slider::cacheValue() const
{
    return m_cacheValue;
}

void Slider::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.setRenderHint(QPainter::SmoothPixmapTransform);

    //BEGIN: background painting
    {
        QRect backgroundRect(event->rect());
        backgroundRect.setLeft(backgroundRect.left() + m_leftBackground.width());
        backgroundRect.setRight(backgroundRect.right() - m_rightBackground.width());
        p.fillRect(backgroundRect, m_bodyBackground);

        p.drawPixmap(0, 0, m_leftBackground);
        p.drawPixmap(backgroundRect.right() + 1, 0, m_rightBackground);
    }
    //END: background painting

    //BEGIN: cache painting
    {
        p.save();
        p.translate(0, 6);
        QRect clipRect(event->rect());
        const double pos = ((double) m_cacheValue - (double) m_minimum) / ((double) m_maximum - (double) m_minimum + 1.0);
        clipRect.setWidth(clipRect.width() * pos);
        p.setClipRect(clipRect);
        QRect foregroundRect(event->rect());
        foregroundRect.setLeft(foregroundRect.left() + m_leftForeground.width() + 7);
        foregroundRect.setRight(foregroundRect.right() - m_rightForeground.width() - 7);
        foregroundRect.setHeight(m_leftForeground.height());
        p.fillRect(foregroundRect, m_bodyForeground);
        p.drawPixmap(7, 0, m_leftForeground);
        p.drawPixmap(foregroundRect.right() + 1, 0, m_rightForeground);
        p.restore();
    }
    //END: cache painting

    //BEGIN: slider element
    {
        const double pos = ((double) m_value - (double) m_minimum) / ((double) m_maximum - (double) m_minimum + 1.0);
        if (isEnabled()) {
            p.drawPixmap(pos * (event->rect().width() - m_slider.width() - 6) + 3, 2, m_slider);
        } else {
            p.drawPixmap(pos * (event->rect().width() - m_slider.width() - 6) + 3, 2, m_disabledSlider);
        }
    }
    //END: slider element

    //BEGIN: overlay
    {
        p.save();
        p.setOpacity(0.5);
        QRect overlayRect(event->rect());
        overlayRect.setTop(overlayRect.top() + 2);
        overlayRect.setLeft(overlayRect.left() + 3);
        overlayRect.setWidth(overlayRect.width() - 3);
        overlayRect.setHeight(overlayRect.height() / 2);
        p.fillRect(overlayRect, Qt::white);
        p.restore();
    }
    //END: overlay
}
