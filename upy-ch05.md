# Porting Micropython

移植 micropython 初期，至少要先實作一些基本的 `function`。

為了讓do_str(); 可以吃簡單的python code，需先實作下面的function

----

定義在 `$(PY_SDK)/py/mphal.h` 

	int  mp_hal_stdin_rx_chr(void);

	void mp_hal_stdout_tx_char(char c );
	void mp_hal_stdout_tx_str(const char * str);
	void mp_hal_stdout_tx_strn(const char * str, size_t len);
	void mp_hal_stdout_tx_strn_cooked(const char * str, size_t len);

	void mp_hal_set_interrupt_char(int c);


可以將這些function實作在 `mphalport.c` 裡面

下面這四個function比較單純，目的在於文字的stdout，所以把 MCU 的 uart 接上就好。
不過會以四種不同的方式輸出指定字元或字串，所以直接參考其他系統怎麼寫的。

	void mp_hal_stdout_tx_char(char c );
	void mp_hal_stdout_tx_str(const char * str);
	void mp_hal_stdout_tx_strn(const char * str, size_t len);
	void mp_hal_stdout_tx_strn_cooked(const char * str, size_t len);
	
下面這個就比較麻煩，是接互動模式下，接收指令的地方，就是stdin 的部份。
但是這邊的input在等待時，不能被block住，所以要實作一個 unblock 的方式。

	int  mp_hal_stdin_rx_chr(void);

下面這個 function，應該是設定一個中斷字元。實作方式就是用一個global 變數
把傳進來的int存放起來就好。

	void mp_hal_set_interrupt_char(int c);

但是該function已經定義且實作在下面兩個檔案。
如果有定義 `MICROPY_KBD_EXCEPTION` 則會被enable。

    lib/utils/interrupt_char.c
    lib/utils/interrupt_char.h

----

定義在 `$(PY_SDK)/py/gc.h`

	void gc_collect(void){}
	這部份的程式碼可以試著直接抄stm32的

定義在 `$(PY_SDK)/py/lexer.h`

	mp_lexer_t *mp_lexer_new_from_file(const char *filename){}
	mp_import_stat_t mp_import_stat(const char *path){}

定義在 `$(PY_SDK)/py/builtin.h`

	mp_obj_t mp_builtin_open(size_t n_args, const mp_obj_t *args, mp_map_t *kwargs){}
	下面是實作完上面那個function之後要做的事
	MP_DEFINE_CONST_FUN_OBJ_KW(mp_builtin_open_obj, 1, mp_builtin_open);

定義在 `$(PY_SDK)/py/nlr.h`

	void nlr_jump_fail(void *val){}
	當系統出現嚴重的fail時，要做的事情。
	大部份都是重置系統，但是這裡先變成while loop-->就是當掉不動的意思

# 直譯器

如果要讓直譯器跑起來，似要實作下面這個。
應該直接用 RTOS 的return現在的ms就好。

	mp_uint_t mp_hal_ticks_ms(void);

# freeRTOS or Not?

後來發現，stm32 系統的 project 並沒有用到 freeRTOS。
如果不想用freeRTOS當做基底的話，就直接參考這個怎麼做吧。

cc3200 有用到freeRTOS，所以很有參考價值。

esp32 雖然有用到freeRTOS，但是其中因為架構並非ARM的，所以裡面用到
很多無法參考的function。在某些接近核心的部份的參考價值較小。

teensy 這個project和stm32一樣沒有用到freeRTOS。可能是因為其RAM很少，
所以要節省資源?

