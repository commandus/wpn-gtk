#include <vector>
#include <iostream>
#include <gtkmm.h>
#include "top-window.h"

#define UI_FILE "src/geometry.ui"

int main(int argc, char *argv[])
{
	Gtk::Main kit(argc, argv);
	Glib::RefPtr<Gtk::Builder> builder = Gtk::Builder::create_from_file("../glade/wpn-gtk.glade");
	Gtk::Window *window;
	builder->get_widget("topWindow", window);
    Gtk::Main::run(*window);
}
