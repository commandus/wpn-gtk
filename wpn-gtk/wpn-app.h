#include <gtkmm.h>

#define UI_FILE "../glade/wpn.glade3"

class WpnApplication : public Gtk::Application
{
protected:
	WpnApplication();
public:
	static Glib::RefPtr<WpnApplication> create();
protected:
	void on_startup() override;
	void on_activate() override;
private:
	Glib::RefPtr<Gtk::Builder> mRefBuilder;
	void create_window();
	void on_window_hide(Gtk::Window* window);
};
