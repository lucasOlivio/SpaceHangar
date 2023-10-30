#pragma once

class iEvent;

class iListener
{
public:
	virtual ~iListener() {};

	virtual void Notify(iEvent* pEvent) = 0;
};