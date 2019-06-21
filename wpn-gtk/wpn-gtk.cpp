#include <vector>
#include <iostream>
#include <gtkmm.h>
#include <glog/logging.h>
#include "wpn-app.h"
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

int main(int argc, char *argv[])
{
	google::InstallFailureSignalHandler();
	FLAGS_logtostderr = 1;
	FLAGS_v = 3;
	google::InitGoogleLogging(argv[0]);
#ifdef HAVE_CONFIG_H
	LOG(INFO) << "Running " << PACKAGE_STRING;
#endif
	Glib::RefPtr<WpnApplication> app = WpnApplication::create();
	return app->run(argc, argv);
}
