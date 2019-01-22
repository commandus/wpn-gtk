#include <iostream>
#include "top-window.h"
#include <gdk/gdkkeysyms.h>

TopWindow::TopWindow()
{
}

TopWindow::TopWindow (BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder)
	: Gtk::ApplicationWindow(cobject), mRefBuilder (refBuilder)
{
	mRefBuilder->get_widget("entryMessage", mEntryMessage);

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
	mAboutDialog.set_version("1.0.0");
	mAboutDialog.set_copyright("Andrei Ivanov");
	mAboutDialog.set_comments("Demo application.");
	mAboutDialog.set_license("MIT");

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
	mFileFilterWPN->add_mime_type("application/json");
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

void TopWindow::setClientEnv(Glib::RefPtr<ClientEnv> value)
{
	mRefClientEnv = value;
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
	std::cout << G_STRFUNC << std::endl;
	mAboutDialog.show();
	mAboutDialog.present();
}

void TopWindow::onFileQuit()
{
	std::cout << G_STRFUNC << std::endl;
	hide();
}

void TopWindow::onFileOpen()
{
	std::cout << G_STRFUNC << std::endl;
	Gtk::FileChooserDialog dialog("Open file", Gtk::FILE_CHOOSER_ACTION_OPEN);
	dialog.set_transient_for(*this);
	// Add response buttons the the dialog:
	dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	dialog.add_button("Select", Gtk::RESPONSE_OK);
	dialog.add_filter(mFileFilterWPN);
	int result = dialog.run();
	// Handle the response:
	switch(result) {
		case(Gtk::RESPONSE_OK):
			std::cout << "File selected: " << dialog.get_filename() << std::endl;
			break;
		default:
			break;
	}	
}

void TopWindow::onFileNew()
{
	std::cout << G_STRFUNC << std::endl;
	mRefClientEnv->newClientFile();
}

void TopWindow::onFileSave()
{
	std::cout << G_STRFUNC << std::endl;
	mRefClientEnv->saveClientFile();
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
			std::cout << "File selected: " << dialog.get_filename() << std::endl;
			mRefClientEnv->saveAsClientFile(dialog.get_filename());
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

