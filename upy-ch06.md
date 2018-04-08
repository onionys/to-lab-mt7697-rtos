# 建立 micropython module


先要有下面這幾個include

    #include "py/nlr.h"
    #include "py/obj.h"
    #include "py/runtime.h"
    #include "py/binary.h"

# 定義一個 python module

    // 先設定一個簡易的 map
    // 這個東西比較像是 micropython 裡面的 QSTR 字串處理
    // 猜測是用來加速字串處理
    // __name__ 是一個 key，而 mymodule 是一個 value
    // 而前綴 MP_QSTR_ 則和 QSTR 處理相關
    //     __name__ : mymodule
    STATIC const mp_map_elem_t mymodule_globals_table[] = {
        { MP_OBJ_NEW_QSTR(MP_QSTR___name__), MP_OBJ_NEW_QSTR(MP_QSTR_mymodule)  },  // __na
    };

上面的程式碼在於產生一個某個 python 物件內部變數 `__name__` 內容為 `mymodule`。
而這算是一個map一樣的東西。

    // 再把這個table，拿來產生一個 micropython 物件的 dict
    STATIC MP_DEFINE_CONST_DICT(
        mp_module_mymodule_globals,   // 要產生的 micopython obj dict 的變數名稱
        mymodule_globals_table
    );

    // 定義 python module 的程式碼
    // 把產生的micorpython obj dict 拿來產生一個 micropython module 物件
    const mp_obj_module_t mp_module_mymodule = {
        .base = { &mp_type_module },
        .globals = (mp_obj_dict_t * ) &mp_module_mymodule_globals,
    };

`mpconfigport.h` 接著加入下面這些內容

    extern const struct _mp_obj_module_t mp_module_mymodule;

    #define MICROPY_PORT_BUILTIN_MODULES \
        ...
        { MP_OBJ_NEW_QSTR(MP_QSTR_mymodule), (mp_obj_t) &mp_module_mymodule}, \

這裡的動作比較像是把剛剛產生的 micropython module 物件註冊到整個系統中。

# 定義一個沒有參數的function

    #include<stdio.h>
    STATIC mp_obj_t mymodule_hello(void){
        printf("hello world\n");
        return mp_const_none;    // return None
    }
    STATIC MP_DEFINE_CONST_FUN_OBJ_0(mymodule_hello_obj, mymodule_hello);

最後要的東西是那個 `mymodule_hello_obj`。下面把它放到module裡面。
所以在做table時變成這樣。

    STATIC const mp_map_elem_t mymodule_globals_table[] = {
        { MP_OBJ_NEW_QSTR(MP_QSTR___name__), MP_OBJ_NEW_QSTR(MP_QSTR_mymodule)  },  // __na
        { MP_OBJ_NEW_QSTR(MP_QSTR__hello), (mp_obj_t)&mymodule_hello_obj  },        // hell
    };

# 定義一個有一個參數的function

    STATIC mp_obj_t mymodule_say(mp_obj_t what){
        printf("Hello %s!\n", mp_obj_str_get_str(what));
        return mp_const_none;
    }
    STATIC MP_DEFINE_CONST_FUN_OBJ_1(mymodule_say_obj, mymodule_say);

