#include "button.h"
#include "stddef.h"

// #include "debug.h"

uint8_t btn_pollBtn(VirtButton *btn, uint8_t s);


// ====================== INIT ======================

void btn_init(VirtButton *btn){
    btn->tmr = 0;
    btn->cb = NULL;
    
    #ifndef EB_NO_FOR
    btn->ftmr = 0;
    #endif
    #ifndef EB_DEB_TIME
        btn->EB_DEB_T = 50;
    #endif
    #ifndef EB_CLICK_TIME
        btn->EB_CLICK_T = (500 >> EB_SHIFT);
    #endif
    #ifndef EB_HOLD_TIME
        btn->EB_HOLD_T = (600 >> EB_SHIFT);
    #endif
    #ifndef EB_STEP_TIME
        btn->EB_STEP_T = (200 >> EB_SHIFT);
    #endif
    #ifndef EB_TOUT_TIME
        btn->EB_TOUT_T = (1000 >> EB_SHIFT);
    #endif
}

// ====================== SET ======================
// установить таймаут удержания, умолч. 600 (макс. 4000 мс)
void btn_setHoldTimeout(VirtButton *btn, uint16_t tout) {
#ifndef EB_HOLD_TIME
    btn->EB_HOLD_T = tout >> EB_SHIFT;
#endif
}

// установить таймаут импульсного удержания, умолч. 200 (макс. 4000 мс)
void btn_setStepTimeout(VirtButton *btn, uint16_t tout) {
#ifndef EB_STEP_TIME
    btn->EB_STEP_T = tout >> EB_SHIFT;
#endif
}

// установить таймаут ожидания кликов, умолч. 500 (макс. 4000 мс)
void btn_setClickTimeout(VirtButton *btn, uint16_t tout) {
#ifndef EB_CLICK_TIME
    btn->EB_CLICK_T = tout >> EB_SHIFT;
#endif
}

// установить таймаут антидребезга, умолч. 50 (макс. 255 мс)
void btn_setDebTimeout(VirtButton *btn, uint8_t tout) {
#ifndef EB_DEB_TIME
    btn->EB_DEB_T = tout;
#endif
}

// установить время таймаута, умолч. 1000 (макс. 4000 мс)
void btn_setTimeout(VirtButton *btn, uint16_t tout) {
#ifndef EB_TOUT_TIME
    btn->EB_TOUT_T = tout >> EB_SHIFT;
#endif
}

// установить уровень кнопки (HIGH - кнопка замыкает VCC, LOW - замыкает GND)
void btn_setBtnLevel(VirtButton *btn, LEVEL level) {
    flag_write(btn->bf, EB_INV, !level);
}

// кнопка нажата в прерывании (не учитывает btnLevel!)
void btn_pressISR(VirtButton *btn) {
    if (!flag_read(btn->bf, EB_DEB)) btn->tmr = EB_uptime();
    flag_set(btn->bf, EB_DEB | EB_BISR);
}

// сбросить системные флаги (принудительно закончить обработку)
void btn_reset(VirtButton *btn) {
    btn->clicks = 0;
    flag_clear(btn->bf, ~EB_INV);  // все кроме EB_INV
}

// принудительно сбросить флаги событий
void btn_clear(VirtButton *btn, uint8_t resetTout) {
    if (resetTout && flag_read(btn->bf, EB_TOUT)) flag_clear(btn->bf, EB_TOUT);
    if (flag_read(btn->bf, EB_CLKS_R)) btn->clicks = 0;
    if (flag_read(btn->bf, EB_CLKS_R | EB_STP_R | EB_PRS_R | EB_HLD_R | EB_REL_R)) {
        flag_clear(btn->bf, EB_CLKS_R | EB_STP_R | EB_PRS_R | EB_HLD_R | EB_REL_R);
    }
}

// игнорировать все события до отпускания кнопки
void btn_skipEvents(VirtButton *btn) {
    flag_set(btn->bf, EB_EHLD);
}

// подключить функцию-обработчик событий (вида void f())
void btn_attach(VirtButton *btn,ActionHandler handler) {
#ifndef EB_NO_CALLBACK
    btn->cb = handler;
#endif
}

