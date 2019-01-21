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
	Glib::RefPtr<Gtk::Builder> m_refBuilder;
	void create_window();
	void on_window_hide(Gtk::Window* window);
	void on_menu_file_new_generic();
	void on_menu_file_quit();
	void on_menu_help_about();
};
