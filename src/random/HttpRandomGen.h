#pragma once
#include "RandomGenProvider.h"

class HttpRandomGen :
	public RandomGenProvider
{
	const static int bufferSize = 101;

	std::string httpServer;
	std::string httpPath;
	std::string httpResponse;
	int sequence[bufferSize];
	int used;
	bool isFull;

	void makeHttpCall();
	void parseResponse();

public:
	HttpRandomGen(void);
	~HttpRandomGen(void);

	int getRandom(int min, int max);
};