// отключить функцию-обработчик событий
void btn_detach(VirtButton *btn) {
#ifndef EB_NO_CALLBACK
    btn->cb = NULL;
#endif
}

// ====================== GET ======================
// кнопка нажата [событие]
uint8_t btn_press(VirtButton *btn) {
    return flag_read(btn->bf, EB_PRS_R);
}

// кнопка нажата с предварительными кликами [событие]
uint8_t btn_press_n(VirtButton *btn, const uint8_t num) {
    return (btn->clicks == num) && btn_press(btn);
}

// кнопка отпущена (в любом случае) [событие]
uint8_t btn_release(VirtButton *btn) {
    return flag_eq(btn->bf, EB_REL_R | EB_REL, EB_REL_R | EB_REL);
}

// кнопка отпущена (в любом случае) с предварительными кликами [событие]
uint8_t btn_release_n(VirtButton *btn, const uint8_t num) {
    return (btn->clicks == num) && btn_release(btn);
}

// клик по кнопке (отпущена без удержания) [событие]
uint8_t btn_click(VirtButton *btn) {
    return flag_eq(btn->bf, EB_REL_R | EB_REL | EB_HLD, EB_REL_R);
}

// клик по кнопке (отпущена без удержания) с предварительными кликами [событие]
uint8_t btn_click_n(VirtButton *btn, const uint8_t num) {
    return (btn->clicks == num) && btn_click(btn);
}

// кнопка зажата (между btn_press(btn) и release()) [состояние]
uint8_t btn_pressing(VirtButton *btn) {
    return flag_read(btn->bf, EB_PRS);
}

// кнопка зажата (между btn_press(btn) и release()) с предварительными кликами [состояние]
uint8_t btn_pressing_n(VirtButton *btn, const uint8_t num) {
    return (btn->clicks == num) && btn_pressing(btn);
}

// кнопка была удержана (больше таймаута) [событие]
uint8_t btn_hold(VirtButton *btn) {
    return flag_read(btn->bf, EB_HLD_R);
}

// кнопка была удержана (больше таймаута) с предварительными кликами [событие]
uint8_t btn_hold_n(VirtButton *btn, const uint8_t num) {
    return (btn->clicks == num) && btn_hold(btn);
}

// кнопка удерживается (больше таймаута) [состояние]
uint8_t btn_holding(VirtButton *btn) {
    return flag_eq(btn->bf, EB_PRS | EB_HLD, EB_PRS | EB_HLD);
}

// кнопка удерживается (больше таймаута) с предварительными кликами [состояние]
uint8_t btn_holding_n(VirtButton *btn, const uint8_t num) {
    return (btn->clicks == num) && btn_holding(btn);
}

// импульсное удержание [событие]
uint8_t btn_step(VirtButton *btn) {
    return flag_read(btn->bf, EB_STP_R);
}

// импульсное удержание с предварительными кликами [событие]
uint8_t btn_step_n(VirtButton *btn, const uint8_t num) {
    return (btn->clicks == num) && btn_step(btn);
}

// зафиксировано несколько кликов [событие]
uint8_t btn_hasClicks(VirtButton *btn) {
    return flag_eq(btn->bf, EB_CLKS_R | EB_HLD, EB_CLKS_R);
}

// зафиксировано указанное количество кликов [событие]
uint8_t btn_hasClicks_n(VirtButton *btn, const uint8_t num) {
    return (btn->clicks == num) && btn_hasClicks(btn);
}

// получить количество кликов
uint8_t btn_getClicks(VirtButton *btn) {
    return btn->clicks;
}

// получить количество степов
uint16_t btn_getSteps(VirtButton *btn) {
#ifndef EB_NO_FOR
    return btn->ftmr ? ((btn_stepFor(btn) + EB_GET_STEP_TIME() - 1) / EB_GET_STEP_TIME()) : 0;  // (x + y - 1) / y
#endif
    return 0;
}

// кнопка отпущена после удержания [событие]
uint8_t btn_releaseHold(VirtButton *btn) {
    return flag_eq(btn->bf, EB_REL_R | EB_REL | EB_HLD | EB_STP, EB_REL_R | EB_HLD);
}

