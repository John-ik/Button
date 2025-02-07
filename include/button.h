#pragma once

#include "flags.h"
#include "io.h"

// ===================== FLAGS ======================
#define EB_PRESS (1 << 0)        // нажатие на кнопку
#define EB_HOLD (1 << 1)         // кнопка удержана
#define EB_STEP (1 << 2)         // импульсное удержание
#define EB_RELEASE (1 << 3)      // кнопка отпущена
#define EB_CLICK (1 << 4)        // одиночный клик
#define EB_CLICKS (1 << 5)       // сигнал о нескольких кликах
#define EB_TURN (1 << 6)         // поворот энкодера
#define EB_REL_HOLD (1 << 7)     // кнопка отпущена после удержания
#define EB_REL_HOLD_C (1 << 8)   // кнопка отпущена после удержания с предв. кликами
#define EB_REL_STEP (1 << 9)     // кнопка отпущена после степа
#define EB_REL_STEP_C (1 << 10)  // кнопка отпущена после степа с предв. кликами
#define EB_TIMEOUT (1 << 11)     // прошёл таймаут после нажатия кнопки или поворота энкодера

typedef enum {
    EBAction_None = 0,
    EBAction_Press = EB_PRESS,
    EBAction_Hold = EB_HOLD,
    EBAction_Step = EB_STEP,
    EBAction_Release = EB_RELEASE,
    EBAction_Click = EB_CLICK,
    EBAction_Clicks = EB_CLICKS,
    EBAction_Turn = EB_TURN,
    EBAction_ReleaseHold = EB_REL_HOLD,
    EBAction_ReleaseHoldClicks = EB_REL_HOLD_C,
    EBAction_ReleaseStep = EB_REL_STEP,
    EBAction_ReleaseStepClicks = EB_REL_STEP_C,
    EBAction_Timeout = EB_TIMEOUT,
} EBAction;

typedef enum {
    LEVEL_LOW = 0,
    LEVEL_HIGH = 1
} LEVEL;

// =================== TOUT BUILD ===================
#define EB_SHIFT 4

// таймаут антидребезга, мс
#ifdef EB_DEB_TIME
#define EB_DEB_T (EB_DEB_TIME)
#else
#endif

// таймаут между клтками, мс
#ifdef EB_CLICK_TIME
#define EB_CLICK_T (EB_CLICK_TIME)
#define EB_GET_CLICK_TIME() ((uint16_t)EB_CLICK_T)
#else
#define EB_GET_CLICK_TIME() (uint16_t)(btn->EB_CLICK_T << EB_SHIFT)
#endif

// таймаут удержания, мс
#ifdef EB_HOLD_TIME
#define EB_HOLD_T (EB_HOLD_TIME)
#define EB_GET_HOLD_TIME() ((uint16_t)EB_HOLD_T)
#else
#define EB_GET_HOLD_TIME() (uint16_t)(btn->EB_HOLD_T << EB_SHIFT)
#endif

// период степа, мс
#ifdef EB_STEP_TIME
#define EB_STEP_T (EB_STEP_TIME)
#define EB_GET_STEP_TIME() ((uint16_t)EB_STEP_T)
#else
#define EB_GET_STEP_TIME() (uint16_t)(btn->EB_STEP_T << EB_SHIFT)
#endif

// время таймаута, мс
#ifdef EB_TOUT_TIME
#define EB_TOUT_T (EB_TOUT_TIME)
#define EB_GET_TOUT_TIME() ((uint16_t)EB_TOUT_T)
#else
#define EB_GET_TOUT_TIME() (uint16_t)(btn->EB_TOUT_T << EB_SHIFT)
#endif

// =================== PACK FLAGS ===================
#define EB_CLKS_R (uint16_t) (1 << 0)
#define EB_PRS_R (uint16_t) (1 << 1)
#define EB_HLD_R (uint16_t) (1 << 2)
#define EB_STP_R (uint16_t) (1 << 3)
#define EB_REL_R (uint16_t) (1 << 4)

#define EB_PRS (uint16_t) (1 << 5)
#define EB_HLD (uint16_t) (1 << 6)
#define EB_STP (uint16_t) (1 << 7)
#define EB_REL (uint16_t) (1 << 8)

