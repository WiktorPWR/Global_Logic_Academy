#ifndef LIS302DL_OBJECT_H
#define LIS302DL_OBJECT_H


struct CTRL_REGs{
    uint8_t CRTL_REG1;
    uint8_t CRTL_REG2;
    uint8_t CRTL_REG3;
};

struct OUT_REGs{
    uint8_t OUT_X;
    uint8_t OUT_Y;
    uint8_t OUT_Z;
};

struct FF_WU_CFGs{
    uint8_t FF_WU_CFG;
    uint8_t FF_WU_SRC;
    uint8_t FF_WU_THS;
    uint8_t FF_WU_DURATION;
};

struct LIS302DL_Object{
    struct CTRL_REGs CTRL_REGS;
    uint8_t HP_FILTER_RESET;
    uint8_t STATUS_REG;
    struct OUT_REGs OUT_REGS;
    struct FF_WU_CFGs FF_WU_CFGS_1;
    struct FF_WU_CFGs FF_WU_CFGS_2;
}


#endif