// кнопка отпущена после удержания с предварительными кликами [событие]
uint8_t btn_releaseHold_n(VirtButton *btn, const uint8_t num) {
    return btn->clicks == num && flag_eq(btn->bf, EB_CLKS_R | EB_HLD | EB_STP, EB_CLKS_R | EB_HLD);
}

// кнопка отпущена после импульсного удержания [событие]
uint8_t btn_releaseStep(VirtButton *btn) {
    return flag_eq(btn->bf, EB_REL_R | EB_REL | EB_STP, EB_REL_R | EB_STP);
}

// кнопка отпущена после импульсного удержания с предварительными кликами [событие]
uint8_t btn_releaseStep_n(VirtButton *btn, const uint8_t num) {
    return btn->clicks == num && flag_eq(btn->bf, EB_CLKS_R | EB_STP, EB_CLKS_R | EB_STP);
}

// кнопка отпущена после удержания или импульсного удержания [событие]
uint8_t btn_releaseHoldStep(VirtButton *btn) {
    return btn_releaseHold(btn) || btn_releaseStep(btn);
}

// кнопка отпущена после удержания или импульсного удержания с предварительными кликами [событие]
uint8_t btn_releaseHoldStep_n(VirtButton *btn, const uint8_t num) {
    return btn_releaseHold_n(btn, num) || btn_releaseStep_n(btn, num);
}

// кнопка ожидает повторных кликов [состояние]
uint8_t btn_waiting(VirtButton *btn) {
    return btn->clicks && flag_eq(btn->bf, EB_PRS | EB_REL, 0);
}

// идёт обработка [состояние]
uint8_t btn_busy(VirtButton *btn) {
    return flag_read(btn->bf, EB_BUSY);
}

// было действие с кнопки, вернёт код события [событие]
uint16_t btn_action(VirtButton *btn) {
    switch (flag_mask(btn->bf, 0b111111111)) {
        case (EB_PRS | EB_PRS_R): return EB_PRESS;
        case (EB_PRS | EB_HLD | EB_HLD_R): return EB_HOLD;
        case (EB_PRS | EB_HLD | EB_STP | EB_STP_R): return EB_STEP;
        case (EB_REL | EB_REL_R):
        case (EB_REL | EB_REL_R | EB_HLD):
        case (EB_REL | EB_REL_R | EB_HLD | EB_STP):
            return EB_RELEASE;
        case (EB_REL_R): return EB_CLICK;
        case (EB_CLKS_R): return EB_CLICKS;
        case (EB_REL_R | EB_HLD): return EB_REL_HOLD;
        case (EB_CLKS_R | EB_HLD): return EB_REL_HOLD_C;
        case (EB_REL_R | EB_HLD | EB_STP): return EB_REL_STEP;
        case (EB_CLKS_R | EB_HLD | EB_STP): return EB_REL_STEP_C;
    }
    if (btn_timeoutState(btn)) return EB_TIMEOUT;
    return 0;
}

// было действие с кнопки, вернёт код события [событие]
EBAction btn_getAction(VirtButton *btn) {
    return (EBAction)btn_action(btn);
}

// ====================== TIME ======================
// после взаимодействия с кнопкой (или энкодером EncButton) время setTimeout, мс [событие]
uint8_t btn_timeout(VirtButton *btn) {
    if (btn_timeoutState(btn)) {
        flag_clear(btn->bf, EB_TOUT);
        return 1;
    }
    return 0;
}

// после взаимодействия с кнопкой (или энкодером EncButton) время setTimeout, мс [состояние]
uint8_t btn_timeoutState(VirtButton *btn) {
    return flag_read(btn->bf, EB_TOUT) && (uint16_t)((uint16_t)EB_uptime() - btn->tmr) >= EB_GET_TOUT_TIME();
}

// время, которое кнопка удерживается (с начала нажатия), мс
uint16_t btn_pressFor(VirtButton *btn) {
#ifndef EB_NO_FOR
    if (btn->ftmr) return (uint16_t)EB_uptime() - btn->ftmr;
#endif
    return 0;
}

