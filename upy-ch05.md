# 互動式直譯器

micropython 核心功能之一為互動式直譯器讓開發者可以直
接透過UART撰寫與執行python程式碼。

要讓直譯器有一個互動介面會需要實作一個無窮迴圈來處理
使用者透過UART傳送過來的程式碼。在這邊實作一個freeRTOS
的task來進行這件事情

# freeRTOS task

在`main.c` 裡面宣告一個task。

    TaskHandle_t mp_task_handle = NULL;
	BaseType_t task_res;
    task_res = xTaskCreate(
		mp_task,
		"mp_task" ,
		MP_TASK_STACK_LEN,
		(void *) 1,
		TASK_PRIORITY_LOW,
		&mp_task_handle);
	if(task_res == pdPASS){
		mp_hal_stdout_tx_str("\r\n[MP Task OK]\r\n");
	}else{
		mp_hal_stdout_tx_str("\r\n[MP Task init failed!]\r\n");
	}

然後在function `mp_task()` 裡面運行直譯器執行的function

	static void mp_task(void *pvParameters){
		printf("[micropython start]\n");
		mp_stdin_pyexec_run_loop();
	}

	void mp_stdin_pyexc_run_loop(){
    #if MICROPY_ENABLE_COMPILER
        #if MICROPY_REPL_EVENT_DRIVEN
            pyexec_event_repl_init();
            for (;;) {
                int c = mp_hal_stdin_rx_chr();
                if (pyexec_event_repl_process_char(c)) {
                    break;
                }
            }
        #else
            pyexec_friendly_repl();
        #endif
    #else
        pyexec_frozen_module("frozentest.py");
    #endif
    }

其中 `MICROPY_REPL_EVENT_DRIVEN` 和 `MICROPY_ENABLE_COMPILER` 這兩個定義在
`mpconfigport.h` 裡面的flag要設為 `(1)`

	...
	#define MICROPY_ENABLE_COMPILER     (1)
	...
	#define MICROPY_REPL_EVENT_DRIVEN   (1)
	...