#define EB_BUSY (uint16_t) (1 << 9)
#define EB_DEB  (uint16_t) (1 << 10)
#define EB_TOUT (uint16_t) (1 << 11)
#define EB_INV  (uint16_t) (1 << 12)
#define EB_BOTH (uint16_t) (1 << 13)
#define EB_BISR (uint16_t) (1 << 14)

#define EB_EHLD (uint16_t) (1 << 15)

struct VirtButton_;

typedef void (*ActionHandler)(struct VirtButton_ *btn);

typedef struct {
    uint16_t tmr;
    Flags bf;
    ActionHandler cb;
    uint8_t clicks;

    #ifndef EB_NO_FOR
    uint16_t ftmr;
    #endif
    #ifndef EB_DEB_TIME
        uint8_t EB_DEB_T;
    #endif
    #ifndef EB_CLICK_TIME
        uint8_t EB_CLICK_T;
    #endif
    #ifndef EB_HOLD_TIME
        uint8_t EB_HOLD_T;
    #endif
    #ifndef EB_STEP_TIME
        uint8_t EB_STEP_T;
    #endif
    #ifndef EB_TOUT_TIME
        uint8_t EB_TOUT_T;
    #endif
} VirtButton;

// ====================== INIT ======================

void btn_init(VirtButton *btn);

// ====================== SET ======================
// установить таймаут удержания, умолч. 600 (макс. 4000 мс)
void btn_setHoldTimeout(VirtButton *btn, uint16_t tout);

// установить таймаут импульсного удержания, умолч. 200 (макс. 4000 мс)
void btn_setStepTimeout(VirtButton *btn, uint16_t tout);

// установить таймаут ожидания кликов, умолч. 500 (макс. 4000 мс)
void btn_setClickTimeout(VirtButton *btn, uint16_t tout);

// установить таймаут антидребезга, умолч. 50 (макс. 255 мс)
void btn_setDebTimeout(VirtButton *btn, uint8_t tout);

// установить время таймаута, умолч. 1000 (макс. 4000 мс)
void btn_setTimeout(VirtButton *btn, uint16_t tout);

// установить уровень кнопки (HIGH - кнопка замыкает VCC, LOW - замыкает GND)
void btn_setBtnLevel(VirtButton *btn, LEVEL level);

// кнопка нажата в прерывании (не учитывает btnLevel!)
void btn_pressISR(VirtButton *btn);

// сбросить системные флаги (принудительно закончить обработку)
void btn_reset(VirtButton *btn);

// принудительно сбросить флаги событий. by defauld resetTout = false
void btn_clear(VirtButton *btn,uint8_t resetTout);

// игнорировать все события до отпускания кнопки
void btn_skipEvents(VirtButton *btn);

// подключить функцию-обработчик событий (вида void f())
void btn_attach(VirtButton *btn,ActionHandler handler);

// отключить функцию-обработчик событий
void btn_detach(VirtButton *btn);

// ====================== GET ======================
// кнопка нажата [событие]
uint8_t btn_press(VirtButton *btn);

// кнопка нажата с предварительными кликами [событие]
uint8_t btn_press_n(VirtButton *btn, const uint8_t num);

// кнопка отпущена (в любом случае) [событие]
uint8_t btn_release(VirtButton *btn);

// кнопка отпущена (в любом случае) с предварительными кликами [событие]
uint8_t btn_release_n(VirtButton *btn, const uint8_t num);

// клик по кнопке (отпущена без удержания) [событие]
uint8_t btn_click(VirtButton *btn);

// клик по кнопке (отпущена без удержания) с предварительными кликами [событие]
uint8_t btn_click_n(VirtButton *btn, const uint8_t num);

// кнопка зажата (между btn_press(btn) и release()) [состояние]
uint8_t btn_pressing(VirtButton *btn);

// кнопка зажата (между btn_press(btn) и release()) с предварительными кликами [состояние]
uint8_t btn_pressing_n(VirtButton *btn, const uint8_t num);

// кнопка была удержана (больше таймаута) [событие]
uint8_t btn_hold(VirtButton *btn);

// кнопка была удержана (больше таймаута) с предварительными кликами [событие]
uint8_t btn_hold_n(VirtButton *btn, const uint8_t num);

// кнопка удерживается (больше таймаута) [состояние]
uint8_t btn_holding(VirtButton *btn);