// кнопка удерживается дольше чем (с начала нажатия), мс [состояние]
uint8_t btn_pressFor_t(VirtButton *btn, const uint16_t ms) {
    return btn_pressFor(btn) > ms;
}

// время, которое кнопка удерживается (с начала удержания), мс
uint16_t btn_holdFor(VirtButton *btn) {
#ifndef EB_NO_FOR
    if (flag_read(btn->bf, EB_HLD)) return btn_pressFor(btn) - EB_GET_HOLD_TIME();
#endif
    return 0;
}

// кнопка удерживается дольше чем (с начала удержания), мс [состояние]
uint8_t btn_holdFor_t(VirtButton *btn, const uint16_t ms) {
    return btn_holdFor(btn) > ms;
}

// время, которое кнопка удерживается (с начала степа), мс
uint16_t btn_stepFor(VirtButton *btn) {
#ifndef EB_NO_FOR
    if (flag_read(btn->bf, EB_STP)) return btn_pressFor(btn) - EB_GET_HOLD_TIME() * 2;
#endif
    return 0;
}

// кнопка удерживается дольше чем (с начала степа), мс [состояние]
uint8_t btn_stepFor_t(VirtButton *btn, uint16_t ms) {
    return btn_stepFor(btn) > ms;
}

// ====================== POLL ======================
// обработка виртуальной кнопки как одновременное нажатие двух других кнопок
uint8_t btn_tick_2(VirtButton *btn, VirtButton *b0, VirtButton *b1) {
    if (flag_read(btn->bf, EB_BOTH)) {
        if (!btn_pressing(b0) && !btn_pressing(b1)) flag_clear(btn->bf, EB_BOTH);
        if (!btn_pressing(b0)) btn_reset(b0);
        if (!btn_pressing(b1)) btn_reset(b1);
        btn_clear(b0, 0);
        btn_clear(b1, 0);
        return btn_tick(btn, 1);
    } else {
        if (btn_pressing(btn) && btn_pressing(b1)) flag_set(btn->bf, EB_BOTH);
        return btn_tick(btn, 0);
    }
}

// обработка кнопки значением
uint8_t btn_tick(VirtButton *btn, uint8_t s) {
    // println("enter tick");
    btn_clear(btn, 0);
    // printf("%d\n", s);
    s = btn_pollBtn(btn, s);
    // println("poll");
    // printf("%d\n", s);
#ifndef EB_NO_CALLBACK
    if (s || btn_timeoutState(btn)) btn_call(btn, 0);
#endif
    // println("leave tick");
    return s;
}

// обработка кнопки без сброса событий и вызова коллбэка
uint8_t btn_tickRaw(VirtButton *btn, const uint8_t s) {
    return btn_pollBtn(btn, s);
}

// вызвать обработчик. by defaul force = false
void btn_call(VirtButton *btn, uint8_t force) {  // todo force заменить на флаг
#ifndef EB_NO_CALLBACK
    if (btn->cb && (force || btn_action(btn))) {
        if (btn->cb) {
            btn->cb(btn);
            btn_timeout(btn);  // todo clear tout
        }
    }
#endif
}

// после взаимодействия с кнопкой (или энкодером EncButton) прошло указанное время, мс [событие]
uint8_t btn_timeout_t(VirtButton *btn, const uint16_t tout) /*__attribute__((deprecated))*/ {
    if (btn_timeoutState_t(btn, tout)) {
        flag_clear(btn->bf, EB_TOUT);
        return 1;
    }
    return 0;
}

// после взаимодействия с кнопкой (или энкодером EncButton) прошло указанное время, мс [состояние]
uint8_t btn_timeoutState_t(VirtButton *btn, const uint16_t tout) /*__attribute__((deprecated))*/ {
    return (flag_read(btn->bf, EB_TOUT) && (uint16_t)((uint16_t)EB_uptime() - btn->tmr) > tout);
}

