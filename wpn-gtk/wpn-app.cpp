#include "wpn-app.h"
#include "top-window.h"
#include <iostream>

WpnApplication::WpnApplication()
	: Gtk::Application("com.commandus.wpn")
{
	Glib::set_application_name("wpn");
}

Glib::RefPtr<WpnApplication> WpnApplication::create()
{
	return Glib::RefPtr<WpnApplication>(new WpnApplication());
}

void WpnApplication::on_startup()
{
	// Call the base class's implementation:
	Gtk::Application::on_startup();

	// Create actions for menus and toolbars.
	//We can use add_action() because Gtk::Application derives from Gio::ActionMap.

	// File|New sub menu:
	add_action("newstandard",
	sigc::mem_fun(*this, &WpnApplication::on_menu_file_new_generic));

	add_action("newfoo",
	sigc::mem_fun(*this, &WpnApplication::on_menu_file_new_generic));

	add_action("newgoo",
	sigc::mem_fun(*this, &WpnApplication::on_menu_file_new_generic));

	//File menu:
	add_action("quit", sigc::mem_fun(*this, &WpnApplication::on_menu_file_quit));

	//Help menu:
	add_action("about", sigc::mem_fun(*this, &WpnApplication::on_menu_help_about));

	m_refBuilder = Gtk::Builder::create();

	try
	{
		m_refBuilder->add_from_file(UI_FILE);
	}
	catch (const Glib::Error& ex)
	{
		std::cerr << "Building menus failed: " << ex.what();
	}

	auto object = m_refBuilder->get_object("menubar");
	Glib::RefPtr<Gio::MenuModel> pMenuBar = Glib::RefPtr<Gio::MenuModel>::cast_dynamic(object);
	/*
	 * auto gmenu;
	auto object = m_refBuilder->get_object("menubar");
	auto gmenu = Glib::RefPtr<Gio::Menu>::cast_dynamic(object);
	*/
	set_menubar(pMenuBar);
/*
	// Get the menubar and the app menu, and add them to the application:
	Glib::RefPtr<Glib::Object> o = m_refBuilder->get_object("menubar");
	Glib::RefPtr<Gio::Menu> gmenu = Glib::RefPtr<Gio::Menu>::cast_dynamic(o);
	//Gtk::MenuBar* pMenuBar = Gtk::make_managed<Gtk::MenuBar>(gmenu);
	set_menubar(gmenu);
	*/
}

void WpnApplication::on_activate()
{
	//std::cout << "debug1: " << G_STRFUNC << std::endl;
	// The application has been started, so let's show a window.
	// A real application might want to reuse this window in on_open(),
	// when asked to open a file, if no changes have been made yet.
	create_window();
}

void WpnApplication::create_window()
{
	// Load the Glade file and instiate its widgets:
	Glib::RefPtr<Gtk::Builder> builder;
	try
	{
		builder = Gtk::Builder::create_from_file(UI_FILE);
	}
	catch (const Glib::FileError & ex)
	{
		std::cerr << ex.what() << std::endl;
	}
	TopWindow* appwindow = 0;
	builder->get_widget_derived("topWindow", appwindow);

	// Make sure that the application runs for as long this window is still open:
	add_window(*appwindow);
	// Delete the window when it is hidden.
	// That's enough for this simple example.
	appwindow->signal_hide().connect(sigc::bind<Gtk::Window*>(
	sigc::mem_fun(*this, &WpnApplication::on_window_hide), appwindow));
	appwindow->show_all();
}

void WpnApplication::on_window_hide(Gtk::Window* window)
{
	delete window;
}

void WpnApplication::on_menu_file_new_generic()
{
	std::cout << "A File|New menu item was selected." << std::endl;
}

void WpnApplication::on_menu_file_quit()
{
	std::cout << G_STRFUNC << std::endl;
	quit(); // Not really necessary, when Gtk::Widget::hide() is called.

	// Gio::Application::quit() will make Gio::Application::run() return,
	// but it's a crude way of ending the program. The window is not removed
	// from the application. Neither the window's nor the application's
	// destructors will be called, because there will be remaining reference
	// counts in both of them. If we want the destructors to be called, we
	// must remove the window from the application. One way of doing this
	// is to hide the window.
	std::vector<Gtk::Window*> windows = get_windows();
	if (windows.size() > 0)
		windows[0]->hide(); // In this simple case, we know there is only one window.
}

void WpnApplication::on_menu_help_about()
{
	std::cout << "App|Help|About was selected." << std::endl;
}
