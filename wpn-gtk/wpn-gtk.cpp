#include <vector>
#include <iostream>
#include <gtkmm.h>
#include "wpn-app.h"

int main(int argc, char *argv[])
{
	Glib::RefPtr<WpnApplication> app = WpnApplication::create();
	return app->run(argc, argv);
}
