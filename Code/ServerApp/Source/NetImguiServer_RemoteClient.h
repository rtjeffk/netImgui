#pragma once

#include <vector>
#include <chrono>
#include <Private/NetImgui_CmdPackets.h>

namespace NetImguiServer { namespace RemoteClient
{

struct TextureEntry
{
	uint64_t	mImguiId;
	void*		mpHAL_Texture;
};

struct Client
{	
	static constexpr uint32_t kInvalidClient = static_cast<uint32_t>(-1);
	using ExchPtrFrame	= NetImgui::Internal::ExchangePtr<NetImgui::Internal::CmdDrawFrame>;
	using ExchPtrInput	= NetImgui::Internal::ExchangePtr<NetImgui::Internal::CmdInput>;

											Client();
											~Client();
											Client(const Client&)	= delete;
											Client(const Client&&)	= delete;
	void									operator=(const Client&) = delete;

	void									Reset();

	void									ReceiveTexture(NetImgui::Internal::CmdTexture*);	
	void									ReceiveDrawFrame(NetImgui::Internal::CmdDrawFrame*);	
	NetImgui::Internal::CmdDrawFrame*		TakeDrawFrame();
		
	void									CaptureImguiInput();
	NetImgui::Internal::CmdInput*			TakePendingInput();
	

	void*									mpHAL_AreaRT			= nullptr;
	void*									mpHAL_AreaTexture		= nullptr;
	uint16_t								mAreaRTSizeX			= 0;	// Currently allocated RenderTarget size
	uint16_t								mAreaRTSizeY			= 0;
	uint16_t								mAreaSizeX				= 0;	// Available area size available to remote client
	uint16_t								mAreaSizeY				= 0;	
	char									mInfoName[128]			= {0};
	char									mWindowID[128+16]		= {0};
	char									mInfoImguiVerName[16]	= {0};
	char									mInfoNetImguiVerName[16]= {0};
	uint32_t								mInfoImguiVerID			= 0;
	uint32_t								mInfoNetImguiVerID		= 0;
	char									mConnectHost[64]		= {0};	//!< Connected Hostname of this remote client
	int										mConnectPort;					//!< Connected Port of this remote client
	NetImgui::Internal::CmdDrawFrame*		mpFrameDraw;					//!< Current valid DrawFrame
	std::vector<TextureEntry>				mvTextures;						//!< List of textures received and used by the client	
	ExchPtrFrame							mPendingFrame;					//!< Frame received and waiting to be displayed
	ExchPtrInput							mPendingInput;					//!< Input command waiting to be sent out to client
	bool									mbIsActive;						//!< If currently selected client for display
	std::atomic_bool						mbIsFree;						//!< If available to use for a new connected client
	std::atomic_bool						mbIsConnected;					//!< If connected to a remote client
	std::atomic_bool						mbPendingDisconnect;			//!< Server requested a disconnect on this item
	std::chrono::steady_clock::time_point	mConnectedTime;					//!< When the connection was established with this remote client
	uint32_t								mClientConfigID;				//!< ID of ClientConfig that connected (if connection came from our list of ClientConfigs)	
	uint32_t								mClientIndex;					//!< Entry idx into table of connected clients
	float									mMousePos[2]		= {0,0};
	float									mMouseWheelPos[2]	= {0,0};
	ImGuiMouseCursor						mMouseCursor		= ImGuiMouseCursor_None;	// Last mosue cursor remote client requested

	static bool								Startup(uint32_t clientCountMax);
	static void								Shutdown();
	static uint32_t							GetCountMax();
	static uint32_t							GetFreeIndex();
	static Client&					Get(uint32_t index);
};

}} // namespace NetImguiServer { namespace Client