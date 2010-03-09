/* Copyright (C) 2005-2010, Thorvald Natvig <thorvald@natvig.com>
   Copyright (C) 2009, Stefan Hacker <dd0t@users.sourceforge.net>

   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.
   - Neither the name of the Mumble Developers nor the names of its
     contributors may be used to endorse or promote products derived from this
     software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef _MAINWINDOW_H
#define _MAINWINDOW_H

#include "mumble_pch.hpp"

#define TI_QEVENT (QEvent::User + 938)
#define MB_QEVENT (QEvent::User + 939)
#define OU_QEVENT (QEvent::User + 940)

class ACLEditor;
class BanEditor;
class UserEdit;
class ServerHandler;
class GlobalShortcut;
class TextToSpeech;
class UserModel;
class Tokens;
class Channel;
class UserInformation;

struct ShortcutTarget;

#include "Message.h"
#include "Usage.h"
#include "ui_MainWindow.h"
#include "CustomElements.h"

class MessageBoxEvent : public QEvent {
	public:
		QString msg;
		MessageBoxEvent(QString msg);
};

class OpenURLEvent : public QEvent {
	public:
		QUrl url;
		OpenURLEvent(QUrl url);
};

class MainWindow : public QMainWindow, public MessageHandler, public Ui::MainWindow {
		friend class UserModel;
	private:
		Q_OBJECT
		Q_DISABLE_COPY(MainWindow)
	public:
		UserModel *pmModel;
		QSystemTrayIcon *qstiIcon;
		QMenu *qmUser;
		QMenu *qmChannel;
		QMenu *qmTray;
		QIcon qiIcon, qiIconMuteSelf, qiIconMuteServer, qiIconDeafSelf, qiIconDeafServer, qiIconMuteSuppressed;
		QIcon qiTalkingOn, qiTalkingWhisper, qiTalkingShout, qiTalkingOff;

		GlobalShortcut *gsPushTalk, *gsResetAudio, *gsMuteSelf, *gsDeafSelf;
		GlobalShortcut *gsUnlink, *gsPushMute, *gsMetaChannel, *gsToggleOverlay;
		GlobalShortcut *gsMinimal, *gsVolumeUp, *gsVolumeDown, *gsWhisper;
		DockTitleBar *dtbLogDockTitle, *dtbChatDockTitle;

		ACLEditor *aclEdit;
		BanEditor *banEdit;
		UserEdit *userEdit;
		Tokens *tokenEdit;

		MumbleProto::Reject_RejectType rtLast;
		QString qsDesiredChannel;

		bool bSuppressAskOnQuit;
		bool bAutoUnmute;

		unsigned int uiContextSession;
		int iContextChannel;

		void recheckTTS();
		void msgBox(QString msg);
		void setOnTop(bool top);
		void updateTrayIcon();
		QPair<QByteArray, QImage> openImageFile();
		void sendChatbarMessage();
		static const QString defaultStyleSheet;

#ifdef Q_OS_WIN
		bool winEvent(MSG *, long *);
		unsigned int uiNewHardware;
#endif
	protected:
		Usage uUsage;
		QTimer *qtReconnect;

		QList<QAction *> qlServerActions;
		QList<QAction *> qlChannelActions;
		QList<QAction *> qlUserActions;

		QSet<ShortcutTarget> qsCurrentTargets;
		QHash<QList<ShortcutTarget>, int> qmTargets;
		QMap<int, int> qmTargetUse;
		Channel *mapChannel(int idx) const;
		int iTargetCounter;
		QMap<unsigned int, UserInformation *> qmUserInformations;

		void createActions();
		void setupGui();
		void customEvent(QEvent *evt);
		void findDesiredChannel();
		void setupView(bool toggle_minimize = true);
		bool launchCompatibilityClient(const QUrl &url);
		virtual void closeEvent(QCloseEvent *e);
		virtual void hideEvent(QHideEvent *e);

		bool handleSpecialContextMenu(const QUrl &url, const QPoint &_pos, bool focus = false);
		Channel* getContextMenuChannel();
		ClientUser* getContextMenuUser();

	public slots:
		void on_qmServer_aboutToShow();
		void on_qaServerConnect_triggered(bool autoconnect = false);
		void on_qaServerDisconnect_triggered();
		void on_qaServerBanList_triggered();
		void on_qaServerUserList_triggered();
		void on_qaServerInformation_triggered();
		void on_qaServerTexture_triggered();
		void on_qaServerTextureRemove_triggered();
		void on_qaServerTokens_triggered();
		void on_qmSelf_aboutToShow();
		void on_qaSelfComment_triggered();
		void on_qaSelfRegister_triggered();
		void qmUser_aboutToShow();
		void on_qaUserCommentReset_triggered();
		void on_qaUserCommentView_triggered();
		void on_qaUserKick_triggered();
		void on_qaUserBan_triggered();
		void on_qaUserMute_triggered();
		void on_qaUserDeaf_triggered();
		void on_qaUserLocalMute_triggered();
		void on_qaUserTextMessage_triggered();
		void on_qaUserRegister_triggered();
		void on_qaUserInformation_triggered();
		void on_qaUserFriendAdd_triggered();
		void on_qaUserFriendRemove_triggered();
		void on_qaUserFriendUpdate_triggered();
		void qmChannel_aboutToShow();
		void on_qaChannelJoin_triggered();
		void on_qaChannelAdd_triggered();
		void on_qaChannelRemove_triggered();
		void on_qaChannelACL_triggered();
		void on_qaChannelLink_triggered();
		void on_qaChannelUnlink_triggered();
		void on_qaChannelUnlinkAll_triggered();
		void on_qaChannelSendMessage_triggered();
		void on_qaAudioReset_triggered();
		void on_qaAudioMute_triggered();
		void on_qaAudioDeaf_triggered();
		void on_qaAudioTTS_triggered();
		void on_qaAudioUnlink_triggered();
		void on_qaAudioStats_triggered();
		void on_qaConfigDialog_triggered();
		void on_qaConfigHideFrame_triggered();
		void on_qmConfig_aboutToShow();
		void on_qaConfigMinimal_triggered();
		void on_qaConfigCert_triggered();
		void on_qaAudioWizard_triggered();
		void on_qaHelpWhatsThis_triggered();
		void on_qaHelpAbout_triggered();
		void on_qaHelpAboutSpeex_triggered();
		void on_qaHelpAboutQt_triggered();
		void on_qaHelpVersionCheck_triggered();
		void on_qaQuit_triggered();
		void on_qleChat_tabPressed();
		void on_qleChat_ctrlSpacePressed();
		void on_qteLog_customContextMenuRequested(const QPoint &pos);
		void on_qteLog_anchorClicked(const QUrl &);
		void on_qteLog_highlighted(const QUrl & link);
		void on_qdwChat_dockLocationChanged(Qt::DockWidgetArea);
		void on_qdwLog_dockLocationChanged(Qt::DockWidgetArea);
		void on_PushToTalk_triggered(bool, QVariant);
		void on_PushToMute_triggered(bool, QVariant);
		void on_VolumeUp_triggered(bool, QVariant);
		void on_VolumeDown_triggered(bool, QVariant);
		void on_gsMuteSelf_down(QVariant);
		void on_gsDeafSelf_down(QVariant);
		void on_gsWhisper_triggered(bool, QVariant);
		void on_Reconnect_timeout();
		void on_Icon_activated(QSystemTrayIcon::ActivationReason);
		void qtvUserCurrentChanged(const QModelIndex &, const QModelIndex &);
		void serverConnected();
		void serverDisconnected(QAbstractSocket::SocketError, QString reason);
		void viewCertificate(bool);
		void openUrl(const QUrl &url);
		void context_triggered();
		void updateTarget();
		void updateMenuPermissions();
		void talkingChanged();
		void destroyUserInformation();
		void trayAboutToShow();
	public:
		MainWindow(QWidget *parent);
		~MainWindow();

		// From msgHandler. Implementation in Messages.cpp
#define MUMBLE_MH_MSG(x) void msg##x(const MumbleProto:: x &);
		MUMBLE_MH_ALL
#undef MUMBLE_MH_MSG
};

#else
class MainWindow;
#endif
