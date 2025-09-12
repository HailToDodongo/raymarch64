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
  vxor $v00, $v00, $v00 # inline-ASM                 ## L:76   |      ^ | asm("vxor $v00, $v00, $v00");
  ori $t0, $zero, 0x8000                             ## L:77   |      1 | u16 tmp = 0x8000;
  mtc2 $t0, $v31.e0                                  ## L:78   |      2 | VSHIFT8.x = tmp; tmp >>= 1;
  srl $t0, $t0, 1                                    ## L:78   |      3 | VSHIFT8.x = tmp; tmp >>= 1;
  mtc2 $t0, $v31.e1                                  ## L:79   |     *5 | VSHIFT8.y = tmp; tmp >>= 1;
  srl $t0, $t0, 1                                    ## L:79   |      6 | VSHIFT8.y = tmp; tmp >>= 1;
  mtc2 $t0, $v31.e2                                  ## L:80   |     *8 | VSHIFT8.z = tmp; tmp >>= 1;
  srl $t0, $t0, 1                                    ## L:80   |      9 | VSHIFT8.z = tmp; tmp >>= 1;
  mtc2 $t0, $v31.e3                                  ## L:81   |    *11 | VSHIFT8.w = tmp; tmp >>= 1;
  srl $t0, $t0, 1                                    ## L:81   |     12 | VSHIFT8.w = tmp; tmp >>= 1;
  mtc2 $t0, $v31.e4                                  ## L:82   |    *14 | VSHIFT8.X = tmp; tmp >>= 1;
  srl $t0, $t0, 1                                    ## L:82   |     15 | VSHIFT8.X = tmp; tmp >>= 1;
  mtc2 $t0, $v31.e5                                  ## L:83   |    *17 | VSHIFT8.Y = tmp; tmp >>= 1;
  srl $t0, $t0, 1                                    ## L:83   |     18 | VSHIFT8.Y = tmp; tmp >>= 1;
  mtc2 $t0, $v31.e6                                  ## L:84   |    *20 | VSHIFT8.Z = tmp; tmp >>= 1;
  srl $t0, $t0, 1                                    ## L:84   |     21 | VSHIFT8.Z = tmp; tmp >>= 1;
  mtc2 $t0, $v31.e7                                  ## L:85   |    *23 | VSHIFT8.W = tmp;
  vmudl $v30, $v31, $v31.e7                          ## L:87   |  ***27 | VSHIFT = VSHIFT8 >>> 8;
  SHIFT_END:
  mtc2 $zero, $v06.e0                                ## L:94   |      ^ | SPHERE_RAD.x = 0.25;
  addiu $at, $zero, 16384                            ## L:94   |     28 | SPHERE_RAD.x = 0.25;
  lsv $v11, 0, 2, $zero                              ## L:107  |     29 | rayPosOrg.x = load(ZERO, 0).x;
  vxor $v05, $v00, $v00                              ## L:91   |      ^ | vec32<$v04> ONE = 1;
  mtc2 $at, $v07.e0                                  ## L:94   |     30 | SPHERE_RAD.x = 0.25;
  lui $a0, 0x0B                                      ## L:117  |     31 | s32<$a0> RENDER_DIST = 11.0 << 16;
  addiu $at, $zero, 4915                             ## L:95   |     32 | SPHERE_RAD.y = 0.075;
  mtc2 $at, $v07.e1                                  ## L:95   |     33 | SPHERE_RAD.y = 0.075;
  ori $at, $zero, 0x8000                             ## L:96   |     34 | SPHERE_RAD.z = 0.5;
  lsv $v10, 2, 4, $zero                              ## L:108  |     35 | rayPosOrg.y = load(ZERO, 4).x;
  mtc2 $at, $v07.e2                                  ## L:96   |     36 | SPHERE_RAD.z = 0.5;
  addiu $at, $zero, 2                                ## L:98   |     37 | SPHERE_RAD.X = 2.5;
  mtc2 $at, $v06.e4                                  ## L:98   |    *39 | SPHERE_RAD.X = 2.5;
  ori $at, $zero, 0x8000                             ## L:98   |     40 | SPHERE_RAD.X = 2.5;
  lsv $v10, 0, 0, $zero                              ## L:107  |     41 | rayPosOrg.x = load(ZERO, 0).x;
  mtc2 $at, $v07.e4                                  ## L:98   |     42 | SPHERE_RAD.X = 2.5;
  addiu $at, $zero, 13107                            ## L:99   |     43 | SPHERE_RAD.Y = 0.2;
  lsv $v10, 4, 8, $zero                              ## L:109  |     44 | rayPosOrg.z = load(ZERO, 8).x;
  mtc2 $at, $v07.e5                                  ## L:99   |     45 | SPHERE_RAD.Y = 0.2;
  ori $at, $zero, 0xFFFF                             ## L:101  |     46 | SPHERE_RAD.Z = -0.25;
  lsv $v11, 2, 6, $zero                              ## L:108  |     47 | rayPosOrg.y = load(ZERO, 4).x;
  mtc2 $at, $v06.e6                                  ## L:101  |     48 | SPHERE_RAD.Z = -0.25;
  lsv $v11, 4, 10, $zero                             ## L:109  |     49 | rayPosOrg.z = load(ZERO, 8).x;
  ori $at, $zero, 0xC000                             ## L:101  |     50 | SPHERE_RAD.Z = -0.25;
  mtc2 $at, $v07.e6                                  ## L:101  |    *52 | SPHERE_RAD.Z = -0.25;
  vmov $v10.e5, $v10.e1                              ## L:112  |      ^ | rayPosOrg.Y = rayPosOrg.y;
  vmov $v11.e5, $v11.e1                              ## L:112  |     53 | rayPosOrg.Y = rayPosOrg.y;
  mtc2 $zero, $v06.e2                                ## L:96   |      ^ | SPHERE_RAD.z = 0.5;
  ori $at, $zero, 0xFFFF                             ## L:102  |     54 | SPHERE_RAD.W = -0.075;
  mtc2 $at, $v06.e7                                  ## L:102  |    *56 | SPHERE_RAD.W = -0.075;
  vmov $v10.e4, $v10.e0                              ## L:111  |      ^ | rayPosOrg.X = rayPosOrg.x;
  mtc2 $zero, $v06.e5                                ## L:99   |     57 | SPHERE_RAD.Y = 0.2;
  vmov $v11.e4, $v11.e0                              ## L:111  |      ^ | rayPosOrg.X = rayPosOrg.x;
  addiu $a1, $zero, 1048                             ## L:118  |     58 | s32<$a1> STOP_DIST   = 0.016 * 0xFFFF;
  ori $at, $zero, 0xECCD                             ## L:102  |     59 | SPHERE_RAD.W = -0.075;
  vxor $v04, $v00, $v30.e7                           ## L:91   |      ^ | vec32<$v04> ONE = 1;
  mtc2 $at, $v07.e7                                  ## L:102  |     60 | SPHERE_RAD.W = -0.075;
  vmov $v10.e6, $v10.e2                              ## L:113  |      ^ | rayPosOrg.Z = rayPosOrg.z;
  vmov $v11.e6, $v11.e2                              ## L:113  |     61 | rayPosOrg.Z = rayPosOrg.z;
  mtc2 $zero, $v06.e1                                ## L:95   |      ^ | SPHERE_RAD.y = 0.075;
