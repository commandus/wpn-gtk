#include <gtkmm.h>
#include "client-env.h"

#define UI_FILE "../glade/wpn.glade3"

class WpnApplication : public Gtk::Application
{
protected:
	WpnApplication();
	Glib::RefPtr<ClientEnv> mRefClientEnv;
public:
	static Glib::RefPtr<WpnApplication> create();
protected:
	void on_startup() override;
	void on_activate() override;
private:
	Glib::RefPtr<Gtk::Builder> mRefBuilder;
	void createWindow();
	void onWindowHide(Gtk::Window* window);
};