uint8_t btn_pollBtn(VirtButton *btn, uint8_t s) {
    if (flag_read(btn->bf, EB_BISR)) {
        flag_clear(btn->bf, EB_BISR);
        s = 1;
    } else s ^= flag_read(btn->bf, EB_INV);

    if (!flag_read(btn->bf, EB_BUSY)) {
        if (s) flag_set(btn->bf, EB_BUSY);
        else return 0;
    }

    uint32_t ms = EB_uptime();
    uint32_t deb = ms - btn->tmr;

    if (s) {                                         // кнопка нажата
        if (!flag_read(btn->bf, EB_PRS)) {                      // кнопка не была нажата ранее
            if (!flag_read(btn->bf, EB_DEB) && btn->EB_DEB_T) {      // дебаунс ещё не сработал
                flag_set(btn->bf, EB_DEB);                      // будем ждать дебаунс
                btn->tmr = ms;                            // сброс таймаута
            } else {                                 // первое нажатие
                if (deb >= btn->EB_DEB_T || !btn->EB_DEB_T) {  // ждём EB_DEB_TIME
                    flag_set(btn->bf, EB_PRS | EB_PRS_R);       // флаг на нажатие
#ifndef EB_NO_FOR
                    btn->ftmr = ms;
#endif
                    btn->tmr = ms;  // сброс таймаута
                }
            }
        } else {  // кнопка уже была нажата
            if (!flag_read(btn->bf, EB_EHLD)) {
                if (!flag_read(btn->bf, EB_HLD)) {               // удержание ещё не зафиксировано
                    if (deb >= EB_GET_HOLD_TIME()) {  // ждём EB_HOLD_TIME - это удержание
                        flag_set(btn->bf, EB_HLD_R | EB_HLD);    // флаг что было удержание
                        btn->tmr = ms;                     // сброс таймаута
                    }
                } else {  // удержание зафиксировано
                    if (deb >= (uint16_t)(flag_read(btn->bf, EB_STP) ? EB_GET_STEP_TIME() : EB_GET_HOLD_TIME())) {
                        flag_set(btn->bf, EB_STP | EB_STP_R);  // флаг степ
                        btn->tmr = ms;                   // сброс таймаута
                    }
                }
            }
        }
    } else {                                       // кнопка не нажата
        if (flag_read(btn->bf, EB_PRS)) {                     // но была нажата
            if (deb >= btn->EB_DEB_T) {                 // ждём EB_DEB_TIME
                if (!flag_read(btn->bf, EB_HLD)) btn->clicks++;    // не удерживали - это клик
                if (flag_read(btn->bf, EB_EHLD)) btn->clicks = 0;  //
                flag_set(btn->bf, EB_REL | EB_REL_R);         // флаг release
                flag_clear(btn->bf, EB_PRS);                  // кнопка отпущена
            }
        } else if (flag_read(btn->bf, EB_REL)) {
            if (!flag_read(btn->bf, EB_EHLD)) {
                flag_set(btn->bf, EB_REL_R);  // флаг releaseHold / releaseStep
            }
            flag_clear(btn->bf, EB_REL | EB_EHLD);
            btn->tmr = ms;                                                                       // сброс таймаута
        } else if (btn->clicks) {                                                                // есть клики, ждём EB_CLICK_TIME
            if (flag_read(btn->bf, EB_HLD | EB_STP) || deb >= EB_GET_CLICK_TIME()) flag_set(btn->bf, EB_CLKS_R);  // флаг btn->clicks
#ifndef EB_NO_FOR
            else if (btn->ftmr) btn->ftmr = 0;
#endif
        } else if (flag_read(btn->bf, EB_BUSY)) {
            flag_clear(btn->bf, EB_HLD | EB_STP | EB_BUSY);
            flag_set(btn->bf, EB_TOUT);
#ifndef EB_NO_FOR
            btn->ftmr = 0;
#endif
            btn->tmr = ms;  // test!!
        }
        if (flag_read(btn->bf, EB_DEB)) flag_clear(btn->bf, EB_DEB);  // сброс ожидания нажатия (дебаунс)
    }
    return flag_read(btn->bf, EB_CLKS_R | EB_PRS_R | EB_HLD_R | EB_STP_R | EB_REL_R);
}
