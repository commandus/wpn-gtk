#ifndef TOPWINDOW_H
#define	TOPWINDOW_H

#include <functional>
#include <gtkmm.h>
#include "client-env.h"
#include "log-window.h"

#include "mcs/mcsclient.h"

using namespace std::placeholders; // for `_1`

class TopWindow: public Gtk::Window {
public:
	TopWindow();
	TopWindow(BaseObjectType*, const Glib::RefPtr<Gtk::Builder>&);
	virtual ~TopWindow();
	void setClientEnv(ClientEnv *value);
protected:
	void onButtonClickSend(int);
	bool on_key_press_event(GdkEventKey *event) override;
	void onHelpAbout();
	void onFileQuit();
	void onFileOpen();
	void onFileNew();
	void onFileSave();
	void onFileSaveAs();
	void onViewLog();
	Glib::RefPtr<Gtk::Builder> mRefBuilder;
	Gtk::Entry *mEntryMessage;
	Gtk::Button *mButtonSend;
	Gtk::TreeView *mTreeViewClient;
	Gtk::TreeView *mTreeViewMessage;
	Gtk::CheckMenuItem *mCheckMenuViewLog;
	Glib::RefPtr<Gtk::TreeSelection> mTreeViewSelectionClient;
	Glib::RefPtr<Gtk::TreeSelection> mTreeViewSelectionMessage;
	Glib::RefPtr<Gtk::ListStore> mRefListStoreClient;
	Glib::RefPtr<Gtk::ListStore> mRefListStoreMessage;
	Gtk::AboutDialog *mAboutDialog;
	LogWindow *mLogWindow;
	ClientEnv *mClientEnv;
	void onAboutDialogResponse(int responseId);
	void onLog(int verbosity, const char *message);
	void onNotify(
		const char *persistent_id,
		const char *from,
		const char *appName,
		const char *appId,
		int64_t sent,
		const NotifyMessageC *msg
	);
private:
	Glib::RefPtr<Gtk::FileFilter> mFileFilterWPN;
	Glib::RefPtr<Gio::SimpleActionGroup> mRefActionGroup;
	void onWindowHideLog(Gtk::Window *window);
};

#endif
