#include <vector>
#include <iostream>
#include <gtkmm.h>
#include "wpn-app.h"
#include "top-window.h"

int main(int argc, char *argv[])
{
	
	// Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "com.commandus.wpn-gtk");
	Glib::RefPtr<WpnApplication> app = WpnApplication::create();
	return app->run(argc, argv);
}
