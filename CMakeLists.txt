cmake_minimum_required(VERSION 3.10)

project(sniffer)

add_executable(${PROJECT_NAME}
		main.cpp
        NetWatcher.cpp )

if(NOT WIN32)
	target_link_libraries(${PROJECT_NAME} 
		-lPcap++
		-lPacket++
		-lCommon++
		-lpcap
		-lpthread)
else()
	target_link_libraries(${PROJECT_NAME} 
		Pcap++
		Packet++
		Common++
		wpcap
		wsock32
		ws2_32)
endif()

