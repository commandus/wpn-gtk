#include <iostream>
#include <sstream>
#include <gdk/gdkkeysyms.h>
#include <glog/logging.h>

#include "google/protobuf/stubs/common.h"
#include "top-window.h"
#include "log-window.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

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
	mRefActionGroup->add_action("new",
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
	// mTreeViewMessage->set_model(mRefTreeModelFilterMessage);

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
	
	if (mRefListStoreMessage && !v.empty()) {
		Gtk::TreeModel::Row row = *mRefListStoreMessage->append();
		row.set_value <Glib::ustring>(0, v); 
	}

	if (mTreeViewSelectionMessage) {
		Gtk::TreeModel::iterator iter = mTreeViewSelectionMessage->get_selected();
		if (iter) {
			Gtk::TreeModel::Row row = *iter;
			Glib::ustring v;
			row.get_value(0, v);
			Glib::ustring from;
			row.get_value(3, from);
			LOG(INFO) << "Selected " << v << " from: " << from;
		}
	}
}

TopWindow::~TopWindow() {
}

void TopWindow::loadClients(
	Glib::RefPtr<Gtk::ListStore> listStore,
	ConfigFile *config
) 
{
LOG(INFO) << 1;	
	if (!listStore)
		return;
LOG(INFO) << 2;	
	// TODO clear
	if (!config)
		return;
LOG(INFO) << 3;	
	for (std::vector<Subscription>::const_iterator it(config->subscriptions->list.begin()); it!= config->subscriptions->list.end(); ++it) {
LOG(INFO) << 4;		
		Gtk::TreeModel::Row row = *listStore->append();
		std::string name;
		name = it->getName();
		if (name.empty())
			name = "noname";
		std::stringstream n;
		n << it->getWpnKeys().id << "(" << name << ")";
		row.set_value <Glib::ustring>(0, n.str()); 
	}
LOG(INFO) << 5;	
}

void TopWindow::setClientEnv(ClientEnv* value)
{
	mClientEnv = value;
	value->addLogHandler(std::bind(&TopWindow::onLog, this, _1, _2));
	value->addNotifyHandler(std::bind(&TopWindow::onNotify, this, _1, _2, _3, _4, _5, _6));
	reOpen();
	loadClients(
		mRefListStoreClient,
		mClientEnv->config
	);
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
	Subscription *subscription = NULL;
	if (mClientEnv) {
		subscription = mClientEnv->config->subscriptions->findByPublicKey(from);
	}
	std::string clientName = "anonymous";
	if (subscription) {
		clientName = subscription->getName();
		if (clientName.empty()) {
			if (subscription->getWpnKeysPtr()->id)
				clientName = std::to_string(subscription->getWpnKeysPtr()->id);
		}
	}
	if (msg) {
		LOG(INFO) << "Notify persistent id: " << persistent_id
			<< " from: " << clientName
			<< ", appId: " << appId << "(" << appName << "), sent: " << sent
			<< ", authorizedEntity: " << msg->authorizedEntity
			<< ", title: " << msg->title
			<< ", body: " << msg->body
			<< ", icon: " << msg->icon				///< Specifies an icon filename or stock icon to display.
			<< ", sound: " << msg->sound			///< sound file name
			<< ", link: " << msg->link				///< click action
			<< ", linkType: " << msg->linkType		///< click action content type
			<< ", category: " << msg->category
			<< ", extra: " << msg->extra
			<< ", data: " << msg->data				///< extra data in JSON format
			<< ", urgency: " << msg->urgency 		///< low- 0, normal, critical
			<< ", timeout: " << msg->timeout;		///< timeout in milliseconds at which to expire the notification.
	}

	if (mTreeViewMessage && msg)
	{
		Gtk::TreeModel::iterator it = mRefListStoreMessage->append();
			Gtk::TreeModel::Row row = *it;
			row.set_value <Glib::ustring>(0, msg->body);
			// row.set_value <Glib::rustring>(1, msg);
			if (persistent_id)
				row.set_value <Glib::ustring>(2, persistent_id);
			if (from)
				row.set_value <Glib::ustring>(3, from);
			if (appName)
				row.set_value <Glib::ustring>(4, appName);
			if (appId)
				row.set_value <Glib::ustring>(5, appId);
			row.set_value <unsigned long>(6, sent);
			// message
			if (msg->title)
				row.set_value <Glib::ustring>(7, msg->title);
			if (msg->icon)
				row.set_value <Glib::ustring>(8, msg->icon);
			if (msg->sound)
				row.set_value <Glib::ustring>(9, msg->sound);
			if (msg->link)
				row.set_value <Glib::ustring>(10, msg->link);
			if (msg->linkType)
				row.set_value <Glib::ustring>(11, msg->linkType);
			if (msg->category)
				row.set_value <Glib::ustring>(12, msg->category);
			if (msg->extra)
				row.set_value <Glib::ustring>(13, msg->extra);
			if (msg->data)
				row.set_value <Glib::ustring>(14, msg->data);
			row.set_value <int>(15, msg->urgency);
			row.set_value <unsigned long>(16, msg->timeout);
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
	mAboutDialog->set_logo(Gdk::Pixbuf::create_from_resource("/../glade/ic_launcher.png", -1, 40, true));
	mAboutDialog->set_program_name("wpn for Linux desktop");
	std::string v;
#ifdef HAVE_CONFIG_H
	v = PACKAGE_STRING;
#else	
	v = "Custom build";
#endif
	mAboutDialog->set_version(v + ", " + protobufVersion());
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

/**
 * 
 * https://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c
 */
inline bool fileExists
(
	const std::string& fn
) {
	struct stat buffer;   
	return (stat (fn.c_str(), &buffer) == 0); 
}

// libwpnpp.a
std::string getDefaultConfigFileName(const std::string &filename);

void TopWindow::reOpen()
{
	if (mClientEnv) {
		std::string fn = getDefaultConfigFileName(DEF_CONFIG_FILE_NAME);
		if (fileExists(fn)) {
			if (!mClientEnv->openClientFile(fn)) {
				LOG(ERROR) << G_STRFUNC << " error open config file " << fn;
			} else {
//				LOG(INFO) << G_STRFUNC << " open config file " << fn << " successfully";
			}
		} else {
			LOG(INFO) << G_STRFUNC << " config file " << fn << " does not exists";
		}
	}
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
	LOG(INFO) << G_STRFUNC;
	if (mClientEnv)
		mClientEnv->newClientFile();
}

void TopWindow::onFileSave()
{
	LOG(INFO) << G_STRFUNC;
	if (mClientEnv)
		mClientEnv->saveClientFile();
}

void TopWindow::onFileSaveAs()
{
	LOG(INFO) << G_STRFUNC;
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
