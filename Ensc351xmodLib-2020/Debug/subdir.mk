################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../PeerX.cpp \
../ReceiverSS.cpp \
../ReceiverX.cpp \
../SenderSS.cpp \
../SenderX.cpp \
../myIO.cpp \
../terminal.cpp 

C_SRCS += \
../crc.c 

OBJS += \
./PeerX.o \
./ReceiverSS.o \
./ReceiverX.o \
./SenderSS.o \
./SenderX.o \
./crc.o \
./myIO.o \
./terminal.o 

CPP_DEPS += \
./PeerX.d \
./ReceiverSS.d \
./ReceiverX.d \
./SenderSS.d \
./SenderX.d \
./myIO.d \
./terminal.d 

C_DEPS += \
./crc.d 


# Each subdirectory must supply rules for building sources it contributes
PeerX.o: ../PeerX.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++1y -I"/mnt/hgfs/VMsf2020/git/ensc351lib/Ensc351" -O0 -g3 -Wall -c -fmessage-length=0 -Wno-deprecated -MMD -MP -MF"$(@:%.o=%.d)" -MT"PeerX.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

ReceiverSS.o: ../ReceiverSS.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++1y -I"/mnt/hgfs/VMsf2020/git/ensc351lib/Ensc351" -O0 -g3 -Wall -c -fmessage-length=0 -Wno-unused-variable -Wno-unknown-pragmas -MMD -MP -MF"$(@:%.o=%.d)" -MT"ReceiverSS.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++1y -I"/mnt/hgfs/VMsf2020/git/ensc351lib/Ensc351" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

SenderSS.o: ../SenderSS.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++1y -I"/mnt/hgfs/VMsf2020/git/ensc351lib/Ensc351" -O0 -g3 -Wall -c -fmessage-length=0 -Wno-unused-variable -Wno-unknown-pragmas -MMD -MP -MF"$(@:%.o=%.d)" -MT"SenderSS.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I"/mnt/hgfs/VMsf2020/git/ensc351lib/Ensc351" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

myIO.o: ../myIO.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++17 -I"/mnt/hgfs/VMsf2020/git/ensc351lib/Ensc351" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"myIO.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


