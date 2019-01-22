#ifndef CLIENT_ENV_H_
#define CLIENT_ENV_H_	1
#include <string>
#include <gtkmm.h>

#include "utilfile.h"

class ClientEnv: public Glib::Object {
public:
	ClientEnv();
	enum VAPID_PROVIDER provider;
	std::string clientFileName;
	bool hasClientFileName;
	bool isClientFileModified;
	bool newClientFile();
	bool openClientFile(const std::string &fileName);
	bool saveClientFile();
	bool saveAsClientFile(const std::string &fileName);
	bool read();
};

#endif