// кнопка удерживается (больше таймаута) с предварительными кликами [состояние]
uint8_t btn_holding_n(VirtButton *btn, const uint8_t num);

// импульсное удержание [событие]
uint8_t btn_step(VirtButton *btn);

// импульсное удержание с предварительными кликами [событие]
uint8_t btn_step_n(VirtButton *btn, const uint8_t num);

// зафиксировано несколько кликов [событие]
uint8_t btn_hasClicks(VirtButton *btn);

// зафиксировано указанное количество кликов [событие]
uint8_t btn_hasClicks_n(VirtButton *btn, const uint8_t num);

// получить количество кликов
uint8_t btn_getClicks(VirtButton *btn);

// получить количество степов
uint16_t btn_getSteps(VirtButton *btn);

// кнопка отпущена после удержания [событие]
uint8_t btn_releaseHold(VirtButton *btn);

// кнопка отпущена после удержания с предварительными кликами [событие]
uint8_t btn_releaseHold_n(VirtButton *btn, const uint8_t num);

// кнопка отпущена после импульсного удержания [событие]
uint8_t btn_releaseStep(VirtButton *btn);

// кнопка отпущена после импульсного удержания с предварительными кликами [событие]
uint8_t btn_releaseStep_n(VirtButton *btn, const uint8_t num);

// кнопка отпущена после удержания или импульсного удержания [событие]
uint8_t btn_releaseHoldStep(VirtButton *btn);

// кнопка отпущена после удержания или импульсного удержания с предварительными кликами [событие]
uint8_t btn_releaseHoldStep_n(VirtButton *btn, const uint8_t num);

// кнопка ожидает повторных кликов [состояние]
uint8_t btn_waiting(VirtButton *btn);

// идёт обработка [состояние]
uint8_t btn_busy(VirtButton *btn);

// было действие с кнопки, вернёт код события [событие]
uint16_t btn_action(VirtButton *btn);

// было действие с кнопки, вернёт код события [событие]
EBAction btn_getAction(VirtButton *btn);

// ====================== TIME ======================
// после взаимодействия с кнопкой (или энкодером EncButton) время setTimeout, мс [событие]
uint8_t btn_timeout(VirtButton *btn);

// после взаимодействия с кнопкой (или энкодером EncButton) время setTimeout, мс [состояние]
uint8_t btn_timeoutState(VirtButton *btn);

// время, которое кнопка удерживается (с начала нажатия), мс
uint16_t btn_pressFor(VirtButton *btn);

// кнопка удерживается дольше чем (с начала нажатия), мс [состояние]
uint8_t btn_pressFor_t(VirtButton *btn, const uint16_t ms);

// время, которое кнопка удерживается (с начала удержания), мс
uint16_t btn_holdFor(VirtButton *btn);

// кнопка удерживается дольше чем (с начала удержания), мс [состояние]
uint8_t btn_holdFor_t(VirtButton *btn, const uint16_t ms);

// время, которое кнопка удерживается (с начала степа), мс
uint16_t btn_stepFor(VirtButton *btn);

// кнопка удерживается дольше чем (с начала степа), мс [состояние]
uint8_t btn_stepFor_t(VirtButton *btn, uint16_t ms);

// ====================== POLL ======================
// обработка виртуальной кнопки как одновременное нажатие двух других кнопок
uint8_t btn_tick_2(VirtButton *btn, VirtButton *b0, VirtButton *b1);

// обработка кнопки значением
uint8_t btn_tick(VirtButton *btn, uint8_t s);

// обработка кнопки без сброса событий и вызова коллбэка
uint8_t btn_tickRaw(VirtButton *btn, const uint8_t s);

// вызвать обработчик. by defaul force = false
void btn_call(VirtButton *btn, uint8_t force);


// после взаимодействия с кнопкой (или энкодером EncButton) прошло указанное время, мс [событие]
uint8_t btn_timeout_t(VirtButton *btn, const uint16_t tout) /*__attribute__((deprecated))*/ ;

// после взаимодействия с кнопкой (или энкодером EncButton) прошло указанное время, мс [состояние]
uint8_t btn_timeoutState_t(VirtButton *btn, const uint16_t tout) /*__attribute__((deprecated))*/ ;
