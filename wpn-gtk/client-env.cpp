#include "client-env.h"
#include "utilrecv.h"
#include "utilinstance.h"
#include "endpoint.h"

ClientEnv::ClientEnv
(
)
	: verbosity(0), lastError(0), lastHttpCode(0), 
	registrationId(""), lastPersistentId(""), privateKey(""), 
	publicKey(""), authSecret(""), androidId(0), securityToken(0), appId(""), 
	client(NULL),
	provider(PROVIDER_CHROME), clientFileName(""), hasClientFileName(false), isClientFileModified(false)
{
	
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
	bool r = read();
	if (!r) {
		hasClientFileName = false;
		isClientFileModified = true;
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

bool ClientEnv::read()
{
	if (!hasClientFileName)
		return false;
	// load config file
	int r = readConfig(
		clientFileName,
		provider,
		registrationId,
		privateKey,
		publicKey,
		authSecret,
		androidId,
		securityToken,
		appId,
		lastPersistentId
	);
	return r == 0;
}

bool ClientEnv::genNew()
{
	appId = mkInstanceId();
	// Initialize client
	lastHttpCode = initClient(registrationId, privateKey, publicKey, authSecret, &androidId, &securityToken, appId, verbosity);
	return  ((lastHttpCode >= 200) && (lastHttpCode <= 300));
}

bool ClientEnv::save()
{
	if (!hasClientFileName)
		return false;	// save 
	lastError = writeConfig(
		clientFileName,
		provider,
		registrationId.c_str(),
		privateKey.c_str(),
		publicKey.c_str(),
		authSecret.c_str(),
		androidId,
		securityToken,
		appId,
		lastPersistentId
	);
	if (lastError > 0)
		lastError = 0;
	return (lastError >= 0);
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
}

void onLog
(
	void *env,
	int severity,
	const char *message
)
{
	std::cerr << message;
}

bool ClientEnv::start()
{
	if (isRunning())
		stop();
	client = (MCSClient*) startClient(
		&lastError,
		lastPersistentId,
		privateKey,
		authSecret,
		androidId,
		securityToken,
		onNotify,
		NULL,
		onLog,
		NULL,
		verbosity
	);
}

bool ClientEnv::stop()
{
	if (!client)
		return false;
	stopClient(client);
	client = NULL;
}

bool ClientEnv::isRunning()
{
	return client != NULL;
}
