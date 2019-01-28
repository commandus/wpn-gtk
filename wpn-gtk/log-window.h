#ifndef LOGWINDOW_H
#define	LOGWINDOW_H

#include <gtkmm.h>
#include <sstream>
#include "client-env.h"

class LogWindow: public Gtk::Window {
private:
	std::stringstream mLogBuffer;
	Glib::RefPtr<Gio::SimpleActionGroup> mRefActionGroup;
public:
	LogWindow();
	LogWindow(BaseObjectType*, const Glib::RefPtr<Gtk::Builder>&);
	virtual ~LogWindow();
	void setClientEnv(ClientEnv *value);
	void put(const std::string &value);
	void clear();
protected:
	Glib::RefPtr<Gtk::Builder> mRefBuilder;
	Gtk::TreeView *mTreeViewLog;
	Glib::RefPtr<Gtk::TreeSelection> mTreeViewSelectionLog;
	Glib::RefPtr<Gtk::ListStore> mRefListStoreLog;
	ClientEnv* mClientEnv;
	void onLogClear();
};

#endif
