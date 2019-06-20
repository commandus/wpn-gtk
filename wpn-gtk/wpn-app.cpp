#include "wpn-app.h"
#include "top-window.h"
#include <iostream>
#include <glog/logging.h>

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
		// remove resource.c, make or rebuild resource.c manually:
		// glib-compile-resources wpn-gtk.gresource.xml --target=resources.c --generate-source
		LOG(INFO) << "Load form";
		mRefBuilder->add_from_resource(RES_GLADE3);
	}
	catch (const Glib::Error& ex)
	{
		LOG(ERROR) << "Building menus failed: " << ex.what();
	}
}

void WpnApplication::on_activate()
{
	createTopWindow();
}

void WpnApplication::createTopWindow()
{
	// Load the Glade file and instiate its widgets:
	TopWindow* topWindow = 0;
	mRefBuilder->get_widget_derived("topWindow", topWindow);
	topWindow->setClientEnv(&mClientEnv);

	// Make sure that the application runs for as long this window is still open:
	add_window(*topWindow);
	// Delete the window when it is hidden.
	topWindow->signal_hide().connect(sigc::bind<Gtk::Window*>(
	sigc::mem_fun(*this, &WpnApplication::onWindowHide), topWindow));
	topWindow->show_all();
}

void WpnApplication::onWindowHide(Gtk::Window* window)
{
	delete window;
}
