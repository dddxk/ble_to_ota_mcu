
# make 编译并下载
# make VERBOSE=1 显示编译详细过程
# make clean 清除编译临时文件
#
# 注意： Linux 下编译方式：
#     1. 从 http://pkgman.jieliapp.com/doc/all 处找到下载链接
#     2. 下载后，解压到 /opt/jieli 目录下，保证
#       /opt/jieli/common/bin/clang 存在（注意目录层次）
#     3. 确认 ulimit -n 的结果足够大（建议大于8096），否则链接可能会因为打开文件太多而失败
#       可以通过 ulimit -n 8096 来设置一个较大的值
#

# 工具路径设置
ifeq ($(OS), Windows_NT)
# Windows 下工具链位置
TOOL_DIR := C:/JL/pi32/bin
CC    := clang.exe
CXX   := clang.exe
LD    := q32s-lto-wrapper.exe
AR    := q32s-lto-ar.exe
MKDIR := mkdir_win -p
RM    := rm -rf

SYS_LIB_DIR := C:/JL/pi32/q32s-lib
SYS_INC_DIR := C:/JL/pi32/q32s-include
EXT_CFLAGS  := # Windows 下不需要 -D__SHELL__
export PATH:=$(TOOL_DIR);$(PATH)

## 后处理脚本
FIXBAT          := tools\utils\fixbat.exe # 用于处理 utf8->gbk 编码问题
POST_SCRIPT     := cpu\bd19\tools\download.bat
RUN_POST_SCRIPT := $(POST_SCRIPT)
else
# Linux 下工具链位置
TOOL_DIR := /opt/jieli/q32s/bin
CC    := clang
CXX   := clang++
LD    := lto-wrapper
AR    := lto-ar
MKDIR := mkdir -p
RM    := rm -rf
export OBJDUMP := $(TOOL_DIR)/objdump
export OBJCOPY := $(TOOL_DIR)/objcopy
export OBJSIZEDUMP := $(TOOL_DIR)/objsizedump

SYS_LIB_DIR := $(TOOL_DIR)/../lib
SYS_INC_DIR := $(TOOL_DIR)/../include
EXT_CFLAGS  := -D__SHELL__ # Linux 下需要这个保证正确处理 download.c
export PATH:=$(TOOL_DIR):$(PATH)

## 后处理脚本
FIXBAT          := touch # Linux下不需要处理 bat 编码问题
POST_SCRIPT     := cpu/bd19/tools/download.sh
RUN_POST_SCRIPT := bash $(POST_SCRIPT)
endif

CC  := $(TOOL_DIR)/$(CC)
CXX := $(TOOL_DIR)/$(CXX)
LD  := $(TOOL_DIR)/$(LD)
AR  := $(TOOL_DIR)/$(AR)
# 输出文件设置
OUT_ELF   := cpu/bd19/tools/sdk.elf
OBJ_FILE  := $(OUT_ELF).objs.txt
# 编译路径设置
BUILD_DIR := objs

# 编译参数设置
CFLAGS := \
	-flto \
	-target q32s \
	-integrated-as \
	-fno-builtin \
	-mllvm -inline-threshold=5 \
	-Oz \
	-Os \
	-Wcast-align \
	-Wundef \
	-w \
	-g \
	-integrated-as \
	-O0 \
	-flto \
	-fallow-pointer-null \
	-Wincompatible-pointer-types \
	-fprefer-gnu-section \
	-Wframe-larger-than=256 \
	-Wreturn-type \
	-Wimplicit-function-declaration \
	-fms-extensions \


# C++额外的编译参数
CXXFLAGS :=


