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
//	Glib::RefPtr<Gtk::ListStore> mRefTreeModelClient;
//	Glib::RefPtr<Gtk::ListStore> mRefTreeModelMessage;
}

void TopWindow::onButtonClickSend(int n) {
	std::string v = "";
	if (mEntryMessage) {
		v = mEntryMessage->get_text();
	}
	std::cout << "The Button " << n << " was clicked, value: " << v << std::endl;
}

TopWindow::~TopWindow() {
}

