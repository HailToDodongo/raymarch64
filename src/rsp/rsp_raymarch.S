## Auto-generated file, transpiled with RSPL
#define RSPQ_BeginOverlayHeader ;
#define RSPQ_EndOverlayHeader ;
#define RSPQ_BeginSavedState ;
#define RSPQ_EndSavedState ;
#define RSPQ_EmptySavedState ;
#define DMEM_RAYPOS_X 0
#define DMEM_RAYPOS_Y 4
#define DMEM_RAYPOS_Z 8
#define DMEM_RAYDIR_A_X 12
#define DMEM_RAYDIR_A_Y 16
#define DMEM_RAYDIR_A_Z 20
#define DMEM_HITPOS_A_X 24
#define DMEM_HITPOS_A_Y 28
#define DMEM_HITPOS_A_Z 32
#define DMEM_LAST_DIST_A 36
#define DMEM_TOTAL_DIST_A 40
#define DMEM_RAYDIR_B_X 44
#define DMEM_RAYDIR_B_Y 48
#define DMEM_RAYDIR_B_Z 52
#define DMEM_HITPOS_B_X 56
#define DMEM_HITPOS_B_Y 60
#define DMEM_HITPOS_B_Z 64
#define DMEM_LAST_DIST_B 68
#define DMEM_TOTAL_DIST_B 72
#include <rsp.inc>

.set noreorder
.set noat
.set nomacro

#undef zero
#undef at
#undef v0
#undef v1
#undef a0
#undef a1
#undef a2
#undef a3
#undef t0
#undef t1
#undef t2
#undef t3
#undef t4
#undef t5
#undef t6
#undef t7
#undef s0
#undef s1
#undef s2
#undef s3
#undef s4
#undef s5
#undef s6
#undef s7
#undef t8
#undef t9
#undef k0
#undef k1
#undef gp
#undef sp
#undef fp
#undef ra
.equ hex.$zero, 0
.equ hex.$at, 1
.equ hex.$v0, 2
.equ hex.$v1, 3
.equ hex.$a0, 4
.equ hex.$a1, 5
.equ hex.$a2, 6
.equ hex.$a3, 7
.equ hex.$t0, 8
.equ hex.$t1, 9
.equ hex.$t2, 10
.equ hex.$t3, 11
.equ hex.$t4, 12
.equ hex.$t5, 13
.equ hex.$t6, 14
.equ hex.$t7, 15
.equ hex.$s0, 16
.equ hex.$s1, 17
.equ hex.$s2, 18
.equ hex.$s3, 19
.equ hex.$s4, 20
.equ hex.$s5, 21
.equ hex.$s6, 22
.equ hex.$s7, 23
.equ hex.$t8, 24
.equ hex.$t9, 25
.equ hex.$k0, 26
.equ hex.$k1, 27
.equ hex.$gp, 28
.equ hex.$sp, 29
.equ hex.$fp, 30
.equ hex.$ra, 31
#define vco 0
#define vcc 1
#define vce 2

.data
  RSPQ_BeginOverlayHeader
  RSPQ_EndOverlayHeader

  RSPQ_BeginSavedState
    STATE_MEM_START:
    .align 4
    _DUMMY_: .ds.b 16
    STATE_MEM_END:
  RSPQ_EndSavedState

.text
OVERLAY_CODE_START:

Main:
  vxor $v00, $v00, $v00 # inline-ASM                 ## L:49   |      ^ | asm("vxor $v00, $v00, $v00");
  ori $t0, $zero, 0x8000                             ## L:50   |      1 | u16 tmp = 0x8000;
  mtc2 $t0, $v31.e0                                  ## L:51   |      2 | VSHIFT8.x = tmp; tmp >>= 1;
  srl $t0, $t0, 1                                    ## L:51   |      3 | VSHIFT8.x = tmp; tmp >>= 1;
  mtc2 $t0, $v31.e1                                  ## L:52   |     *5 | VSHIFT8.y = tmp; tmp >>= 1;
  srl $t0, $t0, 1                                    ## L:52   |      6 | VSHIFT8.y = tmp; tmp >>= 1;
  mtc2 $t0, $v31.e2                                  ## L:53   |     *8 | VSHIFT8.z = tmp; tmp >>= 1;
  srl $t0, $t0, 1                                    ## L:53   |      9 | VSHIFT8.z = tmp; tmp >>= 1;
  mtc2 $t0, $v31.e3                                  ## L:54   |    *11 | VSHIFT8.w = tmp; tmp >>= 1;
  srl $t0, $t0, 1                                    ## L:54   |     12 | VSHIFT8.w = tmp; tmp >>= 1;
  mtc2 $t0, $v31.e4                                  ## L:55   |    *14 | VSHIFT8.X = tmp; tmp >>= 1;
  srl $t0, $t0, 1                                    ## L:55   |     15 | VSHIFT8.X = tmp; tmp >>= 1;
  mtc2 $t0, $v31.e5                                  ## L:56   |    *17 | VSHIFT8.Y = tmp; tmp >>= 1;
  srl $t0, $t0, 1                                    ## L:56   |     18 | VSHIFT8.Y = tmp; tmp >>= 1;
  mtc2 $t0, $v31.e6                                  ## L:57   |    *20 | VSHIFT8.Z = tmp; tmp >>= 1;
  srl $t0, $t0, 1                                    ## L:57   |     21 | VSHIFT8.Z = tmp; tmp >>= 1;
  mtc2 $t0, $v31.e7                                  ## L:58   |    *23 | VSHIFT8.W = tmp;
  vmudl $v30, $v31, $v31.e7                          ## L:60   |  ***27 | VSHIFT = VSHIFT8 >>> 8;
  SHIFT_END:
  addiu $at, $zero, 16384                            ## L:67   |      ^ | SPHERE_RAD.x = 0.25;
  mtc2 $at, $v07.e0                                  ## L:67   |     28 | SPHERE_RAD.x = 0.25;
  addiu $at, $zero, 4915                             ## L:68   |     29 | SPHERE_RAD.y = 0.075;
  lsv $v11, 0, 2, $zero                              ## L:81   |     30 | rayPosOrg.x = load(ZERO, 0).x;
  mtc2 $at, $v07.e1                                  ## L:68   |     31 | SPHERE_RAD.y = 0.075;
  ori $at, $zero, 0x8000                             ## L:69   |     32 | SPHERE_RAD.z = 0.5;
  lsv $v11, 2, 6, $zero                              ## L:82   |     33 | rayPosOrg.y = load(ZERO, 4).x;
  mtc2 $at, $v07.e2                                  ## L:69   |     34 | SPHERE_RAD.z = 0.5;
  ori $at, $zero, 0x93CA                             ## L:70   |     35 | SPHERE_RAD.w = 0.5773;
  lsv $v11, 4, 10, $zero                             ## L:83   |     36 | rayPosOrg.z = load(ZERO, 8).x;
  mtc2 $at, $v07.e3                                  ## L:70   |     37 | SPHERE_RAD.w = 0.5773;
  addiu $at, $zero, 2                                ## L:72   |     38 | SPHERE_RAD.X = 2.5;
  lsv $v10, 0, 0, $zero                              ## L:81   |     39 | rayPosOrg.x = load(ZERO, 0).x;
  mtc2 $at, $v06.e4                                  ## L:72   |     40 | SPHERE_RAD.X = 2.5;
  vmov $v11.e6, $v11.e2                              ## L:87   |      ^ | rayPosOrg.Z = rayPosOrg.z;
  ori $at, $zero, 0x8000                             ## L:72   |     41 | SPHERE_RAD.X = 2.5;
  mtc2 $zero, $v06.e1                                ## L:68   |    *43 | SPHERE_RAD.y = 0.075;
  mtc2 $at, $v07.e4                                  ## L:72   |     44 | SPHERE_RAD.X = 2.5;
  lsv $v10, 2, 4, $zero                              ## L:82   |     45 | rayPosOrg.y = load(ZERO, 4).x;
  addiu $at, $zero, 13107                            ## L:73   |     46 | SPHERE_RAD.Y = 0.2;
  lsv $v10, 4, 8, $zero                              ## L:83   |     47 | rayPosOrg.z = load(ZERO, 8).x;
  vmov $v11.e5, $v11.e1                              ## L:86   |      ^ | rayPosOrg.Y = rayPosOrg.y;
  mtc2 $at, $v07.e5                                  ## L:73   |     48 | SPHERE_RAD.Y = 0.2;
  ori $at, $zero, 0xFFFF                             ## L:75   |     49 | SPHERE_RAD.Z = -0.25;
  mtc2 $at, $v06.e6                                  ## L:75   |    *51 | SPHERE_RAD.Z = -0.25;
  mtc2 $zero, $v06.e5                                ## L:73   |     52 | SPHERE_RAD.Y = 0.2;
  addiu $a1, $zero, 1048                             ## L:92   |     53 | s32<$a1> STOP_DIST   = 0.016 * 0xFFFF;
  ori $at, $zero, 0xC000                             ## L:75   |     54 | SPHERE_RAD.Z = -0.25;
  vmov $v10.e5, $v10.e1                              ## L:86   |      ^ | rayPosOrg.Y = rayPosOrg.y;
  mtc2 $zero, $v06.e0                                ## L:67   |     55 | SPHERE_RAD.x = 0.25;
  mtc2 $at, $v07.e6                                  ## L:75   |     56 | SPHERE_RAD.Z = -0.25;
  ori $at, $zero, 0xFFFF                             ## L:76   |     57 | SPHERE_RAD.W = -0.075;
  mtc2 $at, $v06.e7                                  ## L:76   |    *59 | SPHERE_RAD.W = -0.075;
  mtc2 $zero, $v06.e3                                ## L:70   |     60 | SPHERE_RAD.w = 0.5773;
  vmov $v10.e4, $v10.e0                              ## L:85   |      ^ | rayPosOrg.X = rayPosOrg.x;
  lui $a0, 0x0B                                      ## L:91   |     61 | s32<$a0> RENDER_DIST = 11.0 << 16;
  vmov $v11.e4, $v11.e0                              ## L:85   |      ^ | rayPosOrg.X = rayPosOrg.x;
  vxor $v04, $v00, $v30.e7                           ## L:64   |     62 | vec32<$v04> ONE = 1;
  ori $at, $zero, 0xECCD                             ## L:76   |      ^ | SPHERE_RAD.W = -0.075;
  mtc2 $at, $v07.e7                                  ## L:76   |     63 | SPHERE_RAD.W = -0.075;
  vxor $v05, $v00, $v00                              ## L:64   |      ^ | vec32<$v04> ONE = 1;
  vmov $v10.e6, $v10.e2                              ## L:87   |     64 | rayPosOrg.Z = rayPosOrg.z;
  mtc2 $zero, $v06.e2                                ## L:69   |      ^ | SPHERE_RAD.z = 0.5;