# 宏定义
DEFINES := \
	-DSUPPORT_MS_EXTENSIONS \
	-DCONFIG_RELEASE_ENABLE \
	-DCONFIG_CPU_BD19 \
	-DCONFIG_NEW_BREDR_ENABLE \
	-DCONFIG_FREE_RTOS_ENABLE \
	-DCONFIG_EQ_SUPPORT_ASYNC \
	-DEQ_DESIGN_IN_MASK \
	-DSINE_MAKE_IN_MASK \
	-DCONFIG_MMU_ENABLE \
	-DCONFIG_AEC_M=256 \
	-DCONFIG_AUDIO_ONCHIP \
	-DCONFIG_MEDIA_NEW_ENABLE \
	-D__GCC_Q32S__ \
	-DCONFIG_NEW_ECC_ENABLE \
	-DCONFIG_CRYPTO_TOOLBOX_OSIZE_IN_MASKROM \
	-DEVENT_HANDLER_NUM_CONFIG=2 \
	-DEVENT_TOUCH_ENABLE_CONFIG=0 \
	-DEVENT_POOL_SIZE_CONFIG=256 \
	-DCONFIG_EVENT_KEY_MAP_ENABLE=0 \
	-DTIMER_POOL_NUM_CONFIG=10 \
	-DAPP_ASYNC_POOL_NUM_CONFIG=0 \
	-DVFS_ENABLE=1 \
	-DUSE_SDFILE_NEW=1 \
	-DVFS_FILE_POOL_NUM_CONFIG=0 \
	-DSDFILE_VERSION=0x020000 \
	-DVM_MAX_SIZE_CONFIG=16*1024 \
	-DVM_ITEM_MAX_NUM=128 \
	-DCONFIG_TWS_ENABLE \
	-DCONFIG_SPP_AND_LE_CASE_ENABLE \
	-DCONFIG_TRANSFER_ENABLE \
	-DCONFIG_LITE_AEC_ENABLE=0 \
	-DCONFIG_LITE_AUDIO \
	-DCONFIG_LMP_RESET_ENABLE \
	-DCONFIG_LMP_REFRESH_ENCRYPTION_KEY_ENABLE \
	-DCONFIG_LINK_DISTURB_SCAN_ENABLE=0 \
	-DCONFIG_BTCTRLER_TASK_DEL_ENABLE \
	-DCONFIG_UPDATA_ENABLE \
	-DCONFIG_OTA_UPDATA_ENABLE \
	-DCONFIG_ITEM_FORMAT_VM \
	-DTCFG_APP_BT_EN=1 \
	-DUSB_PC_NO_APP_MODE \
	-D__LD__ \


DEFINES += $(EXT_CFLAGS) # 额外的一些定义

# 头文件搜索路径
INCLUDES := \
	-Iinclude_lib \
	-Iinclude_lib/driver \
	-Iinclude_lib/driver/device \
	-Iinclude_lib/driver/cpu/bd19 \
	-Iinclude_lib/system \
	-Iinclude_lib/system/generic \
	-Iinclude_lib/system/device \
	-Iinclude_lib/system/fs \
	-Iinclude_lib/system/ui \
	-Iinclude_lib/btctrler \
	-Iinclude_lib/btctrler/port/bd19 \
	-Iinclude_lib/update \
	-Iinclude_lib/agreement \
	-Iinclude_lib/btstack/third_party/common \
	-Iinclude_lib/btstack/third_party/rcsp \
	-Iinclude_lib/media/media_new \
	-Iinclude_lib/media/media_new/media \
	-Iinclude_lib/media/media_new/media/cpu/bd19 \
	-Iinclude_lib/media/media_new/media/cpu/bd19/asm \
	-Iapps/spp_and_le/include \
	-Iapps/spp_and_le/board/bd19 \
	-Iapps/common \
	-Iapps/common/device \
	-Iapps/common/include \
	-Iapps/common/power_manage \
	-Icpu/bd19 \
	-Iapps/common/third_party_profile/jieli \
	-Iapps/common/third_party_profile/jieli/JL_rcsp \
	-Iapps/common/third_party_profile/jieli/online_db \
	-Iapps/common/third_party_profile/common \
	-Iinclude_lib/ai_stack/JL_rcsp \
	-Iinclude_lib/btstack \
	-Iapps/common/third_party_profile/jieli/JL_rcsp/bt_trans_data \
	-Iapps/common/third_party_profile/jieli/JL_rcsp/rcsp_updata \
	-Iapps/common/third_party_profile/jieli/gatt_common \
	-Iapps/common/third_party_profile/Tecent_LL/include \
	-Iapps/common/third_party_profile/Tecent_LL/tecent_ll_demo \
	-Iapps/spp_and_le/examples/ll_sync \
	-Iapps/common/cJSON \
	-Iapps/common/third_party_profile/tuya_protocol \
	-Iapps/common/third_party_profile/tuya_protocol/app/demo \
	-Iapps/common/third_party_profile/tuya_protocol/app/product_test \
	-Iapps/common/third_party_profile/tuya_protocol/app/uart_common \
	-Iapps/common/third_party_profile/tuya_protocol/extern_components/mbedtls \
	-Iapps/common/third_party_profile/tuya_protocol/port \
	-Iapps/common/third_party_profile/tuya_protocol/sdk/include \
	-Iapps/common/third_party_profile/tuya_protocol/sdk/lib \
	-Iapps/spp_and_le/examples/tuya \
	-Iapps/common/third_party_profile/hilink_protocol \
	-Iapps/common/third_party_profile/hilink_protocol/mbedtls_protocol \
	-Iapps/common/third_party_profile/hilink_protocol/mbedtls_protocol/mbedtls \
	-Iapps/common/third_party_profile/hilink_protocol/mbedtls_protocol/mbedtls/configs \
	-Iapps/common/third_party_profile/hilink_protocol/mbedtls_protocol/mbedtls/psa \
	-Iapps/spp_and_le/examples/hilink \
	-Icpu/bd19/audio_encode \
	-Icpu/bd19/audio_decode \
	-Iapps/common/audio \
	-Iapps/common/device/usb \
	-Iapps/common/device/usb/device \
	-Iapps/common/device/usb/host \
	-Iapps/polyc_code \
	-Iapps/polyc_code/driver \
	-Iapps/user_code \
	-Iapps/OTA_code \
	-Iapps/user_code/TM1628A \
	-Iapps/user_code/key_drv \
	-Iapps/user_code/AHT20 \
	-I$(SYS_INC_DIR) \


