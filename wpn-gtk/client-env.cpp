#include "client-env.h"
#include "utilrecv.h"
#include "utilinstance.h"
#include "endpoint.h"

#define	EMPTY_LAST_PERSISTENT_ID		""

#define WPNLIB 1
#ifndef WPNLIB
void *startClient
(
	int *retcode,
	const std::string &lastPersistentId,
	const std::string &privateKey,
	const std::string &authSecret,
	uint64_t androidId,
	uint64_t securityToken,
	OnNotifyC onNotify,
	void *onNotifyEnv,
	OnLogC onLog,
	void *onLogEnv,
	int verbosity
)
{
	return NULL;
}

/**
 * Stop client
 */
void stopClient
(
	void *client
)
{
}

/**
 * Load config file
 * @return 0- success, -1: Invalid JSON, -2: Invalid config
 */
int readConfig
(
	const std::string &filename,
	enum VAPID_PROVIDER &provider,
	std::string &registrationId,
	std::string &privateKey,
	std::string &publicKey,
	std::string &authSecret,
	uint64_t &androidId,
	uint64_t &securityToken,
	std::string &appId,
	std::string &lastPersistentId
)
{
	return 0;
}

int writeConfig
(
	const std::string &filename,
	enum VAPID_PROVIDER provider,
 	const char* registrationIdC,EMPTY_LAST_PERSISTENT_ID
	const char* privateKeyC,
	const char* publicKeyC,
	const char* authSecretC,
	uint64_t androidId,
	uint64_t securityToken,
	const std::string &appId,
	const std::string &lastPersistentId
)
{
	return 0;
}

/**
 * Calls generateVAPIDKeys() and checkIn()
 * @return from checkIn()
 */
int initClient
(
	std::string &retRegistrationId,
	std::string &privateKey,
	std::string &publicKey,
	std::string &authSecret,
	uint64_t *androidId,
	uint64_t *securityToken,
	std::string &appId,
	int verbosity
)
{
	return 0;
}

std::string mkInstanceId()
{
	return "";
}

#endif

ClientEnv::ClientEnv
(
)
	: verbosity(0), lastError(0), lastHttpCode(0), 
	onLog(NULL),
	config(NULL), 
	client(NULL),
	provider(PROVIDER_CHROME), clientFileName(""), hasClientFileName(false), isClientFileModified(false)
{
}

ClientEnv::~ClientEnv
(
)
{
	stop();
}

bool ClientEnv::newClientFile()
{
	clientFileName = "";
	hasClientFileName = false;
	isClientFileModified = true;
	// generate a new application name. Is it required?
	bool r = genNew();
	if (r)
		start();
	return r;
}

bool ClientEnv::openClientFile(const std::string &fileName)
{
	clientFileName = fileName;
	bool r = read();
	if (!r) {
		hasClientFileName = false;
		isClientFileModified = true;
		clientFileName = "";
		return false;
	}
	start();
	hasClientFileName = true;
	isClientFileModified = false;
	return true;
}

bool ClientEnv::saveClientFile()
{
	if (!hasClientFileName)
		return false;
	return save();
}

bool ClientEnv::saveAsClientFile(const std::string &fileName)
{
	clientFileName = fileName;
	if (save()) {
		hasClientFileName = true;
		isClientFileModified = false;
	} else {
		hasClientFileName = false;
	}
	return true;
}

/**
 * 	std::string registrationId;
	std::string lastPersistentId;
	std::string privateKey;
	std::string publicKey;
	std::string authSecret;
	uint64_t androidId;
	uint64_t securityToken;
	std::string appId;

 */
bool ClientEnv::read()
{
	// load config file
	config = new ConfigFile(clientFileName);	
	return true;
}

bool ClientEnv::genNew()
{
	if (config) {
		delete config;
		config = NULL;
	}
	config = new ConfigFile("");
}

bool ClientEnv::save()
{
	if (!hasClientFileName)
		return false;
	if (!config)
		return false;
	config->save();
	return true;
}

void onNotify
(
	void *env,
	const char *persistent_id,
	const char *from,				///< e.g. BDOU99-h67HcA6JeFXHbSNMu7e2yNNu3RzoMj8TM4W88jITfq7ZmPvIM1Iv-4_l2LxQcYwhqby2xGpWwzjfAnG4
	const char *appName,
	const char *appId,
	int64_t sent,
	const NotifyMessageC *msg
)
{
	time_t t = sent / 1000;
	struct tm *tm = localtime(&t);
	std::cerr<< "Notify " << "persistent_id: " << persistent_id << std::endl
		<< "from: " << from << std::endl
		<< "sent: " << std::asctime(tm) << std::endl
		<< std::endl << std::endl;
	if (msg) {
		std::cout 
		<< msg->title << std::endl
		<< msg->category << std::endl
		<< msg->extra << std::endl
		<< msg->icon << std::endl
		<< msg->link << std::endl
		<< msg->linkType << std::endl
		<< msg->sound << std::endl
		<< msg->timeout << std::endl
		<< msg->urgency << std::endl
		<< msg->body << std::endl
		<< std::endl;
	}

	ClientEnv* c = (ClientEnv*) env;
	if (c->onNotify)
		c->onNotify(persistent_id, from, appName, appId, sent, msg);
}

void onLogc
(
	void *env,
	int severity,
	const char *message
)
{
	ClientEnv* c = (ClientEnv*) env;
	if (c->onLog)
		c->onLog(severity, message);
}

void onNotifyc
(
	void *env,
	const char *persistent_id,
	const char *from,
	const char *appName,
	const char *appId,
	int64_t sent,
	const NotifyMessageC *msg
)
{
	ClientEnv* c = (ClientEnv*) env;
	if (c->onNotify)
		c->onNotify(persistent_id,
			from,
			appName,
			appId,
			sent,
			msg
		);
}

void ClientEnv::addLogHandler(std::function<void(int, const char *)> handler)
{
	onLog = handler;
}

void ClientEnv::addNotifyHandler(std::function<void(
		const char *persistent_id,
		const char *from,
		const char *appName,
		const char *appId,
		int64_t sent,
		const NotifyMessageC *msg
	)> handler)
{
	onNotify = handler;
}

bool ClientEnv::start()
{
	if (isRunning())
		stop();
	if (!config)
		return false;
	client = (MCSClient*) startClient(
		&lastError,
		EMPTY_LAST_PERSISTENT_ID,
		config->wpnKeys->getPrivateKey(),
		config->wpnKeys->getAuthSecret(),
		config->androidCredentials->getAndroidId(),
		config->androidCredentials->getSecurityToken(),
		onNotifyc, this, onLogc, this,
		verbosity
	);
	std::cerr << "Client started " << client << std::endl;
	return true;
}

bool ClientEnv::stop()
{
	if (!client)
		return true;
	stopClient(client);
	client = NULL;
}

bool ClientEnv::isRunning()
{
	return client != NULL;
}
