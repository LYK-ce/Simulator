set environment AS_SEND_LAT=3
set environment AS_NVLS_ENABLE=1
file ./bin/SimAI_simulator
set args -t 16 -w ./example/microAllReduce.txt -n ./Spectrum-X_8g_8gps_400Gbps_H100 -c astra-sim-alibabacloud/inputs/config/SimAI.conf
