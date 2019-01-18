#include "top-window.h"

TopWindow::TopWindow()
{
}

TopWindow::TopWindow (BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder)
: Gtk::Window (cobject),
  mRefBuilder (refBuilder)
{
	mRefBuilder->get_widget("buttonSend", mButtonSend);
	if (mButtonSend) {
		mButtonSend->signal_clicked().connect(
			sigc::bind<int> (sigc::mem_fun( *this,
			&TopWindow::onButtonClickSend), 0) );
	}
}

void TopWindow::onButtonClickSend(int iShape) {
}

TopWindow::~TopWindow() {
}

