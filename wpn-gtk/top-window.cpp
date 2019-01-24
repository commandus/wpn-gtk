#include <iostream>
#include "top-window.h"
#include <gdk/gdkkeysyms.h>
#include <sstream>
#include "google/protobuf/stubs/common.h"
#define DLG_CAPTION_OPENFILE_ERROR "Error read wpn client file"

#define MIT_LICENSE "Copyright (c) 2019 Andrei Ivanov \n\n\
MIT license\n\n\
Permission is hereby granted, free of charge, to any \n\
person obtaining a copy of this software and associated \n\
documentation files (the \"Software\"), to deal in the \n\
Software without restriction, including without limitation \n\
the rights to use, copy, modify, merge, publish, distribute, \n\
sublicense, and/or sell copies of the Software, and to permit \n\
persons to whom the Software is furnished to do so, subject \n\
to the following conditions:\n\n\
The above copyright notice and this permission notice shall \n\
be included in all copies or substantial portions of the \n\
Software.\n\n\
THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY \n\
KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE \n\
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR\" \n\
PURPOSE AND NONINFRINGEMENT IN NO EVENT SHALL THE AUTHORS \n\
OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR \n\
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT\" \n\
OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION \n\
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN \n\
THE SOFTWARE."

TopWindow::TopWindow()
{
}

static std::string protobufVersion()
{
	std::stringstream r;
	r << "libprotobuf: " << google::protobuf::internal::VersionString(GOOGLE_PROTOBUF_VERSION);
	return r.str();
}

TopWindow::TopWindow (BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder)
	: Gtk::Window(cobject), mRefBuilder (refBuilder), mClientEnv(NULL)
{
	mRefBuilder->get_widget("entryMessage", mEntryMessage);

	// GtkAccelGroup *accelGroup = gtk_accel_group_new();
	// gtk_window_add_accel_group(GTK_WINDOW(this), accelGroup);
	
	mRefActionGroup = Gio::SimpleActionGroup::create();
	mRefActionGroup->add_action("quit",
		sigc::mem_fun(*this, &TopWindow::onFileQuit));
	mRefActionGroup->add_action("about",
		sigc::mem_fun(*this, &TopWindow::onHelpAbout));
	mRefActionGroup->add_action("open",
		sigc::mem_fun(*this, &TopWindow::onFileOpen));
	mRefActionGroup->add_action("hew",
		sigc::mem_fun(*this, &TopWindow::onFileNew));
	mRefActionGroup->add_action("save",
		sigc::mem_fun(*this, &TopWindow::onFileSave));
	mRefActionGroup->add_action("saveas",
		sigc::mem_fun(*this, &TopWindow::onFileSaveAs));
	
	insert_action_group("wpn", mRefActionGroup);
 
	mAboutDialog.set_transient_for(*this);
	// mAboutDialog.set_logo(Gdk::Pixbuf::create_from_resource("/about/gtkmm_logo.gif", -1, 40, true));
	mAboutDialog.set_program_name("WPN for Linux");
	mAboutDialog.set_version("1.0.0 " + protobufVersion());
	mAboutDialog.set_copyright("Andrei Ivanov");
	mAboutDialog.set_comments("Linux wpn client");
	mAboutDialog.set_license(MIT_LICENSE);

	mAboutDialog.set_website("https://www.commandus.com/");
	mAboutDialog.set_website_label("commandus.com");

	std::vector<Glib::ustring> list_authors;
	list_authors.push_back("Andrei Ivanov  mailto:andrei.i.ivanov@commandus.com");
	mAboutDialog.set_authors(list_authors);

	mAboutDialog.signal_response().connect(
		sigc::mem_fun(*this, &TopWindow::onAboutDialogResponse));

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

	mFileFilterWPN = Gtk::FileFilter::create();
	mFileFilterWPN->set_name("wpn client files");
	mFileFilterWPN->add_mime_type("application/javascript");
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

void TopWindow::setClientEnv(ClientEnv* value)
{
	mClientEnv = value;
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

void TopWindow::onHelpAbout()
{
	mAboutDialog.show();
	mAboutDialog.present();
}

void TopWindow::onFileQuit()
{
	hide();
}

void TopWindow::onFileOpen()
{
	Gtk::FileChooserDialog dialog("Open client file", Gtk::FILE_CHOOSER_ACTION_OPEN);
	dialog.set_transient_for(*this);
	// Add response buttons the the dialog:
	dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	dialog.add_button("Open", Gtk::RESPONSE_OK);
	dialog.add_filter(mFileFilterWPN);
	int result = dialog.run();
	// Handle the response:
	switch(result) {
		case(Gtk::RESPONSE_OK):
			if (mClientEnv) {
				std::string fn = dialog.get_filename();
				if (!mClientEnv->openClientFile(fn)) {
					Gtk::MessageDialog dialog(*this, DLG_CAPTION_OPENFILE_ERROR);
					dialog.set_secondary_text(fn);
					dialog.run();
				}
			}
			break;
		default:
			break;
	}	
}

void TopWindow::onFileNew()
{
	std::cout << G_STRFUNC << std::endl;
	if (mClientEnv)
		mClientEnv->newClientFile();
}

void TopWindow::onFileSave()
{
	std::cout << G_STRFUNC << std::endl;
	if (mClientEnv)
		mClientEnv->saveClientFile();
}

void TopWindow::onFileSaveAs()
{
	std::cout << G_STRFUNC << std::endl;
	Gtk::FileChooserDialog dialog("Save file", Gtk::FILE_CHOOSER_ACTION_SAVE);
	dialog.set_transient_for(*this);
	// Add response buttons the the dialog:
	dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	dialog.add_button("Save", Gtk::RESPONSE_OK);
	dialog.add_filter(mFileFilterWPN);
	int result = dialog.run();
	// Handle the response:
	switch(result) {
		case(Gtk::RESPONSE_OK):
			if (mClientEnv)
				mClientEnv->saveAsClientFile(dialog.get_filename());
			break;
		default:
			break;
	}	
}

void TopWindow::onAboutDialogResponse(int responseId)
{
	switch (responseId) {
		case Gtk::RESPONSE_CLOSE:
		case Gtk::RESPONSE_CANCEL:
		case Gtk::RESPONSE_DELETE_EVENT:
			mAboutDialog.hide();
			break;
		default:
			break;
	}
}

