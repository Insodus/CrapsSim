#pragma once
class RandomGenProvider
{
public:
	RandomGenProvider(void);
	~RandomGenProvider(void);

	virtual int getRandom(int min, int max) = 0;
};

