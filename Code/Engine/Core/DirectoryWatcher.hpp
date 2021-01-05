#pragma once

class DirectoryWatcher
{
public:
	DirectoryWatcher();
	~DirectoryWatcher();

	void Startup();
	void BeginFrame();
	void Update( float deltaSeconds );
	void EndFrame();
	void Shutdown();

	//void AddDirectoryToWatchList( LPCTSTR directory , BOOL watchSubtree , DWORD notificationFilters , DWORD bufferSize = 16384 );

private:

};
