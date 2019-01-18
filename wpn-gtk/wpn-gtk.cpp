#include <vector>
#include <iostream>
#include <gtkmm.h>
#include "top-window.h"

#define UI_FILE "../glade/wpn.glade3"

int main(int argc, char *argv[])
{
	Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "com.commandus.wpn-gtk");
	// Load the Glade file and instiate its widgets:
	Glib::RefPtr<Gtk::Builder> builder;
	try
	{
		builder = Gtk::Builder::create_from_file(UI_FILE);
	}
	catch (const Glib::FileError & ex)
	{
		std::cerr << ex.what() << std::endl;
		return 1;
	}
	TopWindow* appwindow = 0;
	builder->get_widget_derived("topWindow", appwindow);

    return app->run(*appwindow);	
}
