#ifndef TOPWINDOW_H
#define	TOPWINDOW_H

#include <gtkmm.h>

class TopWindow: public Gtk::Window {
public:
	TopWindow();
	TopWindow(BaseObjectType*, const Glib::RefPtr<Gtk::Builder>&);
	virtual ~TopWindow();
protected:
	void onButtonClickSend(int);
	bool on_key_press_event(GdkEventKey *event) override;
	Glib::RefPtr<Gtk::Builder> mRefBuilder;
	Gtk::Entry *mEntryMessage;
	Gtk::Button *mButtonSend;
	Gtk::TreeView *mTreeViewClient;
	Gtk::TreeView *mTreeViewMessage;
	Glib::RefPtr<Gtk::TreeSelection> mTreeViewSelectionClient;
	Glib::RefPtr<Gtk::TreeSelection> mTreeViewSelectionMessage;
	Glib::RefPtr<Gtk::ListStore> mRefListStoreClient;
	Glib::RefPtr<Gtk::ListStore> mRefListStoreMessage;
};

#endif
