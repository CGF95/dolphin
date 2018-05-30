// Copyright 2008 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once

#include <array>
#include <memory>

#include <SFML/Network.hpp>

#include "Common/CommonTypes.h"
#include "Core/HW/SI/SI_Device.h"

// GameBoy Advance "Link Cable"

namespace SerialInterface
{
u8 GetNumConnected();
int GetTransferTime(u8 cmd);
void GBAConnectionWaiter_Shutdown();

class GBASockServer
{
public:
  GBASockServer(int device_number);
  ~GBASockServer();
  void Disconnect();
  bool Connect();
  bool IsConnected();

  void ClockSync();

  void Send(const u8* si_buffer);
  int Receive(u8* si_buffer);

  //Dragonbane: Fake GBA
  int CreateFakeResponse(u8* si_buffer);

private:
  std::unique_ptr<sf::TcpSocket> m_client;
  std::unique_ptr<sf::TcpSocket> m_clock_sync;
  unsigned char send_data[5];
  unsigned char recv_data[5];

  u64 m_last_time_slice = 0;
  u64 time_cmd_sent;
  u8 device_number;
  u8 cmd;
  bool m_booted = false;
};

class CSIDevice_GBA : public ISIDevice, private GBASockServer
{
public:
	CSIDevice_GBA(SIDevices device, int device_number);

	int RunBuffer(u8* buffer, int length) override;
	int TransferInterval() override;
	bool GetData(u32& hi, u32& low) override;
	void SendCommand(u32 command, u8 poll) override;

	void DoState(PointerWrap& p) override;

private:
  u8 send_data[5];
  int num_data_received;
  u64 timestamp_sent;
  bool waiting_for_response;
  enum class NextAction
  {
    SendCommand,
    WaitTransferTime,
    ReceiveResponse
  };
  //CGF: Needed to comment out, or it'll error out.. lol
  //GBASockServer m_sock_server;
  NextAction m_next_action = NextAction::SendCommand;
  u8 m_last_cmd;
  u64 m_timestamp_sent = 0;
};
}  // namespace SerialInterface
