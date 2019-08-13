# ======================================================================
# File: multi-arm.S
# ======================================================================

# The function "multi" can not use "mul" instruction!
.globl multi
multi:
     MOV r5, #0
     MOV r4, #0
Loop:
     ADD r4, r4, r0
     ADD r5, r5, #1
     CMP r5, r1
     BLT Loop
     MOV r0, r4
     BX lr 
# ======================================================================
