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
	Glib::RefPtr<Gtk::Builder> mRefBuilder;
	Gtk::Entry *mEntryMessage;
	Gtk::Button *mButtonSend;
	Gtk::TreeView *mTreeViewClient;
	Gtk::TreeView *mTreeViewMessage;
	Glib::RefPtr<Gtk::ListStore> mRefTreeModelClient;
	Glib::RefPtr<Gtk::ListStore> mRefTreeModelMessage;
	
	class ModelColumnsClient : public Gtk::TreeModel::ColumnRecord
	{
	public:
		ModelColumnsClient()
		{
			add(mColClientName);
		}
		Gtk::TreeModelColumn<Glib::ustring> mColClientName;
	};
	
};

#endif
