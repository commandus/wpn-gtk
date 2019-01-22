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
	createWindow();
}

void WpnApplication::createWindow()
{
	// Load the Glade file and instiate its widgets:
	TopWindow* topWindow = 0;
	mRefBuilder->get_widget_derived("topWindow", topWindow);
	topWindow->setClientEnv(mRefClientEnv);

	// Make sure that the application runs for as long this window is still open:
	add_window(*topWindow);
	// Delete the window when it is hidden.
	// That's enough for this simple example.
	topWindow->signal_hide().connect(sigc::bind<Gtk::Window*>(
	sigc::mem_fun(*this, &WpnApplication::onWindowHide), topWindow));
	topWindow->show_all();
}

void WpnApplication::onWindowHide(Gtk::Window* window)
{
	delete window;
}
