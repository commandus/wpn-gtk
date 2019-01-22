#include "client-env.h"

ClientEnv::ClientEnv
(
)
	: provider(PROVIDER_CHROME), clientFileName(""), hasClientFileName(false), isClientFileModified(false)
{
	
}

bool ClientEnv::newClientFile()
{
	clientFileName = "";
	hasClientFileName = false;
	isClientFileModified = true;
	return true;
}

bool ClientEnv::openClientFile(const std::string &fileName)
{
	bool r = true;
	hasClientFileName = r;
	isClientFileModified = false;
	return r;
}

bool ClientEnv::saveClientFile()
{
	if (!hasClientFileName)
		return false;
	return true;
}

bool ClientEnv::saveAsClientFile(const std::string &fileName)
{
	clientFileName = fileName;
	hasClientFileName = true;
	isClientFileModified = false;
	return true;
}

bool ClientEnv::read()
{
	if (!hasClientFileName)
		return false;
	std::string registrationId;
	std::string lastPersistentId;
	std::string privateKey;
	std::string publicKey;
	std::string authSecret;
	uint64_t androidId;
	uint64_t securityToken;
	std::string appId;
	
	// load config file
	int r = readConfig
	(
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