RayMarch:
  lsv $v13, 2, 18, $zero                             ## L:160  |      ^ | rayDir.y = load(ZERO, 16).x;
  lsv $v09, 0, 76, $zero                             ## L:184  |      2 | LERP_FACTOR:ufract.x = load(ZERO, 76).x;
  lsv $v13, 8, 46, $zero                             ## L:163  |      3 | rayDir.X = load(ZERO, 44).x;
  lsv $v12, 8, 44, $zero                             ## L:163  |      4 | rayDir.X = load(ZERO, 44).x;
  lsv $v16, 0, 80, $zero                             ## L:181  |      5 | res.x = load(ZERO, 80).x;
  lsv $v09, 2, 78, $zero                             ## L:185  |      6 | LERP_FACTOR:ufract.y = load(ZERO, 78).x;
  lsv $v12, 10, 48, $zero                            ## L:164  |      7 | rayDir.Y = load(ZERO, 48).x;
  lsv $v12, 4, 20, $zero                             ## L:161  |      8 | rayDir.z = load(ZERO, 20).x;
  lsv $v17, 0, 82, $zero                             ## L:181  |      9 | res.x = load(ZERO, 80).x;
  lsv $v13, 4, 22, $zero                             ## L:161  |     10 | rayDir.z = load(ZERO, 20).x;
  vmov $v09.e5, $v09.e1                              ## L:187  |      ^ | LERP_FACTOR:ufract.Y = LERP_FACTOR:ufract.y;
  lsv $v13, 10, 50, $zero                            ## L:164  |     11 | rayDir.Y = load(ZERO, 48).x;
  vxor $v14, $v00, $v00                              ## L:178  |      ^ | posSq:sint = 0;
  addiu $t6, $zero, 1                                ## L:169  |     12 | u8<$t6> isDoneBFlag = 1;
  vor $v16, $v00, $v16.e0                            ## L:182  |      ^ | res = res.x;
  lsv $v12, 0, 12, $zero                             ## L:159  |     13 | rayDir.x = load(ZERO, 12).x;
  vxor $v18, $v00, $v00                              ## L:189  |      ^ | vec32 totalDist = 0;
  vor $v17, $v00, $v17.e0                            ## L:182  |     14 | res = res.x;
  lsv $v13, 0, 14, $zero                             ## L:159  |      ^ | rayDir.x = load(ZERO, 12).x;
  lsv $v13, 12, 54, $zero                            ## L:165  |     15 | rayDir.Z = load(ZERO, 52).x;
  vmov $v09.e4, $v09.e0                              ## L:186  |      ^ | LERP_FACTOR:ufract.X = LERP_FACTOR:ufract.x;
  vxor $v19, $v00, $v00                              ## L:189  |     16 | vec32 totalDist = 0;
  addiu $t5, $zero, 1                                ## L:168  |      ^ | u8<$t5> isDoneAFlag = 1;
  lsv $v12, 2, 16, $zero                             ## L:160  |     17 | rayDir.y = load(ZERO, 16).x;
  lsv $v12, 12, 52, $zero                            ## L:165  |     18 | rayDir.Z = load(ZERO, 52).x;
  or $t1, $zero, $zero                               ## L:173  |     19 | s32 totalDistB = 0;
  or $t0, $zero, $zero                               ## L:172  |     20 | s32 totalDistA = 0;
  LABEL_RayMarch_0001:
  vaddc $v19, $v19, $v17.v                           ## L:196  |      ^ | totalDist += res;
  ssv $v16, 0, 36, $zero                             ## L:198  |     21 | @Barrier("lenA") store(res.x, ZERO, 36); ## Barrier: 0x1
  vadd $v18, $v18, $v16.v                            ## L:196  |      ^ | totalDist += res;
  vmudl $v29, $v05, $v11.v                           ## L:216  |     22 | nextPos = ONE * rayPosOrg;
  ssv $v16, 8, 68, $zero                             ## L:199  |      ^ | @Barrier("lenB") store(res.X, ZERO, 68); ## Barrier: 0x2
  ssv $v17, 0, 38, $zero                             ## L:198  |     23 | @Barrier("lenA") store(res.x, ZERO, 36); ## Barrier: 0x1
  vmadm $v29, $v04, $v11.v                           ## L:216  |      ^ | nextPos = ONE * rayPosOrg;
  lw $t2, 36($zero)                                  ## L:201  |     24 | @Barrier("lenA") distA = load(ZERO, 36); ## Barrier: 0x1
  vmadn $v21, $v05, $v10.v                           ## L:216  |      ^ | nextPos = ONE * rayPosOrg;
  ssv $v17, 8, 70, $zero                             ## L:199  |     25 | @Barrier("lenB") store(res.X, ZERO, 68); ## Barrier: 0x2
  vmadh $v20, $v04, $v10.v                           ## L:216  |      ^ | nextPos = ONE * rayPosOrg;
  vmadl $v29, $v13, $v19.h0                          ## L:217  |     26 | nextPos = rayDir +* totalDist.xxxxXXXX;
  lw $t3, 68($zero)                                  ## L:202  |      ^ | @Barrier("lenB") distB = load(ZERO, 68); ## Barrier: 0x2
  addu $t0, $t0, $t2                                 ## L:204  |     27 | totalDistA += distA;
  vmadm $v29, $v12, $v19.h0                          ## L:217  |      ^ | nextPos = rayDir +* totalDist.xxxxXXXX;
  slt $t4, $t2, $a1                                  ## L:207  |     28 | u8 isDoneA = distA < STOP_DIST;
  vmadn $v21, $v13, $v18.h0                          ## L:217  |      ^ | nextPos = rayDir +* totalDist.xxxxXXXX;
  vmadh $v20, $v12, $v18.h0                          ## L:217  |     29 | nextPos = rayDir +* totalDist.xxxxXXXX;
  slt $t8, $a0, $t0                                  ## L:210  |      ^ | u8 tmp = totalDistA > RENDER_DIST;
  vmadl $v23, $v05, $v07.e2                          ## L:220  |     30 | nextPosFloor:sint = ONE +* SPHERE_RAD:sfract.z;
  addu $t1, $t1, $t3                                 ## L:205  |      ^ | totalDistB += distB;
  or $t4, $t4, $t8                                   ## L:211  |     31 | isDoneA |= tmp;
  vmadm $v22, $v04, $v07.e2                          ## L:220  |      ^ | nextPosFloor:sint = ONE +* SPHERE_RAD:sfract.z;
  slt $t8, $a0, $t1                                  ## L:212  |     32 | tmp = totalDistB > RENDER_DIST;
  sw $t0, 40($zero)                                  ## L:225  |     33 | store(totalDistA, ZERO, 40);
  slt $t7, $t3, $a1                                  ## L:208  |     34 | u8 isDoneB = distB < STOP_DIST;
  vmadn $v23, $v00, $v00                             ## L:220  |      ^ | nextPosFloor:sint = ONE +* SPHERE_RAD:sfract.z;
  vsub $v20, $v20, $v22.v                            ## L:222  |     35 | nextPos:sint -= nextPosFloor:sint;
  beq $t4, $t5, markDoneA                            ## L:227  |      ^ | if(isDoneA == isDoneAFlag)goto markDoneA;
  or $t7, $t7, $t8                                   ## L:213  |    *37 | isDoneB |= tmp;
  __RET_MARK_DONE_A:
  vmudl $v29, $v21, $v21.v                           ## L:230  |     38 | posSq:sfract = nextPos * nextPos;
  sw $t1, 72($zero)                                  ## L:231  |      ^ | store(totalDistB, ZERO, 72);
  vmadm $v29, $v20, $v21.v                           ## L:230  |     39 | posSq:sfract = nextPos * nextPos;
  beq $t7, $t6, markDoneB                            ## L:233  |      ^ | if(isDoneB == isDoneBFlag)goto markDoneB;
  vmadn $v15, $v21, $v20.v                           ## L:230  |    *41 | posSq:sfract = nextPos * nextPos;
  __RET_MARK_DONE_B:
  vrsqh $v24.e0, $v00.e0                             ## L:123  |     42 | asm_op("vrsqh", out:sint.x, VZERO.x);
  vaddc $v17, $v15, $v15.h2                          ## L:236  |   **45 | res = posSq:sfract + posSq:sfract.zzzzZZZZ;
  ssv $v20, 2, 28, $zero                             ## L:239  |      ^ | @Barrier("p.y") store(nextPos.y, ZERO, 28); ## Barrier: 0x8
  ssv $v21, 2, 30, $zero                             ## L:239  |     46 | @Barrier("p.y") store(nextPos.y, ZERO, 28); ## Barrier: 0x8
  ssv $v20, 4, 32, $zero                             ## L:240  |     47 | @Barrier("p.z") store(nextPos.z, ZERO, 32); ## Barrier: 0x10
  vadd $v16, $v00, $v00.h2                           ## L:236  |      ^ | res = posSq:sfract + posSq:sfract.zzzzZZZZ;
  ssv $v20, 0, 24, $zero                             ## L:238  |     48 | @Barrier("p.x") store(nextPos.x, ZERO, 24); ## Barrier: 0x4
  ssv $v20, 12, 64, $zero                            ## L:244  |     49 | @Barrier("p.Z") store(nextPos.Z, ZERO, 64); ## Barrier: 0x80
  vrsql $v25.e0, $v17.e0                             ## L:124  |      ^ | asm_op("vrsql", out:sfract.x, in:sfract.x);
  ssv $v20, 8, 56, $zero                             ## L:242  |     50 | @Barrier("p.X") store(nextPos.X, ZERO, 56); ## Barrier: 0x20
  vrsqh $v24.e0, $v00.e4                             ## L:131  |      ^ | asm_op("vrsqh", out:sint.X, VZERO.X);
  vrsql $v25.e4, $v17.e4                             ## L:132  |     51 | asm_op("vrsql", out:sfract.X, in:sfract.X);
  vrsqh $v24.e4, $v00.e0                             ## L:133  |     52 | asm_op("vrsqh", out:sint.X, VZERO.x);
  ssv $v21, 0, 26, $zero                             ## L:238  |      ^ | @Barrier("p.x") store(nextPos.x, ZERO, 24); ## Barrier: 0x4
  ssv $v21, 8, 58, $zero                             ## L:242  |     53 | @Barrier("p.X") store(nextPos.X, ZERO, 56); ## Barrier: 0x20
  ssv $v21, 4, 34, $zero                             ## L:240  |     54 | @Barrier("p.z") store(nextPos.z, ZERO, 32); ## Barrier: 0x10
  vaddc $v23, $v17, $v15.h1                          ## L:256  |      ^ | tmpA = res:sfract + posSq:sfract.yyyyYYYY;
  vadd $v22, $v00, $v00.h1                           ## L:256  |     55 | tmpA = res:sfract + posSq:sfract.yyyyYYYY;
  vmudm $v16, $v24, $v31.e7                          ## L:139  |     56 | asm_op("vmudm", out:sint,   in:sint,   VSHIFT8.W);
  vmadl $v17, $v25, $v31.e7                          ## L:140  |     57 | asm_op("vmadl", out:sfract, in:sfract, VSHIFT8.W);
  vrsqh $v26.e0, $v00.e0                             ## L:123  |     58 | asm_op("vrsqh", out:sint.x, VZERO.x);
  vrsql $v27.e0, $v23.e0                             ## L:124  |     59 | asm_op("vrsql", out:sfract.x, in:sfract.x);
  vrsqh $v26.e0, $v00.e0                             ## L:125  |     60 | asm_op("vrsqh", out:sint.x, VZERO.x);
  vrcph $v29.e0, $v16.e0                             ## L:263  |     61 | tmpB:sfract.x = invert_half(res).x;
  vrcpl $v25.e0, $v17.e0                             ## L:263  |     62 | tmpB:sfract.x = invert_half(res).x;
  vrcph $v29.e4, $v16.e4                             ## L:264  |     63 | tmpB:sfract.X = invert_half(res).X;
  vrcpl $v25.e4, $v17.e4                             ## L:264  |     64 | tmpB:sfract.X = invert_half(res).X;
  mfc2 $at, $v26.e0                                  ## L:261  |      ^ | s32 resSphereA = resSphere.x;
  vrsqh $v26.e4, $v00.e4                             ## L:131  |     65 | asm_op("vrsqh", out:sint.X, VZERO.X);
  mfc2 $t8, $v27.e0                                  ## L:261  |      ^ | s32 resSphereA = resSphere.x;
  vrsql $v27.e4, $v23.e4                             ## L:132  |     66 | asm_op("vrsql", out:sfract.X, in:sfract.X);
  vrsqh $v26.e4, $v00.e0                             ## L:133  |     67 | asm_op("vrsqh", out:sint.X, VZERO.x);
  sll $at, $at, 16                                   ## L:261  |      ^ | s32 resSphereA = resSphere.x;
  andi $t8, $t8, 65535                               ## L:261  |     68 | s32 resSphereA = resSphere.x;
  vsubc $v17, $v25, $v07.e0                          ## L:274  |      ^ | res:ufract = tmpB:ufract - SPHERE_RAD:ufract.x;
  vsub $v16, $v00, $v06.e0                           ## L:275  |     69 | res:sint = VZERO - SPHERE_RAD:sint.x;
  or $t8, $t8, $at                                   ## L:261  |      ^ | s32 resSphereA = resSphere.x;
  mfc2 $t9, $v27.e4                                  ## L:267  |     70 | s32 resSphereB = resSphere.X;
  mfc2 $at, $v26.e4                                  ## L:267  |     71 | s32 resSphereB = resSphere.X;
  sra $t8, $t8, 8                                    ## L:269  |     72 | resSphereA >>= 8;
  vmudl $v29, $v17, $v17.v                           ## L:277  |      ^ | res:sfract = res * res;
  vmadm $v29, $v16, $v17.v                           ## L:277  |     73 | res:sfract = res * res;
  andi $t9, $t9, 65535                               ## L:267  |      ^ | s32 resSphereB = resSphere.X;
  sll $at, $at, 16                                   ## L:267  |     74 | s32 resSphereB = resSphere.X;
  vmadn $v17, $v17, $v16.v                           ## L:277  |      ^ | res:sfract = res * res;
  or $t9, $t9, $at                                   ## L:267  |     75 | s32 resSphereB = resSphere.X;
  vmadl $v29, $v05, $v15.h1                          ## L:278  |      ^ | tmpA = ONE +* posSq.yyyyYYYY;
  vmadm $v29, $v04, $v15.h1                          ## L:278  |     76 | tmpA = ONE +* posSq.yyyyYYYY;
  sra $t9, $t9, 8                                    ## L:270  |      ^ | resSphereB >>= 8;
  vmadn $v23, $v05, $v14.h1                          ## L:278  |     77 | tmpA = ONE +* posSq.yyyyYYYY;
  sw $t8, 120($zero)                                 ## L:271  |      ^ | @Barrier("a") store(resSphereA, ZERO, 120); ## Barrier: 0x100
  vmadh $v22, $v04, $v14.h1                          ## L:278  |     78 | tmpA = ONE +* posSq.yyyyYYYY;
  lsv $v24, 0, 120, $zero                            ## L:280  |      ^ | @Barrier("a") tmpB.x = load(ZERO, 120).x; ## Barrier: 0x100
  sw $t9, 124($zero)                                 ## L:272  |     79 | @Barrier("b") store(resSphereB, ZERO, 124); ## Barrier: 0x200
  lsv $v25, 0, 122, $zero                            ## L:280  |     80 | @Barrier("a") tmpB.x = load(ZERO, 120).x; ## Barrier: 0x100
  vrsqh $v16.e0, $v00.e0                             ## L:123  |      ^ | asm_op("vrsqh", out:sint.x, VZERO.x);
  vrsql $v17.e0, $v23.e0                             ## L:124  |     81 | asm_op("vrsql", out:sfract.x, in:sfract.x);
  lsv $v24, 8, 124, $zero                            ## L:281  |      ^ | @Barrier("b") tmpB.X = load(ZERO, 124).x; ## Barrier: 0x200
  vrsqh $v16.e0, $v00.e4                             ## L:131  |     82 | asm_op("vrsqh", out:sint.X, VZERO.X);
  lsv $v25, 8, 126, $zero                            ## L:281  |      ^ | @Barrier("b") tmpB.X = load(ZERO, 124).x; ## Barrier: 0x200
  vrsql $v17.e4, $v23.e4                             ## L:132  |     83 | asm_op("vrsql", out:sfract.X, in:sfract.X);
  vrsqh $v16.e4, $v00.e0                             ## L:133  |     84 | asm_op("vrsqh", out:sint.X, VZERO.x);
  vrcph $v29.e0, $v24.e0                             ## L:289  |     85 | resSphere:sfract.x = invert_half(tmpB).x;
  vrcpl $v27.e0, $v25.e0                             ## L:289  |     86 | resSphere:sfract.x = invert_half(tmpB).x;
  vmudm $v22, $v16, $v31.e7                          ## L:139  |    *88 | asm_op("vmudm", out:sint,   in:sint,   VSHIFT8.W);
  vmadl $v23, $v17, $v31.e7                          ## L:140  |     89 | asm_op("vmadl", out:sfract, in:sfract, VSHIFT8.W);
  vrcph $v29.e4, $v24.e4                             ## L:294  |     90 | resSphere:sfract.X = invert_half(tmpB).X;
  vrcpl $v27.e4, $v25.e4                             ## L:294  |     91 | resSphere:sfract.X = invert_half(tmpB).X;
  vrcph $v29.e0, $v22.e0                             ## L:296  |     92 | res:sfract.x = invert_half(tmpA).x;
  vrcpl $v17.e0, $v23.e0                             ## L:296  |     93 | res:sfract.x = invert_half(tmpA).x;
  vrcph $v29.e4, $v22.e4                             ## L:297  |     94 | res:sfract.X = invert_half(tmpA).X;
  vrcpl $v17.e4, $v23.e4                             ## L:297  |     95 | res:sfract.X = invert_half(tmpA).X;
  vsubc $v27, $v27, $v07.e0                          ## L:299  |     96 | resSphere:ufract = resSphere:ufract - SPHERE_RAD:ufract.x;
  vsub $v26, $v00, $v00.v                            ## L:300  |     97 | resSphere:sint = VZERO - VZERO;
  vsubc $v17, $v17, $v07.e1                          ## L:302  |    *99 | res:ufract = res:ufract - SPHERE_RAD:ufract.y;
  vsub $v16, $v00, $v00.v                            ## L:303  |    100 | res:sint = VZERO - VZERO;
  vmudl $v27, $v27, $v09.h1                          ## L:310  |    101 | resSphere = resSphere * LERP_FACTOR:ufract.yyyyYYYY;
  vmadm $v26, $v26, $v09.h1                          ## L:310  |    102 | resSphere = resSphere * LERP_FACTOR:ufract.yyyyYYYY;
  ssv $v21, 12, 66, $zero                            ## L:244  |      ^ | @Barrier("p.Z") store(nextPos.Z, ZERO, 64); ## Barrier: 0x80
  vmadn $v27, $v00, $v00                             ## L:310  |    103 | resSphere = resSphere * LERP_FACTOR:ufract.yyyyYYYY;
  ssv $v20, 10, 60, $zero                            ## L:243  |      ^ | @Barrier("p.Y") store(nextPos.Y, ZERO, 60); ## Barrier: 0x40
  vmadl $v17, $v17, $v09.h0                          ## L:311  |    104 | res = res +* LERP_FACTOR:ufract.xxxxXXXX;
  ssv $v21, 10, 62, $zero                            ## L:243  |      ^ | @Barrier("p.Y") store(nextPos.Y, ZERO, 60); ## Barrier: 0x40
  vmadm $v16, $v16, $v09.h0                          ## L:311  |    105 | res = res +* LERP_FACTOR:ufract.xxxxXXXX;
  j LABEL_RayMarch_0001                              ## L:311  |      ^ | res = res +* LERP_FACTOR:ufract.xxxxXXXX;
  vmadn $v17, $v00, $v00                             ## L:311  |   *107 | res = res +* LERP_FACTOR:ufract.xxxxXXXX;
  LOOP_END:
   # inline-ASM                                      ## L:317  |      0 | asm("");
  break # inline-ASM                                 ## L:318  |      0 | asm("break");
  markDoneA:
  vmov $v09.e0, $v00.e0                              ## L:322  |    108 | LERP_FACTOR:ufract.x = 0;
  addiu $t5, $zero, 255                              ## L:324  |      ^ | isDoneAFlag = 0xFF;
  beq $t5, $t6, LOOP_END                             ## L:325  |    109 | if(isDoneAFlag == isDoneBFlag)goto LOOP_END;
  nop                                                ## L:381  |   *111 | 
  j __RET_MARK_DONE_A                                ## L:328  |    112 | goto __RET_MARK_DONE_A;
  vmov $v09.e1, $v00.e0                              ## L:327  |   *114 | LERP_FACTOR:ufract.y = 0;
  markDoneB:
  addiu $t6, $zero, 255                              ## L:335  |    115 | isDoneBFlag = 0xFF;
  beq $t5, $t6, LOOP_END                             ## L:336  |    116 | if(isDoneAFlag == isDoneBFlag)goto LOOP_END;
  vmov $v09.e4, $v00.e0                              ## L:333  |   *118 | LERP_FACTOR:ufract.X = 0;
  j __RET_MARK_DONE_B                                ## L:339  |    119 | goto __RET_MARK_DONE_B;
  vmov $v09.e5, $v00.e0                              ## L:338  |   *121 | LERP_FACTOR:ufract.Y = 0;

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