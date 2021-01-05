#pragma once
#include <string>
#include <vector>
#include <functional>
#include "Engine/Core/NamedStrings.hpp"
#include "Engine/Core/NamedProperties.hpp"

//--------------------------------------------------------------------------------------------------------------------------------------------

//typedef void( *EventCallbackFunctionPtrType )( );
typedef NamedStrings EventArgs;
typedef bool( *EventCallbackFunction )( EventArgs& args );

//--------------------------------------------------------------------------------------------------------------------------------------------

struct EventSubscription
{
	std::string m_eventName;
	EventCallbackFunction m_callbackFunctionPtr = nullptr;
};

struct MethodSubscriptions
{
public:
	std::string eventName;
	void* objId = nullptr;
	std::function<bool( EventArgs& )> func = nullptr;
};

//--------------------------------------------------------------------------------------------------------------------------------------------

class EventSystem
{
public:
	EventSystem() {};
	~EventSystem() {};

	void Startup();
	void BeginFrame();
	void EndFrame();
	void Shutdown();

	void SubscribeToEvent( const std::string& eventName , EventCallbackFunction eventToListen );
	void UnSubscribeEvent( const std::string& eventName , EventCallbackFunction eventToUnscribe );
	bool FireEvent(const std::string& eventName , EventArgs& defaultArguments );

	template<typename OBJ_TYPE>
	void SubscribeToMethod( const std::string& eventName , OBJ_TYPE* obj , bool ( OBJ_TYPE::* mcb )( EventArgs& args ) )
	{
		MethodSubscriptions* newSubscription = new MethodSubscriptions();
		newSubscription->eventName = eventName;
		newSubscription->objId = obj;
		newSubscription->func = [ = ] ( EventArgs& args ) -> bool { return ( obj->*mcb )( args ); };
		m_methodSubscriptions.push_back( newSubscription );
	};
	
	template<typename OBJ_TYPE>
	void UnsubscribeMethod( const std::string& eventName , OBJ_TYPE* obj )
	{
		for ( int i = 0; i < m_methodSubscriptions.size(); i++ )
		{
			if ( m_methodSubscriptions[ i ] != nullptr )
			{
				if ( m_methodSubscriptions[ i ]->eventName == eventName )
				{
					if ( m_methodSubscriptions[ i ]->objId == obj )
					{
						delete m_methodSubscriptions[ i ];
						m_methodSubscriptions[ i ] = nullptr;
					}
				}
			}
		}
	}

	//--------------------------------------------------------------------------------------------------------------------------------------------

private:
	std::vector<EventSubscription*> m_eventSubscriptions;
	std::vector<MethodSubscriptions*> m_methodSubscriptions;
};

//--------------------------------------------------------------------------------------------------------------------------------------------