# 需要编译的 .c 文件
c_SRC_FILES := \
	apps/OTA_code/polyc_buffer_manage.c \
	apps/OTA_code/xmodem.c \
	apps/polyc_code/driver/debug.c \
	apps/polyc_code/driver/virtual_mcu_ble_client.c \
	apps/polyc_code/driver/virtual_mcu_ble_nonconn_rx_tx.c \
	apps/polyc_code/driver/virtual_mcu_ble_server.c \
	apps/polyc_code/virtual_mcu_main.c \
	apps/user_code/AHT20/AHT20_commu.c \
	apps/user_code/TM1628A/driver_ic_1628.c \
	apps/user_code/key_drv/polyc_iokey.c \
	apps/user_code/user_mcu_adc.c \
	apps/user_code/user_mcu_ble_client.c \
	apps/user_code/user_mcu_ble_nonconn_rx_tx.c \
	apps/user_code/user_mcu_ble_server.c \
	apps/user_code/user_mcu_charge.c \
	apps/user_code/user_mcu_demo.c \
	apps/user_code/user_mcu_gpio.c \
	apps/user_code/user_mcu_iic_soft.c \
	apps/user_code/user_mcu_memory.c \
	apps/user_code/user_mcu_pulse_cnt.c \
	apps/user_code/user_mcu_pwm.c \
	apps/user_code/user_mcu_spi.c \
	apps/user_code/user_mcu_timer_cap.c \
	apps/user_code/user_mcu_timer_cap_433.c \
	apps/user_code/user_mcu_uart.c \
	apps/user_code/user_mcu_wakeup.c \


# 需要编译的 .S 文件
S_SRC_FILES :=


# 需要编译的 .s 文件
s_SRC_FILES :=


# 需要编译的 .cpp 文件
cpp_SRC_FILES :=


# 需要编译的 .cc 文件
cc_SRC_FILES :=


# 需要编译的 .cxx 文件
cxx_SRC_FILES :=


# 链接参数
LFLAGS := \
	--plugin-opt=-inline-threshold=5 \
	--plugin-opt=save-temps \
	--plugin-opt=-inline-normal-into-special-section=true \
	--plugin-opt=-dont-used-symbol-list=malloc,free,sprintf,printf,puts,putchar \
	--plugin-opt=-warn-stack-size=256 \
	--sort-common \
	--dont-complain-call-overflow \
	--gc-sections \
	--start-group \
	cpu/bd19/liba/cpu.a \
	cpu/bd19/liba/system.a \
	cpu/bd19/liba/btstack.a \
	cpu/bd19/liba/btctrler.a \
	cpu/bd19/liba/agreement.a \
	cpu/bd19/liba/rcsp_stack.a \
	cpu/bd19/liba/media.a \
	cpu/bd19/liba/media_app.a \
	cpu/bd19/liba/lc3_codec_lib.a \
	cpu/bd19/liba/usbc_codec_lib.a \
	cpu/bd19/liba/lib_adpcm_ima_code.a \
	cpu/bd19/liba/wav_dec_lib.a \
	cpu/bd19/liba/wtgv2_dec_lib.a \
	cpu/bd19/liba/opus_enc_lib.a \
	cpu/bd19/liba/update.a \
	cpu/bd19/liba/lib_polyc3.a \
	--end-group \
	-Tcpu/bd19/sdk.ld \
	-M=cpu/bd19/tools/sdk.map \
	-flto \
	--plugin-opt=-inline-threshold=5 \


LIBPATHS := \
	-L$(SYS_LIB_DIR) \


LIBS := \
	$(SYS_LIB_DIR)/libm.a \
	$(SYS_LIB_DIR)/libc.a \
	$(SYS_LIB_DIR)/libm.a \
	$(SYS_LIB_DIR)/libcompiler-rt.a \



