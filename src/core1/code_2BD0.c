#include <ultra64.h>
#include "functions.h"
#include "variables.h"

OSIoMesg D_8027E090;
OSMesg D_8027E0A8;
OSMesgQueue D_8027E0AC;
OSMesg D_8027E0C8[16]; //g_PimgrMesgBuffer
OSMesgQueue D_8027E108; //g_PimgrMesgQueue

void func_802405F0(u32* arg0, s32 arg1, s32 size){
    s32 block_size = 0x20000;
    s32 block_cnt;
    s32 block_remainder;
    s32 i;
    
    osWritebackDCache(arg0, size);
    block_cnt       = size / block_size; 
    block_remainder = size % block_size;
    
    for (i = 0; i < block_cnt; i++){
        osPiStartDma(&D_8027E090, 0, 0, arg1, arg0, 0x20000, &D_8027E0AC);
        osRecvMesg(&D_8027E0AC, 0, 1);

        arg1 = arg1 + 0x20000;
        arg0 = arg0 + 0x8000;
    }
    
    osPiStartDma(&D_8027E090, 0, 0, arg1, arg0, block_remainder, &D_8027E0AC);
    osRecvMesg(&D_8027E0AC, 0, 1);
    osInvalDCache(arg0, size);
}

void piMgr_create(void){
    osCreateMesgQueue(&D_8027E0AC, &D_8027E0A8, 1);
    osCreateMesgQueue(&D_8027E108, &D_8027E0C8[0], 16);
    osCreatePiManager(OS_PRIORITY_PIMGR, &D_8027E108, &D_8027E0C8[0], 16);
}
