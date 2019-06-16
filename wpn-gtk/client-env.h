#ifndef CLIENT_ENV_H_
#define CLIENT_ENV_H_	1
#include <functional>
#include <string>
#include <gtkmm.h>

#include "utilfile.h"
#include "wp-storage-file.h"
#include "mcs/mcsclient.h"

class ClientEnv: public Glib::Object {
private:
	int verbosity;
	int lastHttpCode;
	int lastError;
protected:
	ConfigFile *config;
	MCSClient *client;
public:
	ClientEnv();
	~ClientEnv();
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
	void addLogHandler(std::function<void(int, const char *)> handler);
	void addNotifyHandler(std::function<void(
		const char *persistent_id,
		const char *from,
		const char *appName,
		const char *appId,
		int64_t sent,
		const NotifyMessageC *msg
	)> handler);
	std::function<void(int, const char *)> onLog;
	std::function<void(
		const char *persistent_id,
		const char *from,
		const char *appName,
		const char *appId,
		int64_t sent,
		const NotifyMessageC *msg
	)> onNotify;
};

#endif
