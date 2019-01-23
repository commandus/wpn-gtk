#ifndef CLIENT_ENV_H_
#define CLIENT_ENV_H_	1
#include <string>
#include <gtkmm.h>

#include "utilfile.h"
#include <mcs/mcsclient.h>

class ClientEnv: public Glib::Object {
private:
	int verbosity;
	int lastHttpCode;
	int lastError;
protected:
	std::string registrationId;
	std::string lastPersistentId;
	std::string privateKey;
	std::string publicKey;
	std::string authSecret;
	uint64_t androidId;
	uint64_t securityToken;
	std::string appId;
	MCSClient *client;
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
	bool genNew();
	bool read();
	bool save();
	bool start();
	bool stop();
	bool isRunning();
};

#endif