RayMarch_Main:
  vxor $v17, $v00, $v00                              ## L:154  |      ^ | vec32 totalDist = 0;
  ldv $v12, 0, 16, $zero                             ## L:161  |      ^ | rayDir:sint.xyzw = load(ZERO, 16).xyzw;
  vxor $v14, $v00, $v00.e0                           ## L:150  |      2 | res:sint = 0;
  ldv $v12, 8, 48, $zero                             ## L:162  |      ^ | rayDir:sint.XYZW = load(ZERO, 48).xyzw;
  vxor $v16, $v00, $v00.e0                           ## L:154  |      3 | vec32 totalDist = 0;
  lsv $v15, 0, 82, $zero                             ## L:151  |      ^ | res:sfract.x = load(ZERO, 80).y;
  lsv $v15, 8, 82, $zero                             ## L:152  |      4 | res:sfract.X = load(ZERO, 80).y;
  vxor $v01, $v00, $v00.e0                           ## L:147  |      ^ | posSq:sint = 0;
  addiu $t6, $zero, 1                                ## L:138  |      5 | u8<$t6> isDoneBFlag = 1;
  vmudl $v13, $v13, $v30.e6                          ## L:163  |      ^ | rayDir >>= 15;
  vmadm $v12, $v12, $v30.e6                          ## L:163  |      6 | rayDir >>= 15;
  or $t1, $zero, $zero                               ## L:142  |      ^ | s32 totalDistB = 0;
  vmadn $v13, $v00, $v00                             ## L:163  |      7 | rayDir >>= 15;
  addiu $t5, $zero, 1                                ## L:137  |      ^ | u8<$t5> isDoneAFlag = 1;
  or $t0, $zero, $zero                               ## L:141  |      8 | s32 totalDistA = 0;
  LABEL_RayMarch_Main_0001:
  vaddc $v17, $v17, $v15.v                           ## L:167  |      ^ | totalDist += res;
  ssv $v14, 0, 36, $zero                             ## L:169  |      9 | @Barrier("lenA") store(res.x, ZERO, 36); ## Barrier: 0x1
  vadd $v16, $v16, $v14.v                            ## L:167  |      ^ | totalDist += res;
  vmudl $v29, $v05, $v11.v                           ## L:187  |     10 | nextPos = ONE * rayPosOrg;
  ssv $v14, 8, 68, $zero                             ## L:170  |      ^ | @Barrier("lenB") store(res.X, ZERO, 68); ## Barrier: 0x2
  ssv $v15, 8, 70, $zero                             ## L:170  |     11 | @Barrier("lenB") store(res.X, ZERO, 68); ## Barrier: 0x2
  vmadm $v29, $v04, $v11.v                           ## L:187  |      ^ | nextPos = ONE * rayPosOrg;
  vmadn $v19, $v05, $v10.v                           ## L:187  |     12 | nextPos = ONE * rayPosOrg;
  ssv $v15, 0, 38, $zero                             ## L:169  |      ^ | @Barrier("lenA") store(res.x, ZERO, 36); ## Barrier: 0x1
  lw $t3, 68($zero)                                  ## L:173  |     13 | @Barrier("lenB") distB = load(ZERO, 68); ## Barrier: 0x2
  vmadh $v18, $v04, $v10.v                           ## L:187  |      ^ | nextPos = ONE * rayPosOrg;
  vmadl $v29, $v13, $v17.h0                          ## L:188  |     14 | nextPos = rayDir +* totalDist.xxxxXXXX;
  lw $t2, 36($zero)                                  ## L:172  |      ^ | @Barrier("lenA") distA = load(ZERO, 36); ## Barrier: 0x1
  vmadm $v29, $v12, $v17.h0                          ## L:188  |     15 | nextPos = rayDir +* totalDist.xxxxXXXX;
  vmadn $v19, $v13, $v16.h0                          ## L:188  |     16 | nextPos = rayDir +* totalDist.xxxxXXXX;
  slt $t8, $t3, $a1                                  ## L:179  |      ^ | u8 isDoneB = distB < STOP_DIST;
  vmadh $v18, $v12, $v16.h0                          ## L:188  |     17 | nextPos = rayDir +* totalDist.xxxxXXXX;
  slt $t7, $t2, $a1                                  ## L:178  |      ^ | u8 isDoneA = distA < STOP_DIST;
  addu $t0, $t0, $t2                                 ## L:175  |     18 | totalDistA += distA;
  vmadl $v21, $v05, $v07.e2                          ## L:191  |      ^ | nextPosFloor:sint = ONE +* SPHERE_RAD:sfract.z;
  vmadm $v20, $v04, $v07.e2                          ## L:191  |     19 | nextPosFloor:sint = ONE +* SPHERE_RAD:sfract.z;
  slt $t4, $a0, $t0                                  ## L:181  |      ^ | tmp = totalDistA > RENDER_DIST;
  or $t7, $t7, $t4                                   ## L:182  |     20 | isDoneA |= tmp;
  addu $t1, $t1, $t3                                 ## L:176  |     21 | totalDistB += distB;
  slt $t4, $a0, $t1                                  ## L:183  |     22 | tmp = totalDistB > RENDER_DIST;
  vmadn $v21, $v00, $v00                             ## L:191  |      ^ | nextPosFloor:sint = ONE +* SPHERE_RAD:sfract.z;
  vsub $v18, $v18, $v20.v                            ## L:193  |     23 | nextPos:sint -= nextPosFloor:sint;
  beq $t7, $t5, Main_markDoneA                       ## L:196  |      ^ | if(isDoneA == isDoneAFlag)goto Main_markDoneA;
  or $t8, $t8, $t4                                   ## L:184  |    *25 | isDoneB |= tmp;
  __Main_RET_MARK_DONE_A:
  vmudl $v29, $v19, $v19.v                           ## L:203  |     26 | posSq:sfract = nextPos * nextPos;
  vmadm $v29, $v18, $v19.v                           ## L:203  |     27 | posSq:sfract = nextPos * nextPos;
  beq $t8, $t6, Main_markDoneB                       ## L:206  |      ^ | if(isDoneB == isDoneBFlag)goto Main_markDoneB;
  vmadn $v02, $v19, $v18.v                           ## L:203  |    *29 | posSq:sfract = nextPos * nextPos;
  __Main_RET_MARK_DONE_B:
  lsv $v09, 0, 76, $zero                             ## L:210  |     30 | LERP_FACTOR:ufract.x = load(ZERO, 76).x;
  vrsqh $v22.e0, $v00.e0                             ## L:97   |      ^ | asm_op("vrsqh", out:sint.x, VZERO.x);
  vaddc $v15, $v02, $v02.h2                          ## L:222  |   **33 | res = posSq:sfract + posSq:sfract.zzzzZZZZ;
  lsv $v09, 8, 76, $zero                             ## L:212  |      ^ | LERP_FACTOR:ufract.X = load(ZERO, 76).x;
  vadd $v14, $v00, $v00.h2                           ## L:222  |     34 | res = posSq:sfract + posSq:sfract.zzzzZZZZ;
  vrsql $v23.e0, $v15.e0                             ## L:98   |   **37 | asm_op("vrsql", out:sfract.x, in:sfract.x);
  vrsqh $v22.e0, $v00.e4                             ## L:105  |     38 | asm_op("vrsqh", out:sint.X, VZERO.X);
  vrsql $v23.e4, $v15.e4                             ## L:106  |     39 | asm_op("vrsql", out:sfract.X, in:sfract.X);
  vrsqh $v22.e4, $v00.e0                             ## L:107  |     40 | asm_op("vrsqh", out:sint.X, VZERO.x);
  vaddc $v21, $v15, $v02.h1                          ## L:226  |     41 | tmpA = res:sfract + posSq:sfract.yyyyYYYY;
  vadd $v20, $v00, $v00.h1                           ## L:226  |     42 | tmpA = res:sfract + posSq:sfract.yyyyYYYY;
  vmudm $v14, $v22, $v31.e7                          ## L:113  |    *44 | asm_op("vmudm", out:sint,   in:sint,   VSHIFT8.W);
  vmadl $v15, $v23, $v31.e7                          ## L:114  |     45 | asm_op("vmadl", out:sfract, in:sfract, VSHIFT8.W);
  vrsqh $v24.e0, $v00.e0                             ## L:97   |     46 | asm_op("vrsqh", out:sint.x, VZERO.x);
  vrsql $v25.e0, $v21.e0                             ## L:98   |     47 | asm_op("vrsql", out:sfract.x, in:sfract.x);
  vrsqh $v24.e0, $v00.e0                             ## L:99   |     48 | asm_op("vrsqh", out:sint.x, VZERO.x);
  vrcph $v29.e0, $v14.e0                             ## L:233  |     49 | tmpB:sfract.x = invert_half(res).x;
  vrcpl $v23.e0, $v15.e0                             ## L:233  |     50 | tmpB:sfract.x = invert_half(res).x;
  vrcph $v29.e4, $v14.e4                             ## L:234  |     51 | tmpB:sfract.X = invert_half(res).X;
  mfc2 $t9, $v25.e0                                  ## L:231  |      ^ | s32 resSphereA = resSphere.x;
  vrcpl $v23.e4, $v15.e4                             ## L:234  |     52 | tmpB:sfract.X = invert_half(res).X;
  mfc2 $at, $v24.e0                                  ## L:231  |      ^ | s32 resSphereA = resSphere.x;
  vrsqh $v24.e4, $v00.e4                             ## L:105  |     53 | asm_op("vrsqh", out:sint.X, VZERO.X);
  vrsql $v25.e4, $v21.e4                             ## L:106  |     54 | asm_op("vrsql", out:sfract.X, in:sfract.X);
  andi $t9, $t9, 65535                               ## L:231  |      ^ | s32 resSphereA = resSphere.x;
  sll $at, $at, 16                                   ## L:231  |    *56 | s32 resSphereA = resSphere.x;
  vsubc $v15, $v23, $v07.e0                          ## L:244  |      ^ | res:ufract = tmpB:ufract - SPHERE_RAD:ufract.x;
  vsub $v14, $v00, $v06.e0                           ## L:245  |     57 | res:sint = VZERO - SPHERE_RAD:sint.x;
  or $t9, $t9, $at                                   ## L:231  |      ^ | s32 resSphereA = resSphere.x;
  vrsqh $v24.e4, $v00.e0                             ## L:107  |     58 | asm_op("vrsqh", out:sint.X, VZERO.x);
  sra $t9, $t9, 8                                    ## L:239  |      ^ | resSphereA >>= 8;
  mfc2 $k0, $v25.e4                                  ## L:237  |     59 | s32 resSphereB = resSphere.X;
  sw $t9, 120($zero)                                 ## L:241  |     60 | @Barrier("a") store(resSphereA, ZERO, 120); ## Barrier: 0x4
  vmudl $v29, $v15, $v15.v                           ## L:247  |      ^ | res:sfract = res * res;
  lsv $v22, 0, 120, $zero                            ## L:250  |     61 | @Barrier("a") tmpB.x = load(ZERO, 120).x; ## Barrier: 0x4
  vmadm $v29, $v14, $v15.v                           ## L:247  |      ^ | res:sfract = res * res;
  mfc2 $at, $v24.e4                                  ## L:237  |     62 | s32 resSphereB = resSphere.X;
  vmadn $v15, $v15, $v14.v                           ## L:247  |      ^ | res:sfract = res * res;
  vmadl $v29, $v05, $v02.h1                          ## L:248  |     63 | tmpA = ONE +* posSq.yyyyYYYY;
  andi $k0, $k0, 65535                               ## L:237  |      ^ | s32 resSphereB = resSphere.X;
  vmadm $v29, $v04, $v02.h1                          ## L:248  |     64 | tmpA = ONE +* posSq.yyyyYYYY;
  lsv $v23, 0, 122, $zero                            ## L:250  |      ^ | @Barrier("a") tmpB.x = load(ZERO, 120).x; ## Barrier: 0x4
  vmadn $v21, $v05, $v01.h1                          ## L:248  |     65 | tmpA = ONE +* posSq.yyyyYYYY;
  sll $at, $at, 16                                   ## L:237  |      ^ | s32 resSphereB = resSphere.X;
  or $k0, $k0, $at                                   ## L:237  |     66 | s32 resSphereB = resSphere.X;
  vmadh $v20, $v04, $v01.h1                          ## L:248  |      ^ | tmpA = ONE +* posSq.yyyyYYYY;
  vrsqh $v14.e0, $v00.e0                             ## L:97   |     67 | asm_op("vrsqh", out:sint.x, VZERO.x);
  sra $k0, $k0, 8                                    ## L:240  |      ^ | resSphereB >>= 8;
  sw $k0, 124($zero)                                 ## L:242  |     68 | @Barrier("b") store(resSphereB, ZERO, 124); ## Barrier: 0x8
  lsv $v22, 8, 124, $zero                            ## L:251  |     69 | @Barrier("b") tmpB.X = load(ZERO, 124).x; ## Barrier: 0x8
  vrsql $v15.e0, $v21.e0                             ## L:98   |      ^ | asm_op("vrsql", out:sfract.x, in:sfract.x);
  vrsqh $v14.e0, $v00.e4                             ## L:105  |     70 | asm_op("vrsqh", out:sint.X, VZERO.X);
  lsv $v23, 8, 126, $zero                            ## L:251  |      ^ | @Barrier("b") tmpB.X = load(ZERO, 124).x; ## Barrier: 0x8
  lsv $v09, 2, 78, $zero                             ## L:211  |     71 | LERP_FACTOR:ufract.y = load(ZERO, 78).x;
  vrsql $v15.e4, $v21.e4                             ## L:106  |      ^ | asm_op("vrsql", out:sfract.X, in:sfract.X);
  vrsqh $v14.e4, $v00.e0                             ## L:107  |     72 | asm_op("vrsqh", out:sint.X, VZERO.x);
  vrcph $v29.e0, $v22.e0                             ## L:259  |     73 | resSphere:sfract.x = invert_half(tmpB).x;
  vrcpl $v25.e0, $v23.e0                             ## L:259  |     74 | resSphere:sfract.x = invert_half(tmpB).x;
  vmudm $v20, $v14, $v31.e7                          ## L:113  |    *76 | asm_op("vmudm", out:sint,   in:sint,   VSHIFT8.W);
  vmadl $v21, $v15, $v31.e7                          ## L:114  |     77 | asm_op("vmadl", out:sfract, in:sfract, VSHIFT8.W);
  vrcph $v29.e4, $v22.e4                             ## L:264  |     78 | resSphere:sfract.X = invert_half(tmpB).X;
  vrcpl $v25.e4, $v23.e4                             ## L:264  |     79 | resSphere:sfract.X = invert_half(tmpB).X;
  vrcph $v29.e0, $v20.e0                             ## L:266  |     80 | res:sfract.x = invert_half(tmpA).x;
  vrcpl $v15.e0, $v21.e0                             ## L:266  |     81 | res:sfract.x = invert_half(tmpA).x;
  vrcph $v29.e4, $v20.e4                             ## L:267  |     82 | res:sfract.X = invert_half(tmpA).X;
  vrcpl $v15.e4, $v21.e4                             ## L:267  |     83 | res:sfract.X = invert_half(tmpA).X;
  vsubc $v25, $v25, $v07.e0                          ## L:269  |     84 | resSphere:ufract = resSphere:ufract - SPHERE_RAD:ufract.x;
  vsub $v24, $v00, $v00.v                            ## L:270  |     85 | resSphere:sint = VZERO - VZERO;
  lsv $v09, 10, 78, $zero                            ## L:213  |      ^ | LERP_FACTOR:ufract.Y = load(ZERO, 78).x;
  vsubc $v15, $v15, $v07.e1                          ## L:272  |    *87 | res:ufract = res:ufract - SPHERE_RAD:ufract.y;
  vsub $v14, $v00, $v00.v                            ## L:273  |     88 | res:sint = VZERO - VZERO;
  vmudl $v25, $v25, $v09.h1                          ## L:276  |     89 | resSphere = resSphere * LERP_FACTOR:ufract.yyyyYYYY;
  vmadm $v24, $v24, $v09.h1                          ## L:276  |     90 | resSphere = resSphere * LERP_FACTOR:ufract.yyyyYYYY;
  vmadn $v25, $v00, $v00                             ## L:276  |     91 | resSphere = resSphere * LERP_FACTOR:ufract.yyyyYYYY;
  vmadl $v15, $v15, $v09.h0                          ## L:277  |     92 | res = res +* LERP_FACTOR:ufract.xxxxXXXX;
  vmadm $v14, $v14, $v09.h0                          ## L:277  |     93 | res = res +* LERP_FACTOR:ufract.xxxxXXXX;
  j LABEL_RayMarch_Main_0001                         ## L:277  |      ^ | res = res +* LERP_FACTOR:ufract.xxxxXXXX;
  vmadn $v15, $v00, $v00                             ## L:277  |    *95 | res = res +* LERP_FACTOR:ufract.xxxxXXXX;
  Main_markDoneA:
  addiu $t5, $zero, 255                              ## L:351  |     96 | isDoneAFlag = 0xFF;
  bne $t5, $t6, __Main_RET_MARK_DONE_A               ## L:352  |     97 | if(isDoneAFlag != isDoneBFlag)goto __Main_RET_MARK_DONE_A;
  sw $t0, 40($zero)                                  ## L:350  |    *99 | store(totalDistA, ZERO, 40);
  break # inline-ASM                                 ## L:353  |      0 | asm("break");
  Main_markDoneB:
  addiu $t6, $zero, 255                              ## L:359  |    100 | isDoneBFlag = 0xFF;
  bne $t5, $t6, __Main_RET_MARK_DONE_B               ## L:360  |    101 | if(isDoneAFlag != isDoneBFlag)goto __Main_RET_MARK_DONE_B;
  sw $t1, 72($zero)                                  ## L:358  |   *103 | store(totalDistB, ZERO, 72);
  break # inline-ASM                                 ## L:361  |      0 | asm("break");
