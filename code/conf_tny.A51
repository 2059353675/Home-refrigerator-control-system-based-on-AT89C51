$NOMOD51
;------------------------------------------------------------------------------
;  conf_tny.A51:  RTX51 TINY  Configuration File
;------------------------------------------------------------------------------
;
;  This file contains the configuration for RTX51 Tiny.
;  Modify this file to adapt the RTX51 Tiny kernel to your application.
;
;------------------------------------------------------------------------------

;------------------ Global Configuration --------------------------------------

; Define the number of user tasks (1-15)
USER_TASKS       EQU     5        ; Number of user tasks

; Define the size of the stack for all tasks (in bytes)
STACK_SIZE       EQU     96       ; Size of task stack (min 24 bytes)

; Define the size of the interrupt stack (in bytes)
ISR_STACK_SIZE   EQU     64       ; Size of ISR stack (min 24 bytes)

; Enable/Disable round-robin task switching
; Set to 1 to enable, 0 to disable
ROUND_ROBIN      EQU     1        ; Enable round-robin switching

; Define the round-robin timeout in system ticks
RR_TIMEOUT       EQU     5        ; Timeout for round-robin switching

;------------------ Task Configuration ----------------------------------------

; Define the task IDs for your application
TASK_ID_TEMP_READ    EQU     1    ; Temperature reading task
TASK_ID_DISPLAY      EQU     2    ; Display update task  
TASK_ID_COMPRESSOR   EQU     3    ; Compressor control task
TASK_ID_BUTTON       EQU     4    ; Button detection task
TASK_ID_DOOR_MONITOR       EQU       5  ; Door monitor task
TASK_ID_INIT         EQU     0    ; Initialization task (main)

;------------------ Memory Configuration -------------------------------------

; Define the memory model (Small, Compact, Large)
; This should match your project settings in uVision
MEMORY_MODEL     EQU     1        ; 1=Small, 2=Compact, 3=Large

;------------------ Timer Configuration --------------------------------------

; Define timer register for system tick generation
; Timer 0 is typically used (default)
TIMER            EQU     0        ; Timer 0 for system tick

; System tick interval in milliseconds (typically 10ms)
TICK_INTERVAL    EQU     10       ; 10ms system tick

;------------------ End of Configuration -------------------------------------