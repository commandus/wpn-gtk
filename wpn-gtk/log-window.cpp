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

	mRefActionGroup = Gio::SimpleActionGroup::create();
	mRefActionGroup->add_action("close", sigc::mem_fun(*this, &LogWindow::onLogClose));
	mRefActionGroup->add_action("clear", sigc::mem_fun(*this, &LogWindow::onLogClear));
	insert_action_group("log", mRefActionGroup);
}

LogWindow::~LogWindow() {
}

void LogWindow::setClientEnv(ClientEnv* value)
{
	mClientEnv = value;
}

void LogWindow::put(const std::string &value)
{
	size_t p;
	if ((p = value.find('\n')) != std::string::npos) 
	{
		mLogBuffer << value.substr(0, p - 1);
		std::string s = mLogBuffer.str();
		if (!s.empty() &&  mRefListStoreLog) {
			Gtk::TreeModel::iterator it = mRefListStoreLog->append();
			Gtk::TreeModel::Row row = *it;
			row.set_value <Glib::ustring>(0, s);
		}
		mLogBuffer.str(std::string());
	}
	else
		mLogBuffer << value;
}

void LogWindow::clear()
{
	if (mRefListStoreLog) {
		mRefListStoreLog->clear();
	}
}

void LogWindow::onLogClear()
{
	clear();
}

void LogWindow::onLogClose()
{
	hide();
}
