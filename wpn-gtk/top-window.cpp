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

	// mRefListStoreClient = gtk_builder_get_object(refBuilder, "liststoreClient");
	//mRefBuilder->gtk_builder_get_object("liststoreMessage", mRefListStoreMessage);
}

void TopWindow::onButtonClickSend(int n) {
	std::string v = "";
	if (mEntryMessage) {
		v = mEntryMessage->get_text();
	}
	if (mRefListStoreMessage) {
		Gtk::TreeModel::iterator it = mRefListStoreMessage->append();
		Gtk::TreeModel::Row row = *it;
		row.set_value(0, 0);
	}
	std::cout << "The Button " << n << " was clicked, value: " << v << std::endl;
}

TopWindow::~TopWindow() {
}

