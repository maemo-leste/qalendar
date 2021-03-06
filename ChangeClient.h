#ifndef CHANGECLIENT_H
#define CHANGECLIENT_H

#include "ChangeManager.h"

class ChangeClient
{
public:
    ChangeClient() :
        version(0)
    {
    }

    virtual ~ChangeClient()
    {
        deactivate();
    }

    // Check if this client should be updated
    virtual bool isOutdated()
    {
        return version != ChangeManager::version();
    }

    // The change manager will call this when an update might be required
    virtual void onChange() = 0;

protected:
    // Make a note of the point at which this client last updated its data
    void sync()
    {
        version = ChangeManager::version();
    }

    // Register with the change manager to receive updates
    void activate()
    {
        ChangeManager::activateClient(this);
    }

    // Tell the change manager that we no longer want to receive updates
    void deactivate()
    {
        ChangeManager::deactivateClient(this);
    }

private:
    Version version;
};

#endif // CHANGECLIENT_H
