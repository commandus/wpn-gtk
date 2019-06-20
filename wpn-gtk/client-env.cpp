#include "client-env.h"
#include "utilrecv.h"
#include "utilinstance.h"
#include "endpoint.h"

#define	EMPTY_LAST_PERSISTENT_ID		""

ClientEnv::ClientEnv()
	: lastError(0),
	client(NULL),
	config(NULL), 
	onLog(NULL),
	onNotify(NULL),
	hasClientFileName(false), isClientFileModified(false)
{
}

ClientEnv::ClientEnv
(
	const std::string &fileName
)
	: lastError(0),
	client(NULL),
	config(NULL), 
	onLog(NULL),
	onNotify(NULL),
	hasClientFileName(false), isClientFileModified(false)
{
	openClientFile(fileName);
}

ClientEnv::~ClientEnv
(
)
{
	stop();
}

bool ClientEnv::newClientFile()
{
	hasClientFileName = false;
	isClientFileModified = true;
	// generate a new application name. Is it required?
	bool r = genNew();
	if (r)
		start();
	return r;
}

bool ClientEnv::openClientFile(
	const std::string &fileName
)
{
	if (config) {
		delete config;
		config = NULL;
	}

	config = new ConfigFile(fileName);	
	if (!config)
		return false;
	start();
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
	if (save()) {
		hasClientFileName = true;
		isClientFileModified = false;
	} else {
		hasClientFileName = false;
	}
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
	ClientEnv* c = (ClientEnv*) env;
	if (c && c->config->clientOptions->getVerbosity()) {
		std::cerr << "Client started, configuration file " << c->config->fileName << std::endl
			<< "Notify " << "persistent_id: " << persistent_id << std::endl
			<< "from: " << from << std::endl
			<< "sent: " << std::asctime(tm) << std::endl
			<< std::endl << std::endl;
		if (msg) {
			std::cerr 
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
	}

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
		config->clientOptions->getVerbosity()
	);
	if (config->clientOptions->getVerbosity())
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
