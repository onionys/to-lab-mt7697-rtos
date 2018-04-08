# 如何將MT7697專案移至 Micropython

要將Micropython編輯完成放至MT7697執行，需要雙方SDK環境，我們開發方式還是以Micropython為主要專案，我們要MT7697的基本專案先放置MircoPython進行開發，我們所開發的平台作系統為Linux與xOS

- 下載LinkIt_SDK_V4與Micropython SDK  :請參考[ ch02 : Linux 環境建置](ch02.md)與[ ch 1 : build unix ](./upy-ch01.md)
- 開發環境變數宣告:將其LinkIt_SDK_V4 SDK的路徑宣告為 **LINKIT_SDK_PATH**
```sh
# This is MT7697 build upy
if [ -d "/media/sf_DockerVolumes/micropython-esp32/ports/mt7697" ] && \
   [ -d "/media/sf_DockerVolumes/LinkIt_SDK/" ]; then
        export LINKIT_SDK_PATH=/media/sf_DockerVolumes/LinkIt_SDK
fi
```
- 將複製LinkIt_SDK裡的LinkIt_SDK/project/linkit7697_hdk/templates/freertos_initialize_main_features 至 MicoPython裡的ports/mt7697
```bash
cp -a (LinkIt_SDK_PATH)/project/linkit7697_hdk/templates/freertos_initialize_main_features (MicoPython_SDK_PATH)/ports/mt7697
```
- 修改其Makefile

