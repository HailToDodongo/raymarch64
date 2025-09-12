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
  lsv $v12, 4, 20, $zero                             ## L:181  |      ^ | rayDir.z = load(ZERO, 20).x;
  vxor $v16, $v00, $v00                              ## L:172  |      ^ | vec32 totalDist = 0;
  vxor $v01, $v00, $v00                              ## L:166  |      2 | posSq:sint = 0;
  lsv $v15, 0, 82, $zero                             ## L:169  |      ^ | res.x = load(ZERO, 80).x;
  lsv $v12, 0, 12, $zero                             ## L:179  |      3 | rayDir.x = load(ZERO, 12).x;
  lsv $v12, 2, 16, $zero                             ## L:180  |      4 | rayDir.y = load(ZERO, 16).x;
  lsv $v14, 0, 80, $zero                             ## L:169  |      5 | res.x = load(ZERO, 80).x;
  lsv $v13, 2, 18, $zero                             ## L:180  |      6 | rayDir.y = load(ZERO, 16).x;
  vor $v15, $v00, $v15.e0                            ## L:170  |      ^ | res = res.x;
  lsv $v12, 8, 44, $zero                             ## L:183  |      7 | rayDir.X = load(ZERO, 44).x;
  or $t0, $zero, $zero                               ## L:160  |      8 | s32 totalDistA = 0;
  lsv $v13, 0, 14, $zero                             ## L:179  |      9 | rayDir.x = load(ZERO, 12).x;
  lsv $v13, 4, 22, $zero                             ## L:181  |     10 | rayDir.z = load(ZERO, 20).x;
  vor $v14, $v00, $v14.e0                            ## L:170  |      ^ | res = res.x;
  vxor $v17, $v00, $v00                              ## L:172  |     11 | vec32 totalDist = 0;
  lsv $v13, 8, 46, $zero                             ## L:183  |      ^ | rayDir.X = load(ZERO, 44).x;
  lsv $v12, 10, 48, $zero                            ## L:184  |     12 | rayDir.Y = load(ZERO, 48).x;
  lsv $v13, 10, 50, $zero                            ## L:184  |     13 | rayDir.Y = load(ZERO, 48).x;
  addiu $t6, $zero, 1                                ## L:157  |     14 | u8<$t6> isDoneBFlag = 1;
  or $t1, $zero, $zero                               ## L:161  |     15 | s32 totalDistB = 0;
  addiu $t5, $zero, 1                                ## L:156  |     16 | u8<$t5> isDoneAFlag = 1;
  LABEL_RayMarch_0001:
  ssv $v14, 0, 36, $zero                             ## L:192  |     17 | @Barrier("lenA") store(res.x, ZERO, 36); ## Barrier: 0x1
  vaddc $v17, $v17, $v15.v                           ## L:190  |      ^ | totalDist += res;
  lsv $v13, 12, 54, $zero                            ## L:188  |     18 | rayDir.Z = load(ZERO, 52).x;
  vadd $v16, $v16, $v14.v                            ## L:190  |      ^ | totalDist += res;
  ssv $v14, 8, 68, $zero                             ## L:193  |     19 | @Barrier("lenB") store(res.X, ZERO, 68); ## Barrier: 0x2
  vmudl $v29, $v05, $v11.v                           ## L:210  |      ^ | nextPos = ONE * rayPosOrg;
  lsv $v12, 12, 52, $zero                            ## L:188  |     20 | rayDir.Z = load(ZERO, 52).x;
  vmadm $v29, $v04, $v11.v                           ## L:210  |      ^ | nextPos = ONE * rayPosOrg;
  ssv $v15, 0, 38, $zero                             ## L:192  |     21 | @Barrier("lenA") store(res.x, ZERO, 36); ## Barrier: 0x1
  vmadn $v19, $v05, $v10.v                           ## L:210  |      ^ | nextPos = ONE * rayPosOrg;
  vmadh $v18, $v04, $v10.v                           ## L:210  |     22 | nextPos = ONE * rayPosOrg;
  lw $t2, 36($zero)                                  ## L:195  |      ^ | @Barrier("lenA") distA = load(ZERO, 36); ## Barrier: 0x1
  ssv $v15, 8, 70, $zero                             ## L:193  |     23 | @Barrier("lenB") store(res.X, ZERO, 68); ## Barrier: 0x2
  vmadl $v29, $v13, $v17.h0                          ## L:211  |      ^ | nextPos = rayDir +* totalDist.xxxxXXXX;
  vmadm $v29, $v12, $v17.h0                          ## L:211  |     24 | nextPos = rayDir +* totalDist.xxxxXXXX;
  lw $t3, 68($zero)                                  ## L:196  |      ^ | @Barrier("lenB") distB = load(ZERO, 68); ## Barrier: 0x2
  slt $t7, $t2, $a1                                  ## L:201  |     25 | u8 isDoneA = distA < STOP_DIST;
  vmadn $v19, $v13, $v16.h0                          ## L:211  |      ^ | nextPos = rayDir +* totalDist.xxxxXXXX;
  addu $t0, $t0, $t2                                 ## L:198  |     26 | totalDistA += distA;
  vmadh $v18, $v12, $v16.h0                          ## L:211  |      ^ | nextPos = rayDir +* totalDist.xxxxXXXX;
  vmadl $v21, $v05, $v07.e2                          ## L:214  |     27 | nextPosFloor:sint = ONE +* SPHERE_RAD:sfract.z;
  slt $t8, $t3, $a1                                  ## L:202  |      ^ | u8 isDoneB = distB < STOP_DIST;
  vmadm $v20, $v04, $v07.e2                          ## L:214  |     28 | nextPosFloor:sint = ONE +* SPHERE_RAD:sfract.z;
  addu $t1, $t1, $t3                                 ## L:199  |      ^ | totalDistB += distB;
  slt $t4, $a0, $t0                                  ## L:204  |     29 | tmp = totalDistA > RENDER_DIST;
  or $t7, $t7, $t4                                   ## L:205  |     30 | isDoneA |= tmp;
  slt $t4, $a0, $t1                                  ## L:206  |     31 | tmp = totalDistB > RENDER_DIST;
  vmadn $v21, $v00, $v00                             ## L:214  |      ^ | nextPosFloor:sint = ONE +* SPHERE_RAD:sfract.z;
  vsub $v18, $v18, $v20.v                            ## L:216  |     32 | nextPos:sint -= nextPosFloor:sint;
  beq $t7, $t5, markDoneA                            ## L:219  |      ^ | if(isDoneA == isDoneAFlag)goto markDoneA;
  or $t8, $t8, $t4                                   ## L:207  |    *34 | isDoneB |= tmp;
  __RET_MARK_DONE_A:
  vmudl $v29, $v19, $v19.v                           ## L:222  |     35 | posSq:sfract = nextPos * nextPos;
  vmadm $v29, $v18, $v19.v                           ## L:222  |     36 | posSq:sfract = nextPos * nextPos;
  beq $t8, $t6, markDoneB                            ## L:224  |      ^ | if(isDoneB == isDoneBFlag)goto markDoneB;
  vmadn $v02, $v19, $v18.v                           ## L:222  |    *38 | posSq:sfract = nextPos * nextPos;
  __RET_MARK_DONE_B:
  vrsqh $v22.e0, $v00.e0                             ## L:123  |     39 | asm_op("vrsqh", out:sint.x, VZERO.x);
  vaddc $v15, $v02, $v02.h2                          ## L:227  |   **42 | res = posSq:sfract + posSq:sfract.zzzzZZZZ;
  lsv $v09, 8, 76, $zero                             ## L:232  |      ^ | LERP_FACTOR:ufract.X = load(ZERO, 76).x;
  vadd $v14, $v00, $v00.h2                           ## L:227  |     43 | res = posSq:sfract + posSq:sfract.zzzzZZZZ;
  vrsql $v23.e0, $v15.e0                             ## L:124  |   **46 | asm_op("vrsql", out:sfract.x, in:sfract.x);
  vrsqh $v22.e0, $v00.e4                             ## L:131  |     47 | asm_op("vrsqh", out:sint.X, VZERO.X);
  vrsql $v23.e4, $v15.e4                             ## L:132  |     48 | asm_op("vrsql", out:sfract.X, in:sfract.X);
  vrsqh $v22.e4, $v00.e0                             ## L:133  |     49 | asm_op("vrsqh", out:sint.X, VZERO.x);
  vaddc $v21, $v15, $v02.h1                          ## L:244  |     50 | tmpA = res:sfract + posSq:sfract.yyyyYYYY;
  vadd $v20, $v00, $v00.h1                           ## L:244  |     51 | tmpA = res:sfract + posSq:sfract.yyyyYYYY;
  vmudm $v14, $v22, $v31.e7                          ## L:139  |    *53 | asm_op("vmudm", out:sint,   in:sint,   VSHIFT8.W);
  vmadl $v15, $v23, $v31.e7                          ## L:140  |     54 | asm_op("vmadl", out:sfract, in:sfract, VSHIFT8.W);
  vrsqh $v24.e0, $v00.e0                             ## L:123  |     55 | asm_op("vrsqh", out:sint.x, VZERO.x);
  lsv $v09, 2, 78, $zero                             ## L:231  |      ^ | LERP_FACTOR:ufract.y = load(ZERO, 78).x;
  vrsql $v25.e0, $v21.e0                             ## L:124  |     56 | asm_op("vrsql", out:sfract.x, in:sfract.x);
  lsv $v09, 0, 76, $zero                             ## L:230  |      ^ | LERP_FACTOR:ufract.x = load(ZERO, 76).x;
  lsv $v09, 10, 78, $zero                            ## L:233  |     57 | LERP_FACTOR:ufract.Y = load(ZERO, 78).x;
  vrsqh $v24.e0, $v00.e0                             ## L:125  |      ^ | asm_op("vrsqh", out:sint.x, VZERO.x);
  vrcph $v29.e0, $v14.e0                             ## L:251  |     58 | tmpB:sfract.x = invert_half(res).x;
  vrcpl $v23.e0, $v15.e0                             ## L:251  |     59 | tmpB:sfract.x = invert_half(res).x;
  vrcph $v29.e4, $v14.e4                             ## L:252  |     60 | tmpB:sfract.X = invert_half(res).X;
  mfc2 $t9, $v25.e0                                  ## L:249  |      ^ | s32 resSphereA = resSphere.x;
  mfc2 $at, $v24.e0                                  ## L:249  |     61 | s32 resSphereA = resSphere.x;
  vrcpl $v23.e4, $v15.e4                             ## L:252  |      ^ | tmpB:sfract.X = invert_half(res).X;
  vrsqh $v24.e4, $v00.e4                             ## L:131  |    *63 | asm_op("vrsqh", out:sint.X, VZERO.X);
  andi $t9, $t9, 65535                               ## L:249  |      ^ | s32 resSphereA = resSphere.x;
  sll $at, $at, 16                                   ## L:249  |     64 | s32 resSphereA = resSphere.x;
  vrsql $v25.e4, $v21.e4                             ## L:132  |      ^ | asm_op("vrsql", out:sfract.X, in:sfract.X);
  or $t9, $t9, $at                                   ## L:249  |     65 | s32 resSphereA = resSphere.x;
  vsubc $v15, $v23, $v07.e0                          ## L:262  |      ^ | res:ufract = tmpB:ufract - SPHERE_RAD:ufract.x;
  vsub $v14, $v00, $v06.e0                           ## L:263  |     66 | res:sint = VZERO - SPHERE_RAD:sint.x;
  vrsqh $v24.e4, $v00.e0                             ## L:133  |     67 | asm_op("vrsqh", out:sint.X, VZERO.x);
  sra $t9, $t9, 8                                    ## L:257  |      ^ | resSphereA >>= 8;
  vmudl $v29, $v15, $v15.v                           ## L:265  |    *69 | res:sfract = res * res;
  sw $t9, 120($zero)                                 ## L:259  |      ^ | @Barrier("a") store(resSphereA, ZERO, 120); ## Barrier: 0x4
  vmadm $v29, $v14, $v15.v                           ## L:265  |     70 | res:sfract = res * res;
  mfc2 $k0, $v25.e4                                  ## L:255  |      ^ | s32 resSphereB = resSphere.X;
  vmadn $v15, $v15, $v14.v                           ## L:265  |     71 | res:sfract = res * res;
  mfc2 $at, $v24.e4                                  ## L:255  |      ^ | s32 resSphereB = resSphere.X;
  lsv $v22, 0, 120, $zero                            ## L:268  |     72 | @Barrier("a") tmpB.x = load(ZERO, 120).x; ## Barrier: 0x4
  vmadl $v29, $v05, $v02.h1                          ## L:266  |      ^ | tmpA = ONE +* posSq.yyyyYYYY;
  andi $k0, $k0, 65535                               ## L:255  |     73 | s32 resSphereB = resSphere.X;
  vmadm $v29, $v04, $v02.h1                          ## L:266  |      ^ | tmpA = ONE +* posSq.yyyyYYYY;
  vmadn $v21, $v05, $v01.h1                          ## L:266  |     74 | tmpA = ONE +* posSq.yyyyYYYY;
  sll $at, $at, 16                                   ## L:255  |      ^ | s32 resSphereB = resSphere.X;
  vmadh $v20, $v04, $v01.h1                          ## L:266  |     75 | tmpA = ONE +* posSq.yyyyYYYY;
  or $k0, $k0, $at                                   ## L:255  |      ^ | s32 resSphereB = resSphere.X;
  vrsqh $v14.e0, $v00.e0                             ## L:123  |     76 | asm_op("vrsqh", out:sint.x, VZERO.x);
  sra $k0, $k0, 8                                    ## L:258  |      ^ | resSphereB >>= 8;
  sw $k0, 124($zero)                                 ## L:260  |     77 | @Barrier("b") store(resSphereB, ZERO, 124); ## Barrier: 0x8
  lsv $v22, 8, 124, $zero                            ## L:269  |     78 | @Barrier("b") tmpB.X = load(ZERO, 124).x; ## Barrier: 0x8
  vrsql $v15.e0, $v21.e0                             ## L:124  |      ^ | asm_op("vrsql", out:sfract.x, in:sfract.x);
  lsv $v23, 0, 122, $zero                            ## L:268  |     79 | @Barrier("a") tmpB.x = load(ZERO, 120).x; ## Barrier: 0x4
  vrsqh $v14.e0, $v00.e4                             ## L:131  |      ^ | asm_op("vrsqh", out:sint.X, VZERO.X);
  lsv $v23, 8, 126, $zero                            ## L:269  |     80 | @Barrier("b") tmpB.X = load(ZERO, 124).x; ## Barrier: 0x8
  vrsql $v15.e4, $v21.e4                             ## L:132  |      ^ | asm_op("vrsql", out:sfract.X, in:sfract.X);
  vrsqh $v14.e4, $v00.e0                             ## L:133  |     81 | asm_op("vrsqh", out:sint.X, VZERO.x);
  vrcph $v29.e0, $v22.e0                             ## L:277  |     82 | resSphere:sfract.x = invert_half(tmpB).x;
  vrcpl $v25.e0, $v23.e0                             ## L:277  |    *84 | resSphere:sfract.x = invert_half(tmpB).x;
  vmudm $v20, $v14, $v31.e7                          ## L:139  |     85 | asm_op("vmudm", out:sint,   in:sint,   VSHIFT8.W);
  vmadl $v21, $v15, $v31.e7                          ## L:140  |     86 | asm_op("vmadl", out:sfract, in:sfract, VSHIFT8.W);
  vrcph $v29.e4, $v22.e4                             ## L:282  |     87 | resSphere:sfract.X = invert_half(tmpB).X;
  vrcpl $v25.e4, $v23.e4                             ## L:282  |     88 | resSphere:sfract.X = invert_half(tmpB).X;
  vrcph $v29.e0, $v20.e0                             ## L:284  |     89 | res:sfract.x = invert_half(tmpA).x;
  vrcpl $v15.e0, $v21.e0                             ## L:284  |     90 | res:sfract.x = invert_half(tmpA).x;
  vrcph $v29.e4, $v20.e4                             ## L:285  |     91 | res:sfract.X = invert_half(tmpA).X;
  vrcpl $v15.e4, $v21.e4                             ## L:285  |     92 | res:sfract.X = invert_half(tmpA).X;
  vsubc $v25, $v25, $v07.e0                          ## L:287  |     93 | resSphere:ufract = resSphere:ufract - SPHERE_RAD:ufract.x;
  vsub $v24, $v00, $v00.v                            ## L:288  |     94 | resSphere:sint = VZERO - VZERO;
  vsubc $v15, $v15, $v07.e1                          ## L:290  |    *96 | res:ufract = res:ufract - SPHERE_RAD:ufract.y;
  vsub $v14, $v00, $v00.v                            ## L:291  |     97 | res:sint = VZERO - VZERO;
  vmudl $v25, $v25, $v09.h1                          ## L:294  |     98 | resSphere = resSphere * LERP_FACTOR:ufract.yyyyYYYY;
  vmadm $v24, $v24, $v09.h1                          ## L:294  |     99 | resSphere = resSphere * LERP_FACTOR:ufract.yyyyYYYY;
  vmadn $v25, $v00, $v00                             ## L:294  |    100 | resSphere = resSphere * LERP_FACTOR:ufract.yyyyYYYY;
  vmadl $v15, $v15, $v09.h0                          ## L:295  |    101 | res = res +* LERP_FACTOR:ufract.xxxxXXXX;
  vmadm $v14, $v14, $v09.h0                          ## L:295  |    102 | res = res +* LERP_FACTOR:ufract.xxxxXXXX;
  j LABEL_RayMarch_0001                              ## L:295  |      ^ | res = res +* LERP_FACTOR:ufract.xxxxXXXX;
  vmadn $v15, $v00, $v00                             ## L:295  |   *104 | res = res +* LERP_FACTOR:ufract.xxxxXXXX;
  markDoneA:
  addiu $t5, $zero, 255                              ## L:303  |    105 | isDoneAFlag = 0xFF;
  bne $t5, $t6, __RET_MARK_DONE_A                    ## L:304  |    106 | if(isDoneAFlag != isDoneBFlag)goto __RET_MARK_DONE_A;
  sw $t0, 40($zero)                                  ## L:302  |   *108 | store(totalDistA, ZERO, 40);
  break # inline-ASM                                 ## L:305  |      0 | asm("break");
  markDoneB:
  addiu $t6, $zero, 255                              ## L:311  |    109 | isDoneBFlag = 0xFF;
  bne $t5, $t6, __RET_MARK_DONE_B                    ## L:312  |    110 | if(isDoneAFlag != isDoneBFlag)goto __RET_MARK_DONE_B;
  sw $t1, 72($zero)                                  ## L:310  |   *112 | store(totalDistB, ZERO, 72);
  break # inline-ASM                                 ## L:313  |      0 | asm("break");

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