RayMarch_Sphere:
  ldv $v12, 0, 16, $zero                             ## L:402  |      ^ | rayDir:sint.xyzw = load(ZERO, 16).xyzw;
  vxor $v16, $v00, $v00.e0                           ## L:395  |      ^ | vec32 totalDist = 0;
  ldv $v12, 8, 48, $zero                             ## L:403  |      2 | rayDir:sint.XYZW = load(ZERO, 48).xyzw;
  vxor $v14, $v00, $v00.e0                           ## L:391  |      ^ | res:sint = 0;
  vxor $v01, $v00, $v00.e0                           ## L:388  |      3 | posSq:sint = 0;
  lsv $v15, 0, 82, $zero                             ## L:392  |      ^ | res:sfract.x = load(ZERO, 80).y;
  lsv $v15, 8, 82, $zero                             ## L:393  |      4 | res:sfract.X = load(ZERO, 80).y;
  vxor $v17, $v00, $v00                              ## L:395  |      ^ | vec32 totalDist = 0;
  vmudl $v13, $v13, $v30.e6                          ## L:404  |      5 | rayDir >>= 15;
  addiu $t6, $zero, 1                                ## L:379  |      ^ | u8<$t6> isDoneBFlag = 1;
  vmadm $v12, $v12, $v30.e6                          ## L:404  |      6 | rayDir >>= 15;
  addiu $t5, $zero, 1                                ## L:378  |      ^ | u8<$t5> isDoneAFlag = 1;
  or $t0, $zero, $zero                               ## L:382  |      7 | s32 totalDistA = 0;
  vmadn $v13, $v00, $v00                             ## L:404  |      ^ | rayDir >>= 15;
  or $t1, $zero, $zero                               ## L:383  |      8 | s32 totalDistB = 0;
  LABEL_RayMarch_Sphere_0007:
  vaddc $v17, $v17, $v15.v                           ## L:408  |      ^ | totalDist += res;
  ssv $v14, 8, 68, $zero                             ## L:411  |      9 | @Barrier("lenB") store(res.X, ZERO, 68); ## Barrier: 0x2
  vadd $v16, $v16, $v14.v                            ## L:408  |      ^ | totalDist += res;
  vmudl $v29, $v05, $v11.v                           ## L:428  |     10 | nextPos = ONE * rayPosOrg;
  ssv $v14, 0, 36, $zero                             ## L:410  |      ^ | @Barrier("lenA") store(res.x, ZERO, 36); ## Barrier: 0x1
  ssv $v15, 0, 38, $zero                             ## L:410  |     11 | @Barrier("lenA") store(res.x, ZERO, 36); ## Barrier: 0x1
  vmadm $v29, $v04, $v11.v                           ## L:428  |      ^ | nextPos = ONE * rayPosOrg;
  lw $t2, 36($zero)                                  ## L:413  |     12 | @Barrier("lenA") distA = load(ZERO, 36); ## Barrier: 0x1
  vmadn $v19, $v05, $v10.v                           ## L:428  |      ^ | nextPos = ONE * rayPosOrg;
  vmadh $v18, $v04, $v10.v                           ## L:428  |     13 | nextPos = ONE * rayPosOrg;
  ssv $v15, 8, 70, $zero                             ## L:411  |      ^ | @Barrier("lenB") store(res.X, ZERO, 68); ## Barrier: 0x2
  lw $t3, 68($zero)                                  ## L:414  |     14 | @Barrier("lenB") distB = load(ZERO, 68); ## Barrier: 0x2
  vmadl $v29, $v13, $v17.h0                          ## L:429  |      ^ | nextPos = rayDir +* totalDist.xxxxXXXX;
  addu $t0, $t0, $t2                                 ## L:416  |     15 | totalDistA += distA;
  vmadm $v29, $v12, $v17.h0                          ## L:429  |      ^ | nextPos = rayDir +* totalDist.xxxxXXXX;
  slt $t4, $a0, $t0                                  ## L:422  |     16 | tmp = totalDistA > RENDER_DIST;
  vmadn $v19, $v13, $v16.h0                          ## L:429  |      ^ | nextPos = rayDir +* totalDist.xxxxXXXX;
  slt $t7, $t2, $a1                                  ## L:419  |     17 | u8 isDoneA = distA < STOP_DIST;
  vmadh $v18, $v12, $v16.h0                          ## L:429  |      ^ | nextPos = rayDir +* totalDist.xxxxXXXX;
  or $t7, $t7, $t4                                   ## L:423  |     18 | isDoneA |= tmp;
  vmadl $v21, $v05, $v07.e2                          ## L:433  |      ^ | nextPosFloor:sint = ONE +* SPHERE_RAD:sfract.z;
  addu $t1, $t1, $t3                                 ## L:417  |     19 | totalDistB += distB;
  vmadm $v20, $v04, $v07.e2                          ## L:433  |      ^ | nextPosFloor:sint = ONE +* SPHERE_RAD:sfract.z;
  slt $t4, $a0, $t1                                  ## L:424  |     20 | tmp = totalDistB > RENDER_DIST;
  slt $t8, $t3, $a1                                  ## L:420  |     21 | u8 isDoneB = distB < STOP_DIST;
  or $t8, $t8, $t4                                   ## L:425  |     22 | isDoneB |= tmp;
  vmadn $v21, $v00, $v00                             ## L:433  |      ^ | nextPosFloor:sint = ONE +* SPHERE_RAD:sfract.z;
  vsub $v18, $v18, $v20.v                            ## L:435  |     23 | nextPos:sint -= nextPosFloor:sint;
  beq $t7, $t5, Sphere_markDoneA                     ## L:443  |      ^ | if(isDoneA == isDoneAFlag)goto Sphere_markDoneA;
  nop                                                ## L:645  |    *25 | 
  __Sphere_RET_MARK_DONE_A:
  vmudl $v29, $v19, $v19.v                           ## L:450  |     26 | posSq:sfract = nextPos * nextPos;
  vmadm $v29, $v18, $v19.v                           ## L:450  |     27 | posSq:sfract = nextPos * nextPos;
  beq $t8, $t6, Sphere_markDoneB                     ## L:453  |      ^ | if(isDoneB == isDoneBFlag)goto Sphere_markDoneB;
  vmadn $v02, $v19, $v18.v                           ## L:450  |    *29 | posSq:sfract = nextPos * nextPos;
  __Sphere_RET_MARK_DONE_B:
  vrsqh $v20.e0, $v00.e0                             ## L:97   |     30 | asm_op("vrsqh", out:sint.x, VZERO.x);
  lsv $v09, 0, 76, $zero                             ## L:458  |      ^ | LERP_FACTOR:ufract.x = load(ZERO, 76).x;
  lsv $v09, 2, 78, $zero                             ## L:459  |   **33 | LERP_FACTOR:ufract.y = load(ZERO, 78).x;
  vaddc $v15, $v02, $v02.h2                          ## L:532  |      ^ | res = posSq:sfract + posSq:sfract.zzzzZZZZ;
  vadd $v14, $v00, $v00.h2                           ## L:532  |     34 | res = posSq:sfract + posSq:sfract.zzzzZZZZ;
  vaddc $v15, $v15, $v02.h1                          ## L:533  |   **37 | res += posSq:sfract.yyyyYYYY;
  vadd $v14, $v14, $v00.h1                           ## L:533  |     38 | res += posSq:sfract.yyyyYYYY;
  vrsql $v21.e0, $v15.e0                             ## L:98   |   **41 | asm_op("vrsql", out:sfract.x, in:sfract.x);
  vrsqh $v20.e0, $v00.e4                             ## L:105  |     42 | asm_op("vrsqh", out:sint.X, VZERO.X);
  vrsql $v23.e4, $v15.e4                             ## L:106  |     43 | asm_op("vrsql", out:sfract.X, in:sfract.X);
  vrsqh $v22.e4, $v00.e0                             ## L:107  |     44 | asm_op("vrsqh", out:sint.X, VZERO.x);
  vmudm $v20, $v20, $v31.e7                          ## L:113  |    *46 | asm_op("vmudm", out:sint,   in:sint,   VSHIFT8.W);
  vmadl $v21, $v21, $v31.e7                          ## L:114  |     47 | asm_op("vmadl", out:sfract, in:sfract, VSHIFT8.W);
  vmudm $v22, $v22, $v31.e7                          ## L:113  |     48 | asm_op("vmudm", out:sint,   in:sint,   VSHIFT8.W);
  vmadl $v23, $v23, $v31.e7                          ## L:114  |     49 | asm_op("vmadl", out:sfract, in:sfract, VSHIFT8.W);
  vrcph $v14.e0, $v20.e0                             ## L:541  |     50 | res.x = invert_half(tmpA).x;
  lsv $v09, 8, 76, $zero                             ## L:460  |      ^ | LERP_FACTOR:ufract.X = load(ZERO, 76).x;
  vrcpl $v15.e0, $v21.e0                             ## L:541  |     51 | res.x = invert_half(tmpA).x;
  lsv $v09, 10, 78, $zero                            ## L:461  |      ^ | LERP_FACTOR:ufract.Y = load(ZERO, 78).x;
  vrcph $v14.e0, $v22.e4                             ## L:542  |     52 | res.X = invert_half(tmpB).X;
  vrcpl $v15.e4, $v23.e4                             ## L:542  |     53 | res.X = invert_half(tmpB).X;
  vrcph $v14.e4, $v00.e4                             ## L:542  |     54 | res.X = invert_half(tmpB).X;
  vsubc $v15, $v15, $v09.h1                          ## L:544  |   **57 | res -= LERP_FACTOR.yyyyYYYY;
  j LABEL_RayMarch_Sphere_0007                       ## L:544  |      ^ | res -= LERP_FACTOR.yyyyYYYY;
  vsub $v14, $v14, $v08.h1                           ## L:544  |    *59 | res -= LERP_FACTOR.yyyyYYYY;
  Sphere_markDoneA:
  addiu $t5, $zero, 255                              ## L:580  |     60 | isDoneAFlag = 0xFF;
  bne $t5, $t6, __Sphere_RET_MARK_DONE_A             ## L:581  |     61 | if(isDoneAFlag != isDoneBFlag)goto __Sphere_RET_MARK_DONE_A;
  sw $t0, 40($zero)                                  ## L:579  |    *63 | store(totalDistA, ZERO, 40);
  break # inline-ASM                                 ## L:582  |      0 | asm("break");
  Sphere_markDoneB:
  addiu $t6, $zero, 255                              ## L:588  |     64 | isDoneBFlag = 0xFF;
  bne $t5, $t6, __Sphere_RET_MARK_DONE_B             ## L:589  |     65 | if(isDoneAFlag != isDoneBFlag)goto __Sphere_RET_MARK_DONE_B;
  sw $t1, 72($zero)                                  ## L:587  |    *67 | store(totalDistB, ZERO, 72);
  break # inline-ASM                                 ## L:590  |      0 | asm("break");
