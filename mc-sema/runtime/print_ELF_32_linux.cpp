/* Copyright 2016 Peter Goodman (peter@trailofbits.com), all rights reserved. */

#include <cstdio>

#define ONLY_STRUCT
#include "../common/RegisterState.h"

static const unsigned kStackSize = 1UL << 20U;
static const unsigned kStackArgSize = 256U;

int main(void) {

  printf("/* Auto-generated file! Don't modify! */\n\n");
  printf("  .file __FILE__\n");
  printf("  .intel_syntax noprefix\n");
  printf("\n");

  // Thread-local state structure, named by `__mcsema_reg_state`.
  printf("  .type __mcsema_reg_state,@object\n");
  printf("  .section .tbss,\"awT\",@nobits\n");
  printf("__mcsema_reg_state:\n");
  printf("  .zero %u\n", sizeof(mcsema::RegState));
  printf("  .size __mcsema_reg_state, 100\n");
  printf("\n");

  // Thread-local stack structure, named by `__mcsema_stack`.
  printf("  .type __mcsema_stack,@object\n");
  printf("  .section .tbss,\"awT\",@nobits\n");
  printf("__mcsema_stack:\n");
  printf("  .zero %u\n", kStackSize);  // 1 MiB.
  printf("  .size __mcsema_stack, 100\n");
  printf("\n");

  // Thread-local stack structure, named by `__mcsema_stack_args`
  // used to store stack-passed function arguments
  printf("  .type __mcsema_stack_args,@object\n");
  printf("  .section .tbss,\"awT\",@nobits\n");
  printf("__mcsema_stack_args:\n");
  printf("  .zero %u\n", kStackArgSize);  // 1 MiB.
  printf("  .size __mcsema_stack_args, 100\n");
  printf("\n");

  printf("  .text\n");
  printf("\n");

  // Forward declarations.
  printf("  .globl mcsema_main\n");
  printf("  .globl __mcsema_detach_ret_cdecl\n");
  printf("\n");

  ///////////////////////////////////////////////////////////////
  //
  //         __mcsema_attach_call_cdecl
  //
  ///////////////////////////////////////////////////////////////

  // Implements `__mcsema_attach_call_cdecl`. This goes from native state into lifted code.
  // The lifted code function pointer is already on the stack.
  printf("  .globl __mcsema_attach_call_cdecl\n");
  printf("  .type __mcsema_attach_call_cdecl,@function\n");
  printf("__mcsema_attach_call_cdecl:\n");
  printf("  .cfi_startproc\n");

  // Pop the target function into the `RegState` structure. This resets `ESP`
  // to what it should be on entry to `__mcsema_attach_call`.
  printf("  pop DWORD PTR gs:[__mcsema_reg_state@NTPOFF + %u]\n", __builtin_offsetof(mcsema::RegState, EIP));

  // General purpose registers.
  printf("  mov gs:[__mcsema_reg_state@NTPOFF + %u], eax\n", __builtin_offsetof(mcsema::RegState, EAX));
  printf("  mov gs:[__mcsema_reg_state@NTPOFF + %u], ebx\n", __builtin_offsetof(mcsema::RegState, EBX));
  printf("  mov gs:[__mcsema_reg_state@NTPOFF + %u], ecx\n", __builtin_offsetof(mcsema::RegState, ECX));
  printf("  mov gs:[__mcsema_reg_state@NTPOFF + %u], edx\n", __builtin_offsetof(mcsema::RegState, EDX));
  printf("  mov gs:[__mcsema_reg_state@NTPOFF + %u], edi\n", __builtin_offsetof(mcsema::RegState, EDI));
  printf("  mov gs:[__mcsema_reg_state@NTPOFF + %u], esi\n", __builtin_offsetof(mcsema::RegState, ESI));
  printf("  mov gs:[__mcsema_reg_state@NTPOFF + %u], ebp\n", __builtin_offsetof(mcsema::RegState, EBP));


  // copy posible stack args from native ESP into temporary holding area
  //printf("  mov eax, gs:[0]\n");
  //printf("  lea edi, [eax + __mcsema_stack_args@NTPOFF]\n");
  //printf("  mov esi, esp\n");
  //printf("  mov ecx, %u\n", kStackArgSize);
  //printf("  rep movsb\n");

  printf("  xchg esp, DWORD PTR gs:[__mcsema_reg_state@NTPOFF + %u]\n", __builtin_offsetof(mcsema::RegState, ESP));

  // XMM registers.
  printf("  movdqu gs:[__mcsema_reg_state@NTPOFF + %u], xmm0\n", __builtin_offsetof(mcsema::RegState, XMM0));
  printf("  movdqu gs:[__mcsema_reg_state@NTPOFF + %u], xmm1\n", __builtin_offsetof(mcsema::RegState, XMM1));
  printf("  movdqu gs:[__mcsema_reg_state@NTPOFF + %u], xmm2\n", __builtin_offsetof(mcsema::RegState, XMM2));
  printf("  movdqu gs:[__mcsema_reg_state@NTPOFF + %u], xmm3\n", __builtin_offsetof(mcsema::RegState, XMM3));
  printf("  movdqu gs:[__mcsema_reg_state@NTPOFF + %u], xmm4\n", __builtin_offsetof(mcsema::RegState, XMM4));
  printf("  movdqu gs:[__mcsema_reg_state@NTPOFF + %u], xmm5\n", __builtin_offsetof(mcsema::RegState, XMM5));
  printf("  movdqu gs:[__mcsema_reg_state@NTPOFF + %u], xmm6\n", __builtin_offsetof(mcsema::RegState, XMM6));
  printf("  movdqu gs:[__mcsema_reg_state@NTPOFF + %u], xmm7\n", __builtin_offsetof(mcsema::RegState, XMM7));

  // If `ESP` is null then we need to initialize it to our new stack.
  printf("  cmp esp, 0\n");
  printf("  jnz .Lhave_stack\n");
  printf("  mov esp, DWORD PTR gs:[0]\n");
  printf("  lea esp, [esp + __mcsema_stack@NTPOFF + %u]\n", kStackSize);
  printf(".Lhave_stack:\n");

  // copy posible stack args from holding area to mcsema stack
  // allocate space for our arguments on stack
  //printf("  sub esp, %u\n", kStackArgSize);
  //// we need to save these 
  //printf("  push esi\n");
  //printf("  push edi\n");
  //printf("  push ecx\n");
  //// get the stack arg location
  //printf("  lea edi, [esp + %u]\n", 4+4+4);
  //// source is temp area
  //printf("  mov eax, DWORD PTR gs:[0]\n");
  //printf("  lea esi, [eax + __mcsema_stack_args@NTPOFF]\n");
  //printf("  mov ecx, %u\n", kStackArgSize);
  //// copy
  //printf("  rep movsb\n");

  //// restore saved regs
  //printf("  pop ecx\n");
  //printf("  pop edi\n");
  //printf("  pop esi\n");

  // the state struture is the first and only arg to lifted functions
  printf("  mov eax, DWORD PTR gs:[0]\n");
  printf("  lea eax, [eax + __mcsema_reg_state@NTPOFF]\n");
  printf("  push eax\n");

  // `esp` holds the address of the mcsema stack.
  //    1) Set up a return address on the mcsema stack.
  //    2) Tail-call to the lifted function.
  //
  // Note:  When the lifted function returns, it will go to `__mcsema_detach_ret_cdecl`,
  //        which will return to native code.
  //

  // do not push __mcsema_detach_ret_cdecl directly
  // to work around llvm assembler bug that emits it
  // as a 16-bit push
  printf("  lea eax, __mcsema_detach_ret_cdecl\n");
  printf("  push eax\n");
  printf("  jmp DWORD PTR gs:[__mcsema_reg_state@NTPOFF + %u]\n", __builtin_offsetof(mcsema::RegState, EIP));

  printf(".Lfunc_end1:\n");
  printf("  .size __mcsema_attach_call_cdecl,.Lfunc_end1-__mcsema_attach_call_cdecl\n");
  printf("  .cfi_endproc\n");
  printf("\n");


  ///////////////////////////////////////////////////////////////
  //
  //         __mcsema_attach_ret_cdecl
  //
  ///////////////////////////////////////////////////////////////


  // Implements `__mcsema_attach_ret_cdecl`. This goes from native state into lifted code.
  printf("  .globl __mcsema_attach_ret_cdecl\n");
  printf("  .type __mcsema_attach_ret_cdecl,@function\n");
  printf("__mcsema_attach_ret_cdecl:\n");
  printf("  .cfi_startproc\n");

  // this should be valid for cdecl:
  // return stack to where it was before we pasted
  // some arguments to it, so the caller can clean
  // up as expected
  //
  // add an extra 4 bytes to compensate for the fake return address
  printf("  add esp, %u\n", kStackArgSize+4);
  // Swap into the mcsema stack.
  printf("  xchg esp, DWORD PTR gs:[__mcsema_reg_state@NTPOFF + %u]\n", __builtin_offsetof(mcsema::RegState, ESP));

  // Return registers.
  printf("  mov gs:[__mcsema_reg_state@NTPOFF + %u], eax\n", __builtin_offsetof(mcsema::RegState, EAX));
  printf("  mov gs:[__mcsema_reg_state@NTPOFF + %u], edx\n", __builtin_offsetof(mcsema::RegState, EDX));
  printf("  movdqu gs:[__mcsema_reg_state@NTPOFF + %u], xmm0\n", __builtin_offsetof(mcsema::RegState, XMM0));

  // Callee-saved registers.
  printf("  mov gs:[__mcsema_reg_state@NTPOFF + %u], ebp\n", __builtin_offsetof(mcsema::RegState, EBP));
  printf("  mov gs:[__mcsema_reg_state@NTPOFF + %u], ebx\n", __builtin_offsetof(mcsema::RegState, EBX));
  printf("  mov gs:[__mcsema_reg_state@NTPOFF + %u], esi\n", __builtin_offsetof(mcsema::RegState, ESI));
  printf("  mov gs:[__mcsema_reg_state@NTPOFF + %u], edi\n", __builtin_offsetof(mcsema::RegState, EDI));

  // Unstash the callee-saved registers.
  printf("  pop ebp\n");
  printf("  pop ebx\n");
  printf("  pop esi\n");
  printf("  pop edi\n");

  printf("  ret\n");

  printf(".Lfunc_end2:\n");
  printf("  .size __mcsema_attach_ret_cdecl,.Lfunc_end2-__mcsema_attach_ret_cdecl\n");
  printf("  .cfi_endproc\n");
  printf("\n");

  ///////////////////////////////////////////////////////////////
  //
  //         __mcsema_attach_ret_value
  //
  ///////////////////////////////////////////////////////////////

  // Implements `__mcsema_attach_ret_value`. This is the "opposite" of
  // `__mcsema_detach_call_value`.
  printf("  .globl __mcsema_attach_ret_value\n");
  printf("  .type __mcsema_attach_ret_value,@function\n");
  printf("__mcsema_attach_ret_value:\n");
  printf("  .cfi_startproc\n");

  // General purpose registers.
  printf("  mov gs:[__mcsema_reg_state@NTPOFF + %u], eax\n", __builtin_offsetof(mcsema::RegState, EAX));
  printf("  mov gs:[__mcsema_reg_state@NTPOFF + %u], ebx\n", __builtin_offsetof(mcsema::RegState, EBX));
  printf("  mov gs:[__mcsema_reg_state@NTPOFF + %u], ecx\n", __builtin_offsetof(mcsema::RegState, ECX));
  printf("  mov gs:[__mcsema_reg_state@NTPOFF + %u], edx\n", __builtin_offsetof(mcsema::RegState, EDX));
  printf("  mov gs:[__mcsema_reg_state@NTPOFF + %u], edi\n", __builtin_offsetof(mcsema::RegState, EDI));
  printf("  mov gs:[__mcsema_reg_state@NTPOFF + %u], esi\n", __builtin_offsetof(mcsema::RegState, ESI));
  printf("  mov gs:[__mcsema_reg_state@NTPOFF + %u], ebp\n", __builtin_offsetof(mcsema::RegState, EBP));
  // this may ony be valid for caller clenup?
  // TODO(artem): check if esp is changed from pre-call ESP;
  // if so, pop off some regs from mcsema's ESP, for callee cleanup
  // the +4 compensates for the fake return address we pushed earlier
  printf("  add esp, %u\n", kStackArgSize+4);

  printf("  xchg esp, DWORD PTR gs:[__mcsema_reg_state@NTPOFF + %u]\n", __builtin_offsetof(mcsema::RegState, ESP));

  // XMM registers.
  printf("  movdqu gs:[__mcsema_reg_state@NTPOFF + %u], xmm0\n", __builtin_offsetof(mcsema::RegState, XMM0));
  printf("  movdqu gs:[__mcsema_reg_state@NTPOFF + %u], xmm1\n", __builtin_offsetof(mcsema::RegState, XMM1));
  printf("  movdqu gs:[__mcsema_reg_state@NTPOFF + %u], xmm2\n", __builtin_offsetof(mcsema::RegState, XMM2));
  printf("  movdqu gs:[__mcsema_reg_state@NTPOFF + %u], xmm3\n", __builtin_offsetof(mcsema::RegState, XMM3));
  printf("  movdqu gs:[__mcsema_reg_state@NTPOFF + %u], xmm4\n", __builtin_offsetof(mcsema::RegState, XMM4));
  printf("  movdqu gs:[__mcsema_reg_state@NTPOFF + %u], xmm5\n", __builtin_offsetof(mcsema::RegState, XMM5));
  printf("  movdqu gs:[__mcsema_reg_state@NTPOFF + %u], xmm6\n", __builtin_offsetof(mcsema::RegState, XMM6));
  printf("  movdqu gs:[__mcsema_reg_state@NTPOFF + %u], xmm7\n", __builtin_offsetof(mcsema::RegState, XMM7));

  // Unstash the callee-saved registers.
  printf("  pop ebp\n");
  printf("  pop ebx\n");
  printf("  pop esi\n");
  printf("  pop edi\n");

  printf("  ret\n");

  printf(".Lfunc_end0:\n");
  printf("  .size __mcsema_attach_ret_value,.Lfunc_end0-__mcsema_attach_ret_value\n");
  printf("  .cfi_endproc\n");
  printf("\n");

  ///////////////////////////////////////////////////////////////
  //
  //         __mcsema_dettach_ret_cdecl
  //
  ///////////////////////////////////////////////////////////////

  // Implements `__mcsema_detach_ret_cdecl`. This goes from lifted code into native code.
  // The native code pointer is located at the native `[RegState::ESP - 4]`
  // address.
  printf("  .globl __mcsema_detach_ret_cdecl\n");
  printf("  .type __mcsema_detach_ret_cdecl,@function\n");
  printf("__mcsema_detach_ret_cdecl:\n");
  printf("  .cfi_startproc\n");

  // The lifted code emulated a ret, which did incremented `esp` by 4.
  // We "undo" that, then swap back to the native stack. When we swap, we
  // save into `RegState::ESP` where we are in the lifted stack, so that the
  // next attach can continue on where we left off.
  printf("  sub DWORD PTR gs:[__mcsema_reg_state@NTPOFF + %u], 4\n", __builtin_offsetof(mcsema::RegState, ESP));

  // General purpose registers.
  printf("  mov eax, gs:[__mcsema_reg_state@NTPOFF + %u]\n", __builtin_offsetof(mcsema::RegState, EAX));
  printf("  mov ebx, gs:[__mcsema_reg_state@NTPOFF + %u]\n", __builtin_offsetof(mcsema::RegState, EBX));
  printf("  mov ecx, gs:[__mcsema_reg_state@NTPOFF + %u]\n", __builtin_offsetof(mcsema::RegState, ECX));
  printf("  mov edx, gs:[__mcsema_reg_state@NTPOFF + %u]\n", __builtin_offsetof(mcsema::RegState, EDX));
  printf("  mov edi, gs:[__mcsema_reg_state@NTPOFF + %u]\n", __builtin_offsetof(mcsema::RegState, EDI));
  printf("  mov esi, gs:[__mcsema_reg_state@NTPOFF + %u]\n", __builtin_offsetof(mcsema::RegState, ESI));
  printf("  mov ebp, gs:[__mcsema_reg_state@NTPOFF + %u]\n", __builtin_offsetof(mcsema::RegState, EBP));
  printf("  xchg esp, DWORD PTR gs:[__mcsema_reg_state@NTPOFF + %u]\n", __builtin_offsetof(mcsema::RegState, ESP));

  // XMM registers.
  printf("  movdqu xmm0, gs:[__mcsema_reg_state@NTPOFF + %u]\n", __builtin_offsetof(mcsema::RegState, XMM0));
  printf("  movdqu xmm1, gs:[__mcsema_reg_state@NTPOFF + %u]\n", __builtin_offsetof(mcsema::RegState, XMM1));
  printf("  movdqu xmm2, gs:[__mcsema_reg_state@NTPOFF + %u]\n", __builtin_offsetof(mcsema::RegState, XMM2));
  printf("  movdqu xmm3, gs:[__mcsema_reg_state@NTPOFF + %u]\n", __builtin_offsetof(mcsema::RegState, XMM3));
  printf("  movdqu xmm4, gs:[__mcsema_reg_state@NTPOFF + %u]\n", __builtin_offsetof(mcsema::RegState, XMM4));
  printf("  movdqu xmm5, gs:[__mcsema_reg_state@NTPOFF + %u]\n", __builtin_offsetof(mcsema::RegState, XMM5));
  printf("  movdqu xmm6, gs:[__mcsema_reg_state@NTPOFF + %u]\n", __builtin_offsetof(mcsema::RegState, XMM6));
  printf("  movdqu xmm7, gs:[__mcsema_reg_state@NTPOFF + %u]\n", __builtin_offsetof(mcsema::RegState, XMM7));

  printf("  ret\n");

  printf(".Lfunc_end3:\n");
  printf("  .size __mcsema_detach_ret_cdecl,.Lfunc_end3-__mcsema_detach_ret_cdecl\n");
  printf("  .cfi_endproc\n");
  printf("\n");

  ///////////////////////////////////////////////////////////////
  //
  //         __mcsema_detach_call_cdecl
  //
  ///////////////////////////////////////////////////////////////

  // Implements `__mcsema_detach_call`. This partially goes from lifted code
  // into native code.
  printf("  .globl __mcsema_detach_call_cdecl\n");
  printf("  .type __mcsema_detach_call_cdecl,@function\n");
  printf("__mcsema_detach_call_cdecl:\n");
  printf("  .cfi_startproc\n");

  // Pop the target function into the `RegState` structure. This resets `EIP`
  // to what it should be on entry to `__mcsema_detach_call`.
  printf("  pop DWORD PTR gs:[__mcsema_reg_state@NTPOFF + %u]\n", __builtin_offsetof(mcsema::RegState, EIP));
  // Marshal the callee-saved registers (of the emulated code) into the native
  // state. We don't touch the argument registers.

  // Stash the callee-saved registers.
  printf("  push edi\n");
  printf("  push esi\n");
  printf("  push ebx\n");
  printf("  push ebp\n");
  // assume we can clobber eax and ecx

  // copy posible stack args into temporary holding area
  printf("  mov eax, gs:[0]\n");
  printf("  lea edi, [eax + __mcsema_stack_args@NTPOFF]\n");
  printf("  lea esi, [esp + %u]\n", 4 + 4+4+4+4);
  printf("  mov ecx, %u\n", kStackArgSize);
  printf("  rep movsb\n");

  printf("  mov edi, gs:[__mcsema_reg_state@NTPOFF + %u]\n", __builtin_offsetof(mcsema::RegState, EDI));
  printf("  mov esi, gs:[__mcsema_reg_state@NTPOFF + %u]\n", __builtin_offsetof(mcsema::RegState, ESI));
  printf("  mov ebx, gs:[__mcsema_reg_state@NTPOFF + %u]\n", __builtin_offsetof(mcsema::RegState, EBX));
  printf("  mov ebp, gs:[__mcsema_reg_state@NTPOFF + %u]\n", __builtin_offsetof(mcsema::RegState, EBP));

  // Swap onto the native stack.
  printf("  xchg DWORD PTR gs:[__mcsema_reg_state@NTPOFF + %u], esp\n", __builtin_offsetof(mcsema::RegState, ESP));

  // copy posible stack args from holding area to native stack
  // allocate space for our arguments on stack
  printf("  sub esp, %u\n", kStackArgSize);
  // we need to save these 
  printf("  push esi\n");
  printf("  push edi\n");
  printf("  push ecx\n");
  // get the stack arg location
  printf("  lea edi, [esp + %u]\n", 4+4+4);
  // source is temp area
  printf("  mov eax, DWORD PTR gs:[0]\n");
  printf("  lea esi, [eax + __mcsema_stack_args@NTPOFF]\n");
  printf("  mov ecx, %u\n", kStackArgSize);
  // copy
  printf("  rep movsb\n");

  // restore saved regs
  printf("  pop ecx\n");
  printf("  pop edi\n");
  printf("  pop esi\n");

  // Set up a re-attach return address.
  // do not push __mcsema_attach_ret_cdecl directly
  // to work around llvm assembler bug that emits it
  // as a 16-bit push
  printf("  lea eax, __mcsema_attach_ret_cdecl\n");
  printf("  push eax\n");

  printf("  jmp DWORD PTR gs:[__mcsema_reg_state@NTPOFF + %u]\n", __builtin_offsetof(mcsema::RegState, EIP));

  printf(".Lfunc_end4:\n");
  printf("  .size __mcsema_detach_call_cdecl,.Lfunc_end4-__mcsema_detach_call_cdecl\n");
  printf("  .cfi_endproc\n");
  printf("\n");

  ///////////////////////////////////////////////////////////////
  //
  //         __mcsema_detach_call_value
  //
  ///////////////////////////////////////////////////////////////

  // Implements `__mcsema_detach_call_value`. This is a thin wrapper around
  // `__mcsema_detach_call`.
  printf("  .globl __mcsema_detach_call_value\n");
  printf("  .type __mcsema_detach_call_value,@function\n");
  printf("__mcsema_detach_call_value:\n");
  printf("  .cfi_startproc\n");

  // Note: the bitcode has already put the target address into `RegState::EIP`.

  // Stash the callee-saved registers.
  printf("  push edi\n");
  printf("  push esi\n");
  printf("  push ebx\n");
  printf("  push ebp\n");

  // copy posible stack args into temporary holding area
  printf("  mov eax, DWORD PTR gs:[0]\n");
  printf("  lea edi, [eax + __mcsema_stack_args@NTPOFF]\n");
  // this is not ESP since for do_call_value there is no spilling via an 
  // intermediate function
  printf("  mov esi, DWORD PTR gs:[__mcsema_reg_state@NTPOFF + %u]\n", __builtin_offsetof(mcsema::RegState, ESP));
  printf("  mov ecx, %u\n", kStackArgSize);
  printf("  rep movsb\n");

  // General purpose registers.
  printf("  mov eax, gs:[__mcsema_reg_state@NTPOFF + %u]\n", __builtin_offsetof(mcsema::RegState, EAX));
  printf("  mov ebx, gs:[__mcsema_reg_state@NTPOFF + %u]\n", __builtin_offsetof(mcsema::RegState, EBX));
  printf("  mov ecx, gs:[__mcsema_reg_state@NTPOFF + %u]\n", __builtin_offsetof(mcsema::RegState, ECX));
  printf("  mov edx, gs:[__mcsema_reg_state@NTPOFF + %u]\n", __builtin_offsetof(mcsema::RegState, EDX));
  printf("  mov edi, gs:[__mcsema_reg_state@NTPOFF + %u]\n", __builtin_offsetof(mcsema::RegState, EDI));
  printf("  mov esi, gs:[__mcsema_reg_state@NTPOFF + %u]\n", __builtin_offsetof(mcsema::RegState, ESI));
  printf("  mov ebp, gs:[__mcsema_reg_state@NTPOFF + %u]\n", __builtin_offsetof(mcsema::RegState, EBP));
  printf("  xchg DWORD PTR gs:[__mcsema_reg_state@NTPOFF + %u], esp\n", __builtin_offsetof(mcsema::RegState, ESP));

  // XMM registers.
  printf("  movdqu xmm0, gs:[__mcsema_reg_state@NTPOFF + %u]\n", __builtin_offsetof(mcsema::RegState, XMM0));
  printf("  movdqu xmm1, gs:[__mcsema_reg_state@NTPOFF + %u]\n", __builtin_offsetof(mcsema::RegState, XMM1));
  printf("  movdqu xmm2, gs:[__mcsema_reg_state@NTPOFF + %u]\n", __builtin_offsetof(mcsema::RegState, XMM2));
  printf("  movdqu xmm3, gs:[__mcsema_reg_state@NTPOFF + %u]\n", __builtin_offsetof(mcsema::RegState, XMM3));
  printf("  movdqu xmm4, gs:[__mcsema_reg_state@NTPOFF + %u]\n", __builtin_offsetof(mcsema::RegState, XMM4));
  printf("  movdqu xmm5, gs:[__mcsema_reg_state@NTPOFF + %u]\n", __builtin_offsetof(mcsema::RegState, XMM5));
  printf("  movdqu xmm6, gs:[__mcsema_reg_state@NTPOFF + %u]\n", __builtin_offsetof(mcsema::RegState, XMM6));
  printf("  movdqu xmm7, gs:[__mcsema_reg_state@NTPOFF + %u]\n", __builtin_offsetof(mcsema::RegState, XMM7));

  // copy posible stack args from holding area to native stack
  // allocate space for our arguments on stack
  printf("  sub esp, %u\n", kStackArgSize);
  // we need to save these 
  printf("  push esi\n");
  printf("  push edi\n");
  printf("  push ecx\n");
  // get the stack arg location
  printf("  lea edi, [esp + %u]\n", 4+4+4);
  // source is temp area
  printf("  mov eax, DWORD PTR gs:[0]\n");
  printf("  lea esi, [eax + __mcsema_stack_args@NTPOFF]\n");
  printf("  mov ecx, %u\n", kStackArgSize);
  // copy
  printf("  rep movsb\n");

  // restore saved regs
  printf("  pop ecx\n");
  printf("  pop edi\n");
  printf("  pop esi\n");

  // Set up a re-attach return address.
  // Set up a re-attach return address.
  // do not push __mcsema_attach_ret_value directly
  // to work around llvm assembler bug that emits it
  // as a 16-bit push
  printf("  lea eax, __mcsema_attach_ret_value\n");
  printf("  push eax\n");

  // Go native.
  printf("  jmp DWORD PTR gs:[__mcsema_reg_state@NTPOFF + %u]\n", __builtin_offsetof(mcsema::RegState, EIP));

  printf(".Lfunc_end5:\n");
  printf("  .size __mcsema_detach_call_value,.Lfunc_end5-__mcsema_detach_call_value\n");
  printf("  .cfi_endproc\n");
  printf("\n");

  // Implements `__mcsema_debug_get_reg_state`. This is useful when debugging in
  // gdb.
  printf("  .globl __mcsema_debug_get_reg_state\n");
  printf("  .type __mcsema_debug_get_reg_state,@function\n");
  printf("__mcsema_debug_get_reg_state:\n");
  printf("  .cfi_startproc\n");
  printf("  mov eax, DWORD PTR gs:[0]\n");
  printf("  lea eax, [eax + __mcsema_reg_state@NTPOFF]\n");
  printf("  ret\n");
  printf(".Lfunc_end6:\n");
  printf("  .size __mcsema_debug_get_reg_state,.Lfunc_end6-__mcsema_debug_get_reg_state\n");
  printf("  .cfi_endproc\n");
  printf("\n");
  return 0;
}

