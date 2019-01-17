#include <vector>
#include <iostream>
#include <gtkmm.h>
#include "top-window.h"

#define UI_FILE "src/geometry.ui"

int main(int argc, char *argv[])
{
  Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(argc, argv, "org.apmpc.geometry");

  //Load the Glade file and instiate its widgets:
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
	MainWindow* appwindow = 0;
	builder->get_widget_derived("TopWindow", appwindow);
    return app->run(*appwindow);
}