c_OBJS    := $(c_SRC_FILES:%.c=%.c.o)
S_OBJS    := $(S_SRC_FILES:%.S=%.S.o)
s_OBJS    := $(s_SRC_FILES:%.s=%.s.o)
cpp_OBJS  := $(cpp_SRC_FILES:%.cpp=%.cpp.o)
cxx_OBJS  := $(cxx_SRC_FILES:%.cxx=%.cxx.o)
cc_OBJS   := $(cc_SRC_FILES:%.cc=%.cc.o)

OBJS      := $(c_OBJS) $(S_OBJS) $(s_OBJS) $(cpp_OBJS) $(cxx_OBJS) $(cc_OBJS)
DEP_FILES := $(OBJS:%.o=%.d)


OBJS      := $(addprefix $(BUILD_DIR)/, $(OBJS))
DEP_FILES := $(addprefix $(BUILD_DIR)/, $(DEP_FILES))


VERBOSE ?= 0
ifeq ($(VERBOSE), 1)
QUITE :=
else
QUITE := @
endif

# 一些旧的 make 不支持 file 函数，需要 make 的时候指定 LINK_AT=0 make
LINK_AT ?= 1

# 表示下面的不是一个文件的名字，无论是否存在 all, clean, pre_build 这样的文件
# 还是要执行命令
# see: https://www.gnu.org/software/make/manual/html_node/Phony-Targets.html
.PHONY: all clean pre_build

# 不要使用 make 预设置的规则
# see: https://www.gnu.org/software/make/manual/html_node/Suffix-Rules.html
.SUFFIXES:

all: pre_build $(OUT_ELF)
	$(info +POST-BUILD)
	$(QUITE) $(RUN_POST_SCRIPT) sdk
	$(QUITE) cpu\bd19\tools\download\data_trans\burnfile_1to8.bat

pre_build:
	$(info +PRE-BUILD)
	$(QUITE) $(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -E -P cpu/bd19/tools/download.c -o $(POST_SCRIPT)
	$(QUITE) $(FIXBAT) $(POST_SCRIPT)
	$(QUITE) $(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -E -P cpu/bd19/burnfile_1to8.c -o cpu/bd19/tools/download/data_trans/burnfile_1to8.bat

clean:
	$(QUITE) $(RM) $(OUT_ELF)
	$(QUITE) $(RM) $(BUILD_DIR)



ifeq ($(LINK_AT), 1)
$(OUT_ELF): $(OBJS)
	$(info +LINK $@)
	$(shell $(MKDIR) $(@D))
	$(file >$(OBJ_FILE), $(OBJS))
	$(QUITE) $(LD) -o $(OUT_ELF) @$(OBJ_FILE) $(LFLAGS) $(LIBPATHS) $(LIBS)
else
$(OUT_ELF): $(OBJS)
	$(info +LINK $@)
	$(shell $(MKDIR) $(@D))
	$(QUITE) $(LD) -o $(OUT_ELF) $(OBJS) $(LFLAGS) $(LIBPATHS) $(LIBS)
endif


$(BUILD_DIR)/%.c.o : %.c
	$(info +CC $<)
	@$(MKDIR) $(@D)
	@$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -MM -MT $@ $< -o $(@:.o=.d)
	$(QUITE) $(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/%.S.o : %.S
	$(info +AS $<)
	@$(MKDIR) $(@D)
	@$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -MM -MT $@ $< -o $(@:.o=.d)
	$(QUITE) $(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/%.s.o : %.s
	$(info +AS $<)
	@$(MKDIR) $(@D)
	@$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -MM -MT $@ $< -o $(@:.o=.d)
	$(QUITE) $(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/%.cpp.o : %.cpp
	$(info +CXX $<)
	@$(MKDIR) $(@D)
	@$(CXX) $(CFLAGS) $(CXXFLAGS) $(DEFINES) $(INCLUDES) -MM -MT $@ $< -o $(@:.o=.d)
	$(QUITE) $(CXX) $(CXXFLAGS) $(CFLAGS) $(DEFINES) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/%.cxx.o : %.cxx
	$(info +CXX $<)
	@$(MKDIR) $(@D)
	@$(CXX) $(CFLAGS) $(CXXFLAGS) $(DEFINES) $(INCLUDES) -MM -MT $@ $< -o $(@:.o=.d)
	$(QUITE) $(CXX) $(CXXFLAGS) $(CFLAGS) $(DEFINES) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/%.cc.o : %.cc
	$(info +CXX $<)
	@$(MKDIR) $(@D)
	@$(CXX) $(CFLAGS) $(CXXFLAGS) $(DEFINES) $(INCLUDES) -MM -MT $@ $< -o $(@:.o=.d)
	$(QUITE) $(CXX) $(CXXFLAGS) $(CFLAGS) $(DEFINES) $(INCLUDES) -c $< -o $@

-include $(DEP_FILES)