```diff
    diff --git a/ports/mt7697/GCC/Makefile b/ports/mt7697/GCC/Makefile
    index d20d9f52..987e1745 100644
    --- a/ports/mt7697/GCC/Makefile
    +++ b/ports/mt7697/GCC/Makefile
    @@ -35,15 +35,17 @@
     ###################################################
     # Project vairables assignment

    -SOURCE_DIR = ../../../../..
    +SOURCE_DIR = $(LINKIT_SDK_PATH)
     PWD= $(shell pwd)
    -SDK_PATH    = $(abspath $(PWD)/$(SOURCE_DIR))
    +SDK_PATH    = $(LINKIT_SDK_PATH)
    +
     FEATURE = feature.mk
     include $(FEATURE)

     DEBUG = 0
     FLOAT_TYPE = hard
     BUILD_DIR = $(PWD)/build
    +MY_BUILD_DIR = $(PWD)/build

     # Project name
     PROJ_NAME       = $(notdir $(abspath $(dir $(PWD))))
    @@ -53,6 +55,8 @@ OUTPATH = $(PWD)/build
     C_OBJS = $(C_FILES:%.c=$(BUILD_DIR)/%.o)
     CXX_OBJS        = $(CXX_FILES:%.cpp=$(BUILD_DIR)/%.o)
     S_OBJS = $(S_FILES:%.s=$(BUILD_DIR)/%.o)
    +MY_C_OBJS = $(MY_C_FILES:%.c=$(MY_BUILD_DIR)/%.o)
    +MY_S_OBJS = $(MY_S_FILES:%.s=$(MY_BUILD_DIR)/%.o)

     # LD Flags
     LDFLAGS = $(ALLFLAGS) $(FPUFLAGS) --specs=nano.specs -lnosys -nostartfiles
    @@ -91,11 +95,6 @@ else
     include $(SOURCE_DIR)/prebuilt/middleware/MTK/minisupp/module.mk
     endif

    -
    -
    -
    -
    -
     ###################################################
     # Sources or Lib
     #include lwip
    @@ -134,25 +133,24 @@ ifeq ($(MTK_MINICLI_ENABLE),y)
     APP_FILES += driver/board/linkit7697_hdk/util/src/io_def.c
     endif

    -C_FILES += $(APP_FILES)
    +MY_C_FILES = $(APP_FILES)

     # add startup file to build
    -S_FILES += $(APP_PATH)/GCC/startup_mt7687.s
    -
    -
    +MY_S_FILES += $(APP_PATH)/GCC/startup_mt7687.s

     # include path
     CFLAGS += -I$(SOURCE_DIR)/driver/chip/mt7687/inc
     CFLAGS += -I$(SOURCE_DIR)/driver/chip/inc
     CFLAGS += -I$(SOURCE_DIR)/middleware/MTK/wifi_service/combo/inc
    -CFLAGS += -I$(SOURCE_DIR)/$(APP_PATH)/inc
    +CFLAGS += -I$(APP_PATH)/inc
     CFLAGS += -I$(SOURCE_DIR)/driver/CMSIS/Include
     CFLAGS += -I$(SOURCE_DIR)/driver/CMSIS/Device/MTK/mt7687/Include
     CFLAGS += -I$(SOURCE_DIR)/driver/board/linkit7697_hdk/ept/inc
    +CFLAGS += -I$(SOURCE_DIR)/driver/board/linkit7697_hdk/external_flash/driver/board/linkit7697_hdk/external_flash/inc

     # BLE include path
     CFLAGS += -D__BT_DEBUG__
    -CFLAGS += -I$(SOURCE_DIR)/$(APP_PATH)/src/ut_app
    +CFLAGS += -I$(APP_PATH)/src/ut_app
     CFLAGS += -I$(SOURCE_DIR)/middleware/MTK/bluetooth/inc

     ###################################################
    @@ -176,8 +174,8 @@ include $(SOURCE_DIR)/middleware/MTK/bluetooth/module.mk

     all: cleanlog proj ; $(info $$var is [${var}])echo Hello world
     	@mkdir -p $(BUILD_DIR)
    -	@$(SIZE) $(OUTPATH)/$(PROJ_NAME).elf
    -	@$(SOURCE_DIR)/tools/scripts/build/copy_firmware.sh $(SOURCE_DIR) $(OUTPATH) $(IC_CONFIG) $(BOARD_CONFIG) $(PROJ_NAME).bin $(PWD)
    +	$(SIZE) $(OUTPATH)/$(PROJ_NAME).elf
    +	./copy_firmware.sh $(SOURCE_DIR) $(OUTPATH) $(IC_CONFIG) $(BOARD_CONFIG) $(PROJ_NAME).bin $(PWD) linkit7697_hdk

     MOD_EXTRA = BUILD_DIR=$(BUILD_DIR) OUTPATH=$(OUTPATH) PROJ_PATH=$(PROJ_PATH)

    @@ -188,21 +186,21 @@ $(MODULE_PATH):

     proj: $(OUTPATH)/$(PROJ_NAME).elf

    -$(OUTPATH)/$(PROJ_NAME).elf: $(C_OBJS) $(CXX_OBJS) $(S_OBJS) $(LIBS)
    +$(OUTPATH)/$(PROJ_NAME).elf: $(C_OBJS) $(CXX_OBJS) $(S_OBJS) $(LIBS) $(MY_C_OBJS) $(MY_S_OBJS)
     	@echo Linking...
    -	@if [ -e "$@" ]; then rm -f "$@"; fi
    -	@if [ -e "$(OUTPATH)/$(PROJ_NAME).map" ]; then rm -f "$(OUTPATH)/$(PROJ_NAME).map"; fi
    -	@if [ -e "$(OUTPATH)/$(PROJ_NAME).dis" ]; then rm -f "$(OUTPATH)/$(PROJ_NAME).dis"; fi
    -	@if [ -e "$(OUTPATH)/$(PROJ_NAME).hex" ]; then rm -f "$(OUTPATH)/$(PROJ_NAME).hex"; fi
    -	@if [ -e "$(OUTPATH)/$(PROJ_NAME).bin" ]; then rm -f "$(OUTPATH)/$(PROJ_NAME).bin"; fi
    +	if [ -e "$@" ]; then rm -f "$@"; fi
    +	if [ -e "$(OUTPATH)/$(PROJ_NAME).map" ]; then rm -f "$(OUTPATH)/$(PROJ_NAME).map"; fi
    +	if [ -e "$(OUTPATH)/$(PROJ_NAME).dis" ]; then rm -f "$(OUTPATH)/$(PROJ_NAME).dis"; fi
    +	if [ -e "$(OUTPATH)/$(PROJ_NAME).hex" ]; then rm -f "$(OUTPATH)/$(PROJ_NAME).hex"; fi
    +	if [ -e "$(OUTPATH)/$(PROJ_NAME).bin" ]; then rm -f "$(OUTPATH)/$(PROJ_NAME).bin"; fi
     	@$(CC) $(LDFLAGS) -Wl,--start-group $^ -Wl,--end-group -Wl,-Map=$(OUTPATH)/$(PROJ_NAME).map -lm -o $@ >>$(ERR_LOG)
    -	@$(OBJDUMP) -D $(OUTPATH)/$(PROJ_NAME).elf > $(OUTPATH)/$(PROJ_NAME).dis
    -	@$(OBJCOPY) -O ihex $(OUTPATH)/$(PROJ_NAME).elf $(OUTPATH)/$(PROJ_NAME).hex
    -	@$(OBJCOPY) -O binary $(OUTPATH)/$(PROJ_NAME).elf $(OUTPATH)/$(PROJ_NAME).bin
    +	$(OBJDUMP) -D $(OUTPATH)/$(PROJ_NAME).elf > $(OUTPATH)/$(PROJ_NAME).dis
    +	$(OBJCOPY) -O ihex $(OUTPATH)/$(PROJ_NAME).elf $(OUTPATH)/$(PROJ_NAME).hex
    +	$(OBJCOPY) -O binary $(OUTPATH)/$(PROJ_NAME).elf $(OUTPATH)/$(PROJ_NAME).bin
     	@echo Done


    -include $(SOURCE_DIR)/.rule.mk
    +include ./rule.mk

     clean:
     	rm -rf $(OUTPATH)
    	--
```

