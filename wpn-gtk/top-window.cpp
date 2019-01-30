#include <iostream>
#include "top-window.h"
#include <gdk/gdkkeysyms.h>
#include <sstream>
#include "google/protobuf/stubs/common.h"
#include "log-window.h"

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
	: mLogWindow(NULL)
{
}

static std::string protobufVersion()
{
	std::stringstream r;
	r << "libprotobuf: " << google::protobuf::internal::VersionString(GOOGLE_PROTOBUF_VERSION);
	return r.str();
}

TopWindow::TopWindow (BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &refBuilder)
	: Gtk::Window(cobject), mRefBuilder (refBuilder), mClientEnv(NULL), mLogWindow(NULL)
{
	mLogWindow = 0;
	mRefBuilder->get_widget_derived("logWindow", mLogWindow);
	mLogWindow->setClientEnv(mClientEnv);
	mLogWindow->signal_hide().connect(sigc::bind<Gtk::Window *>(
		sigc::mem_fun(*this, &TopWindow::onWindowHideLog), mLogWindow));

	mRefBuilder->get_widget("entryMessage", mEntryMessage);
	if (mEntryMessage) {
	}
	mRefBuilder->get_widget("checkmenuitemLog", mCheckMenuViewLog);

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
	mRefActionGroup->add_action("log",
		sigc::mem_fun(*this, &TopWindow::onViewLog));
	insert_action_group("wpn", mRefActionGroup);
 
	mRefBuilder->get_widget("buttonSend", mButtonSend);
	if (mButtonSend) {
		mButtonSend->signal_clicked().connect(
			sigc::bind<int> (sigc::mem_fun(*this, &TopWindow::onButtonClickSend), 0));
	}
	mRefBuilder->get_widget("treeviewClient", mTreeViewClient);
	mRefBuilder->get_widget("treeviewMessage", mTreeViewMessage);

	mRefListStoreClient = Glib::RefPtr<Gtk::ListStore>::cast_static(mRefBuilder->get_object("liststoreClient"));
	mRefListStoreMessage = Glib::RefPtr<Gtk::ListStore>::cast_static(mRefBuilder->get_object("liststoreMessage"));
	
	mRefTreeModelFilterMessage = Gtk::TreeModelFilter::create(mRefListStoreMessage);
	mRefTreeModelFilterMessage->set_visible_func(
	[this] (const Gtk::TreeModel::const_iterator& it) -> bool
	{
		if (!it)
			return true;
		Gtk::TreeModel::Row row = *it;
		Glib::ustring v;
		row.get_value(0, v);
		return false;
	});
	mTreeViewMessage->set_model(mRefTreeModelFilterMessage);

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
			Glib::ustring v;
			row.get_value(0, v);
		}
	}
}

TopWindow::~TopWindow() {
}

void TopWindow::setClientEnv(ClientEnv* value)
{
	mClientEnv = value;
	value->addLogHandler(std::bind(&TopWindow::onLog, this, _1, _2));
	value->addNotifyHandler(std::bind(&TopWindow::onNotify, this, _1, _2, _3, _4, _5, _6));
}

void TopWindow::onLog(int verbosity, const char *message)
{
	if (mLogWindow) {
		mLogWindow->put(message);
	}
}

void TopWindow::onNotify(
		const char *persistent_id,
		const char *from,
		const char *appName,
		const char *appId,
		int64_t sent,
		const NotifyMessageC *msg
	)
{
	std::cerr << "Notify persistent id: " << persistent_id << std::endl;
	std::string s = "";
	if (msg)
	{
		s = msg->body;
		/* const char *
		authorizedEntity;	///< e.g. 246829423295
		title;
		body;
		icon;				///< Specifies an icon filename or stock icon to display.
		sound;				///< sound file name
		link;				///< click action
		linkType;			///< click action content type
		category;
		extra;
		data;				///< extra data in JSON format
		int urgency; 					///< low- 0, normal, critical
		int timeout; 					///< timeout in milliseconds at which to expire the notification.
		*/
	}
	std::cerr << std::endl;

	if (mTreeViewMessage)
	{
		Gtk::TreeModel::iterator it = mRefListStoreMessage->append();
			Gtk::TreeModel::Row row = *it;
			row.set_value <Glib::ustring>(0, s);
			/*
			row.set_value <Glib::rustring>(1, msg);
			row.set_value <Glib::ustring>(2, persistent_id);
			row.set_value <Glib::ustring>(3, from);
			row.set_value <Glib::ustring>(4, appName);
			row.set_value <Glib::ustring>(5, appId);
			row.set_value <Glib::gint64>(6, sent);
			*/
	}
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
	mAboutDialog = new Gtk::AboutDialog();
	mAboutDialog->set_transient_for(*this);
	// mAboutDialog.set_logo(Gdk::Pixbuf::create_from_resource("/about/gtkmm_logo.gif", -1, 40, true));
	mAboutDialog->set_program_name("WPN for Linux");
	mAboutDialog->set_version("1.0.0 " + protobufVersion());
	mAboutDialog->set_copyright("Andrei Ivanov");
	mAboutDialog->set_comments("Linux wpn client");
	mAboutDialog->set_license(MIT_LICENSE);

	mAboutDialog->set_website("https://www.commandus.com/");
	mAboutDialog->set_website_label("commandus.com");

	std::vector<Glib::ustring> list_authors;
	list_authors.push_back("Andrei Ivanov  mailto:andrei.i.ivanov@commandus.com");
	mAboutDialog->set_authors(list_authors);

	mAboutDialog->signal_response().connect(
		sigc::mem_fun(*this, &TopWindow::onAboutDialogResponse));

	mAboutDialog->show();
	mAboutDialog->present();
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
			mAboutDialog->hide();
			break;
		default:
			break;
	}
}

void TopWindow::onViewLog()
{
	if (mCheckMenuViewLog->get_active())
		mLogWindow->show_all();
	else
		mLogWindow->hide();
}

void TopWindow::onWindowHideLog(Gtk::Window *window)
{
	if (mCheckMenuViewLog)
		mCheckMenuViewLog->set_active(false);
	window->hide();
}
