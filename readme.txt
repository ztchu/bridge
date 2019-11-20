The content of this file is about how to use bridge.exe.

1. If you want to capture desktop and multicast by udp, you should call the program like this:
bridge.exe -start -json_file ../config.ini -protocol 0 -video_source 0 -local_ip 172.18.146.20 -local_port 9999 -multicast_ip 239.0.0.200 -multicast_port 5004

config.ini is a json file about video parameters, for example:
{
	"ScreenWidth":640,
	"ScreenHeight":360,
	"MaxBandWidth":600,
	"AvgBitrateCoef":0.6,
	"MaxBitrateCoef":0.8,
	"Fps":15,
	"KeyFrameTimes":0.2,
}

2. If you want to push stream by 1602's server, you should call the program like this:
bridge.exe -start -protocol 5 -video_source 4

3. If you want to stop the stream, you should call this program like this:
bridge.exe -stop