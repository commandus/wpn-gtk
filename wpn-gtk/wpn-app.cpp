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
	mRefBuilder = Gtk::Builder::create();
	try
	{
		mRefBuilder->add_from_file(UI_FILE);
	}
	catch (const Glib::Error& ex)
	{
		std::cerr << "Building menus failed: " << ex.what();
	}

}

void WpnApplication::on_activate()
{
	create_window();
}

void WpnApplication::create_window()
{
	// Load the Glade file and instiate its widgets:
	TopWindow* appwindow = 0;
	mRefBuilder->get_widget_derived("topWindow", appwindow);
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
