#include <iostream>
#include "top-window.h"
#include <gdk/gdkkeysyms.h>

TopWindow::TopWindow()
{
}

TopWindow::TopWindow (BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder)
: Gtk::Window (cobject),
  mRefBuilder (refBuilder)
{
	mRefBuilder->get_widget("entryMessage", mEntryMessage);
	if (mEntryMessage) {
	}
	mRefBuilder->get_widget("buttonSend", mButtonSend);
	if (mButtonSend) {
		mButtonSend->signal_clicked().connect(
			sigc::bind<int> (sigc::mem_fun(*this, &TopWindow::onButtonClickSend), 0));
	}
	mRefBuilder->get_widget("treeviewClient", mTreeViewClient);
	mRefBuilder->get_widget("treeviewMessage", mTreeViewMessage);

	mRefListStoreClient = Glib::RefPtr<Gtk::ListStore>::cast_static(mRefBuilder->get_object("liststoreClient"));
	mRefListStoreMessage = Glib::RefPtr<Gtk::ListStore>::cast_static(mRefBuilder->get_object("liststoreMessage"));
	
	mTreeViewSelectionClient = Glib::RefPtr<Gtk::TreeSelection>::cast_static(mRefBuilder->get_object("treeviewSelectionClient"));
	mTreeViewSelectionMessage = Glib::RefPtr<Gtk::TreeSelection>::cast_static(mRefBuilder->get_object("treeviewSelectionMessage"));
	
	add_events(Gdk::KEY_PRESS_MASK);
}

void TopWindow::onButtonClickSend(int n) {
	std::string v = "";
	if (mEntryMessage) {
		v = mEntryMessage->get_text();
	}
	mEntryMessage->set_text("");
	
	if (mRefListStoreMessage) {
		Gtk::TreeModel::iterator it = mRefListStoreMessage->append();
		Gtk::TreeModel::Row row = *it;
		row.set_value <Glib::ustring>(0, v); 
	}

	if (mTreeViewSelectionMessage) {
		Gtk::TreeModel::iterator iter = mTreeViewSelectionMessage->get_selected();
		if (iter) {
			Gtk::TreeModel::Row row = *iter;
			std::cout << row << std::endl;
		}
	}
}

TopWindow::~TopWindow() {
}

bool TopWindow::on_key_press_event(GdkEventKey* event)
{
	switch (event->keyval)
	{
		case GDK_KEY_Return:
			this->onButtonClickSend(0);
			break;
		default:
			return Gtk::Window::on_key_press_event(event);
	}
	return FALSE; 
}