- 複製copy_firmware.sh 至專案並且修改
```sh
cp (LinkIt_SDK_PATH)/tools/scripts/build/copy_firmware.sh (MicoPython_SDK_PATH)/ports/mt7697/GCC/copy_firmware.sh
```
```diff
    --- .//LinkIt_SDK/tools/scripts/build/copy_firmware.sh	2017-11-15 18:47:58.000000000 +0800
    +++ .//micropython-esp32/ports/mt7697/GCC/copy_firmware.sh	2018-03-25 10:33:15.000000000 +0800
    @@ -10,6 +10,7 @@
     BOARD_CONFIG=$4
     BIN_FILENAME=$5
     PROJ_DIR=$6
    +BOARD=$7
     FLASHBIN_NAME=flash.bin
     BOOTLOADERMAXSIZE=65536  # 64KB

    @@ -112,7 +113,6 @@
     ###############################################################################
     #Begin here
     echo `basename $0`"...."
    -get_board

     if [ "$BOARD_CONFIG" == "mt2523_hdk" ]; then
```

- 複製rule.mk至專案並且修改
```sh
cp -a (LinkIt_SDK_PATH)/.rule.mk (MicoPython_SDK_PATH)/ports/mt7697/GCC/rule.mk
```
    ```diff
        --- .//LinkIt_SDK/.rule.mk	2018-02-20 13:05:15.000000000 +0800
        +++ .//micropython-esp32/ports/mt7697/GCC/rule.mk	2018-03-25 10:33:15.000000000 +0800
        @@ -121,8 +121,93 @@
         		echo "Build... $$(basename $@) PASS"; \
         		echo "Build... $@ PASS" >> $(BUILD_LOG); \
         	fi;
        +# ============================================================================
        +$(MY_BUILD_DIR)/%.o: %.c
        +	@mkdir -p $(dir $@)
        +	@echo Build... $$(basename $@)
        +	@echo Build... $@ >> $(BUILD_LOG)
        +	@if [ -e "$@" ]; then rm -f "$@"; fi
        +	@if [ -n "$(OVERRIDE_CFLAGS)" ]; then \
        +		echo $(CC) $(OVERRIDE_CFLAGS) $@ >> $(BUILD_LOG); \
        +		$(CC) $(OVERRIDE_CFLAGS) -c $< -o $@ 2>>$(ERR_LOG); \
        +	else \
        +		echo $(CC) $(CFLAGS) $@ >> $(BUILD_LOG); \
        +		$(CC) $(CFLAGS) -c $< -o $@ 2>>$(ERR_LOG); \
        +	fi; \
        +	if [ "$$?" != "0" ]; then \
        +		echo "Build... $$(basename $@) FAIL"; \
        +		echo "Build... $@ FAIL" >> $(BUILD_LOG); \
        +	else \
        +		echo "Build... $$(basename $@) PASS"; \
        +		echo "Build... $@ PASS" >> $(BUILD_LOG); \
        +	fi;
        +
        +$(MY_BUILD_DIR)/%.d: %.c
        +	@mkdir -p $(dir $@)
        +	@set -e; rm -f $@; \
        +	export D_FILE="$@"; \
        +	export B_NAME=`echo $$D_FILE | sed 's/\.d//g'`; \
        +	if [ -n "$(OVERRIDE_CFLAGS)" ]; then \
        +		$(CC) -MM $(OVERRIDE_CFLAGS) $< > $@.$$$$; \
        +	else \
        +		$(CC) -MM $(CFLAGS) $< > $@.$$$$; \
        +	fi; \
        +	sed 's@\(.*\)\.o@'"$$B_NAME\.o $$B_NAME\.d"'@g' < $@.$$$$ > $@; \
        +	rm -f $@.$$$$
        +
        +$(MY_BUILD_DIR)/%.o: %.cpp
        +	@mkdir -p $(dir $@)
        +	@echo Build... $$(basename $@)
        +	@echo Build... $@ >> $(BUILD_LOG)
        +	@if [ -e "$@" ]; then rm -f "$@"; fi
        +	@if [ -n "$(OVERRIDE_CFLAGS)" ]; then \
        +		echo $(CXX) $(OVERRIDE_CFLAGS) $@ >> $(BUILD_LOG); \
        +		$(CXX) $(OVERRIDE_CFLAGS) -c $< -o $@ 2>>$(ERR_LOG); \
        +	else \
        +		echo $(CXX) $(CXXFLAGS) $@ >> $(BUILD_LOG); \
        +		$(CXX) $(CXXFLAGS) -c $< -o $@ 2>>$(ERR_LOG); \
        +	fi; \
        +	if [ "$$?" != "0" ]; then \
        +		echo "Build... $$(basename $@) FAIL"; \
        +		echo "Build... $@ FAIL" >> $(BUILD_LOG); \
        +	else \
        +		echo "Build... $$(basename $@) PASS"; \
        +		echo "Build... $@ PASS" >> $(BUILD_LOG); \
        +	fi;
        +
        +$(MY_BUILD_DIR)/%.d: %.cpp
        +	@mkdir -p $(dir $@)
        +	@set -e; rm -f $@; \
        +	export D_FILE="$@"; \
        +	export B_NAME=`echo $$D_FILE | sed 's/\.d//g'`; \
        +	if [ -n "$(OVERRIDE_CFLAGS)" ]; then \
        +		$(CXX) -MM $(OVERRIDE_CFLAGS) $< > $@.$$$$; \
        +	else \
        +		$(CXX) -MM $(CXXFLAGS) $< > $@.$$$$; \
        +	fi; \
        +	sed 's@\(.*\)\.o@'"$$B_NAME\.o $$B_NAME\.d"'@g' < $@.$$$$ > $@; \
        +	rm -f $@.$$$$
        +
        +$(MY_BUILD_DIR)/%.o: %.s
        +	@mkdir -p $(dir $@)
        +	@echo Build... $$(basename $@)
        +	@echo Build... $@ >> $(BUILD_LOG)
        +	@if [ -e "$@" ]; then rm -f "$@"; fi
        +	@if [ -n "$(OVERRIDE_CFLAGS)" ]; then \
        +		$(CC) $(OVERRIDE_CFLAGS) -c $< -o $@; \
        +	else \
        +		$(CC) $(CFLAGS) -c $< -o $@; \
        +	fi; \
        +	if [ "$$?" != "0" ]; then \
        +		echo "Build... $$(basename $@) FAIL"; \
        +		echo "Build... $@ FAIL" >> $(BUILD_LOG); \
        +	else \
        +		echo "Build... $$(basename $@) PASS"; \
        +		echo "Build... $@ PASS" >> $(BUILD_LOG); \
        +	fi;

         ifneq ($(MAKECMDGOALS),clean)
         -include $(C_OBJS:.o=.d)
         -include $(CXX_OBJS:.o=.d)
        +-include $(MY_C_OBJS:.o=.d)
         endif

     ```

- 測試編譯
```
cd (MicoPython_SDK_PATH)/ports/mt7697/GCC/
make
```

- 將產build/ 產生的bin檔，燒錄至MT7697請參考[ ch02 : Linux 環境建置](ch02.md)
