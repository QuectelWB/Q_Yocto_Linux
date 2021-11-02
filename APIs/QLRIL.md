QLRILD
======

https://www.it610.com/article/1304147824019017728.htm
<br>
https://blog.csdn.net/fulinus/article/details/107960128
<br>
<br>
QLRIL接口支持通话、短信、网络、GNSS和AT命令查询等功能。由于采用了双RIL ( Radio Interface Layer ) 的架构，两个QCRILD守护进程分别负责对接SIM 1卡和SIM 2卡的交互，因此支持双卡双待。QLRILD守护进程负责统筹管理双RIL服务、GNSS服务和AT命令服务，也为了方便应用层使用对上提供统一的QLRIL APIs，参考下面的示意图（QLRIL进程关系图）所示。

[](https://img.it610.com/image/info8/2f716c8e0cd0492183be978c533c2a42.jpg)
<br>
高通msm8909/msm8953 Linux系统首次实现双卡双待功能:
	
		
	/ # qlril-api-test
	QLRIL_Init success
	Copyright © 2020 Quectel, Smart Linux
	Group Name:QL RIL API, Supported test cases:
	0: Help, Show all the API
	1: QLRIL_Init
	2: QLRIL_Exit
	3: QLRIL_GetVersion
	4: QLRIL_GetOperator
	5: QLRIL_GetCurrentCalls
	6: QLRIL_GetSimCardSlotId
	7: QLRIL_SetSimCardSlotId
	8: QLRIL_Dial
	9: QLRIL_SendSms
	10: QLRIL_SendSmsByPDU
	11: QLRIL_AcceptCall
	12: QLRIL_RejectCall
	13: QLRIL_SetupDataCall
	14: QLRIL_GetVoiceRegistrationState
	15: QLRIL_SetDataAllowed
	16: QLRIL_GetPreferredNetworkType
	17: QLRIL_SetPreferredNetworkType
	18: QLRIL_GetIccCardStatus
	19: QLRIL_SupplyIccPin
	20: QLRIL_SupplyIccPuk
	21: QLRIL_ChangeIccPin
	22: QLRIL_GetIMSI
	23: QLRIL_GetIMSIForApp
	24: QLRIL_GetDataCallList
	25: QLRIL_DeactivateDataCall
	26: QLRIL_SetRadioPower
	27: QLRIL_RequestShutdown
	28: QLRIL_GetVoiceRadioTechnology
	29: QLRIL_GetImsRegistrationState
	30: QLRIL_GetIMEI
	31: QLRIL_HangupConnection
	32: QLRIL_HangupWaitingOrBackground
	33: QLRIL_HangupForegroundResumeBackground
	34: QLRIL_GetMute
	35: QLRIL_SetMute
	36: QLRIL_GetSignalStrength
	37: QLRIL_SendAtCmd
	38: QLRIL_GetSIMPhoneNumber
	39: QLRIL_GetDeviceIdentity
	40: QLRIL_SetScreenState
	41: QLRIL_GetDataRegistrationState
	42: QLRIL_ResetQcrild
	50: QLRIL_RegisterEventsListener
	51: QLRIL_UnregisterEventsListener
	52: QLRIL_RegisterUnsolEvents
	53: QLRIL_UnregisterUnsolEvents
	54: QLRIL_GNSS_AddListener
	55: QLRIL_GNSS_DelListener
	56: QLRIL_GNSS_SetAttribute
	57: QLRIL_GNSS_RegisterEvents
	58: QLRIL_GNSS_UnregisterEvents
	59: QLRIL_GNSS_StartNavigation
	60: QLRIL_GNSS_StopNavigation
	61: QLRIL_GNSS_GetLocation
	62: Run the GNSS API test
	-1: Exit!

	Please input cmd index (-1: exit, 0: help):4
	ONS: CHN-UNICOM
	EONS: UNICOM
	MCCMNC: 46001

	Please input cmd index (-1: exit, 0: help):13
	Input an character: y - default; n - others;
	y
	setupDataCall response:
	status:0, retry:-1, callID:0, active:up
	type:IP, ifname:rmnet_data0, addresses:10.78.153.37/30, dnses:218.104.78.2 58.242.2.2
	gateways:10.78.153.38,pcscf:,mtu:1500

	QLRIL_SetupDataCall success

	Please input cmd index (-1: exit, 0: help):