RayMarch_Cylinder:
  vxor $v14, $v00, $v00.e0                           ## L:627  |      ^ | res:sint = 0;
  ldv $v12, 0, 16, $zero                             ## L:638  |      ^ | rayDir:sint.xyzw = load(ZERO, 16).xyzw;
  vxor $v01, $v00, $v00.e0                           ## L:624  |      2 | posSq:sint = 0;
  ldv $v12, 8, 48, $zero                             ## L:639  |      ^ | rayDir:sint.XYZW = load(ZERO, 48).xyzw;
  vxor $v17, $v00, $v00                              ## L:631  |      3 | vec32 totalDist = 0;
  lsv $v15, 0, 82, $zero                             ## L:628  |      ^ | res:sfract.x = load(ZERO, 80).y;
  vxor $v16, $v00, $v00.e0                           ## L:631  |      4 | vec32 totalDist = 0;
  lsv $v15, 8, 82, $zero                             ## L:629  |      ^ | res:sfract.X = load(ZERO, 80).y;
  addiu $t5, $zero, 1                                ## L:614  |      5 | u8<$t5> isDoneAFlag = 1;
  vmudl $v13, $v13, $v30.e6                          ## L:640  |      ^ | rayDir >>= 15;
  addiu $t6, $zero, 1                                ## L:615  |      6 | u8<$t6> isDoneBFlag = 1;
  vmadm $v12, $v12, $v30.e6                          ## L:640  |      ^ | rayDir >>= 15;
  or $t0, $zero, $zero                               ## L:618  |      7 | s32 totalDistA = 0;
  vmadn $v13, $v00, $v00                             ## L:640  |      ^ | rayDir >>= 15;
  or $t1, $zero, $zero                               ## L:619  |      8 | s32 totalDistB = 0;
  LABEL_RayMarch_Cylinder_000D:
  vaddc $v17, $v17, $v15.v                           ## L:644  |      ^ | totalDist += res;
  vadd $v16, $v16, $v14.v                            ## L:644  |      9 | totalDist += res;
  ssv $v14, 8, 68, $zero                             ## L:647  |      ^ | @Barrier("lenB") store(res.X, ZERO, 68); ## Barrier: 0x2
  ssv $v14, 0, 36, $zero                             ## L:646  |     10 | @Barrier("lenA") store(res.x, ZERO, 36); ## Barrier: 0x1
  vmudl $v29, $v05, $v11.v                           ## L:664  |      ^ | nextPos = ONE * rayPosOrg;
  vmadm $v29, $v04, $v11.v                           ## L:664  |     11 | nextPos = ONE * rayPosOrg;
  vmadn $v19, $v05, $v10.v                           ## L:664  |     12 | nextPos = ONE * rayPosOrg;
  ssv $v15, 0, 38, $zero                             ## L:646  |      ^ | @Barrier("lenA") store(res.x, ZERO, 36); ## Barrier: 0x1
  lw $t2, 36($zero)                                  ## L:649  |     13 | @Barrier("lenA") distA = load(ZERO, 36); ## Barrier: 0x1
  vmadh $v18, $v04, $v10.v                           ## L:664  |      ^ | nextPos = ONE * rayPosOrg;
  ssv $v15, 8, 70, $zero                             ## L:647  |     14 | @Barrier("lenB") store(res.X, ZERO, 68); ## Barrier: 0x2
  vmadl $v29, $v13, $v17.h0                          ## L:665  |      ^ | nextPos = rayDir +* totalDist.xxxxXXXX;
  vmadm $v29, $v12, $v17.h0                          ## L:665  |     15 | nextPos = rayDir +* totalDist.xxxxXXXX;
  lw $t3, 68($zero)                                  ## L:650  |      ^ | @Barrier("lenB") distB = load(ZERO, 68); ## Barrier: 0x2
  addu $t0, $t0, $t2                                 ## L:652  |     16 | totalDistA += distA;
  vmadn $v19, $v13, $v16.h0                          ## L:665  |      ^ | nextPos = rayDir +* totalDist.xxxxXXXX;
  vmadh $v18, $v12, $v16.h0                          ## L:665  |     17 | nextPos = rayDir +* totalDist.xxxxXXXX;
  slt $t4, $a0, $t0                                  ## L:658  |      ^ | tmp = totalDistA > RENDER_DIST;
  slt $t7, $t2, $a1                                  ## L:655  |     18 | u8 isDoneA = distA < STOP_DIST;
  vmadl $v21, $v05, $v07.e2                          ## L:668  |      ^ | nextPosFloor:sint = ONE +* SPHERE_RAD:sfract.z;
  addu $t1, $t1, $t3                                 ## L:653  |     19 | totalDistB += distB;
  vmadm $v20, $v04, $v07.e2                          ## L:668  |      ^ | nextPosFloor:sint = ONE +* SPHERE_RAD:sfract.z;
  or $t7, $t7, $t4                                   ## L:659  |     20 | isDoneA |= tmp;
  slt $t8, $t3, $a1                                  ## L:656  |     21 | u8 isDoneB = distB < STOP_DIST;
  slt $t4, $a0, $t1                                  ## L:660  |     22 | tmp = totalDistB > RENDER_DIST;
  vmadn $v21, $v00, $v00                             ## L:668  |      ^ | nextPosFloor:sint = ONE +* SPHERE_RAD:sfract.z;
  vsub $v18, $v18, $v20.v                            ## L:670  |     23 | nextPos:sint -= nextPosFloor:sint;
  beq $t7, $t5, Cylinder_markDoneA                   ## L:673  |      ^ | if(isDoneA == isDoneAFlag)goto Cylinder_markDoneA;
  or $t8, $t8, $t4                                   ## L:661  |    *25 | isDoneB |= tmp;
  __Cylinder_RET_MARK_DONE_A:
  vmudl $v29, $v19, $v19.v                           ## L:676  |     26 | posSq:sfract = nextPos * nextPos;
  vmadm $v29, $v18, $v19.v                           ## L:676  |     27 | posSq:sfract = nextPos * nextPos;
  beq $t8, $t6, Cylinder_markDoneB                   ## L:678  |      ^ | if(isDoneB == isDoneBFlag)goto Cylinder_markDoneB;
  vmadn $v02, $v19, $v18.v                           ## L:676  |    *29 | posSq:sfract = nextPos * nextPos;
  __Cylinder_RET_MARK_DONE_B:
  vrsqh $v20.e0, $v00.e0                             ## L:96   |     30 | asm_op("vrsqh", out:sint.x, VZERO.x);
  vaddc $v15, $v02, $v02.h2                          ## L:773  |   **33 | res = posSq:sfract + posSq:sfract.zzzzZZZZ;
  vrsql $v21.e0, $v15.e0                             ## L:97   |  ***37 | asm_op("vrsql", out:sfract.x, in:sfract.x);
  vrsqh $v20.e0, $v00.e4                             ## L:104  |     38 | asm_op("vrsqh", out:sint.X, VZERO.X);
  vrsql $v23.e4, $v15.e4                             ## L:105  |     39 | asm_op("vrsql", out:sfract.X, in:sfract.X);
  vrsqh $v22.e4, $v00.e0                             ## L:106  |     40 | asm_op("vrsqh", out:sint.X, VZERO.x);
  vadd $v14, $v00, $v00.h2                           ## L:773  |     41 | res = posSq:sfract + posSq:sfract.zzzzZZZZ;
  vmudm $v20, $v20, $v31.e7                          ## L:112  |     42 | asm_op("vmudm", out:sint,   in:sint,   VSHIFT8.W);
  vmadl $v21, $v21, $v31.e7                          ## L:113  |     43 | asm_op("vmadl", out:sfract, in:sfract, VSHIFT8.W);
  vmudm $v22, $v22, $v31.e7                          ## L:112  |     44 | asm_op("vmudm", out:sint,   in:sint,   VSHIFT8.W);
  lsv $v09, 2, 78, $zero                             ## L:683  |      ^ | LERP_FACTOR:ufract.y = load(ZERO, 78).x;
  vmadl $v23, $v23, $v31.e7                          ## L:113  |     45 | asm_op("vmadl", out:sfract, in:sfract, VSHIFT8.W);
  lsv $v09, 0, 76, $zero                             ## L:682  |      ^ | LERP_FACTOR:ufract.x = load(ZERO, 76).x;
  vrcph $v14.e0, $v20.e0                             ## L:781  |     46 | res.x = invert_half(tmpA).x;
  lsv $v09, 8, 76, $zero                             ## L:684  |      ^ | LERP_FACTOR:ufract.X = load(ZERO, 76).x;
  vrcpl $v15.e0, $v21.e0                             ## L:781  |     47 | res.x = invert_half(tmpA).x;
  lsv $v09, 10, 78, $zero                            ## L:685  |      ^ | LERP_FACTOR:ufract.Y = load(ZERO, 78).x;
  vrcph $v14.e0, $v22.e4                             ## L:782  |     48 | res.X = invert_half(tmpB).X;
  vrcpl $v15.e4, $v23.e4                             ## L:782  |     49 | res.X = invert_half(tmpB).X;
  vrcph $v14.e4, $v00.e4                             ## L:782  |     50 | res.X = invert_half(tmpB).X;
  vsubc $v15, $v15, $v09.h1                          ## L:784  |   **53 | res -= LERP_FACTOR.yyyyYYYY;
  j LABEL_RayMarch_Cylinder_000D                     ## L:784  |      ^ | res -= LERP_FACTOR.yyyyYYYY;
  vsub $v14, $v14, $v08.h1                           ## L:784  |    *55 | res -= LERP_FACTOR.yyyyYYYY;
  Cylinder_markDoneA:
  addiu $t5, $zero, 255                              ## L:816  |     56 | isDoneAFlag = 0xFF;
  bne $t5, $t6, __Cylinder_RET_MARK_DONE_A           ## L:817  |     57 | if(isDoneAFlag != isDoneBFlag)goto __Cylinder_RET_MARK_DONE_A;
  sw $t0, 40($zero)                                  ## L:815  |    *59 | store(totalDistA, ZERO, 40);
  break # inline-ASM                                 ## L:818  |      0 | asm("break");
  Cylinder_markDoneB:
  addiu $t6, $zero, 255                              ## L:824  |     60 | isDoneBFlag = 0xFF;
  bne $t5, $t6, __Cylinder_RET_MARK_DONE_B           ## L:825  |     61 | if(isDoneAFlag != isDoneBFlag)goto __Cylinder_RET_MARK_DONE_B;
  sw $t1, 72($zero)                                  ## L:823  |    *63 | store(totalDistB, ZERO, 72);
  break # inline-ASM                                 ## L:826  |      0 | asm("break");
