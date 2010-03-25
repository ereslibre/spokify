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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <KXmlGuiWindow>

#include <QtCore/QModelIndex>

#include <alsa/asoundlib.h>

#include "api_key.h"
#include <spotify/api.h>

class QLabel;
class QBuffer;
class QListView;
class QProgressBar;

class KAction;
class KComboBox;
class KLineEdit;
class KStatusNotifierItem;

class MainWidget;
class PlaylistModel;

class MainWindow
    : public KXmlGuiWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

    sp_session *session() const;

    static MainWindow *self();
    MainWidget *mainWidget() const;
    QListView *playlistView() const;

    void spotifyLoggedIn();
    void spotifyLoggedOut();

    void showTemporaryMessage(const QString &message);
    void showRequest(const QString &request);

    snd_pcm_t *pcmHandle() const;

public Q_SLOTS:
    void restoreStatusBarSlot();

protected:
    virtual bool event(QEvent *event);

private Q_SLOTS:
    void loginSlot();
    void logoutSlot();
    void previousSlot();
    void playSlot();
    void pauseSlot();
    void nextSlot();
    void shuffleSlot();
    void repeatSlot();
    void performSearch();
    void playListChanged(const QModelIndex &index);
    void trackRequested(const QModelIndex &index);

private:
    void initSound();
    QWidget *createSearchWidget();
    void setupActions();
    void clearAllWidgets();
    void fillPlaylistModel();

private:
    snd_pcm_t            *m_snd;

    sp_session_config     m_config;
    sp_session           *m_session;
    sp_playlistcontainer *m_pc;
    sp_search            *m_search;

    KAction              *m_login;
    KAction              *m_logout;
    KAction              *m_previous;
    KAction              *m_play;
    KAction              *m_pause;
    KAction              *m_next;
    KAction              *m_shuffle;
    KAction              *m_repeat;
    QLabel               *m_statusLabel;
    QProgressBar         *m_progress;
    KStatusNotifierItem  *m_notifierItem;
    bool                  m_loggedIn;
    static MainWindow    *s_self;

    KComboBox            *m_searchCategory;
    KLineEdit            *m_searchField;

    MainWidget           *m_mainWidget;
    PlaylistModel        *m_playlistModel;
    QListView            *m_playlistView;
};
 
#endif
