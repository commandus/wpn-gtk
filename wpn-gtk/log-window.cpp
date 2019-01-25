#include <iostream>
#include "log-window.h"
#include <gdk/gdkkeysyms.h>

LogWindow::LogWindow()
{
}

LogWindow::LogWindow (BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder)
	: Gtk::Window(cobject), mRefBuilder (refBuilder), mClientEnv(NULL)
{
	mRefBuilder->get_widget("treeviewLog", mTreeViewLog);
	mRefListStoreLog = Glib::RefPtr<Gtk::ListStore>::cast_static(mRefBuilder->get_object("liststoreLog"));
	mTreeViewSelectionLog = Glib::RefPtr<Gtk::TreeSelection>::cast_static(mRefBuilder->get_object("treeviewSelectionLog"));
}

LogWindow::~LogWindow() {
}

void LogWindow::setClientEnv(ClientEnv* value)
{
	mClientEnv = value;
}

void LogWindow::put(const std::string &value)
{
	if (mRefListStoreLog) {
		Gtk::TreeModel::iterator it = mRefListStoreLog->append();
		Gtk::TreeModel::Row row = *it;
		row.set_value <Glib::ustring>(0, value);
	}
}
