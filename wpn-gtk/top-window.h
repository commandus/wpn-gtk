#ifndef TOPWINDOW_H
#define	TOPWINDOW_H

#include <gtkmm.h>
#include "client-env.h"

class TopWindow: public Gtk::ApplicationWindow {
public:
	TopWindow();
	TopWindow(BaseObjectType*, const Glib::RefPtr<Gtk::Builder>&);
	virtual ~TopWindow();
	void setClientEnv(Glib::RefPtr<ClientEnv> value);
protected:
	void onButtonClickSend(int);
	bool on_key_press_event(GdkEventKey *event) override;
	void onHelpAbout();
	void onFileQuit();
	void onFileOpen();
	void onFileNew();
	void onFileSave();
	void onFileSaveAs();
	Glib::RefPtr<Gtk::Builder> mRefBuilder;
	Gtk::Entry *mEntryMessage;
	Gtk::Button *mButtonSend;
	Gtk::TreeView *mTreeViewClient;
	Gtk::TreeView *mTreeViewMessage;
	Glib::RefPtr<Gtk::TreeSelection> mTreeViewSelectionClient;
	Glib::RefPtr<Gtk::TreeSelection> mTreeViewSelectionMessage;
	Glib::RefPtr<Gtk::ListStore> mRefListStoreClient;
	Glib::RefPtr<Gtk::ListStore> mRefListStoreMessage;
	Gtk::AboutDialog mAboutDialog;
	Glib::RefPtr<ClientEnv> mRefClientEnv;
	void onAboutDialogResponse(int responseId);
private:
	Glib::RefPtr<Gtk::FileFilter> mFileFilterWPN;
	Glib::RefPtr<Gio::SimpleActionGroup> mRefActionGroup;
};

#endif
