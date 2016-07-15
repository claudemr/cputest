Build for ARM target:

```/opt/arm-2009q1/bin/arm-none-linux-gnueabi-gcc -c Probe_arm.S```

```/opt/arm-2009q1/bin/arm-none-linux-gnueabi-gcc CpuInfo.c Cpu.cpp CpuArm.cpp CpuLinux.cpp CycleCounter.cpp Dispatch.cpp Init.cpp Strings.cpp Unsafe.cpp Probe_arm.o -D__LITTLE_ENDIAN__ -o cpuinfo```

```/opt/arm-2009q1/bin/arm-none-linux-gnueabi-gcc CpuInfoEx.c Cpu.cpp CpuArm.cpp CpuLinux.cpp CycleCounter.cpp Dispatch.cpp Init.cpp Strings.cpp Unsafe.cpp Probe_arm.o -D__LITTLE_ENDIAN__ -o cpuinfoex```
