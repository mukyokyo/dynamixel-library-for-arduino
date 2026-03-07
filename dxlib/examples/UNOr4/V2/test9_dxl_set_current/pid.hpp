/*
  シンプルかつ基本的なPID制御操作。
  アンチワインドアップ付き。
  クラス本体。

  Copyright (C) 2023 T.Uemitsu
*/

#pragma once

#include <stdint.h>

//----------------------------------------------------------------
// PID演算処理
//----------------------------------------------------------------
struct CPID {
  typedef struct {
    double Kp, Ki, Kd, Kf, delta_t;
    double min, max;
    double err[2];
    double integral;
    double prevcalc;
  } TPIDParam, *PPIDParam;

  void reset(PPIDParam p){
    p->err[0] = p->err[1] = p->integral = p->prevcalc = 0;
  }

  double calc(PPIDParam p, double feedback, double target) {
    double p_term, i_term, d_term, f_term, integ, result;
    p->err[0] = p->err[1];
    p->err[1] = feedback - target;
    integ = p->integral + (p->err[1] + p->err[0]) * 0.5 * p->delta_t;
    p_term = p->Kp * p->err[1];
    i_term = p->Ki * p->integral;
    d_term = p->Kd * (p->err[1] - p->err[0]) / p->delta_t;
    f_term = p->Kf * target;
    result = p_term + i_term + d_term - f_term;

    // Reset wind-up measure
    if ((p->max == 0.0) && (p->min == 0.0)) p->integral = integ;
    else {
        if(result >= p->max) return (p->prevcalc = p->max);
        else if(result <= p->min) return (p->prevcalc = p->min);
        else p->integral = integ;
    }
    return (p->prevcalc = result);
  }
};