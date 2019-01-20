#include <iostream>
#include "top-window.h"

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
			sigc::bind<int> (sigc::mem_fun( *this,
			&TopWindow::onButtonClickSend), 0) );
	}
	mRefBuilder->get_widget("treeviewClient", mTreeViewClient);
	mRefBuilder->get_widget("treeviewMessage", mTreeViewMessage);

	mRefListStoreClient = Glib::RefPtr<Gtk::ListStore>::cast_static(mRefBuilder->get_object("liststoreClient"));
	mRefListStoreMessage = Glib::RefPtr<Gtk::ListStore>::cast_static(mRefBuilder->get_object("liststoreMessage"));
}

void TopWindow::onButtonClickSend(int n) {
	std::string v = "";
	if (mEntryMessage) {
		v = mEntryMessage->get_text();
	}
	if (mRefListStoreMessage) {
		Gtk::TreeModel::iterator it = mRefListStoreMessage->append();
		Gtk::TreeModel::Row row = *it;
		std::string s = "message";
		row.set_value <Glib::ustring>(0, s); 
	}
	std::cout << "The Button " << n << " was clicked, value: " << v << std::endl;
}

TopWindow::~TopWindow() {
}