RayMarch_Octa:
  vxor $v01, $v00, $v00.e0                           ## L:868  |      ^ | posSq:sint = 0;
  ldv $v12, 0, 16, $zero                             ## L:882  |      ^ | rayDir:sint.xyzw = load(ZERO, 16).xyzw;
  or $t0, $zero, $zero                               ## L:862  |      2 | s32 totalDistA = 0;
  ldv $v12, 8, 48, $zero                             ## L:883  |      3 | rayDir:sint.XYZW = load(ZERO, 48).xyzw;
  vxor $v14, $v00, $v00.e0                           ## L:871  |      ^ | res:sint = 0;
  lsv $v15, 0, 82, $zero                             ## L:872  |      4 | res:sfract.x = load(ZERO, 80).y;
  addiu $t5, $zero, 1                                ## L:858  |      5 | u8<$t5> isDoneAFlag = 1;
  vxor $v17, $v00, $v00                              ## L:875  |      ^ | vec32 totalDist = 0;
  vxor $v16, $v00, $v00.e0                           ## L:875  |      6 | vec32 totalDist = 0;
  lsv $v15, 8, 82, $zero                             ## L:873  |      ^ | res:sfract.X = load(ZERO, 80).y;
  or $t1, $zero, $zero                               ## L:863  |      7 | s32 totalDistB = 0;
  vmudl $v13, $v13, $v30.e6                          ## L:884  |      ^ | rayDir >>= 15;
  vmadm $v12, $v12, $v30.e6                          ## L:884  |      8 | rayDir >>= 15;
  addiu $t6, $zero, 1                                ## L:859  |      ^ | u8<$t6> isDoneBFlag = 1;
  vmadn $v13, $v00, $v00                             ## L:884  |      9 | rayDir >>= 15;
  LABEL_RayMarch_Octa_0013:
  ssv $v14, 0, 36, $zero                             ## L:890  |      ^ | @Barrier("lenA") store(res.x, ZERO, 36); ## Barrier: 0x1
  ssv $v14, 8, 68, $zero                             ## L:891  |     10 | @Barrier("lenB") store(res.X, ZERO, 68); ## Barrier: 0x2
  vaddc $v17, $v17, $v15.v                           ## L:888  |      ^ | totalDist += res;
  vadd $v16, $v16, $v14.v                            ## L:888  |     11 | totalDist += res;
  ssv $v15, 8, 70, $zero                             ## L:891  |      ^ | @Barrier("lenB") store(res.X, ZERO, 68); ## Barrier: 0x2
  vmudl $v29, $v05, $v11.v                           ## L:908  |     12 | nextPos = ONE * rayPosOrg;
  ssv $v15, 0, 38, $zero                             ## L:890  |      ^ | @Barrier("lenA") store(res.x, ZERO, 36); ## Barrier: 0x1
  lw $t3, 68($zero)                                  ## L:894  |     13 | @Barrier("lenB") distB = load(ZERO, 68); ## Barrier: 0x2
  lw $t2, 36($zero)                                  ## L:893  |     14 | @Barrier("lenA") distA = load(ZERO, 36); ## Barrier: 0x1
  vmadm $v29, $v04, $v11.v                           ## L:908  |      ^ | nextPos = ONE * rayPosOrg;
  lsv $v09, 2, 78, $zero                             ## L:920  |     15 | LERP_FACTOR:ufract.y = load(ZERO, 78).x;
  vmadn $v19, $v05, $v10.v                           ## L:908  |      ^ | nextPos = ONE * rayPosOrg;
  vmadh $v18, $v04, $v10.v                           ## L:908  |     16 | nextPos = ONE * rayPosOrg;
  slt $t8, $t3, $a1                                  ## L:900  |      ^ | u8 isDoneB = distB < STOP_DIST;
  addu $t0, $t0, $t2                                 ## L:896  |     17 | totalDistA += distA;
  slt $t7, $t2, $a1                                  ## L:899  |     18 | u8 isDoneA = distA < STOP_DIST;
  slt $t4, $a0, $t0                                  ## L:902  |     19 | tmp = totalDistA > RENDER_DIST;
  vmadl $v29, $v13, $v17.h0                          ## L:909  |      ^ | nextPos = rayDir +* totalDist.xxxxXXXX;
  vmadm $v29, $v12, $v17.h0                          ## L:909  |     20 | nextPos = rayDir +* totalDist.xxxxXXXX;
  or $t7, $t7, $t4                                   ## L:903  |      ^ | isDoneA |= tmp;
  vmadn $v19, $v13, $v16.h0                          ## L:909  |     21 | nextPos = rayDir +* totalDist.xxxxXXXX;
  addu $t1, $t1, $t3                                 ## L:897  |      ^ | totalDistB += distB;
  slt $t4, $a0, $t1                                  ## L:904  |     22 | tmp = totalDistB > RENDER_DIST;
  vmadh $v18, $v12, $v16.h0                          ## L:909  |      ^ | nextPos = rayDir +* totalDist.xxxxXXXX;
  beq $t7, $t5, Octa_markDoneA                       ## L:923  |     23 | if(isDoneA == isDoneAFlag)goto Octa_markDoneA;
  or $t8, $t8, $t4                                   ## L:905  |    *25 | isDoneB |= tmp;
  __Octa_RET_MARK_DONE_A:
  vxor $v01, $v00, $v00.e0                           ## L:927  |     26 | posSq:sint = 0;
  beq $t8, $t6, Octa_markDoneB                       ## L:933  |      ^ | if(isDoneB == isDoneBFlag)goto Octa_markDoneB;
  vabs $v02, $v19, $v19                              ## L:928  |    *28 | posSq:sfract = abs(nextPos:sfract);
  __Octa_RET_MARK_DONE_B:
  vsubc $v21, $v02, $v09.e1                          ## L:1047 |  ***32 | tmpA = posSq - LERP_FACTOR.y;
  vsub $v20, $v01, $v08.e1                           ## L:1047 |     33 | tmpA = posSq - LERP_FACTOR.y;
  vaddc $v23, $v02, $v02.h2                          ## L:1048 |     34 | tmpB = posSq + posSq:sfract.zzzzZZZZ;
  vadd $v22, $v01, $v00.h2                           ## L:1048 |     35 | tmpB = posSq + posSq:sfract.zzzzZZZZ;
  vaddc $v23, $v23, $v21.h1                          ## L:1049 |   **38 | tmpB = tmpB + tmpA.yyyyYYYY;
  vadd $v22, $v22, $v20.h1                           ## L:1049 |     39 | tmpB = tmpB + tmpA.yyyyYYYY;
  vmudl $v29, $v23, $v07.e3                          ## L:1051 |   **42 | res = tmpB * SPHERE_RAD.w;
  vmadm $v29, $v22, $v07.e3                          ## L:1051 |     43 | res = tmpB * SPHERE_RAD.w;
  vmadn $v15, $v23, $v06.e3                          ## L:1051 |     44 | res = tmpB * SPHERE_RAD.w;
  j LABEL_RayMarch_Octa_0013                         ## L:1051 |      ^ | res = tmpB * SPHERE_RAD.w;
  vmadh $v14, $v22, $v06.e3                          ## L:1051 |    *46 | res = tmpB * SPHERE_RAD.w;
  Octa_markDoneA:
  addiu $t5, $zero, 255                              ## L:1060 |     47 | isDoneAFlag = 0xFF;
  bne $t5, $t6, __Octa_RET_MARK_DONE_A               ## L:1061 |     48 | if(isDoneAFlag != isDoneBFlag)goto __Octa_RET_MARK_DONE_A;
  sw $t0, 40($zero)                                  ## L:1059 |    *50 | store(totalDistA, ZERO, 40);
  break # inline-ASM                                 ## L:1062 |      0 | asm("break");
  Octa_markDoneB:
  addiu $t6, $zero, 255                              ## L:1068 |     51 | isDoneBFlag = 0xFF;
  bne $t5, $t6, __Octa_RET_MARK_DONE_B               ## L:1069 |     52 | if(isDoneAFlag != isDoneBFlag)goto __Octa_RET_MARK_DONE_B;
  sw $t1, 72($zero)                                  ## L:1067 |    *54 | store(totalDistB, ZERO, 72);
  break # inline-ASM                                 ## L:1070 |      0 | asm("break");

OVERLAY_CODE_END:

#define zero $0
#define v0 $2
#define v1 $3
#define a0 $4
#define a1 $5
#define a2 $6
#define a3 $7
#define t0 $8
#define t1 $9
#define t2 $10
#define t3 $11
#define t4 $12
#define t5 $13
#define t6 $14
#define t7 $15
#define s0 $16
#define s1 $17
#define s2 $18
#define s3 $19
#define s4 $20
#define s5 $21
#define s6 $22
#define s7 $23
#define t8 $24
#define t9 $25
#define k0 $26
#define k1 $27
#define gp $28
#define sp $29
#define fp $30
#define ra $31

.set at
.set macro